#pragma once
#include "RingBuffer.h"

template<typename Data>
inline LockFreeRing<Data>::LockFreeRing(const int bufsize) : Tail(0), Head(0), MaxSize(bufsize), DataBuffer(nullptr)
{
	DataBuffer = new Data[bufsize];
	memset(DataBuffer, 0, bufsize);
}

template<typename Data>
inline LockFreeRing<Data>::~LockFreeRing()
{
	if (DataBuffer)
	{
		delete[] DataBuffer;
	}
}

template<typename Data>
inline bool LockFreeRing<Data>::Enqueue(Data& d)
{
	if (IsFull()) return false;

	DataBuffer[Tail] = d;
	Tail = NextIndex(Tail);
	return true;
}

template<typename Data>
inline bool LockFreeRing<Data>::Dequeue(Data& d)
{
	if (IsEmpty()) return false;

	d = DataBuffer[Head];
	Head = NextIndex(Head);
	return true;
}

template<typename Data>
inline int LockFreeRing<Data>::NextIndex(const int current_index)
{
	return (current_index + 1) % MaxSize;
}

template<typename Data>
inline bool LockFreeRing<Data>::IsFull()
{
	return (NextIndex(Tail) == Head);
}

template<typename Data>
inline bool LockFreeRing<Data>::IsEmpty()
{
	return (Head == Tail);
}

template<typename Data>
inline const int LockFreeRing<Data>::GetTail()
{
	return Tail;
}

template<typename Data>
inline const int LockFreeRing<Data>::GetHead()
{
	return Head;
}

template<typename Data>
inline const int LockFreeRing<Data>::GetCurrentCount()
{
	return (Tail - Head + MaxSize) % MaxSize;
}

template<typename Data>
inline const int LockFreeRing<Data>::GetMaxSize()
{
	return MaxSize;
}

template<typename Data>
inline const Data* LockFreeRing<Data>::GetData()
{
	return DataBuffer;
}
