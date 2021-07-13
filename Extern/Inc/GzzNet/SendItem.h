#pragma once
#define WIN32_LEAN_AND_MEAN

#include "TcpSocketEX.h"
#include "ISend.h"
#include "CompletionItemDefine.h"

#include "LockedCQ.hpp"

namespace gzz
{
#pragma region Old Send Item
	//class SendItem :
	//	public ICompletionItem
	//{
	//private:
	//	TcpSocket* Token;
	//	ISend* Item;

	//public:
	//	SendItem() = delete;
	//	SendItem(TcpSocket* sock, ISend* item) { Token = sock; Item = item; };
	//	virtual ~SendItem();
	//public:
	//	virtual int SendMsg(PACKET pack);
	//	virtual int Process(const int completionBytes);
	//	virtual bool Switching() { return true; };
	//};


	//class SendItemEX :
	//	public ICompletionItemEX
	//{
	//protected:
	//	TcpSocketEX* Socket;
	//	ISend* Send;

	//public:
	//	SendItemEX() = delete;
	//	SendItemEX(int id);
	//	SendItemEX(int id, TcpSocketEX* sock, ISend* item);
	//	virtual ~SendItemEX();

	//public:
	//	virtual int SendMsg(PACKET pack);
	//	void SettingItem(TcpSocketEX* sock, ISend* item);

	//	virtual int Process(const int completionBytes, LPOVERLAPPED other);
	//};
#pragma endregion
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class SendItemEX :
		public ICompletionItemEX
	{
	protected:
		TcpSocketEX* Socket;
		ISend* Send;

	public:
		SendItemEX() = delete;
		SendItemEX(int id);
		SendItemEX(int id, TcpSocketEX* sock, ISend* item);
		virtual ~SendItemEX();
	public:
		virtual int SendMsg(PACKET pack);
		void SettingItem(TcpSocketEX* sock, ISend* item);

		virtual int Process(const int completionBytes, LPOVERLAPPED other);
	};
}