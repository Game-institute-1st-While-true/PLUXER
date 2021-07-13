#pragma once

#include "NetDefine.h"
#include "InterfacesHead.h"
#include <mutex>

/// <summary>
///	[2020-11-03 김효곤]
/// 비동기 Send 처리를 담당하는 클래스.
/// </summary>

namespace gzz
{
#pragma region OldCode

	class AsyncTcpSend : public ISend
	{
	private:
		char* SendBuf;
		char* WriteBuf;

		int RemainSize;
		int WriteSize;
		int BufferPosition;

		std::mutex WriteLock;

		WSABUF Wsabuf;

	public:
		AsyncTcpSend();
		virtual ~AsyncTcpSend();

	public:
		virtual int SendMsg(const SOCKET& sock, const LPOVERLAPPED& overraped, PACKET& packet);
		virtual int RemainingMsg(const SOCKET& sock, const LPOVERLAPPED& overraped);
		virtual int SendProcess(const int completionBytes);
		virtual void Release();

	private:
		int SendIO(const SOCKET& sock, const LPOVERLAPPED& overraped);
		bool Write(PACKET packet);
		void SwapBuffer();
	};
#pragma endregion

	//class AsyncTcpSend : public ISend
	//{
	//private:
	//	WSABUF Wsabuf;

	//public:
	//	AsyncTcpSend();
	//	virtual ~AsyncTcpSend();

	//public:
	//	virtual int SendMsg(const SOCKET& sock, const LPOVERLAPPED& overraped, PACKET& packet);
	//	virtual int SendProcess(const int completionBytes);
	//	virtual void Release();

	//	int SendIO(const SOCKET& sock, const LPOVERLAPPED& overraped);
	//};
}