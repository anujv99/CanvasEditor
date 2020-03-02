
#ifndef __WIN32_SOCKET_H__
#define __WIN32_SOCKET_H__

#ifdef __WINDOWS__

#include <WinSock2.h>

#include "common/socket.h"

namespace network { namespace win32 {

	class Win32Socket : public Socket {
	public:
		Win32Socket();
		Win32Socket(SOCKET handle);
		~Win32Socket();
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

#endif //__WINDOWS__

#endif //__WIN32_SOCKET_H__
