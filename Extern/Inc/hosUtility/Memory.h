#ifndef _MEMORY_H
#define _MEMORY_H

// [2021/1/7 노종원]
// 메모리관리를 위한 헤더

// LOG

namespace hos
{
	template<class T>
	__forceinline void SafeRelease(T*& t)
	{
		if (t)
		{
			t->release();
			t = nullptr;
		}
	}

	template<class T>
	__forceinline void DxSafeRelease(T*& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}

	template<class T>
	__forceinline void SafeDelete(T*& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}

	template<class T>
	__forceinline void SafeDeleteArray(T*& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}
}

#endif // !_MEMORY_H