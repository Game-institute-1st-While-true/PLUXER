#pragma once
#include "NetDefine.h"
#include "InterfacesHead.h"
#include "TcpSocketEX.h"
#include "AsyncTcpSend.h"
#include "AsyncTcpRecv.h"

namespace gzz
{
	class TcpGuestEX :
		public TcpSocketEX,
		public AsyncTcpRecv,
		public AsyncTcpSend
	{
	public:
		TcpGuestEX() = delete;
		TcpGuestEX(int id, SOCKET listen, HANDLE iocpHandle, LPOVERLAPPED overLapped);
		virtual ~TcpGuestEX();

	public:
		virtual int Open();
		virtual int Bind(SOCKET sock, HANDLE iocpHandle);
		virtual void Release();

	public:
		virtual ISend* GetInterfaceSend() override;
		virtual IReceive* GetInterfaceRecv() override;

	public:
		int Accept(SOCKET listen, LPOVERLAPPED overLapped);
		int PostAcception();
		int SetSocketOption(SOCKET listen);
	private:
	};
}