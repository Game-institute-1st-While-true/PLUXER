#ifndef _WINDOWS_INPUT_H
#define _WINDOWS_INPUT_H

// [2021/02/24 노종원]
// 인풋 클래스

// LOG

#include "DXTK/DirectXTK.h"
#include "hosUtility/hosUtility.h"
#include "KeyCode.h"

namespace hos
{
	class WindowsInput
	{
	public:
		enum class eMouseMode
		{
			NONE,
			LOCKED,
			CONFINED
		};

	private:
		DirectX::GamePad* gpDevice;
		DirectX::Keyboard* kbDevice;
		DirectX::Mouse* mDevice;

		DirectX::GamePad::ButtonStateTracker* gpTracker;
		DirectX::Keyboard::KeyboardStateTracker* kbTracker;
		DirectX::Mouse::ButtonStateTracker* mTracker;
		eMouseMode MouseMode;
		HWND H_Wnd;
		RECT rect;

	public:
		WindowsInput(HWND hWnd);
		~WindowsInput();

	public:
		void Update();

		bool AnyKey();
		bool AnyKeyDown();
		bool AnyKeyUp();
		bool GetKey(KeyCode key);
		bool GetKeyDown(KeyCode key);
		bool GetKeyUp(KeyCode key);
		float GetAxis(Axis axis);
		KeyCode GetInputKey();

		//toggle keys
		bool IsNumLock();
		bool IsCapsLock();
		bool IsScrLock();
		bool IsHangul();
		bool IsShift();
		bool IsLShift();
		bool IsRShift();
		bool IsCtrl();
		bool IsLCtrl();
		bool IsRCtrl();
		bool IsAlt();
		bool IsLAlt();
		bool IsRAlt();

		//mouse
		POINT GetMousePosition()const;
		void SetMouseCursor(HCURSOR cur);
		void SetMouseVisible(bool b);
		eMouseMode GetMouseMode()const;
		void SetMouseMode(eMouseMode e);
	};
}

extern hos::WindowsInput* Input;

#endif // !_WINDOWS_INPUT_H