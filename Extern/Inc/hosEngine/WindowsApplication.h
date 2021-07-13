#ifndef _APPLICATION_H
#define _APPLICATION_H

// [2020/11/27 노종원]
// Windows Application Class
// 윈도우 어플리케이션 클래스

// LOG
// [2021/03/09] 노종원 : hos네임스페이스 추가, 생성자 수정

#include "hosUtility\InternalWindow.h"
#include "hosUtility\hosUtility.h"

namespace hos
{
	class WindowsApplication
	{
	private:
		HWND H_Wnd;
		HINSTANCE H_Ins;
		string ClassName;

		inline static DWORD WS_Style = WS_OVERLAPPEDWINDOW;
		inline static RECT ClientSize = { 0,0,1920,1080 };
		inline static RECT WorkSpaceSize = { 0,0,1920,1080 };

		static inline HCURSOR Cursor = NULL;

	public:
		WindowsApplication(string_view Name = L"HOS_Window");
		~WindowsApplication();

	public:
		bool Create(HINSTANCE hInstance, int width, int height, int x = 0, int y = 0, DWORD style = WS_OVERLAPPEDWINDOW, HICON icon = NULL, HCURSOR cursor = NULL);
		void Destory();
		HRESULT Run();
		static LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		void SetName(string name);
		void SetSize(RECT rect);

		HWND GetH_Wnd()const;
		HINSTANCE GetH_Ins()const;
		string GetName()const;
		RECT GetClientSize()const;
		RECT GetSize()const;
	};
}

#endif // !_APPLICATION_H