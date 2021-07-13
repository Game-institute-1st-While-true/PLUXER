#pragma once
#include "NetDefine.h"
#include "InterfacesHead.h"
#include "TcpSocketEX.h"
#include "AsyncTcpSend.h"
#include "AsyncTcpRecv.h"

namespace gzz
{
	class TcpClientEX :
		public TcpSocketEX,
		public AsyncTcpRecv,
		public AsyncTcpSend
	{
	public:
		TcpClientEX() = delete;
		TcpClientEX(int id, const char* dst, const short port, HANDLE iocpHandle, LPOVERLAPPED overLapped);
		TcpClientEX(int id, HANDLE iocpHandle);
		virtual ~TcpClientEX();

		virtual int  Open();
		virtual int  Bind(SOCKET sock, HANDLE iocpHandle);
		virtual void Release();

	public:
		virtual ISend* GetInterfaceSend() override;
		virtual IReceive* GetInterfaceRecv() override;

	public:
		int	Connect(const char* dst, const short port, LPOVERLAPPED overLapped);
		int	PostConnection();
		int	SetSocketOption();


	private:
	};
}