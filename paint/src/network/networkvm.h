
#ifndef __NETWORK_VM_H__
#define __NETWORK_VM_H__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>

#include <utils/singleton.h>
#include <common/network.h>
#include <common/socket.h>

struct lua_State;

namespace paint {

	enum class SocketType {
		INVALID,
		SERVER,
		CLIENT,
	};

	class NetworkVM : public app::utils::Singleton<NetworkVM> {
		friend class app::utils::Singleton<NetworkVM>;
	private:
		NetworkVM();
		~NetworkVM();
	public:
		void Create(const char * ip, unsigned short port, SocketType type);
		bool IsConnected();

		void Send(const std::string & msg);
		std::vector<std::string> GetInputBuffer();

		void LuaBindNetworkLib(lua_State * L);
	private:
		void ThreadFunc();
		void SendThread();
		void RecvThread();
	public:
		std::thread m_NT; // Network Thread
		std::thread m_SendT, m_RecvT;
		std::mutex m_MTX;
		std::condition_variable m_Sock, m_Send;
		bool m_IsRunning;

		SocketType m_Type;
		std::string m_IP;
		unsigned short m_Port;

		network::Socket * m_Conn;

		// Buffer
		std::vector<std::string> m_InputBuffer;
		std::string m_SendBuffer;
	};

}

#endif //__NETWORK_VM_H__