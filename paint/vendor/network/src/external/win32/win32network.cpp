#ifdef __WINDOWS__

#include "common/network.h"
#include "common/log.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace network {

	bool Network::Initialize() {
		WSADATA wsaData;
		ZeroMemory(&wsaData, sizeof(wsaData));
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0) {
			LOG("[WinSock] Failed to initialize WinSock [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
			return false;
		}
		LOG("[WinSock] WinSock successfully initialized");
		return true;
	}

	bool Network::Shutdown() {
		int result = WSACleanup();
		if (result != 0) {
			LOG("[WinSock] Failed to shutdown WinSock [Error Code : %s]", gai_strerrorA(WSAGetLastError()));
			return false;
		}
		LOG("[WinSock] Shutting down WinSock");
		return true;
	}

}

#endif //__WINDOWS__