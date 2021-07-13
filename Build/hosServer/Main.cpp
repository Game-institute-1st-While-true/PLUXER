#include "Application.h"
#include "hosUtility\hosMath.h"
#include "hosUtility\Dump.h"

#include <Windows.h>

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(136111);
#endif
	Dump minidump(L"minidump");

	Application::GetIns()->Create();
	Application::GetIns()->Wait();
	Application::GetIns()->Destory();

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}