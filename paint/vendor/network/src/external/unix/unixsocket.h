#ifndef __UNIX_SOCKET_H__
#define __UNIX_SOCKET_H__

#ifdef __UNIX__

#include <sys/socket.h>	
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "common/socket.h"

typedef int SOCKET;

namespace network { namespace unix_linux {

	class UnixSocket : public Socket {
	public:
		UnixSocket();
		UnixSocket(SOCKET handle);
		~UnixSocket();
	public:
		virtual bool Init(const char * ip, unsigned short port) override;
		virtual bool Bind() override;
		virtual bool Listen() override;
		virtual Socket * Accept() override;
		virtual bool Connect() override;
		virtual bool Send(const void * data, int numBytes, int & bytesSent) override;
		virtual bool Recv(char * buff, int buffLen, int & bytesRecevied) override;
	private:
		SOCKET m_Handle;
		addrinfo * m_Info;
	};

} }

#endif //__UNIX__

#endif //__UNIX_SOCKET_H__




