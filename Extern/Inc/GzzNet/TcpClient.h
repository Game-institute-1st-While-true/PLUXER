#pragma once

#include "NetDefine.h"
#include "InterfacesHead.h"

#include "TcpSocketEX.h"
#include "AsyncTcpSend.h"
#include "AsyncTcpRecv.h"

namespace gzz
{
	class TcpClient :
		public TcpSocketEX,
		public AsyncTcpRecv,
		public AsyncTcpSend
	{
	private:
		LPOVERLAPPED RecvItem;
		HANDLE IocpHandle;
	public:
		TcpClient() = delete;
		TcpClient(int id, const char* dst, const short port, HANDLE iocpHandle, LPOVERLAPPED overLapped = nullptr);
		TcpClient(int id, HANDLE iocpHandle);
		virtual ~TcpClient();		

		virtual int  Open();
		virtual int  Bind(SOCKET sock, HANDLE iocpHandle);
		virtual void Release();

	public:
		virtual ISend* GetInterfaceSend() override;
		virtual IReceive* GetInterfaceRecv() override;

	public:
		virtual int Close(LPOVERLAPPED overlapped = nullptr);
		virtual int PostClose();
		virtual int ForceClose();
		
		int	Connect(const char* dst, const short port, LPOVERLAPPED overLapped = nullptr);

		void OnConnect();
		void OnDisconnect();
		void OnConnectFail();

		int	PostConnection();
		int	SetSocketOption();
	};
}