#ifdef __UNIX__

#include <cstring>
#include <string>

#include "unixsocket.h"
#include "common/log.h"

#include <errno.h>
#include <arpa/inet.h>

#define INVALID_SOCKET -1

#define CHECK_HANDLE() if (m_Handle == INVALID_SOCKET) { LOG("[UnixSocket] Invalid socket handle. Did you forget to call Socket::Init()"); return false; }

namespace network {

	Socket * Socket::Create() {
		return new unix_linux::UnixSocket();
	}

	namespace unix_linux {

		UnixSocket::UnixSocket() : m_Handle(INVALID_SOCKET), m_Info(nullptr) {}

		UnixSocket::UnixSocket(SOCKET handle) : m_Handle(INVALID_SOCKET), m_Info(nullptr) {
			if (handle != INVALID_SOCKET) {
				m_Handle = handle;
			}
		}

		UnixSocket::~UnixSocket() {
			
		}

		bool UnixSocket::Init(const char * ip, unsigned short port) {
			addrinfo hints;

			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			int result = 0;

			result = getaddrinfo(ip, std::to_string(port).c_str(), &hints, &m_Info);
			if (result != 0) {
				LOG("[UnixSocket] Failed to get addrinfo [Error Code : %s]", gai_strerror(result));
				return false;
			}

			m_Handle = socket(m_Info->ai_family, m_Info->ai_socktype, m_Info->ai_protocol);
			if (m_Handle == INVALID_SOCKET) {
				LOG("[UnixSocket] Failed to create server [Error Code : %s]", strerror(errno));
				return false;
			}

			int socketDelay = 1;

			result = setsockopt(m_Handle, m_Info->ai_protocol, TCP_NODELAY, (const void *)&socketDelay, sizeof(socketDelay));
			if (result != 0) {
				LOG("[UnixSocket] Failed to set socket option [Error Code : %s]", strerror(errno));
			}

			LOG("[UnixSocket] socket successfully created. IP : %s, Port : %d", ip, (int)port);
			return true;
		}

		bool UnixSocket::Bind() {
			CHECK_HANDLE();
			int result = bind(m_Handle, m_Info->ai_addr, (int)m_Info->ai_addrlen);
			if (result != 0) {
				LOG("[UnixSocket] Failed to bind socket [Error Code : %s]", strerror(result));
				return false;
			}
			return true;
		}

		bool UnixSocket::Listen() {
			CHECK_HANDLE();
			int result = listen(m_Handle, SOMAXCONN);
			if (result != 0) {
				LOG("[UnixSocket] listen failed [Error Code : %s]", strerror(errno));
				return false;
			}
			return true;
		}

		Socket * UnixSocket::Accept() {
			if (m_Handle == INVALID_SOCKET) { LOG("[UnixSock] Invalid socket handle. Did you forget to call Socket::Init()"); return nullptr; }
			SOCKET clientHandle = INVALID_SOCKET;
			clientHandle = accept(m_Handle, nullptr, nullptr);
			if (clientHandle == INVALID_SOCKET) {
				LOG("[UnixSocket] accept failed [Error Code : %s]", strerror(errno));
				return nullptr;
			}

			LOG("[UnixSocket] Successfully accepted connection");

			sockaddr addr;
			socklen_t size = (socklen_t)sizeof(addr);
			int result = getpeername(clientHandle, &addr, &size);
			if (result != 0) {
				LOG("[UnixSocket] Failed to get connection info [Error Code : %s]", strerror(errno));
			} else {
				char ip[16];
				inet_ntop(AF_INET, &((sockaddr_in *)&addr)->sin_addr, ip, sizeof(ip));
				LOG("[UnixSocket] IP : %s", ip);
			}

			return new UnixSocket(clientHandle);
		}

		bool UnixSocket::Connect() {
			CHECK_HANDLE();
			int result = connect(m_Handle, m_Info->ai_addr, (int)m_Info->ai_addrlen);
			if (result != 0) {
				LOG("[UnixSocket] Failed to connect socket [Error Code : %s]", strerror(errno));
				return false;
			}
			return true;
		}

		bool UnixSocket::Send(const void * data, int numBytes, int & bytesSent) {
			CHECK_HANDLE();
			int sent = send(m_Handle, (const char *)data, numBytes, 0);
			if (sent == -1) {
				LOG("[UnixSocket] Failed to send data [Error Code : %s]", strerror(errno));
				return false;
			}
			bytesSent = sent;
			return true;
		}

		bool UnixSocket::Recv(char * buff, int buffLen, int & bytesRecevied) {
			CHECK_HANDLE();
			int received = recv(m_Handle, buff, buffLen, 0);
			if (received == -1) {
				LOG("[UnixSocket] Failed to receive data [Error Code : %s]", strerror(errno));
				return false;
			}
			bytesRecevied = received;
			return true;
		}

		std::string UnixSocket::GetIP() const {
			CHECK_HANDLE();

			sockaddr addr;
			socklen_t size = (socklen_t)sizeof(addr);
			int result = getpeername(m_Handle, &addr, &size);
			if (result != 0) {
				LOG("[UnixSocket] Failed to get connection ip [Error Code : %s]", strerror(errno));
				return "";
			} else {
				char ip[16];
				inet_ntop(AF_INET, &((sockaddr_in *)&addr)->sin_addr, ip, sizeof(ip));
				return std::string(ip);
			}

		}

		unsigned short UnixSocket::GetPort() const {
			CHECK_HANDLE();

			sockaddr addr;
			socklen_t size = (socklen_t)sizeof(addr);
			int result = getpeername(m_Handle, &addr, &size);
			if (result != 0) {
				LOG("[UnixSocket] Failed to get connection port [Error Code : %s]", strerror(errno));
				return 0;
			} else {
				char ip[16];
				inet_ntop(AF_INET, &((sockaddr_in *)&addr)->sin_addr, ip, sizeof(ip));
				return ((sockaddr_in *)&addr)->sin_port;
			}

		}
	}

}

#endif //__UNIX__