#pragma once

class RingBuffer
{
public:;

private:
	char* Buffer;

	int Head;
	int Tail;
	
	int MaxSize;

public:
	RingBuffer() = delete;
	RingBuffer(int size);
	~RingBuffer();

private:
	bool IsEmpty();										// 버퍼가 비어있는지 확인
	bool IsFull();										// 버퍼가 가득찼는지 확인

	int NextIndex(const int current, const int offset);	// 자신의 다음 index 확인

public:
	int Push(char* src, int size);
	int Pop(char* dst, int size);
	int Get(char* dst, int size);

	char* Front();
	char* End();

	int CurrentSize();
	int GetMax() { return MaxSize - 1; }
};

