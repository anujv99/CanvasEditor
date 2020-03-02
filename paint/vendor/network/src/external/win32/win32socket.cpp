#ifdef __WINDOWS__

#include <WS2tcpip.h>
#include <string>

#include "win32socket.h"

#include "common/log.h"

#define CHECK_HANDLE() if (m_Handle == INVALID_SOCKET) { LOG("[WinSock] Invalid socket handle. Did you forget to call Socket::Init()"); return false; }

namespace network {

	Socket * Socket::Create() {
		return new win32::Win32Socket();
	}

	namespace win32 {

		Win32Socket::Win32Socket() : m_Handle(INVALID_SOCKET), m_Info(nullptr) {}

		Win32Socket::Win32Socket(SOCKET handle) : m_Handle(INVALID_SOCKET), m_Info(nullptr) {
			if (handle != INVALID_SOCKET) {
				m_Handle = handle;
			}
		}

		Win32Socket::~Win32Socket() {
			if (m_Info != nullptr) {
				freeaddrinfo(m_Info);
				m_Info = nullptr;
			}

			if (m_Handle != INVALID_SOCKET) {
				shutdown(m_Handle, SD_BOTH);
				closesocket(m_Handle);
				m_Handle = INVALID_SOCKET;
				LOG("[WinSock] Socket successfully closed");
			}
		}

		bool Win32Socket::Init(const char * ip, unsigned short port) {
			addrinfo hints;

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			int result = 0;

			result = getaddrinfo(ip, std::to_string(port).c_str(), &hints, &m_Info);
			if (result != 0) {
				LOG("[WinSock] Failed to get addrinfo [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
				return false;
			}

			m_Handle = socket(m_Info->ai_family, m_Info->ai_socktype, m_Info->ai_protocol);
			if (m_Handle == INVALID_SOCKET) {
				LOG("[WinSock] Failed to create server socket [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
				return false;
			}

			BOOL socketDelay = TRUE;

			result = setsockopt(m_Handle, m_Info->ai_protocol, TCP_NODELAY, (const char *)&socketDelay, sizeof(socketDelay));
			if (result != 0) {
				LOG("[WinSock] Failed to set socket option [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
			}

			LOG("[WinSock] socket successfully created. IP : %s, Port : %d", ip, (int)port);
			return true;
		}

		bool Win32Socket::Bind() {
			CHECK_HANDLE();
			int result = bind(m_Handle, m_Info->ai_addr, (int)m_Info->ai_addrlen);
			if (result != 0) {
				LOG("[WinSock] Failed to bind server socket [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
				return false;
			}
			return true;
		}

		bool Win32Socket::Listen() {
			CHECK_HANDLE();
			int result = listen(m_Handle, SOMAXCONN);
			if (result == SOCKET_ERROR) {
				LOG("[WinSock] listen failed [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
				return false;
			}
			return true;
		}

		Socket * Win32Socket::Accept() {
			if (m_Handle == INVALID_SOCKET) { LOG("[WinSock] Invalid socket handle. Did you forget to call Socket::Create()"); return nullptr; }
			SOCKET clientHandle = INVALID_SOCKET;
			clientHandle = accept(m_Handle, nullptr, nullptr);
			if (clientHandle == INVALID_SOCKET) {
				LOG("[WinSock] accept failed [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
				return nullptr;
			}

			LOG("[WinSock] Successfully accepted connection");

			sockaddr addr;
			int size = sizeof(addr);
			int result = getpeername(clientHandle, &addr, &size);
			if (result != 0) {
				LOG("[WinSock] Failed to get connection info [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
			} else {
				char ip[16];
				inet_ntop(AF_INET, &((sockaddr_in *)&addr)->sin_addr, ip, sizeof(ip));
				LOG("[WinSock] IP : %s", ip);
			}

			return new Win32Socket(clientHandle);
		}

		bool Win32Socket::Connect() {
			CHECK_HANDLE();
			int result = connect(m_Handle, m_Info->ai_addr, (int)m_Info->ai_addrlen);
			if (result == SOCKET_ERROR) {
				LOG("[WinSock] Failed to connect socket [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
				return false;
			}
			return true;
		}

		bool Win32Socket::Send(const void * data, int numBytes, int & bytesSent) {
			CHECK_HANDLE();
			int sent = send(m_Handle, (const char *)data, numBytes, 0);
			if (sent == SOCKET_ERROR) {
				LOG("[WinSock] Failed to send data [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
				return false;
			}
			bytesSent = sent;
			return true;
		}

		bool Win32Socket::Recv(char * buff, int buffLen, int & bytesRecevied) {
			CHECK_HANDLE();
			int received = recv(m_Handle, buff, buffLen, 0);
			if (received == SOCKET_ERROR) {
				LOG("[WinSock] Failed to receive data [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
				return false;
			}
			bytesRecevied = received;
			return true;
		}

	}

}

#endif //__WINDOWS__