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

	// 0. �̵�����ȭ Scene
	//JustMoveSystem();	
	MakeLobby();
	MakeGameTest();

	// ���⼭���� ���� �׷� ����
	// ���� �׷� ���� ��.
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
	//	// ������� network component�� ������ �ִ� ���� ������Ʈ�� �����մϴ�.
	//	GameObject* netcoms = new GameObject();
	//	Networkcomponent* netcom = new Networkcomponent(Engine->Network);

	//	netcoms->AddComponent(netcom);

	//	// networkcomponent�� �����մϴ�.
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
	// �׷��� 3�� ����ϴ�. (�׽�Ʈ�� 0 : �κ�, 1 : Ȱ��ȭ �׷�, 2 : ��Ȱ��ȭ �׷�)
	// Initialzie�� GropuVector�� nullptr �� ����մϴ�. �����Ѵٰ� ���� ��ŭ �׷��� �������� �մϴ�. 
	

	Engine->Initialize(3);

	// lobby�� user pool ������ ���� max session�� �޾ƿɴϴ�.
	int max_session = Engine->Network->GetMaxSession();
	
	// SceneManager, Scene, Group�� �����մϴ�. �����Ǵ� ������ group->scenemanager->scene ���� ���԰��踦 �����ϴ�.
	Scene* lobby_scene = new Scene();
	Group* main_group = new Group(0, max_session, 0, 0);
	main_group->AddScene(lobby_scene);

	Engine->AddGroup(main_group);

	// group call back ������Ʈ�� �ش� scene�� ���� �����۾��� ���ӿ�����Ʈ ������ �ý��ϴ�. 
	// �� ������Ʈ�� addcomponent�� ���������� �ϹǷ� ������ �ʾƵ� �˴ϴ�.
	GameObject* lobby_manager = new GameObject(L"lobbymanager");
	lobby_scene->AddGameObject(lobby_manager);

	// �׷쿡 �ݹ� ������ ���ݴϴ�. ���� ������ ���ɼ��� �ֽ��ϴ�.
	LobbyGroup* lobby_callback = new LobbyGroup(Engine, main_group, lobby_manager, -1.f);

	//// ����͸� ������Ʈ�� LobbyManager �� ����մϴ�.
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
	// ���� �׷� ���� �����ϴ� SceneManager, Scene, Group�� �������ݴϴ�.	
	Scene* active_game_scene = new Scene();

	Group* active_game_group = new Group(0, 2, 0, 0);
	Engine->AddGroup(active_game_group);

	active_game_group->AddScene(active_game_scene);

	// group call back ������Ʈ�� �ش� scene�� ���� �����۾��� ���ӿ�����Ʈ ������ �ý��ϴ�. 
	// �� ������Ʈ�� addcomponent�� ���������� �ϹǷ� ������ �ʾƵ� �˴ϴ�.
	GameObject* active_game_manager = new GameObject(L"gamemanager");
	active_game_scene->AddGameObject(active_game_manager);

	// �׷쿡 �ݹ� ������ ���ݴϴ�. ���� ������ ���ɼ��� �ֽ��ϴ�.
	GameGroup* active_game_callback = new GameGroup(Engine, active_game_group, active_game_manager, 0.01f);

	/// �̹��� Ȱ��ȭ ���� ���� �׷��Դϴ�. �Ȱ��� �������ݴϴ�.

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
