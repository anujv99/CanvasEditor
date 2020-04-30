#include "networkvm.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <cstring>

#include <vm/lua.hpp>
#include <vm/luabind/luabind.h>
#include <core/window.h>
#include <core/timer.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace paint {

	NetworkVM::NetworkVM() : m_IsRunning(true), m_Type(SocketType::INVALID), m_Port(0), m_Conn(nullptr), m_Server(nullptr) {
		network::Network::Initialize();
		m_NT = std::thread(&NetworkVM::ThreadFunc, this);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	NetworkVM::~NetworkVM() {
		m_IsRunning = false;

		if (m_Server != nullptr) {
			delete m_Server;
			m_Server = nullptr;
		}

		m_Send.notify_all();
		m_Sock.notify_all();
		if (m_NT.joinable()) m_NT.join();
		if (m_SendT.joinable()) m_SendT.join();

		if (m_Conn != nullptr) {
			delete m_Conn;
			m_Conn = nullptr;
		}

		if (m_RecvT.joinable()) m_RecvT.join();

		network::Network::Shutdown();
	}

	void NetworkVM::ThreadFunc() {
		std::unique_lock<std::mutex> lock(m_MTX);
		network::Network::Initialize();
		m_Sock.wait(lock);

		bool success = true;

		if (m_Type == SocketType::SERVER) {
			m_Server = network::Socket::Create();

			if (success) {
				success = m_Server->Init(m_IP.c_str(), m_Port);
			}

			if (success) {
				success = m_Server->Bind();
			}

			if (success) {
				success = m_Server->Listen();
			}

			if (success) {
				m_Conn = m_Server->Accept();
				if (m_Conn == nullptr) {
					success = false;
				}
			}
			
			std::this_thread::sleep_for(std::chrono::seconds(1));

			if (m_Server != nullptr) {
				delete m_Server;
				m_Server = nullptr;
			}
		} else if (m_Type == SocketType::CLIENT) {
			m_Conn = network::Socket::Create();
			if (success) {
				success = m_Conn->Init(m_IP.c_str(), m_Port);
			}
			if (success) {
				success = m_Conn->Connect();
			}
			if (!success) {
				delete m_Conn;
				m_Conn = nullptr;
			}
		}

		if (success) {
			m_SendT = std::thread(&NetworkVM::SendThread, this);
			m_RecvT = std::thread(&NetworkVM::RecvThread, this);
		}

		network::Network::Shutdown();
	}

	void NetworkVM::SendThread() {
		std::unique_lock<std::mutex> lock(m_MTX);
		network::Network::Initialize();
		
		while (m_IsRunning && m_Conn != nullptr) {
			m_Send.wait(lock);
			if (m_SendBuffer == nullptr) continue;
			if (m_SendBuffer->Size() <= 0) continue;

			int bytesSent = 0;
			std::size_t messageSize = m_SendBuffer->Size();

			m_Conn->Send(&messageSize, sizeof(messageSize), bytesSent);
			if ((size_t)bytesSent != sizeof(messageSize)) {
				LOG_ERROR("Failed to send complete message size");
			}

			m_Conn->Send(m_SendBuffer->Buffer, m_SendBuffer->Size(), bytesSent);
			if ((size_t)bytesSent != messageSize) {
				LOG_ERROR("Failed to send complete message");
			}

			m_SendBuffer = new NetworkMessage(0);
		}
		
		network::Network::Shutdown();
	}

	void NetworkVM::RecvThread() {
		network::Network::Initialize();

		while (m_IsRunning && m_Conn != nullptr) {
			
			int bytesReceived = 0;
			size_t messageSize = 0;
			m_Conn->Recv((char *)&messageSize, sizeof(messageSize), bytesReceived);

			if ((size_t)bytesReceived != sizeof(messageSize)) {
				LOG_ERROR("Failed to recv complete message size");
				continue;
			}

			if (messageSize == 0) continue;

			app::utils::StrongHandle<NetworkMessage> buff = new NetworkMessage(messageSize);

			int receivedMessageSize = 0;
			while (receivedMessageSize != (int)messageSize) {
				m_Conn->Recv(buff->Buffer + receivedMessageSize, (int)messageSize - receivedMessageSize, bytesReceived);
				receivedMessageSize += bytesReceived;
			}

			{
				std::lock_guard<std::mutex> lock(m_MTX);
				m_InputBuffer.push_back(buff);
			}

			app::core::Window::Ref().ForceWindowUpdate();
		}
		network::Network::Shutdown();
	}

	void NetworkVM::Create(const char * ip, unsigned short port, SocketType type) {
		{
			std::lock_guard<std::mutex> lock(m_MTX);
			m_IP = ip;
			m_Port = port;
			m_Type = type;
		}
		m_Sock.notify_all();
	}

	bool NetworkVM::IsConnected() {
		return m_Conn != nullptr;
	}

	void NetworkVM::Send(const void * msg, std::size_t msgSize) {
		if (msgSize <= 0) return;

		{
			std::lock_guard<std::mutex> lock(m_MTX);
			m_SendBuffer = new NetworkMessage(msgSize);
			std::memcpy(m_SendBuffer->Buffer, msg, msgSize);
		}

		m_Send.notify_all();
	}

	std::vector<app::utils::StrongHandle<NetworkMessage>> NetworkVM::GetInputBuffer() {
		if (m_Conn == nullptr) return std::vector<app::utils::StrongHandle<NetworkMessage>>();
		std::lock_guard<std::mutex> lock(m_MTX);
		auto msg =  m_InputBuffer;
		m_PrevInputBuffer = m_InputBuffer;
		m_InputBuffer.resize(0);
		return msg;
	}

	void NetworkVM::LuaBindNetworkLib(lua_State * L) {
		struct LuaNetworkLibFunc {
			static int Send(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(1);
				if (lua_type(L, 1) == LUA_TSTRING) {
					LUA_STRING_PARAM(1, msg);
					NetworkVM::Ref().Send(msg, std::strlen(msg));
				} else if (lua_type(L, 1) == LUA_TUSERDATA) {
					void * data = lua_touserdata(L, 1);
					if (data == nullptr) return 0;

					std::size_t dataSize = lua_rawlen(L, 1);
					NetworkVM::Ref().Send(data, dataSize);
				}
				return 0;
			}

			static int Recv(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(0);
				std::vector<app::utils::StrongHandle<NetworkMessage>> messages = NetworkVM::Ref().GetInputBuffer();

				for (auto & msg : messages) {
					void * userData = lua_newuserdata(L, msg->Size());
					std::memcpy(userData, msg->Buffer, msg->Size());
				}

				return (int)messages.size();
			}

			static int IsConnected(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(0);
				lua_pushboolean(L, (int)NetworkVM::Ref().IsConnected());
				return 1;
			}

			static int Marshall(lua_State * L) {
				if (lua_gettop(L) <= 0) {
					return 0;
				}

				std::vector<char> buffer;
				buffer.reserve(4096);

				buffer.push_back('[');
				buffer.push_back('R');
				buffer.push_back('P');
				buffer.push_back('C');
				buffer.push_back(']');

				// Push number of params
				buffer.push_back((char)lua_gettop(L));

				for (int i = 1; i <= lua_gettop(L); i++) {
					char type = (char)lua_type(L, i);
					if (type == LUA_TSTRING) {
						const char * str = lua_tostring(L, i);
						uint32_t strSize = (uint32_t)std::strlen(str);
						if (strSize == 0) continue;

						// Push Type
						buffer.push_back(type);

						// Push Data Size
						std::size_t prevSize = buffer.size();
						buffer.resize(prevSize + sizeof(strSize));
						std::memcpy(&buffer[prevSize], (void *)(&strSize), sizeof(strSize));

						// Push Data
						prevSize = buffer.size();
						buffer.resize(prevSize + strSize);
						std::memcpy(&buffer[prevSize], str, strSize);

					} else if (type == LUA_TNUMBER) {
						float number = lua_tonumber(L, i);
						uint32_t numberSize = sizeof(number);

						// Push Type
						buffer.push_back(type);

						// Push Data Size
						std::size_t prevSize = buffer.size();
						buffer.resize(prevSize + sizeof(numberSize));
						std::memcpy(&buffer[prevSize], (void *)(&numberSize), sizeof(numberSize));

						// Push Data
						prevSize = buffer.size();
						buffer.resize(prevSize + numberSize);
						std::memcpy(&buffer[prevSize], (void *)(&number), numberSize);
					}
				}

				void * luaBuffer = lua_newuserdata(L, buffer.size());
				std::memcpy(luaBuffer, buffer.data(), buffer.size());

				return 1;
			}

			static int DeMarshall(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(1);
				
				char * buffer = (char * )lua_touserdata(L, 1);
				if (buffer == nullptr) return 0;

				std::size_t bufferSize = lua_rawlen(L, 1);
				if (bufferSize < 5) return 0;

				char header[6];
				std::memcpy(header, buffer, 5);
				header[5] = '\0';

				if (std::string(header) != "[RPC]")
					return 0;

				std::size_t bufferIndex = 5;

				// Extract number of params
				char numParams = buffer[bufferIndex++];
				unsigned int paramsPushed = 0;

				for (std::size_t i = 0; i < (std::size_t)numParams; i++) {
					// Extract type of param
					int type = (int)buffer[bufferIndex++];

					if (type == LUA_TSTRING) {
						// Extract param size
						uint32_t paramSize = (uint32_t)(buffer[bufferIndex]);
						bufferIndex += sizeof(paramSize);
						if (paramSize == 0) continue;

						// Extract params
						char * stringParam = new char[(std::size_t)paramSize + (std::size_t)1];
						std::memcpy(stringParam, &buffer[bufferIndex], paramSize);
						stringParam[paramSize] = '\0';
						bufferIndex += paramSize;

						// Push param to lua
						lua_pushstring(L, stringParam);
						delete[] stringParam;

						// Update params pushed
						paramsPushed++;
					} else if (type == LUA_TNUMBER) {
						// Extract param size
						uint32_t paramSize = (uint32_t)(buffer[bufferIndex]);
						bufferIndex += sizeof(paramSize);
						if (paramSize == 0) continue;

						// Extract params
						float numberParam = *((float *)&buffer[bufferIndex]);
						bufferIndex += paramSize;

						// Push param to lua
						lua_pushnumber(L, numberParam);

						// Update params pushed
						paramsPushed++;
					}
				}

				return paramsPushed;
			}
		};

		lua_newtable(L);

		lua_pushstring(L, "Send");
		lua_pushcfunction(L, LuaNetworkLibFunc::Send);
		lua_settable(L, -3);

		lua_pushstring(L, "Recv");
		lua_pushcfunction(L, LuaNetworkLibFunc::Recv);
		lua_settable(L, -3);

		lua_pushstring(L, "IsConnected");
		lua_pushcfunction(L, LuaNetworkLibFunc::IsConnected);
		lua_settable(L, -3);

		lua_pushstring(L, "Marshall");
		lua_pushcfunction(L, LuaNetworkLibFunc::Marshall);
		lua_settable(L, -3);

		lua_pushstring(L, "DeMarshall");
		lua_pushcfunction(L, LuaNetworkLibFunc::DeMarshall);
		lua_settable(L, -3);

		lua_setglobal(L, "Network");
	}

	void NetworkVM::Update() {

		static float prevTime = 0.0f;
		float currentTime = app::core::Timer::GetTime().GetMS();

		// Latency Check
		if (m_Conn != nullptr) {
			static constexpr const char SEND_MESSAGE[] = "LATENCY_SEND";
			static constexpr const char RECV_MESSAGE[] = "LATENCY_RECV";

			char message[sizeof(SEND_MESSAGE) + sizeof(currentTime)];

			if (currentTime - prevTime > 10) {	// Only send latency packet once per 100 ms
				// Send a packet with current time
				memcpy(message, SEND_MESSAGE, sizeof(SEND_MESSAGE));
				memcpy(message + sizeof(SEND_MESSAGE), &currentTime, sizeof(currentTime));

				// Send the packet
				Send(message, sizeof(message));
			}

			// Check if the packed is received

			{
				for (const auto & msg : m_PrevInputBuffer) {
					// In this context we are only concerned about the latency check packet
					// And the sizeof that packet must be equal to sizeof(message)
					if (msg->BufferSize != sizeof(message)) continue;

					char tag[sizeof(SEND_MESSAGE)];
					memcpy(tag, msg->Buffer, sizeof(tag));

					if (std::strcmp(tag, SEND_MESSAGE) == 0) {
						// Sent from another user
						float singleTripTime = *(float *)(msg->Buffer + sizeof(SEND_MESSAGE));

						memcpy(message, RECV_MESSAGE, sizeof(RECV_MESSAGE));
						memcpy(message + sizeof(RECV_MESSAGE), &singleTripTime, sizeof(singleTripTime));
						Send(message, sizeof(message));
					} else if (std::strcmp(tag, RECV_MESSAGE) == 0) {
						// Reply of the latency message
						float roundTripTime = currentTime - *(float *)(msg->Buffer + sizeof(SEND_MESSAGE));
						if (m_LatencyBuffer.size() >= 100) {
							m_LatencyBuffer.erase(m_LatencyBuffer.begin());
						}
						m_LatencyBuffer.push_back(roundTripTime);
					}
				}
			}

			prevTime = currentTime;
		}
	}

	void NetworkVM::Gui() {

		static int inputPort = 7454;
		static std::string inputIP = "192.168.1.105";
		bool isConnectionActive = false;

		ImGui::Begin("Network", (bool *)0, ImGuiWindowFlags_AlwaysAutoResize);

		if (m_Conn == nullptr && m_Type == SocketType::INVALID) {
			ImGui::Text("Status : IDLE");
		} else if (m_Conn == nullptr && m_Type != SocketType::INVALID && m_Server == nullptr) {
			ImGui::Text("Status : Disconnected");
		} else if (m_Conn != nullptr) {

			// Send some data to check weather is connection is active or not
			size_t messageSize = 0;
			int bytesSent = 0;
			isConnectionActive = m_Conn->Send((const void *)&messageSize, sizeof(messageSize), bytesSent);

			if (!isConnectionActive) {
				std::lock_guard<std::mutex> lock(m_MTX);
				delete m_Conn;
				m_Conn = nullptr;
			} else {
				ImGui::Text("Status : Connected [IP : %s, Port : %d]", m_Conn->GetIP().c_str(), (int)m_Conn->GetPort());
			}

		} else if (m_Server != nullptr) {
			ImGui::Text("Status : Waiting for connection on");
			ImGui::Text("[IP : %s, Port : %d]", m_IP.c_str(), (int)m_Port);
		} else {
			ImGui::Text("Status : Some error has occured, check console");
		}

		ImGui::Separator();

		if (m_Conn == nullptr && m_Type == SocketType::INVALID) {
			ImGui::InputText("IP", &inputIP);
			ImGui::InputInt("Port", &inputPort);

			if (inputPort > 65535 || inputPort < 0) {
				ImGui::PushStyleColor(0, ImVec4(1, 0, 0, 1));
				ImGui::Text("Invalid port number, must be within the range [0, 65535]");
				ImGui::PopStyleColor();
			}

			if (ImGui::Button("Open Connection")) {
				Create(inputIP.c_str(), (unsigned short)inputPort, SocketType::SERVER);
			}
			if (ImGui::Button("Connect")) {
				Create(inputIP.c_str(), (unsigned short)inputPort, SocketType::CLIENT);
			}

		}

		static float AVERAGE_LATENCY = 0.0f;

		if (m_Conn != nullptr) {
			ImGui::PlotLines("Latency Graph", 
				[](void * data, int index) -> float { 
					auto latencyBuffer = (std::list<float> *)data;
					auto it = latencyBuffer->begin();
					std::advance(it, index);

					AVERAGE_LATENCY += *it;

					return *it;
				},
				&m_LatencyBuffer, m_LatencyBuffer.size()
			);

			AVERAGE_LATENCY /= m_LatencyBuffer.size();

			ImGui::Text("Average Latency %fms", AVERAGE_LATENCY);

			AVERAGE_LATENCY = 0.0f; 

			if (ImGui::Button("Disconnect")) {
				// Probably Unsafe
				NetworkVM::DestroyInst();
				NetworkVM::CreateInst();
			}
		}

		if (m_Conn == nullptr && m_Type != SocketType::INVALID) {
			if (ImGui::Button("Restart Connection")) {
				// Probably Unsafe
				NetworkVM::DestroyInst();
				NetworkVM::CreateInst();
			}
		}

		ImGui::End();

	}

}