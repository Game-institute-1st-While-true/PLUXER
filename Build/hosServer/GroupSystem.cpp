#include "hosEngine/Object.h"
#include "GroupSystem.h"
#include "DB_Connection.h"
#include "hosUtility/hosUtility.h"

#include "hosEngine/DebugConsolePaper.h"
#include "GroupCallBackcomponent.h"
#include "PacketDefine.h"

#include "Lobby.h"
#include "BossRoom.h"
#include "Player.h"
#include "NoteManager.h"

#include <filesystem> 

using namespace hos;
using namespace hos::Packet;

hos::com::GroupSystem* System = nullptr;

hos::com::GroupSystem::GroupSystem(GroupSystemControlCallBack* call_back)
	:CallBack(call_back), NetCallBack(nullptr), MonitorCheckTime(0), OnBtn(false)
{
}

hos::com::GroupSystem::~GroupSystem()
{
	if (GroupSystemThread.joinable() == true)
	{
		GroupSystemThread.join();
	}
}

void hos::com::GroupSystem::Init(int max_group)
{

	if (!CallBack)
	{
		NetLogger::GetIns()->Write(L"callback is nullptr");
	}

	system("mode con:cols=250 lines=50");
	Debug->GetIns()->ConsoleReset();
	NetCallBack = new GroupNetworkCallBack(this);

	Time = new GameTimer();
	g_Navigation = new hos::nv::Navigation();
	Network = new NetworkManager();
	Network->Initialize(SERVER_THREAD_COUNT, MAX_SESSION_COUNT, 1, NetCallBack, new DebugConsolePaper(), 200);

	//COM 초기화
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		throw std::invalid_argument("COM Init FAIL");
	}

	for (int i = 0; i < max_group; i++)
	{
		Groups.push_back(nullptr);
		GroupIdQueue.push_back(i);
	}

	// 로비 셋팅
	int max_session = Network->GetMaxSession();

	Groups[0] = new Group(0);
	Groups[0]->SetUpMembers(Network->GetMaxSession());
	Groups[0]->SetCallBack(new Lobby(Groups[0]));
	Groups[0]->Load(L"LobbyScene");
	Groups[0]->State = GROUP_STATE::WAIT;

	auto lobbyScene = Groups[0]->GetScene();

	// 게임 오브젝트 Max 만큼 생성해서 Scene에 등록
	for (int i = 0; i < max_session; ++i)
	{
		string name = L"Player" + std::to_wstring(i);
		GameObject* go = new GameObject(name);

		Networkcomponent* network_com = new Networkcomponent(Network);
		network_com->RequestToken(i);

		Player* play = new Player();

		go->AddComponent(network_com);
		go->AddComponent(play);

		static_cast<Lobby*>(Groups[0]->CallBack)->AddPlayer(play);
		lobbyScene->AddGameObject(go);
		NetLogger::GetIns()->Write(L"Lobby", L"Add GameObject Name : ", go->GetName(), L" / ", max_session);

		Groups[0]->AddMember(network_com->GetToken(), L"Empty", -1);
	}

	lobbyScene->Awake();
	lobbyScene->Start();

	// 게임 룸 셋팅
	for (int i = 1; i < max_group; i++)
	{
		Groups[i] = new Group(i);
		Groups[i]->SetUpMembers(2);
		Groups[i]->SetCallBack(new BossRoom(Groups[i])); // game group call back 등록 필요
		Groups[i]->Load(L"BossScene");

		GameObject* note_manager = new GameObject(L"NoteManager");
		NoteManager* nm = new NoteManager();

		note_manager->AddComponent(nm);

		for (int j = 0; j < 2; ++j)
		{
			string name = L"Player" + std::to_wstring(j);
			GameObject* go = new GameObject(name);
			Networkcomponent* network_com = new Networkcomponent(Network);

			Player* play = new Player();
			PlayerState* state = new PlayerState();
			PlayerMove* move = new PlayerMove();
			hos::com::CapsuleCollision* cap = new CapsuleCollision();

			cap->SetCenter(hos::Vector3(0, 0.7f, 0));
			cap->SetRadius(0.25);
			cap->SetHeight(0.5);

			play->Move = move;
			play->State = state;
			play->Capsule = cap;

			move->SetPlayerState(state);

			go->AddComponent(network_com);
			go->AddComponent(cap);
			go->AddComponent(play);
			go->AddComponent(state);
			go->AddComponent(move);

			static_cast<BossRoom*>(Groups[i]->CallBack)->AddPlayer(play, state, move);
			Groups[i]->m_Scene->AddGameObject(go);
			NetLogger::GetIns()->Write(L"GameRoom", L"Add GameObject Name : ", go->GetName(), L" / ", 2);
		}
		Groups[i]->m_Scene->AddGameObject(note_manager);
	}
	if (g_Navigation)
	{
		g_Navigation->Load();
		g_Navigation->Run();
	}
}

void hos::com::GroupSystem::Start(unsigned short listen_port)
{
	Network->Start();
	Network->CreateWorkerThread();
	//PlayGroups.push_back(1);
	//Groups[1]->SetState(GROUP_STATE::RELEASE);

	NetLogger::GetIns()->Write(L"system", L"group system thread start");

	GroupSystemThread = std::thread([=]()
		{
			OnBtn.store(true);
			if (Groups.empty() == true)
			{
				std::wcout << L"don't have a group." << std::endl;
				OnBtn.store(false);
			}
			Network->Listen(listen_port);
			DB_Connection::DBC = new DB_Connection();
			DB_Connection::DBC->MappingProtocol();
			Network->AddExtraToken(DB_Connection::DBC);
			Network->Connect(L"127.0.0.1", DBA_PORT);
			while (OnBtn.load())
			{
				Monitoring();
				Loop();
#ifdef _DEBUG
				if (GetAsyncKeyState(VK_ESCAPE))
				{
					Stop();
					NetLogger::GetIns()->Write(L"system", L"app close");
				}
#endif // _DEBUG
			}
			PlayGroups.clear();
			ReleaseGroups.clear();
			for (int i = 0; i < Groups.size(); i++)
			{
				SafeDelete(Groups[i]);
			}
			SafeDelete(Network);
			SafeDelete(Time);
			SafeDelete(g_Navigation);
			SafeDelete(g_DataManager);
			CallBack->OnExit();
			NetLogger::GetIns()->Destory();
			Debug->Destory();
		});
}

void hos::com::GroupSystem::Load()
{
	g_Path = std::filesystem::current_path();

	std::wfstream file;
	file.open(g_Path + L"\\Data_Server.hos");

	if (file.is_open())
	{
		while (!file.eof())
		{
			string sceneName;
			std::getline(file, sceneName);
			g_DataManager->LoadScene(g_Path + Scene::FILE_PATH + sceneName + Scene::FILE_EXTENSION);
		}
	}
}

void hos::com::GroupSystem::Stop()
{
	OnBtn.store(false);
}

void hos::com::GroupSystem::Loop()
{
	EnableGroupMovePlayPool();
	GroupCleanUp();

	for (auto index : PlayGroups)
	{
		Groups[index]->Loop();
	}
}

void hos::com::GroupSystem::EnableGroupMovePlayPool()
{
	for (auto group : Groups)
	{
		if (group->State == GROUP_STATE::ENABLE && !group->IsPlaying())
		{
			PlayGroups.push_back(group->Id);
			group->Playing = true;
			group->m_Scene->SceneTime->Start();
			group->m_Scene->Awake();
			group->m_Scene->Start();
		}
	}
}

void hos::com::GroupSystem::GroupCleanUp()
{
	// Play Group 중 Release 상태에 있는 그룹들을 정리해준다.
	// 그리고 Release 리스트에 넣어서 다음 단계에서 Play 리스트에 있는 그룹들을 제거해준다.
	for (int idx : PlayGroups)
	{
		if (Groups[idx]->GetState() == GROUP_STATE::RELEASE)
		{
			std::scoped_lock<std::mutex> lock(Groups[idx]->SceneMutex);

			Groups[idx]->m_Scene->OnDisable();

			Groups[idx]->SetState(GROUP_STATE::DISABLE);
			Groups[idx]->Playing = false;

			ReleaseGroups.push_back(idx);
			for (auto token : Groups[idx]->Members)
			{
				if (token != nullptr)
				{
					// 토큰을 제거해준다.
					Groups[idx]->DelMember(token);

					// 토큰을 0번그룹에 넣어준다.
					Groups[0]->AddMember(token, L"Empty", -1);
				}
			}

			SafeDelete(Groups[idx]->m_Scene);
			Groups[idx]->CallBack = nullptr;
			Groups[idx]->SetUpMembers(2);
			Groups[idx]->SetCallBack(new BossRoom(Groups[idx]));
			Groups[idx]->Load();

			GameObject* note_manager = new GameObject(L"NoteManager");
			NoteManager* nm = new NoteManager();

			note_manager->AddComponent(nm);

			for (int j = 0; j < 2; ++j)
			{
				string name = L"Player" + std::to_wstring(j);
				GameObject* go = new GameObject(name);
				Networkcomponent* network_com = new Networkcomponent(Network);

				Player* play = new Player();
				PlayerState* state = new PlayerState();
				PlayerMove* move = new PlayerMove();
				hos::com::CapsuleCollision* cap = new CapsuleCollision();

				cap->SetCenter(hos::Vector3(0, 0.7f, 0));
				cap->SetRadius(0.25);
				cap->SetHeight(0.5);

				play->Move = move;
				play->State = state;
				play->Capsule = cap;
				play->Capsule->SetActive(false);

				move->SetPlayerState(state);

				go->AddComponent(network_com);
				go->AddComponent(cap);
				go->AddComponent(play);
				go->AddComponent(state);
				go->AddComponent(move);

				static_cast<BossRoom*>(Groups[idx]->CallBack)->AddPlayer(play, state, move);
				Groups[idx]->m_Scene->AddGameObject(go);
				NetLogger::GetIns()->Write(L"GameRoom", L"Add GameObject Name : ", go->GetName(), L" / ", 2);
			}
			Groups[idx]->m_Scene->AddGameObject(note_manager);
		}
	}

	// Release 리스트에 있는 그룹들을 플레이 그룹에서 제거해준다.
	for (int idx : ReleaseGroups)
	{
		PlayGroups.remove(idx);
	}
	// 다음 로직 수행을 위해 Release 리스트를 초기화해준다.
	ReleaseGroups.clear();
}

void hos::com::GroupSystem::Monitoring()
{
	MonitorCheckTime += Time->DeltaTime();

	if (MonitorCheckTime >= 60.0f)
	{
		NetLogger::GetIns()->Write(L"system", L"", L"",
			L"CPU USE : ", Monitor.CpuUsage(),
			L"  PROCESS CPU USE :", Monitor.ThisProcessCpuUsage(),
			L"  PHYSIC MEMORY : ", Monitor.PhysicMemory(), L"  bytes"
			L"  WORKING SIZE : ", Monitor.WorkingSet(), L"  bytes");
		MonitorCheckTime = 0;
	}
}

bool hos::com::GroupSystem::AddGroup(Group* group)
{
	int index = GroupIdQueue.front();
	if (GroupIdQueue.size() == 0)
	{
		return false;
	}
	GroupIdQueue.pop_front();
	Groups[index] = group;
	Groups[index]->Id = index;
	return true;
}

void hos::com::GroupSystem::DelGroup(int index)
{
	Groups[index] = nullptr;
	GroupIdQueue.push_back(index);
}

void hos::com::GroupSystem::Ans_ActiveRoom(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	mbstring user_id = m["UserId"].AsString().str();
	int sessionid = package.id;

	// 남는방 중 하나 준다.
	int size = static_cast<int>(Groups.size());
	int dst = 0;
	for (dst; dst < size + 1; ++dst)
	{
		if (Groups[dst]->GetState() == GROUP_STATE::DISABLE)
			break;
	}
	// 남는방이 없다면 실패 패킷 날려줌.
	if (dst == size)
	{
		flexbuffers::Builder builder;

		builder.Map([&] {
			builder.String("UserId", "");
			builder.String("RoomName", "");
			builder.Int("RoomNo", -5);
			});

		builder.Finish();
		builder.GetSize();
		const uint8_t* buffer = builder.GetBuffer().data();
		PACKET pack;
		pack.head.size = PACKET_HEADER_SIZE;
		pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_ENABLE);
		pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
		Network->SendPacket(sessionid, pack);
	}
	else
	{
		DB_Connection::DBC->Req_ActiveRoom(package.id, user_id.c_str(), dst, user_id.c_str());
	}
}

void hos::com::GroupSystem::Ans_UserInRoom(PACKAGE& package)
{
	// 해당 토큰의 이전 그룹 확인
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	mbstring manager_name = m["RoomName"].AsString().str();
	mbstring user_id = m["UserId"].AsString().str();

	DB_Connection::DBC->Req_UserInRoom(package.id, user_id.c_str(), manager_name.c_str());
}

void hos::com::GroupSystem::DB_Ans_ActiveRoom(PACKAGE& package)
{
	auto active = GetRoomSetting(package.packet.memory + PACKET_HEADER_SIZE);

	int roomno_ = active->roomid();
	int sessionid = active->session_id();

	string userid = ut::AnsiToUTF16(active->userid()->c_str());
	string name = ut::AnsiToUTF16(active->roomname()->c_str());

	flexbuffers::Builder builder;

	if (roomno_ > 0)
	{
		Groups[roomno_]->SetName(name);
		Groups[roomno_]->SetState(GROUP_STATE::ENABLE);
	}

	builder.Map([&] {
		builder.String("UserId", active->userid()->c_str());
		builder.String("RoomName", active->roomname()->c_str());
		builder.Int("RoomNo", active->roomid());
		});

	builder.Finish();
	builder.GetSize();
	const uint8_t* buffer = builder.GetBuffer().data();
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_ENABLE);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	Network->SendPacket(sessionid, pack);
}

void hos::com::GroupSystem::DB_Ans_UserInRoom(PACKAGE& package)
{
	auto userin = GetRoomSetting(package.packet.memory + PACKET_HEADER_SIZE);

	int roomno_ = userin->roomid();
	int sessionid = userin->session_id();
	mbstring room_name = userin->roomname()->c_str();
	mbstring userid = userin->userid()->c_str();

	auto token = Network->GetToken(sessionid);
	int old_group = token->GetGroupId();
	if (old_group < 0 || old_group >= Groups.size())
	{
		// old 그룹이 잘못되어있다 disconnect
		NetLogger::GetIns()->Write(L"net", L"잘못 된 방에 접근합니다.  ID : ", ut::AnsiToUTF16(userid.c_str()));
		token->SetGroupId(0);
		Network->Disconnect(sessionid);
		return;
	}

	Groups[old_group]->DelMember(token);

	if (token == nullptr)
	{
		NetLogger::GetIns()->Write(L"net", __FUNCTIONW__, L"   Token Is Nullptr");
		Network->Disconnect(package.id);
		return;
	}
	if (roomno_ < 0)		// 에러 코드가 리턴된 경우
	{
		flexbuffers::Builder builder;
		builder.Map([&] {
			builder.String("RoomName", userin->roomname()->c_str());
			builder.String("UserID", userin->userid()->c_str());
			builder.Int("RoomNo", roomno_);
			});
		builder.Finish();
		builder.GetSize();
		const uint8_t* buffer = builder.GetBuffer().data();
		PACKET pack;
		pack.head.size = PACKET_HEADER_SIZE;
		pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_IN);
		pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
		Groups[0]->AddMember(token, ut::AnsiToUTF16(userid), -1);
		Network->SendPacket(sessionid, pack);
		return;
	}

	bool result = Groups[roomno_]->AddMember(token, ut::AnsiToUTF16(userid), -1);
	if (!result)
	{
		Groups[0]->AddMember(token, ut::AnsiToUTF16(userid), -1);
		roomno_ = 0;
	}

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.String("RoomName", userin->roomname()->c_str());
		builder.String("UserID", userin->userid()->c_str());
		builder.Int("RoomNo", roomno_);
		});

	builder.Finish();
	builder.GetSize();
	const uint8_t* buffer = builder.GetBuffer().data();
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_IN);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	Network->SendPacket(sessionid, pack);
}

hos::com::GroupNetworkCallBack::GroupNetworkCallBack(GroupSystem* engine)
	:System(engine)
{
}

hos::com::GroupNetworkCallBack::~GroupNetworkCallBack()
{
}

void hos::com::GroupNetworkCallBack::OnDisconnect_Accept(PACKAGE& package)
{
	// 0번에 속한 녀석일 경우 따로 해줄 것 없음.
	int token_id = package.id;
	auto token = Network->GetToken(package.id);

	int oldid = token->GetGroupId();

	if (oldid != 0)
	{
		// 0번에 속하지 않은 녀석일 경우 0번 그룹에 AddToken 실행
		// 타 그룹에 속해있을 경우? DelToken 실행
		if (oldid > 0 ||
			oldid < System->Groups.size())
		{
			if (System->Groups[oldid]->FindMember(token) >= 0)
			{
				System->Groups[oldid]->DelMember(token);
			}
		}
		token->SetGroupId(0);
		if (System->Groups[0]->FindMember(token) < 0)
		{
			System->Groups[0]->AddMember(token, L"Empty", -1);
		}
	}
}

void hos::com::GroupNetworkCallBack::OnDisconnect_Connect(PACKAGE& package)
{
	// 아마도 DB 관련 콜백함수를 호출할 예정임. 현재는 서버 사이의 통신이 DB 밖에 없다.
	// 서버가 추가 될 경우 커넥션 타입을 만들고, 커넥션 타입별로 적용할 것.
}