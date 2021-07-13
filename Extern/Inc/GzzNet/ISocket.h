#pragma once
#include "NetDefine.h"
#include <string>

/// <summary>
/// [2020-11-03 김효곤]
/// 서버의 시작을 알리는 클래스.
///	Bind, StartUp, Socket 설정 등을 해주는 클래스.
/// </summary>

namespace gzz
{
	class ISocket abstract
	{
	private:
		std::string		Ip;
		unsigned short	Port;

	protected:
		SOCKET			Sock;
		SOCKADDR_IN		Addr;
		unsigned int	TimeWait;

	public:
		ISocket() { Sock = INVALID_SOCKET; Port = 0; ZeroMemory(&Addr, sizeof(SOCKADDR_IN)); };
		virtual ~ISocket() = default;

	public:
		virtual int Open(SOCKET sock) abstract;
		virtual int Bind(const char* ip, const short port) abstract;
		virtual int Close() abstract;
		virtual int ForceClose() abstract;

		virtual int SetSocketOption() abstract;
		virtual void Release() abstract;

	public:
		inline void SetIP(const char* ip) { Ip = ip; }
		inline void SetPort(const short port) { Port = port; }

		inline std::string& GetIP() { return Ip; }
		inline unsigned short GetPort() { return Port; }

		inline SOCKET GetSock() { return Sock; }
	};
}