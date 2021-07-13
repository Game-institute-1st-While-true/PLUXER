#pragma once
#include "IoDefine.h"
#include "JobQueue.h"
#include "LockedCQ.hpp"

#include "Note.h"

#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>
#include <deque>
#include <mutex>

namespace gzz
{
	class GzzIOCP_EX
	{
	private:

		int MaxIoThreadCount;
		int MaxSessionCount;

		int TimeWait;

		// message Queue
		JobQueue<PACKAGE> MessageQueue;
		LockedCQ<int>* SendItemIdPool;

		// Completion Handle
		HANDLE IocpHandle;

		// Listener
		TcpHostEX* Host;

		// Works
		std::vector<std::thread> IoWorkers;
		bool IsOnOff;
		std::atomic<int> CloseCount;

		// Session °ü¸® Pool
		std::vector<TcpSocketEX*> SessionPool;

		// Overlapped Item Pool
		std::vector<LPOVERLAPPED> SendItemPool;

		std::vector<LPOVERLAPPED> RecvItemPool;
		std::vector<LPOVERLAPPED> SessionItemPool;

	public:
		GzzIOCP_EX() = delete;
		GzzIOCP_EX(const int maxIoThreadCount, const int maxSessionCount);
		~GzzIOCP_EX();

	public:
		virtual int	 Init();
		virtual int	 Run();
		virtual int	 Stop();

		virtual int	 Open(unsigned short maxConnection, unsigned short port);
		virtual int	 Connect(const char* dst, const short port);
		virtual bool Connect(int id, const char* dst, const short port);

		virtual int	 UnExConnect(const char* dst, const short port);
		virtual bool UnExConnect(int id, const char* dst, const short port);

		virtual int	 Disconnect(const int Id);

		void		 FourceEnquePackage(PACKAGE& package);
		virtual bool GetPackage(PACKAGE& package);

		virtual int	 SendMsg(const int id, PACKET packet);

	private:
		int  CreateCompletionPort();
		void CreateIoWoker();

		int  IoFunc();

		int  DestroyThread();

		void IoCloseFlag();
		void CloseSession();

		void ReservationSession(SESSION_TYPE type, const int id, IReceive* recv, ISend* send);
		void CreateCompletionItem(const int size);

		//void ReservationSession(SESSION_TYPE type, const int id, IReceive* recv);		
		//void CreateCompletionItemPool(const int size);
	};
}
