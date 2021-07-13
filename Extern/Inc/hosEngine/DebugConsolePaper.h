#pragma once
#include <mutex>
#include "../hosUtility/hosUtility.h"

// [2021/01/25 김효곤]
// 먼저 어쩌다보니.. 네트워크 로거와 엔진 로거가 분리되었다. (내가 만들고있었는데 자꾸 다른사람이 또만들어서 삐졌었음.)
// 인터페이스를 이용하여 Note에 Page를 추가시키자.

// Logger 클래스와 Output을 함께 사용한다.

using namespace hos::ut;

class DebugConsolePaper : public IPaper
{
private:

public:
	DebugConsolePaper() = default;
	virtual ~DebugConsolePaper() = default;

public:
	virtual void Print(std::wstring text)
	{
		Logger::GetIns()->OutSideLog(text);
	}
};