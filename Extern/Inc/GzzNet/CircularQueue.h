#pragma once
#include <iostream>
#include <mutex>

template <typename Data>
class CircularQueue
{
private:
	unsigned int MaxSize;

	unsigned int Tail;
	unsigned int Head;

	Data* DataBuffer;

public:
	CircularQueue() = delete;
	CircularQueue(const int bufsize);
	~CircularQueue();

public:
	bool Enqueue(Data& d);
	bool Dequeue(Data& d);

private:
	bool IsFull();
	bool IsEmpty();
	int NextIndex(const int current_index);

public:
	const int GetTail();
	const int GetHead();
	const int GetCurrentCount();
	const int GetMaxSize();

	const Data* GetData();
};

// Lock이 걸리는 링버퍼
// 스레드 Safe, 성능은 싱글스레드보다 구리다.

template <typename Data>
class LockedCQ
{
private:
	unsigned int MaxSize;

	unsigned int Tail;
	unsigned int Head;

	Data* DataBuffer;

	std::mutex RingLock;

public:
	LockedCQ() = delete;
	LockedCQ(const int bufsize);
	~LockedCQ();

public:
	bool Enqueue(Data& d);
	bool Dequeue(Data& d);

private:
	bool IsFull();
	bool IsEmpty();
	int NextIndex(const int current_index);

public:
	const int GetTail();
	const int GetHead();
	const int GetCurrentCount();
	const int GetMaxSize();
};

// Lock 이 없는 링버퍼 --> Atomic
// Lock Free 기반으로 구현 하였다.

// 현재는 미구현 상태.

template <typename Data>
class LockFreeCQ
{
private:
	unsigned int MaxSize;

	unsigned int Tail;
	unsigned int Head;

	Data* DataBuffer;

public:
	LockFreeCQ() = delete;
	LockFreeCQ(const int bufsize);
	~LockFreeCQ();

public:
	bool Enqueue(Data& d);
	bool Dequeue(Data& d);

private:
	bool IsFull();
	bool IsEmpty();
	int NextIndex(const int current_index);

public:
	const int GetTail();
	const int GetHead();
	const int GetCurrentCount();
	const int GetMaxSize();

	const Data* GetData();
};