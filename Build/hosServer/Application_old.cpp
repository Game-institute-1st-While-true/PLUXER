#pragma once
#define WIN32_LEAN_AND_MEAN

#include "Application.h"
#include "Move.h"
#include "hosEngine/BoxCollision.h"
#include "hosEngine/Rigidbody.h"
#include "hosEngine/GroupSystem.h"

#include "LobbyGroup.h"
#include "GameGroup.h"
#include "PositionTest.h"
#include "User.h"

#include <thread>

using namespace hos;
using namespace com;

Application::Application() : Engine(nullptr), CallBack(nullptr)
{
}

Application::~Application()
{
	SafeDelete(Engine);
	SafeDelete(CallBack);
}

bool Application::Create()
{
	ExitEvent = CreateEvent(NULL, true, false, nullptr);
	CallBack = new EngineCallBack(ExitEvent);
	Engine = new GroupSystem(CallBack);
	// Engine = new GroupSystem();

	// 0. 이동동기화 Scene
	//JustMoveSystem();	
	MakeLobby();
	MakeGameTest();

	// 여기서부터 게임 그룹 구축
	// 게임 그룹 구축 끝.
	return true;
}

void Application::JustMoveSystem()
{
	//Engine->Initialize(3);

	//SceneManager* Sm = new SceneManager();
	//Scene* scene = new Scene();
	//scene->SetUpFuncTables();
	//Sm->AddScene(scene);
	//
	//Group* run = new Group(0, 3, 0, 0);
	//Engine->AddGroup(run);
	//run->AddSceneManager(Sm);

	//for (int i = 0; i < MAX_GROUP_USER_COUNT; i++)
	//{
	//	// 대기중인 network component를 가지고 있는 게임 오브젝트를 생성합니다.
	//	GameObject* netcoms = new GameObject();
	//	Networkcomponent* netcom = new Networkcomponent(Engine->Network);

	//	netcoms->AddComponent(netcom);

	//	// networkcomponent를 셋팅합니다.
	//	netcom->GetToken(i);
	//	netcom->MapingReqGroupFuncs(Engine);
	//	scene->AddGameObject(netcoms);
	//}

	//auto firstsm = run->GetSceneManager();
	//auto firstscene = firstsm->GetScene(0);

	//int objcount = 3;
	//auto objects = firstscene->GetObjects();

	//BoxCollision* bc[3];
	//Rigidbody* rb[3];
	//User* u[3];
	//Move* mv[3];

	//for (int i = 0; i < objcount; i++)
	//{
	//	rb[i] = new Rigidbody();
	//	rb[i]->SetUseGravity(false);
	//	bc[i] = new BoxCollision();
	//	u[i] = new User();
	//	mv[i] = new Move();
	//	objects[i]->AddComponent(rb[i]);
	//	objects[i]->AddComponent(bc[i]);
	//	objects[i]->AddComponent(u[i]);
	//	objects[i]->AddComponent(mv[i]);
	//	objects[i]->GetComponent<Transform>()->SetPosition(Vector3(i * -2, 1, 0));
	//}

	//GameObject* Obstacle = new GameObject(L"Obstacle");
	//BoxCollision* obc = new BoxCollision();
	//Obstacle->AddComponent(obc);
	//Obstacle->GetComponent<Transform>()->SetPosition(Vector3(3, 1, 0));
	//firstscene->AddGameObject(Obstacle);

	//GameObject* event_manager = new GameObject(L"EventManager");
	//SystemMonitor* monitor = new SystemMonitor(1.0f);

	//event_manager->AddComponent(monitor);
	//firstscene->AddGameObject(event_manager);
}

void Application::MakeLobby()
{
	// 그룹을 3개 만듭니다. (테스트용 0 : 로비, 1 : 활성화 그룹, 2 : 비활성화 그룹)
	// Initialzie는 GropuVector에 nullptr 을 등록합니다. 생성한다고 적은 만큼 그룹을 등록해줘야 합니다. 
	

	Engine->Initialize(3);

	// lobby의 user pool 생성을 위해 max session을 받아옵니다.
	int max_session = Engine->Network->GetMaxSession();
	
	// SceneManager, Scene, Group을 생성합니다. 설정되는 순서는 group->scenemanager->scene 으로 포함관계를 가집니다.
	Scene* lobby_scene = new Scene();
	Group* main_group = new Group(0, max_session, 0, 0);
	main_group->AddScene(lobby_scene);

	Engine->AddGroup(main_group);

	// group call back 컴포넌트는 해당 scene에 대한 예약작업과 게임오브젝트 생성을 맡습니다. 
	// 이 컴포넌트는 addcomponent를 내부적으로 하므로 해주지 않아도 됩니다.
	GameObject* lobby_manager = new GameObject(L"lobbymanager");
	lobby_scene->AddGameObject(lobby_manager);

	// 그룹에 콜백 셋팅을 해줍니다. 추후 정리할 가능성이 있습니다.
	LobbyGroup* lobby_callback = new LobbyGroup(Engine, main_group, lobby_manager, -1.f);

	//// 모니터링 컴포넌트를 LobbyManager 에 등록합니다.
	//SystemMonitor* monitor = new SystemMonitor(1.0f);
	//lobby_manager->AddComponent(monitor);

	main_group->SetActive(true);
	for (int i = 0; i < max_session; i++)
	{
		main_group->AddToken(Engine->Network->GetToken(i));
	}
}

void Application::MakeGameTest()
{
	// 게임 그룹 역시 같습니다 SceneManager, Scene, Group을 생성해줍니다.	
	Scene* active_game_scene = new Scene();

	Group* active_game_group = new Group(0, 2, 0, 0);
	Engine->AddGroup(active_game_group);

	active_game_group->AddScene(active_game_scene);

	// group call back 컴포넌트는 해당 scene에 대한 예약작업과 게임오브젝트 생성을 맡습니다. 
	// 이 컴포넌트는 addcomponent를 내부적으로 하므로 해주지 않아도 됩니다.
	GameObject* active_game_manager = new GameObject(L"gamemanager");
	active_game_scene->AddGameObject(active_game_manager);

	// 그룹에 콜백 셋팅을 해줍니다. 추후 정리할 가능성이 있습니다.
	GameGroup* active_game_callback = new GameGroup(Engine, active_game_group, active_game_manager, 0.01f);

	/// 이번엔 활성화 되지 않은 그룹입니다. 똑같이 생성해줍니다.

	Scene* game_scene = new Scene();

	Group* game_group = new Group(0, 2, 0, 0);
	game_group->AddScene(game_scene);
	Engine->AddGroup(game_group);


	GameObject* game_manager = new GameObject(L"gamemanager");
	game_scene->AddGameObject(game_manager);

	GameGroup* game_callback = new GameGroup(Engine, game_group, game_manager, 0.01f);
}

void Application::MakeGame()
{
}

void Application::SetUp()
{
	Engine->SetUp();
}

void Application::Run()
{
	Engine->Start(9000);
}

void Application::Wait()
{
	WaitForSingleObject(ExitEvent, INFINITE);
}


EngineCallBack::EngineCallBack(HANDLE event):
	ExitEvent(event)
{
}

EngineCallBack::~EngineCallBack()
{
}
