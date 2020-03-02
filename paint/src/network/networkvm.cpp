#include "networkvm.h"

#include <chrono>
#include <iostream>
#include <sstream>

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
		m_SendBuffer = CONNECTION_CLOSE_MESSAGE;
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
			if (m_SendBuffer.size() <= 0) continue;
			int bytesSent = 0;
			m_Conn->Send(m_SendBuffer.c_str(), m_SendBuffer.size(), bytesSent);
			m_SendBuffer.resize(0);
		}
		
		network::Network::Shutdown();
	}

	void NetworkVM::RecvThread() {
		network::Network::Initialize();
		while (m_IsRunning) {
			char buff[1024];
			int bytesRecv = 0;
			m_Conn->Recv(buff, sizeof(buff), bytesRecv);
			buff[bytesRecv] = '\0';
			m_InputBuffer.push_back(std::string(buff));

			// -To Terminate-
			if (std::string(buff) == CONNECTION_CLOSE_MESSAGE) {
				m_Conn->Send(CONNECTION_CLOSE_MESSAGE, sizeof(CONNECTION_CLOSE_MESSAGE), bytesRecv);
				break;
			}
			// --------------
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

	void NetworkVM::Send(const std::string & msg) {
		{
			std::lock_guard<std::mutex> lock(m_MTX);
			m_SendBuffer = msg;
		}
		m_Send.notify_all();
	}

	std::vector<std::string> NetworkVM::GetInputBuffer() {
		if (m_Conn == nullptr) return std::vector<std::string>();
		std::lock_guard<std::mutex> lock(m_MTX);
		auto msg =  m_InputBuffer;
		m_InputBuffer.resize(0);
		return msg;
	}

	void NetworkVM::LuaBindNetworkLib(lua_State * L) {
		struct LuaNetworkLibFunc {
			static int Send(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(1);
				LUA_STRING_PARAM(1, msg);
				NetworkVM::Ref().Send(msg);
				return 0;
			}

			static int Recv(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(0);
				std::vector<std::string> buff = NetworkVM::Ref().GetInputBuffer();
				if (buff.size() > 0) {
					for (auto & str : buff) {
						lua_pushstring(L, str.c_str());
					}
				} else
					lua_pushstring(L, "");
				app::core::Window::Ref().ForceWindowUpdate();
				return buff.size() == 0 ? 1 : buff.size();
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

				std::string msg;
				for (int i = 1; i <= lua_gettop(L); i++) {
					msg += std::to_string(lua_type(L, i));
					const char * temp = lua_tostring(L, i);
					std::string str = temp;
					msg += str;
					msg += ";";
				}
				lua_pushstring(L, msg.c_str());
				return 1;
			}

			static int DeMarshall(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(1);
				LUA_STRING_PARAM(1, str);
				std::string msg = str;
				if (msg.length() <= 0) return 0;
				
				std::vector<std::string> strings;
				std::istringstream f(msg);
				std::string s;
				while (std::getline(f, s, ';')) {
					strings.push_back(s);
				}

				int params = 0;

				for (auto & str : strings) {
					int type = std::atoi(str.substr(0, 1).c_str());
					switch (type) {
					case LUA_TNUMBER:
						lua_pushnumber(L, std::atof(str.substr(1).c_str()));
						params++;
						break;
					case LUA_TSTRING:
						lua_pushstring(L, str.substr(1).c_str());
						params++;
						break;
					default:
						break;
					}
				}

				return params;
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