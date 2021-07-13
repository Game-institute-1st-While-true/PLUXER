// <Concept>
// Source Name		: DebugConsole.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.02.04

// <Explanation>
// 그래픽 렌더러의 초기화를 확인하기 위한 Console Debuger 클래스.
// 싱글턴으로 작동합니다.

// <Working Log>
// 2021.02.04.Hosik	: 기존 AngD3DEnine_Mk1에서 Import.
// 2021.03.25.Hosik : message 매개변수의 대응 케이스를 확장, 파라미터를 모두 std의 string, wstring으로 교체.

#pragma once

#include "framework.h"
#include <string>
#include <iostream>

namespace hos::cg
{
	class DebugConsole
	{
	private:
		DebugConsole();		// 생성자
		~DebugConsole();	// 소멸자

	private:
		static DebugConsole* m_pInstance;	// 싱글턴 인스턴스 주소
		static FILE* stream;				// 출력스트림

	public:
		static DebugConsole* GetInstance();

		void Shutdown();

		void PrintDebug(const char* className, std::wstring message);
		void PrintDebug(const char* className, std::string message);

		void PrintDebug_TargetInfo(const char* className, std::string targetName, std::string message);
		void PrintDebug_TargetInfo(const char* className, std::string targetName, std::wstring message);

		void PrintDebug_TargetInfo(const char* className, std::wstring targetName, std::wstring message);
		void PrintDebug_TargetInfo(const char* className, std::wstring targetName, std::string message);

	private:
		void PrintTime(const char* className);

		void WcharToChar(const WCHAR* wcharStr, CHAR* dest);
		void CharToWChar(const CHAR* charStr, WCHAR* dest);
	};
}

// 디버그 콘솔
#ifdef _DEBUG
#define DEBUG_CONSOLE(message) DebugConsole::GetInstance()->PrintDebug(__FUNCTION__, message)
#define DEBUG_CONSOLET(targetName, message) DebugConsole::GetInstance()->PrintDebug_TargetInfo(__FUNCTION__, targetName, message)
#else
#define DEBUG_CONSOLE(message)
#define DEBUG_CONSOLET(targetName, message)
#endif