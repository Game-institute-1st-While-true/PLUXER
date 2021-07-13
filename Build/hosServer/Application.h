#pragma once
#include "Defines.h"
#include "GroupSystem.h"
#include "hosUtility/hosUtility.h"

using namespace hos;
using namespace com;

class EngineCallBack;
class Application
{
	SINGLETON_CLASS(Application)

private:
	EngineCallBack* CallBack;
	HANDLE ExitEvent;					// ������ ���μ��� ���Ḧ ���� �̺�Ʈ

	std::chrono::system_clock::time_point StartTime;
	std::chrono::system_clock::time_point EndTime;

public:
	bool Create();
	void Wait();
};

class EngineCallBack : public GroupSystemControlCallBack
{
private:
	HANDLE ExitEvent;

public:
	EngineCallBack(HANDLE event);
	virtual ~EngineCallBack();

public:
	virtual void OnStart() {};
	virtual bool OnExitCheck() { return true; };
	virtual void OnExit();
};