#pragma once
#include <deque>
#include <mutex>
#include "NetDefine.h"
#include "IoDefine.h"

namespace gzz
{
	class PackageQueue
	{
	private:
		std::deque<PACKAGE>* ReadQueue;
		std::deque<PACKAGE>* WriteQueue;

		std::recursive_mutex Lock;

	public:
		PackageQueue();
		~PackageQueue();

	public:
		bool Safe_Enqueue(PACKAGE& pack);
		bool Safe_Dequeue(PACKAGE& pack);
		bool Enqueue(PACKAGE& pack);
		bool Dequeue(PACKAGE& pack);
		void Swap();
	};
}