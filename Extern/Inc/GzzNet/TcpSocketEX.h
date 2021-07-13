#pragma once
#include "NetDefine.h"
#include "InterfacesHead.h"

namespace gzz
{
	class TcpSocketEX abstract :
		public ISocketEX,
		public SocketInterfaceHelper
	{
	protected:
		char ConnectionBuffer[MAX_PACKET_SIZE];
		int	 ConnectionBufSize;

		int	 Id;
		bool ConnFlag;

	public:
		TcpSocketEX() = delete;
		TcpSocketEX(int id);
		virtual ~TcpSocketEX();

	public:
		virtual int Open();
		virtual int Bind(SOCKET sock, HANDLE iocpHandle) abstract;

		virtual int Close(LPOVERLAPPED overlapped);
		virtual int PostClose();
		virtual int ForceClose();

		virtual void Release() abstract;

	public:
		int	GetId() { return Id; }
		void SetId(const int id) { Id = id; }

		bool IsConnection() { return ConnFlag; }

	protected:
		int SetKeepAlived();
		void SetNagle();
	};
}

