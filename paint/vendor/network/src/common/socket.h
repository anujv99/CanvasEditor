
#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>

namespace network {

	class Socket {
	protected:
		Socket() {}
	public:
		virtual ~Socket() {}
		virtual bool Init(const char * ip, unsigned short port) = 0;
		virtual bool Bind() = 0;
		virtual bool Listen() = 0;
		virtual Socket * Accept() = 0;
		virtual bool Connect() = 0;
		virtual bool Send(const void * data, int numBytes, int & bytesSent) = 0;
		virtual bool Recv(char * buff, int buffLen, int & bytesRecevied) = 0;
		virtual std::string GetIP() const = 0;
		virtual unsigned short GetPort() const = 0;

	public:
		static Socket * Create();
	};

}

#endif //__SOCKET_H__
