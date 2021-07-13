#pragma once
#define WIN32_LEAN_AND_MEAN
#include <functional>
#include <mutex>	

#include "CompletionItemDefine.h"
#include "TcpSocketEX.h"
#include "TcpGuestEX.h"
#include "TcpClientEX.h"

#include "RecvItem.h"

namespace gzz
{
	enum class SESSION_TYPE : int
	{
		NONE,
		ACCEPT,
		CONNECT,
	};

	class SessionItem :
		public ICompletionItemEX
	{
	private:
		SESSION_TYPE SessionType;
		std::function<int(SessionItem&, int)> SessionFunc;
		SOCKET ListenSock;
		TcpSocketEX* Socket;
		LPOVERLAPPED RecvItem;

	public:
		SessionItem() = delete;
		SessionItem(int id);
		SessionItem(int id, SESSION_TYPE type, SOCKET listener, LPOVERLAPPED recvItem, TcpSocketEX* sock);
		virtual ~SessionItem();

	public:
		virtual int	 Process(int completionBytes, LPOVERLAPPED other);

	public:
		void SettingItem(SESSION_TYPE type, SOCKET listener, LPOVERLAPPED recvItem, TcpSocketEX* sock);

	private:
		int AcceptProcess(int completionBytes);
		int ConnectProcess(int completionBytes);

		void SwitchingConnected();
		void SwitchingDisconnected();

		int Disconnect_Accept(int completionBytes);
		int Disconnect_Connect(int completionBytes);
	};
}
