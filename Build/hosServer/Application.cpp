#include "Application.h"
#include "ScriptFactory.h"

Application::Application() : CallBack(nullptr), ExitEvent(NULL)
{
}

Application::~Application()
{
	SafeDelete(CallBack);
	SafeDelete(System);
	SafeDelete(g_Factory);
}

bool Application::Create()
{
	g_Factory = new ScriptFactory();
	g_DataManager = new DataManager();
	std::chrono::duration<double> ResultTime;

	ExitEvent = CreateEvent(NULL, true, false, nullptr);
	CallBack = new EngineCallBack(ExitEvent);
	System = new GroupSystem(CallBack);
	
	System->Load();
	StartTime = std::chrono::system_clock::now();
	System->Init(MAX_SESSION_COUNT);
	EndTime = std::chrono::system_clock::now();

	ResultTime = EndTime - StartTime;
	NetLogger::GetIns()->Write(L"system", L"Initialize Time : ", ResultTime.count());

	System->Start(LISTEN_PORT);
	return true;
}

void Application::Wait()
{
	WaitForSingleObject(ExitEvent, INFINITE);
}

EngineCallBack::EngineCallBack(HANDLE event) :
	ExitEvent(event)
{
}

EngineCallBack::~EngineCallBack()
{
}

void EngineCallBack::OnExit()
{
	SetEvent(ExitEvent);
}
