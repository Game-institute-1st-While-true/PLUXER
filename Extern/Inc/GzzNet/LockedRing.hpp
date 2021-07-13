#pragma once
#include "RingBuffer.h"

template<typename Data>
inline LockedRing<Data>::LockedRing(const int bufsize) : Tail(0), Head(0), MaxSize(bufsize), DataBuffer(nullptr)
{
	DataBuffer = new Data[bufsize];
	memset(DataBuffer, 0, bufsize);
}

template<typename Data>
inline LockedRing<Data>::~LockedRing()
{
	if (DataBuffer)
	{
		delete[] DataBuffer;
	}
}

template<typename Data>
inline bool LockedRing<Data>::Enqueue(Data& d)
{
	std::lock_guard<std::mutex> lock(RingLock);
	if (IsFull()) return false;

	DataBuffer[Tail] = d;
	Tail = NextIndex(Tail);
	return true;
}

template<typename Data>
inline bool LockedRing<Data>::Dequeue(Data& d)
{
	std::lock_guard<std::mutex> lock(RingLock);
	if (IsEmpty()) return false;

	d = DataBuffer[Head];
	Head = NextIndex(Head);
	return true;
}

template<typename Data>
inline int LockedRing<Data>::NextIndex(const int current_index)
{
	return (current_index + 1) % MaxSize;
}

template<typename Data>
inline bool LockedRing<Data>::IsFull()
{
	return (NextIndex(Tail) == Head);
}

template<typename Data>
inline bool LockedRing<Data>::IsEmpty()
{
	return (Head == Tail);
}

template<typename Data>
inline const int LockedRing<Data>::GetTail()
{
	return Tail;
}

template<typename Data>
inline const int LockedRing<Data>::GetHead()
{
	return Head;
}

template<typename Data>
inline const int LockedRing<Data>::GetCurrentCount()
{
	return (Tail - Head + MaxSize) % MaxSize;
}

template<typename Data>
inline const int LockedRing<Data>::GetMaxSize()
{
	return MaxSize;
}