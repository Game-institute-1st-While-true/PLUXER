#ifndef _CONSOLE_H
#define _CONSOLE_H

// [2020/11/13 노종원]
// 콘솔클래스
// 윈도우에 어테치 되는 콘솔이 필요해서 만들었다

// LOG
// [2020/12/31] 노종원 : ut네임스페이스 추가

#include "InternalWindow.h"
#include "BaseTypes.h"
#include "Singleton.h"

namespace hos::ut
{
	class DebugConsole
	{
	private:
		inline static HANDLE WriteHandle = NULL;
		inline static HWND Hwnd = NULL;
	public:
		constexpr static I32 DEFAULT_WIDTH = 120;
		constexpr static I32 DEFAULT_HEIGHT = 9001;
		constexpr static U32 DEFAULT_READ_SIZE = 256;
	public:
		DebugConsole();
		virtual ~DebugConsole();
		DebugConsole(const DebugConsole&) = delete;
		DebugConsole& operator=(const DebugConsole&) = delete;
		DebugConsole(DebugConsole&&) = delete;
		DebugConsole& operator=(DebugConsole&&) = delete;

		void* operator new(std::size_t) = delete;
		void* operator new[](std::size_t) = delete;
		void operator delete(void*) = delete;
		void operator delete[](void*) = delete;

		DebugConsole& operator<<(string_view string);
		DebugConsole& operator<<(mbstring_view string);

		void Reset();
		void Show();
		void Hide();
	};

	class SubConsole : public DebugConsole
	{
	private:
		inline static HANDLE ReadHandle = NULL;
	public:
		SubConsole();
		~SubConsole();

		SubConsole& operator>>(string_view& string);
		SubConsole& operator>>(string& string);
	};
}

#endif // !_CONSOLE_H