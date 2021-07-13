#include "ScriptFactory.h"

#include "Lobby.h"
#include "BossRoom.h"

#include "Player.h"
#include "PlayerState.h"	
#include "PlayerMove.h"

#include "BossFrog.h"
#include "BossFrogState.h"

#include "NoteManager.h"

//아래에 유저스크립트 추가

using namespace hos;
hos::ScriptFactory* g_Factory = nullptr;

constexpr string_literal Script[] = {
		L"LobbyScene",
		L"BossScene",

		L"Player",
		L"PlayerState",
		L"PlayerMove",

		L"BossFrog",
		L"BossFrogState",

		L"NoteManager"
};

//Script Name Helper
constexpr U32 SNH(string_view name)
{
	for (U32 i = 0; i < sizeof(Script); i++)
	{
		if (name.compare(Script[i]) == 0)
			//Script[i] == name.data())
		{
			return i;
		}
	}
	return -1;
}

hos::ScriptFactory::ScriptFactory()
{
}

hos::ScriptFactory::~ScriptFactory()
{
}

com::Script* hos::ScriptFactory::Create(string_view name)
{
	switch (SNH(name))
	{
	case SNH(L"LobbyScene"):
		return nullptr;				// return new Lobby();
	case SNH(L"BossScene"):
		return nullptr;				// return new GameRoom();	
	
	case SNH(L"Player"):
		return new Player();
	case SNH(L"PlayerMove"):
		return new PlayerMove();
	case SNH(L"PlayerState"):
		return new PlayerState();
	case SNH(L"BossFrog"):
		return new BossFrog();
	case SNH(L"BossFrogState"):
		return new BossFrogState();
	case SNH(L"NoteManager"):
		return new NoteManager();
	}
	return nullptr;
}

void hos::ScriptFactory::BindObject(com::GameObject* object)
{
	std::vector<com::Script*> RemoveScriptComponent;

	com::Script* script;
	for (int i = 0; i < object->Components.size(); i++)
	{
		script = dynamic_cast<com::Script*>(object->Components[i]);
		if (script)
		{
			RemoveScriptComponent.push_back(script);
		}
	}
	int _ScriptCount = static_cast<int>(RemoveScriptComponent.size());

	for (int i = 0; i < _ScriptCount; i++)
	{
		com::Script* newScript = Create(RemoveScriptComponent[i]->GetScriptName());
		object->AddComponent(newScript);
	}
	for (auto _iter : RemoveScriptComponent)
	{
		object->RemoveComponent(_iter);
		SafeDelete(_iter);
	}
}

void hos::ScriptFactory::BindObject(com::Scene* Scene)
{
	std::vector<com::GameObject*> objs = Scene->GetObjects();
	for (com::GameObject* obj : objs)
	{
		BindObject(obj);
	}
}
