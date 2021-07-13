#include "ScriptFactory.h"

//아래에 유저스크립트 추가
#include "UserScriptList.h"

using namespace hos;

constexpr string_literal Script[] = {
		L"CameraMove",
		L"GameManager",
		L"EnemyManager",
		L"NoteManager",
		L"UIManager",
		L"ProjectileManager",
		L"ToxicManager",
		L"PlayerState",
		L"PlayerAction",
		L"PlayerMove",
		L"PlayerAttack",
		L"PlayerLookEnemy",
		L"ShieldFrogState",
		L"ShieldFrogAction",
		L"ShieldFrogMeleeDetectZone",
		L"ShieldFrogRangeDetectZone",
		L"MeleeFrog",
		L"MeleeFrogState",
		L"MeleeFrogAction",
		L"MeleeFrogAttackArea",
		L"BossFrogState",
		L"BossFrogAction",
		L"BossFrogAttackArea",
		L"RangeFrogState",
		L"RangeFrogAction",
		L"RangeFrogAttackArea",
		L"Projectile",
		L"ToxicFloor",
		L"StageManager",
		L"StageInCheck",
		L"TestSystem",
		L"LobbyEventSystem",
		L"UIManager",
		L"ArcadeNetworkEvent",
		L"ArcadePlayer",
		L"Portal"
};

//Script Name Helper
constexpr U32 SNH(string_view name)
{
	for (U32 i = 0; i < sizeof(Script); i++)
	{
		if (name.compare(Script[i]) == 0)
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
	case SNH(L"PlayerState"):
		return new PlayerState();
		break;
	case SNH(L"PlayerAction"):
		return new PlayerAction();
		break;
	case SNH(L"PlayerMove"):
		return new PlayerMove();
		break;
	case SNH(L"PlayerAttack"):
		return new PlayerAttack();
		break;
	case SNH(L"PlayerLookEnemy"):
		return new PlayerLookEnemy();
		break;
	case SNH(L"CameraMove"):
		return new CameraMove();
		break;
	case SNH(L"TestSystem"):
		return new TestSystem();
		break;
	case SNH(L"ShieldFrogState"):
		return new ShieldFrogState();
		break;
	case SNH(L"ShieldFrogAction"):
		return new ShieldFrogAction();
		break;
	case SNH(L"ShieldFrogMeleeDetectZone"):
		return new ShieldFrogMeleeDetectZone();
		break;
	case SNH(L"ShieldFrogRangeDetectZone"):
		return new ShieldFrogRangeDetectZone();
		break;
	case SNH(L"MeleeFrog"):
		return new MeleeFrog();
		break;
	case SNH(L"MeleeFrogState"):
		return new MeleeFrogState();
		break;
	case SNH(L"MeleeFrogAction"):
		return new MeleeFrogAction();
		break;
	case SNH(L"MeleeFrogAttackArea"):
		return new MeleeFrogAttackArea();
		break;
	case SNH(L"BossFrogState"):
		return new BossFrogState();
		break;
	case SNH(L"BossFrogAction"):
		return new BossFrogAction();
		break;
	case SNH(L"BossFrogAttackArea"):
		return new BossFrogAttackArea();
		break;
	case SNH(L"RangeFrogState"):
		return new RangeFrogState();
		break;
	case SNH(L"RangeFrogAction"):
		return new RangeFrogAction();
		break;
	case SNH(L"RangeFrogAttackArea"):
		return new RangeFrogAttackArea();
		break;
	case SNH(L"GameManager"):
		return new GameManager();
		break;
	case SNH(L"EnemyManager"):
		return new EnemyManager();
		break;
	case SNH(L"NoteManager"):
		return new NoteManager();
		break;
	case SNH(L"UIManager"):
		return new UIManager();
		break;
	case SNH(L"ProjectileManager"):
		return new ProjectileManager();
		break;
	case SNH(L"Projectile"):
		return new Projectile();
		break;
	case SNH(L"ToxicManager"):
		return new ToxicManager();
		break;
	case SNH(L"ToxicFloor"):
		return new ToxicFloor();
		break;
	case SNH(L"StageManager"):
		return new StageManager();
		break;
	case SNH(L"StageInCheck"):
		return new StageInCheck();
		break;
	case SNH(L"LobbyEventSystem"):
		return new LobbyEventSystem();
		break;
	case SNH(L"ArcadeNetworkEvent"):
		return new ArcadeNetworkEvent();
		break;
	case SNH(L"ArcadePlayer"):
		return new ArcadePlayer();
		break;
	case SNH(L"Portal"):
		return new Portal();
		break;
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

	int _ScriptCount = (int)RemoveScriptComponent.size();

	for (int i = 0; i < _ScriptCount; i++)
	{
		com::Script* newScript = Create(RemoveScriptComponent[i]->GetScriptName());
		object->AddComponent(newScript);
	}

	for (com::Script* _iter : RemoveScriptComponent)
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