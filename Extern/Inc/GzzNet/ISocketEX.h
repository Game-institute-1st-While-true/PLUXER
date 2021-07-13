#pragma once
#define WIN32_LEAN_AND_MEAN

#include "NetDefine.h"
#include <string>

namespace gzz
{
	class ISocketEX abstract
	{
	protected:
		std::string		Ip;
		unsigned short	Port;
		SOCKET			Sock;
		SOCKADDR_IN		Addr;

	public:
		ISocketEX() { Sock = INVALID_SOCKET; Port = 0; ZeroMemory(&Addr, sizeof(SOCKADDR_IN)); };
		virtual ~ISocketEX() = default;

	public:
		virtual int Open() abstract;
		virtual int Bind(SOCKET sock, HANDLE iocpHandle) abstract;
		virtual int Close(LPOVERLAPPED overlapped) abstract;
		virtual int ForceClose() abstract;
		virtual void Release() abstract;

	public:
		inline void SetIP(const char* ip) { Ip = ip; }
		inline void SetPort(const short port) { Port = port; }

		inline std::string& GetIP() { return Ip; }
		inline unsigned short GetPort() { return Port; }

		inline SOCKET GetSock() { return Sock; }
	};
}