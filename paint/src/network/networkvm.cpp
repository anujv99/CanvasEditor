#include "networkvm.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>

#include <vm/lua.hpp>
#include <vm/luabind/luabind.h>
#include <core/window.h>

static constexpr const char CONNECTION_CLOSE_MESSAGE[] = "CLOSE_CONNECTION";

namespace paint {

	NetworkVM::NetworkVM() : m_IsRunning(true), m_Type(SocketType::INVALID), m_Port(0), m_Conn(nullptr) {
		network::Network::Initialize();
		m_NT = std::thread(&NetworkVM::ThreadFunc, this);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	NetworkVM::~NetworkVM() {
		m_IsRunning = false;

		m_SendBuffer = new NetworkMessage(sizeof(CONNECTION_CLOSE_MESSAGE));
		std::memcpy(m_SendBuffer->Buffer, CONNECTION_CLOSE_MESSAGE, sizeof(CONNECTION_CLOSE_MESSAGE));

		m_Send.notify_all();
		m_Sock.notify_all();
		m_NT.join();
		m_SendT.join();
		m_RecvT.join();
		if (m_Conn != nullptr) {
			delete m_Conn;
		}
		network::Network::Shutdown();
	}

	void NetworkVM::ThreadFunc() {
		std::unique_lock<std::mutex> lock(m_MTX);
		network::Network::Initialize();
		m_Sock.wait(lock);

		if (m_Type == SocketType::SERVER) {
			network::Socket * server = network::Socket::Create();
			server->Init(m_IP.c_str(), m_Port);
			server->Bind();
			server->Listen();
			m_Conn = server->Accept();
			delete server;
		} else if (m_Type == SocketType::CLIENT) {
			m_Conn = network::Socket::Create();
			m_Conn->Init(m_IP.c_str(), m_Port);
			m_Conn->Connect();
		}

		m_SendT = std::thread(&NetworkVM::SendThread, this);
		m_RecvT = std::thread(&NetworkVM::RecvThread, this);

		network::Network::Shutdown();
	}

	void NetworkVM::SendThread() {
		std::unique_lock<std::mutex> lock(m_MTX);
		network::Network::Initialize();
		
		while (m_IsRunning) {
			m_Send.wait(lock);
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

		while (m_IsRunning) {
			
			int bytesReceived = 0;
			size_t messageSize = 0;
			m_Conn->Recv((char *)&messageSize, sizeof(messageSize), bytesReceived);

			if ((size_t)bytesReceived != sizeof(messageSize)) {
				LOG_ERROR("Failed to recv complete message size");
				continue;
			}

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
				if (bufferSize == 0) return 0;

				std::size_t bufferIndex = 0;

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

}