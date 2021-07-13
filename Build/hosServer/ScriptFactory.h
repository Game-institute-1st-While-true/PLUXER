#ifndef _SCRIPT_FACTORY_H
#define _SCRIPT_FACTORY_H

// [2021/02/18 노종원]
// 스크립트를 맵핑하기위한 팩토리 클래스
// 유저스크립트를 만든경우 해당 스크립트를 프리팹, 씬에 연결하기위해 이곳에 추가해야한다

#include "Defines.h"
#include "hosUtility/hosUtility.h"

namespace hos
{
	class ScriptFactory
	{
	public:
		ScriptFactory();
		~ScriptFactory();

	public:
		com::Script* Create(string_view name);
		void BindObject(com::GameObject* object);
		void BindObject(com::Scene* Scene);
	};
}

extern hos::ScriptFactory* g_Factory;

#endif // !_SCRIPT_FACTORY_H