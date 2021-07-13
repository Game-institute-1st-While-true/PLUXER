#pragma once
#include <mutex>
#include <deque>

namespace gzz
{
	template<typename T>
	class JobQueue
	{
	private:
		enum {READ = 0, WRITE = 1};
		//std::array<std::deque<T>*, 2> QueueArray;

		std::deque<T>* ReadQueue;
		std::deque<T>* WriteQueue;

		std::recursive_mutex Lock;

	public:
		JobQueue();
		~JobQueue();
	public:
		bool Enqueue(T& job);
		bool Dequeue(T& job);
		void Swap();
		int Size();

	};
	template<typename T>
	inline JobQueue<T>::JobQueue()
	{
		ReadQueue = new std::deque<T>();
		WriteQueue = new std::deque<T>();

		//QueueArray[JobQueue::READQUEUE] = ReadQueue;
		//QueueArray[JobQueue::WRITE] = WriteQueue;
	}

	template<typename T>
	inline JobQueue<T>::~JobQueue()
	{
		ReadQueue->clear();
		WriteQueue->clear();
		delete ReadQueue;
		delete WriteQueue;
	}

	template<typename T>
	inline bool JobQueue<T>::Enqueue(T& job)
	{
		std::scoped_lock<std::recursive_mutex> lock(Lock);
		WriteQueue->push_back(job);
		return true;
	}
	template<typename T>
	inline bool JobQueue<T>::Dequeue(T& job)
	{
		std::scoped_lock<std::recursive_mutex> lock(Lock);
		if (ReadQueue->empty() == true)
		{
			if (WriteQueue->empty() == true)
			{
				return false;
			}
			Swap();
		}
		job = ReadQueue->front();
		ReadQueue->pop_front();
		return true;
	}
	template<typename T>
	inline void JobQueue<T>::Swap()
	{
		std::scoped_lock<std::recursive_mutex> a(Lock);
		std::deque<T>* tempQueue = ReadQueue;
		ReadQueue = WriteQueue;
		WriteQueue = tempQueue;
	}

	template<typename T>
	inline int JobQueue<T>::Size()
	{
		std::scoped_lock<std::recursive_mutex> aa(Lock);
		return static_cast<int>(ReadQueue->size() + WriteQueue->size());
	}
}