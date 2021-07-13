#include "BossRoom.h"
#include "PacketDefine.h"
#include "ProjectileManager.h"
#include "BossArea.h"
#include "DB_Connection.h"

BossRoom::BossRoom(hos::com::Group* group)
	:GroupCallBackcomponent(group), Manager(0), LoadFlag(false), PositionTime(0.1), PositionCheckTime(0), LoopCount(0), MapCollider(nullptr),
	IsGameEnd(false), IsGameRelease(false), GotGameInfo(false), GameInfoSetupRequest(false), GameCloseInfoGetCount(0), GameClearResult(false), FirstConnectUserCount(0)
{
	ExecutionOrder = 1;
	UpdateFunc = &BossRoom::UpdateNotImplemented;
	MapCollider = new MapCollisionChecker();
}

BossRoom::~BossRoom()
{
	hos::SafeDelete(MapCollider);
}

void BossRoom::Req_LoadState()
{
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_LOAD_EVENT);
	m_Group->Send(pack);
}

void BossRoom::Req_WaitState()
{
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_WAIT_EVENT);
	m_Group->Send(pack);
}

void BossRoom::Req_GameState()
{
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_GAME_EVENT);
	m_Group->Send(pack);
}

void BossRoom::Req_GameEnd()
{
	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Int("1p", Players[0]->State->GetCharacterId());
		builder.Int("combo1", Players[0]->State->GetNowCombo());
		builder.Int("score1", Players[0]->State->GetScore());

		builder.Int("2p", Players[1]->State->GetCharacterId());
		builder.Int("combo2", Players[1]->State->GetNowCombo());
		builder.Int("score2", Players[1]->State->GetScore());

		builder.Int("best", BestScore);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_GAMEEND_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	m_Group->Send(pack);
}

void BossRoom::Req_ReleaseState()
{
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_RELEASE_EVENT);

	m_Group->Send(pack);
}

void BossRoom::Req_RoomInEvent(hos::string name, int tokenIndex)
{
	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.String("Id", hos::ut::UTF16ToAnsi(name.c_str()));
		builder.Int("Index", tokenIndex);
		});
	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_IN_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	m_Group->Send(pack);
}

void BossRoom::Req_RoomOutEvent(hos::string name, int tokenIndex)
{
	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.String("Id", hos::ut::UTF16ToAnsi(name.c_str()));
		builder.Int("Index", tokenIndex);
		});
	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_OUT_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	m_Group->Send(pack);
}

void BossRoom::Req_GameInfo(Player& player)
{
	// 플레이가 업데이트 될 때마다 전체 유저에게 게임 정보를 전달합니다.
	// 피버게이지, 콤보, 점수 

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Int("id", player.State->GetCharacterId());
		builder.Float("Fiber", player.State->GetNowFeverGage());
		builder.Int("Score", player.State->GetScore());
		builder.Int("Best", BestScore);
		builder.Int("Combo", player.State->GetNowCombo());
		});
	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::GAME_INFO_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	m_Group->Send(pack);
}

void BossRoom::Req_GameInfo(BossFrog& boss)
{
	// 플레이가 업데이트 될 때마다 전체 유저에게 게임 정보를 전달합니다.
	// 피버게이지, 콤보, 점수 

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Int("id", 2);
		builder.Int("Best", BestScore);
		builder.Float("Speed", boss.GetJumpAttackSpeed());
		});
	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::GAME_INFO_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	m_Group->Send(pack);
}

void BossRoom::Awake()
{
	using namespace hos;
	using namespace com;

	for (int i = 0; i < Players.size(); i++)
	{
		Players[i]->State->SetCharacterId(i);
	}

	MapCollider->RectPoolClear();

	GameObject* map_collision = m_GameObject->m_Scene->FindGameObjectWithName(L"BossMapCollision");
	BoxCollision* s_wall = map_collision->transform->GetChild(0)->m_GameObject->GetComponent<BoxCollision>();
	BoxCollision* n_wall = map_collision->transform->GetChild(1)->m_GameObject->GetComponent<BoxCollision>();
	BoxCollision* e_wall = map_collision->transform->GetChild(2)->m_GameObject->GetComponent<BoxCollision>();
	BoxCollision* w_wall = map_collision->transform->GetChild(3)->m_GameObject->GetComponent<BoxCollision>();

	MapCollider->AddBoxCollision(s_wall);
	MapCollider->AddBoxCollision(n_wall);
	MapCollider->AddBoxCollision(e_wall);
	MapCollider->AddBoxCollision(w_wall);

	Boss = m_GameObject->m_Scene->FindGameObjectWithName(L"BossFrog")->GetComponent<BossFrog>();
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();
	Boss->SetNoteManager(m_NoteManager);
}

void BossRoom::Update()
{
	UpdateFunc(*this);
}

void BossRoom::LateUpdate()
{
}

void BossRoom::OnClose()
{
	Boss->m_NavAgent->ReleasePathFind();
}

void BossRoom::AddPlayer(Player* player, PlayerState* state, PlayerMove* move)
{
	player->Room = this;
	Players.push_back(player);
	player->m_GameObject->network->Mapping(m_Group, static_cast<int>(PROTOCOL::GROUPSEND), &hos::com::Group::GroupSend);
	player->m_GameObject->network->Mapping(m_Group, static_cast<int>(PROTOCOL::EXPORT_ME_GROUPSEND), &hos::com::Group::ExceptMeGroupSend);
}

void BossRoom::Send(PACKET& pack)
{
	m_Group->Send(pack);
}

void BossRoom::OnLoad()
{
}

void BossRoom::OnAddToken(hos::com::NetworkToken* token, int tokenIndex, std::wstring name, int uuid)
{
	if (Players[tokenIndex]->m_GameObject->network)
	{
		Players[tokenIndex]->m_GameObject->network->RequestToken(token->GetSessionId());
		Players[tokenIndex]->SetId(name);
		Players[tokenIndex]->SetUuid(uuid);
		Req_RoomInEvent(name, tokenIndex);

		if (token->IsConnection())
		{
		}
		if (m_Group->GetCurrentMenber() == m_Group->GetMaxMember())
		{
		}
	}
}

void BossRoom::OnDelToken(hos::com::NetworkToken* token, int tokenIndex)
{
	using namespace hos;
	using namespace com;

	Networkcomponent* network = Players[tokenIndex]->m_GameObject->network;
	if (network != nullptr)
	{
		Players[tokenIndex]->State->SetNowPlayerAction(PlayerState::PlayerActionList::Dead);
		Players[tokenIndex]->m_GameObject->network->RemoveToken();
		Req_RoomOutEvent(Players[tokenIndex]->GetId().c_str(), tokenIndex);

		if (tokenIndex == Manager && m_Group->GetState() != GROUP_STATE::GAME)
		{
			m_Group->SetState(GROUP_STATE::RELEASE);
		}
		if (m_Group->GetCurrentMenber() == 0)
		{
			m_Group->SetState(GROUP_STATE::RELEASE);
		}
	}
}

void BossRoom::OnStateUpdate()
{
	switch (m_Group->GetState())
	{
	case hos::com::GROUP_STATE::NONE:
		break;
	case hos::com::GROUP_STATE::ENABLE:
		UpdateFunc = &BossRoom::UpdateReadyProcess;
		break;
	case hos::com::GROUP_STATE::WAIT:
		UpdateFunc = &BossRoom::UpdateNotImplemented;
		Req_WaitState();
		break;
	case hos::com::GROUP_STATE::GAME:

		Players[0]->m_GameObject->transform->SetPosition(hos::Vector3(3, 0, 0));
		Players[0]->State->SetNetworkSender(Players[0]->m_GameObject);
		Players[0]->State->SetCharacterId(0);
		Players[1]->m_GameObject->transform->SetPosition(hos::Vector3(-3, 0, 0));
		Players[1]->State->SetNetworkSender(Players[1]->m_GameObject);
		Players[1]->State->SetCharacterId(1);
		UpdateFunc = &BossRoom::UpdateGameProcess;

		Boss->m_GameObject->transform->SetPosition(hos::Vector3(0, 0, -10));

		for (auto player : Players)
		{
			auto token = player->m_GameObject->network->GetToken();
			if (token)
			{
				if (player->m_GameObject->network->IsConnection())
				{
					FirstConnectUserCount++;
					DB_Connection::DBC->Req_GameInfoUpdate(token->GetSessionId(), 0, player->GetId().c_str(), 0);
				}
			}
		}

		Req_GameState();

		PlayStartTime = std::chrono::high_resolution_clock::now();
		m_NoteManager->MusicStart();

		Boss->GameStart();
		break;

	case hos::com::GROUP_STATE::LOAD:
		UpdateFunc = &BossRoom::UpdateLoadProcess;
		Req_LoadState();
		break;
	case hos::com::GROUP_STATE::RELEASE:
		Boss->GameClose();
		Req_ReleaseState();
		break;
	case hos::com::GROUP_STATE::DISABLE:
		break;
	}
}

void BossRoom::OutPlayerCheck()
{
	if (FirstConnectUserCount != m_Group->GetCurrentMenber())
	{
		for (auto player : Players)
		{
			if (player->State->m_GameObject->network == nullptr)
			{
				player->State->SetNowPlayerAction(PlayerState::PlayerActionList::Dead);
			}
			else
			{
				if (player->State->m_GameObject->network->IsConnection() == false)
				{
					player->State->SetNowPlayerAction(PlayerState::PlayerActionList::Dead);
				}
			}
		}
	}
}

void BossRoom::GameEndCheck()
{
	using namespace std;
	using namespace chrono;

	// 보스가 죽으면 게임이 종료.
	if (Boss->State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		if (!IsGameEnd)
		{
			IsGameEnd = true;
			GameClearResult = true;
		}
	}

	int cnt = 0;

	// 플레이어 전체가 죽으면 게임이 종료.
	for (Player* player : Players)
	{
		if (player)
		{
			if (player->State->GetNowPlayerAction() == PlayerState::PlayerActionList::Dead)
			{
				cnt++;
			}
		}
	}

	if (cnt == FirstConnectUserCount)
	{
		if (!IsGameEnd)
		{
			IsGameEnd = true;
			GameClearResult = false;
		}
	}

	// 게임이 종료되었다면 DB에 게임의 정보를 저장한다.
	if (IsGameEnd)
	{
		duration<double> now = high_resolution_clock::now() - PlayStartTime;
		int total_score = 0;
		if (!GameInfoSetupRequest)
		{
			for (auto player : Players)
			{
				total_score += player->State->GetScore();
			}

			for (auto player : Players)
			{
				auto token = player->m_GameObject->network->GetToken();

				if (token)
				{
					if (player->m_GameObject->network->IsConnection())
					{
						if (GameClearResult)
						{
							DB_Connection::DBC->Req_GameInfoUpdate(token->GetSessionId(), total_score, player->GetId().c_str(), now.count());
						}
						else
						{
							DB_Connection::DBC->Req_GameInfoUpdate(token->GetSessionId(), 0, player->GetId().c_str(), now.count());
						}
					}
				}
			}
			GameInfoSetupRequest = true;
		}
	}
}

void BossRoom::AddGameCloseCount()
{
	if (++GameCloseInfoGetCount == m_Group->GetCurrentMenber())
	{
		GotGameInfo = true;
		EndTime = 2.0;
		EndTimer = 0;
	}
}

void BossRoom::GameLifeCycle()
{
	if (m_Group->GetState() == hos::com::GROUP_STATE::RELEASE) return;

	if (IsGameRelease)
	{
		if (EndTimer >= EndTime)
		{
			m_Group->SetState(hos::com::GROUP_STATE::RELEASE);
			IsGameRelease = false;
			IsGameEnd = false;
		}
		else
		{
			EndTimer += m_GameObject->m_Scene->GetTimer()->DeltaTime();
		}
	}

	// 게임이 끝났는가의 인증은 유저의 정보를 DB와의 통신을 통해 다 받은 상태를 의미한다.
	if (IsGameEnd && GotGameInfo)
	{
		if (EndTimer >= EndTime)
		{
			EndTimer = 0;
			EndTime = 10.f;

			IsGameRelease = true;
			Req_GameEnd();
		}
		else
		{
			EndTimer += m_GameObject->m_Scene->GetTimer()->DeltaTime();
		}
	}
}

void BossRoom::PositionUpdate()
{
	if (PositionCheckTime >= PositionTime)
	{
		Req_PositionEvent();
		PositionCheckTime = 0;
	}
	else
	{
		PositionCheckTime += m_GameObject->m_Scene->SceneTime->DeltaTime();
	}
}

void BossRoom::Req_PositionEvent()
{
	flexbuffers::Builder builder;

	builder.Vector([&]()
		{
			for (auto player : Players)
			{
				std::scoped_lock<std::mutex> lock(player->Move->PositionMutex);
				builder.Blob(player->m_GameObject->transform->Serialize());
			};

			builder.Blob(Boss->m_GameObject->transform->Serialize());

			for (int i = 0; i < 3; i++)
			{
				auto tr = Boss->Projectile->ProjectileGetTransform(i);
				if (tr != nullptr)
				{
					builder.Blob(tr->Serialize());
				}
			}
		});

	builder.Finish();

	const uint8_t* buffer = builder.GetBuffer().data();
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::POSITION_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	m_Group->Send(pack);
	builder.Finish();
}

void BossRoom::UpdateNotImplemented()
{
	// Notting
}

void BossRoom::UpdateReadyProcess()
{
	using namespace hos;
	using namespace com;
	int ReadyCnt = 0;
	for (auto player : Players)
	{
		if (player)
		{
			if (player->GetReady())
			{
				++ReadyCnt;
			}
		}
	}
	if (ReadyCnt == m_Group->GetCurrentMenber() && m_Group->GetCurrentMenber() != 0)
	{
		m_Group->SetState(GROUP_STATE::LOAD);
	}
}

void BossRoom::UpdateLoadProcess()
{
	using namespace hos;
	using namespace com;

	if (!LoadFlag)
	{
		int LoadCnt = 0;
		for (auto player : Players)
		{
			if (player)
			{
				if (player->GetLoad())
				{
					++LoadCnt;
				}
			}
		}
		if (LoadCnt == m_Group->GetCurrentMenber() && m_Group->GetCurrentMenber() != 0)
		{
			LoadFlag = true;
			for (auto player : Players)
			{
				if (player)
				{
					player->PlayerComponentsEnable();
				}
			}
			m_Group->SetState(GROUP_STATE::GAME);
		}
	}
}

void BossRoom::UpdateGameProcess()
{
	using namespace hos;
	using namespace com;

#pragma region DebugLog
	//hos::string_literal now[] = {
	//	   L"Dead",
	//	   L"Damaged", L"Charge", L"Dash", L"Parrying", L"Smash", L"Smash1", L"Smash2", L"Smash3", L"DashAttack", L"Attack1", L"Attack2", L"Attack3", L"Move", L"Idle"
	//};

	//hos::string_literal now[] = {
	//		L"Idle", L"Dead", L"Move", L"JumpAttack", L"BackJump", L"RangeAttack", L"TongueAttack", L"RushAttack", L"BackMove", L"RotateToTarget", L"SearchTarget"
	//};
	int nowidx = static_cast<int>(Boss->State->GetNowAction());

	//NetLogger::GetIns()->Write(L"Room", L"Player 0 ## " L"X : ", Players[0]->m_GameObject->transform->GetPosition().x, L" Y : ", Players[0]->m_GameObject->transform->GetPosition().y, L" Z : ", Players[0]->m_GameObject->transform->GetPosition().z);
	/*NetLogger::GetIns()->Write(L"Room", L"Player 0 ## " L"X : ",
		Players[0]->m_GameObject->transform->GetForward().x,
		L" Y : ", Players[0]->m_GameObject->transform->GetForward().y,
		L" Z : ", Players[0]->m_GameObject->transform->GetForward().z,
		L" DELTA : ", m_GameObject->m_Scene->SceneTime->DeltaTime());*/
		//NetLogger::GetIns()->Write(L"Room", L"Player 0 State ## ", now[nowidx]);

		//NetLogger::GetIns()->Write(L"Room", L"Boss State ## ", now[nowidx]);
	// NetLogger::GetIns()->Write(L"Room", L"Player 0 ## " L"X : ", Players[0]->m_GameObject->transform->GetPosition().x, L" Y : ", Players[0]->m_GameObject->transform->GetPosition().y, L" Z : ", Players[0]->m_GameObject->transform->GetPosition().z);

	//NetLogger::GetIns()->Write(L"BossRoom", L"DeltaTime : ", m_GameObject->m_Scene->GetTimer()->DeltaTime());
#pragma endregion
	//OutPlayerCheck();
	PositionUpdate();
	GameEndCheck();
	GameLifeCycle();
}
