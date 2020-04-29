
#ifndef __NETWORK_VM_H__
#define __NETWORK_VM_H__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
#include <list>

#include <utils/singleton.h>
#include <utils/stronghandle.h>
#include <utils/handledobject.h>
#include <common/network.h>
#include <common/socket.h>

struct lua_State;

namespace paint {

	enum class SocketType {
		INVALID,
		SERVER,
		CLIENT,
	};

	struct NetworkMessage : public app::utils::HandledObject<NetworkMessage> {
		NetworkMessage(std::size_t bufferSize) : BufferSize(bufferSize), Buffer(nullptr) {
			Buffer = new char[bufferSize];
		}

		~NetworkMessage() {
			if (Buffer) {
				delete[] Buffer;
			}
		}

		std::size_t Size() { return BufferSize; }

		char * Buffer;
		std::size_t BufferSize;
	};

	class NetworkVM : public app::utils::Singleton<NetworkVM> {
		friend class app::utils::Singleton<NetworkVM>;
	private:
		NetworkVM();
		~NetworkVM();
	public:
		void Create(const char * ip, unsigned short port, SocketType type);
		bool IsConnected();

		void Send(const void * msg, std::size_t msgsize);
		std::vector<app::utils::StrongHandle<NetworkMessage>> GetInputBuffer();

		void LuaBindNetworkLib(lua_State * L);

		void Update();
		void Gui();
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
		network::Socket * m_Server;

		// Buffer
		std::vector<app::utils::StrongHandle<NetworkMessage>> m_InputBuffer;
		std::vector<app::utils::StrongHandle<NetworkMessage>> m_PrevInputBuffer; // Previous Loop Buffer
		std::list<float> m_LatencyBuffer;
		app::utils::StrongHandle<NetworkMessage> m_SendBuffer;
	};

}

#endif //__NETWORK_VM_H__
