#include "ArcadeNetworkEvent.h"
#include "NoteManager.h"

#include "PacketDefine.h"

#include "BossFrogAction.h"
#include "BossFrogAttackArea.h"
#include "BossFrogState.h"
#include "PlayerAction.h"

using namespace hos;
using namespace com;

ArcadeNetworkEvent::ArcadeNetworkEvent()
	: Player(nullptr), Remote(nullptr), m_NoteManager(nullptr), Boss(nullptr)
{
}

ArcadeNetworkEvent::~ArcadeNetworkEvent()
{
}

void ArcadeNetworkEvent::Awake()
{
	g_Renderer->SetFogOption(0.5f, 0.5f, 0.5f, 0.008f);
	g_Renderer->SetViewFrustum(false);

	if (m_GameObject->network)
	{
		m_GameObject->network->RequestToken(1);

		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::ROOM_GAME_EVENT), &ArcadeNetworkEvent::Ans_GameStart);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::ROOM_GAMEEND_EVENT), &ArcadeNetworkEvent::Ans_GameEnd);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::ROOM_RELEASE_EVENT), &ArcadeNetworkEvent::Ans_ReleaseGame);

		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::POSITION_EVENT), &ArcadeNetworkEvent::Ans_PositionEvent);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::STATE_EVENT), &ArcadeNetworkEvent::Ans_StateUpdate);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::GET_DEMEGED_EVENT), &ArcadeNetworkEvent::Ans_GetDamagedEvent);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::TIME_SCALE_EVENT), &ArcadeNetworkEvent::Ans_TimeScaleUpdate);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::MOVE_STATE_EVENT), &ArcadeNetworkEvent::Ans_MoveStateEvent);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::PROJECTILE_ACTIVE_EVENT), &ArcadeNetworkEvent::Ans_ProjectileActiveEvent);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::TOXIC_ACTIVE_EVENT), &ArcadeNetworkEvent::Ans_ToxicActiveEvent);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::GAME_INFO_EVENT), &ArcadeNetworkEvent::Ans_GameInfoUpdate);
		m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::FEVER_EVENT), &ArcadeNetworkEvent::Ans_FeverEvent);
	}

	Player = m_GameObject->m_Scene->FindGameObjectWithName(L"Player");
	Remote = m_GameObject->m_Scene->FindGameObjectWithName(L"RemotePlayer");
	Boss = m_GameObject->m_Scene->FindGameObjectWithName(L"BossFrog");


	//if (!Weapon1)
	//{
	//	Weapon1 = 
	//}
	//if (!Weapon2)
	//{
	//	Weapon2 = m_GameObject->m_Scene->FindGameObjectWithName(L"Weapon_02(1)");
	//}


	Boss->GetComponent<BossFrogState>()->SetMaxHealthPoint(21000.f);

	m_ProjectileManager = m_GameObject->m_Scene->FindGameObjectWithName(L"ProjectileManager")->GetComponent<ProjectileManager>();

	m_GameObject->m_Scene->FindGameObjectWithName(L"Wall")->GetComponent<BoxCollision>()->SetActive(false);
	m_GameObject->m_Scene->FindGameObjectWithName(L"Wall(1)")->GetComponent<BoxCollision>()->SetActive(false);
	m_GameObject->m_Scene->FindGameObjectWithName(L"Wall(2)")->GetComponent<BoxCollision>()->SetActive(false);
	m_GameObject->m_Scene->FindGameObjectWithName(L"Wall(3)")->GetComponent<BoxCollision>()->SetActive(false);

	Player->GetComponent<CapsuleCollision>()->SetActive(false);
	Remote->GetComponent<CapsuleCollision>()->SetActive(false);

	Remote->GetComponent<PlayerState>()->Weapon1 = m_GameObject->m_Scene->FindGameObjectWithName(L"Weapon_01(1)");
	Remote->GetComponent<PlayerState>()->Weapon2 = m_GameObject->m_Scene->FindGameObjectWithName(L"Weapon_02(1)");
	Remote->GetComponent<PlayerState>()->Weapon1MeshRenderer = Remote->GetComponent<PlayerState>()->Weapon1->GetComponent<hos::com::MeshRenderer>();

	g_Renderer->SetShadowMapCullMode(hos::cg::RendererDesc::ShadowMapCullList::SHADOWMAP_CULLFRONT);
}

void ArcadeNetworkEvent::Start()
{
	wchar_t szSrc[MAX_PATH];
	GetPrivateProfileStringW(L"Login", L"ISHOST", L"", szSrc, MAX_PATH, (g_Path + L"\\cookie.ini").c_str());

	string t = L"T";
	if (t.compare(szSrc) != 0)
	{
		Player->transform->SetPosition(Vector3(-3.f, 0.f, 0.f));
		Remote->transform->SetPosition(Vector3(3.f, 0.f, 0.f));
		Player->GetComponent<PlayerState>()->SetCharacterId(1);
		Remote->GetComponent<PlayerState>()->SetCharacterId(0);
	}
	else
	{
		Player->transform->SetPosition(Vector3(3.f, 0.f, 0.f));
		Remote->transform->SetPosition(Vector3(-3.f, 0.f, 0.f));
		Player->GetComponent<PlayerState>()->SetCharacterId(0);
		Remote->GetComponent<PlayerState>()->SetCharacterId(1);
	}
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager");

	Player->GetComponent<PlayerState>()->SetNetworkSender(m_GameObject);
	Boss->GetComponent<BossFrogState>()->SetAIMode(1);

	Boss->transform->SetPosition(Vector3(0.f, 0.f, -10.f));
	Boss->GetComponent<BossFrogAction>()->AttackArea->SetAttackMode(1);

	Req_SceneSetting();
}

void ArcadeNetworkEvent::Update()
{
	/*if (Input->GetKey(KeyCode::O))
	{
		if (m_NoteManager)
		{
			NoteManager* note = m_NoteManager->GetComponent<NoteManager>();
			if (!note->GetIsBGMStart())
			{
				note->MusicStart();
			}
		}
	}*/
}

void ArcadeNetworkEvent::FixedUpdate()
{
}

void ArcadeNetworkEvent::LateUpdate()
{
}

void ArcadeNetworkEvent::Render()
{
	/*g_Renderer->_DebugText(700, 100, 15, hos::Vector4(1, 1, 1, 1), L"Player Pos : %f, %f, %f", Player->transform->GetPosition().x, Player->transform->GetPosition().y, Player->transform->GetPosition().z);
	g_Renderer->_DebugText(700, 200, 15, hos::Vector4(1, 1, 1, 1), L"Player Foward : %f, %f, %f", Player->transform->GetForward().x, Player->transform->GetForward().y, Player->transform->GetForward().z);

	int nowidx = static_cast<int>(Player->GetComponent<PlayerState>()->GetNowPlayerAction());

	hos::string_literal now[] = {
		L"Dead",
		L"Damaged", L"Charge", L"Dash", L"Parrying", L"Smash", L"Smash1", L"Smash2", L"Smash3", L"DashAttack", L"Attack1", L"Attack2", L"Attack3", L"Move", L"Idle"
	};
	g_Renderer->_DebugText(700, 300, 15, hos::Vector4(1, 1, 1, 1), L"State : %s", now[nowidx]);*/
	//g_Renderer->_DebugText(700, 400, 15, hos::Vector4(1, 1, 1, 1), L"Delta : %f", Time->DeltaTime());

	//int x = 200;
	//int y = 200;
	//int font = 14;

	//if (Player->GetComponent<PlayerState>()->GetStop())
	//{
	//	g_Renderer->_DebugText(x, y + 40, font, hos::Vector4(1, 1, 1, 1), L"Player Position : %f\n, %f\n, %f\n", Player->transform->GetPosition().x,
	//		m_GameObject->transform->GetPosition().y,
	//		m_GameObject->transform->GetPosition().z);
	//	g_Renderer->_DebugText(x, y + 240, font, hos::Vector4(1, 1, 1, 1), L"Boss Position : %f\n, %f\n, %f\n", Remote->transform->GetPosition().x,
	//		Remote->transform->GetPosition().y,
	//		Remote->transform->GetPosition().z);

	//	g_Renderer->_DebugText(x, y + 440, font, hos::Vector4(1, 1, 1, 1), L"Remote Position : %f\n, %f\n, %f\n", Boss->transform->GetPosition().x,
	//		Boss->transform->GetPosition().y,
	//		Boss->transform->GetPosition().z);
	//}
}

void ArcadeNetworkEvent::Req_SceneSetting()
{
	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Bool("IsLoad", true);
		});
	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<U16>(PROTOCOL::LOAD);
	pack.Make(pack, pack.head, buffer, (int)builder.GetSize());
	m_GameObject->network->SendMsg(pack);
}

bool ArcadeNetworkEvent::GetIsGameEnd()
{
	return IsGameEnd;
}

void ArcadeNetworkEvent::Ans_GameStart(PACKAGE& package)
{
	if (m_NoteManager)
	{
		NoteManager* note = m_NoteManager->GetComponent<NoteManager>();
		if (note)
		{
			note->ChangeBGM(NoteManager::BGMList::BossZone);
			note->MusicStart();
		}
		Input->SetMouseMode(hos::WindowsInput::eMouseMode::LOCKED);
		Input->SetMouseVisible(false);
	}
}

void ArcadeNetworkEvent::Ans_GameEnd(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int p1 = m["1p"].AsInt32();
	int p2 = m["2p"].AsInt32();

	int combo1 = m["combo1"].AsInt32();
	int combo2 = m["combo2"].AsInt32();

	int score1 = m["score1"].AsInt32();
	int score2 = m["score2"].AsInt32();

	MyScore = score1 + score2;

	BestScore = m["best"].AsInt32();
	// 콤보, 점수 등을 보여준다.
	IsGameEnd = true;

	Input->SetMouseMode(hos::WindowsInput::eMouseMode::NONE);
	Input->SetMouseVisible(true);

	int id = m_GameObject->network->GetToken()->GetSessionId();
	Network->Disconnect(id);
}

void ArcadeNetworkEvent::Ans_ReleaseGame(PACKAGE& package)
{
	// 방에서 들어온 강퇴 사인 
	// 무조건 나간다.

	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int id = m_GameObject->network->GetToken()->GetSessionId();
	Network->Disconnect(id);
}

void ArcadeNetworkEvent::Ans_PositionEvent(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	hos::com::Transform trans;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsVector();
	for (int i = 0; i < m.size(); i++)
	{
		mbstring tr(reinterpret_cast<const char*>(m[i].AsBlob().data()), m[i].AsBlob().size());

		switch (i)
		{
		case 2:
			Boss->transform->Deserialize(tr);
			break;
		case 3:
			m_ProjectileManager->GetProjectile(0)->m_GameObject->transform->Deserialize(tr);
			break;
		case 4:
			m_ProjectileManager->GetProjectile(1)->m_GameObject->transform->Deserialize(tr);
			break;
		case 5:
			m_ProjectileManager->GetProjectile(2)->m_GameObject->transform->Deserialize(tr);
			break;
		default:
			if (Player->GetComponent<PlayerState>()->GetCharacterId() == i)
			{
				trans.Deserialize(tr);
				Player->transform->SetPosition(trans.GetPosition());
			}
			else if (Remote->GetComponent<PlayerState>()->GetCharacterId() == i)
			{
				Remote->transform->Deserialize(tr);
			}
			break;
		}

	}
}

void ArcadeNetworkEvent::Ans_StateUpdate(PACKAGE& package)
{
	// index 구별
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int cid = m["Id"].AsInt32();
	int state = m["State"].AsInt32();

	hos::Rotator rot;
	hos::Vector3 dir;

	rot.x = m["RotX"].AsFloat();
	rot.y = m["RotY"].AsFloat();
	rot.z = m["RotZ"].AsFloat();

	dir.x = m["DirX"].AsFloat();
	dir.y = m["DirY"].AsFloat();
	dir.z = m["DirZ"].AsFloat();

	bool action = m["On"].AsBool();

	bool stop = m["Stop"].AsBool();

	float hp = m["Hp"].AsFloat();
	int st = m["St"].AsInt32();
	float fiber = m["Fiber"].AsFloat();

	// state set
	if (Player->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		auto user_state = Player->GetComponent<PlayerState>();
		user_state->SetNowPlayerAction(static_cast<PlayerState::PlayerActionList>(state));
		//user_state->SetDirection(dir, true);
		user_state->SetNowFeverGage(fiber);
		user_state->SetStop(stop);
		user_state->SetHealthPoint((int)hp);
		//Player->transform->SetRotation(rot);
	}

	else if (Remote->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		auto remote_state = Remote->GetComponent<PlayerState>();

		remote_state->SetNowPlayerAction(static_cast<PlayerState::PlayerActionList>(state));
		remote_state->SetDirection(dir, true);
		remote_state->SetIsChangeAction(action);
		//remote_state->SetNowFeverGage(fiber);
		remote_state->SetStamina(st);
		remote_state->SetStop(stop);

		Remote->transform->SetRotation(rot);
	}
	else
	{
		int next = m["Next"].AsInt32();
		auto boss_action = Boss->GetComponent<BossFrogAction>();
		auto boss_state = Boss->GetComponent<BossFrogState>();

		boss_state->SetNowAction(static_cast<BossFrogState::BossFrogActionList>(state));
		boss_state->SetNextAction(static_cast<BossFrogState::BossFrogActionList>(next));
		boss_state->SetDirection(dir, true);
		boss_state->SetHealthPoint(hp);
		boss_state->SetStop(stop);

		Boss->transform->SetRotation(rot);
	}
}

void ArcadeNetworkEvent::Ans_GetDamagedEvent(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int cid = m["Id"].AsInt32();
	int damage = m["Damage"].AsInt32();
	int type = m["Type"].AsInt32();
	float angle = m["Angle"].AsFloat();
	int hp = m["hp"].AsInt32();

	Vector3 foward(m["Fx"].AsFloat(), m["Fy"].AsFloat(), m["Fz"].AsFloat());

	if (Player->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		Player->GetComponent<PlayerState>()->GetDamaged(damage, type, angle);
		Player->GetComponent<PlayerState>()->SetHealthPoint(hp);
		Player->transform->SetForward(foward);
	}
	else if (Remote->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		Remote->GetComponent<PlayerState>()->GetDamaged(damage, type, angle);
		Remote->transform->SetForward(foward);
	}
	else
	{
		Boss->GetComponent<BossFrogState>()->GetDamaged((float)damage, type);
	}
}

void ArcadeNetworkEvent::Ans_TimeScaleUpdate(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int cid = m["Id"].AsInt32();
	int state = m["State"].AsInt32();
	float scale = m["Scale"].AsFloat();

	if (Player->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		// 아무것도 안함.
	}
	else if (Remote->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		// 타임스케일 셋팅.
		Remote->GetComponent<PlayerState>()->SetSmashTimeScale(scale);
	}
}

void ArcadeNetworkEvent::Ans_MoveStateEvent(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int cid = m["Id"].AsInt32();
	bool stop = m["Stop"].AsBool();
	bool slow = m["Slow"].AsBool();

	Vector3 pos(m["Px"].AsFloat(), m["Py"].AsFloat(), m["Pz"].AsFloat());
	Rotator rot(m["Rx"].AsFloat(), m["Ry"].AsFloat(), m["Rz"].AsFloat());

	if (Player->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		Player->GetComponent<PlayerState>()->SetStop(stop);
		Player->GetComponent<PlayerState>()->SetIsSlow(slow, true);
		Player->transform->SetPosition(pos);
		Player->transform->SetRotation(rot);
	}
	else if (Remote->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		Remote->GetComponent<PlayerState>()->SetStop(stop);
		Remote->GetComponent<PlayerState>()->SetIsSlow(stop, true);
		Remote->transform->SetPosition(pos);
		Remote->transform->SetRotation(rot);
	}
	else
	{
		Boss->GetComponent<BossFrogState>()->SetStop(stop);
	}
}

void ArcadeNetworkEvent::Ans_GameInfoUpdate(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int cid = m["id"].AsInt32();
	int fiber = m["Fiber"].AsInt32();
	int score = m["Score"].AsInt32();
	BestScore = m["Best"].AsInt32();
	int combo = m["Combo"].AsInt32();
	float speed = m["Speed"].AsFloat();

	if (Player->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		if (!Player->GetComponent<PlayerState>()->GetIsFever())
		{
			Player->GetComponent<PlayerState>()->SetNowFeverGage(static_cast<float>(fiber));
		}
		Player->GetComponent<PlayerState>()->SetNowScore(score);
	}
	else if (Remote->GetComponent<PlayerState>()->GetCharacterId() == cid)
	{
		Remote->GetComponent<PlayerState>()->SetNowFeverGage(static_cast<float>(fiber));
		Remote->GetComponent<PlayerState>()->SetNowScore(score);
		Remote->GetComponent<PlayerState>()->SetNowCombo(combo);
	}
	else
	{
		Boss->GetComponent<BossFrogAction>()->SetJumpMoveSpeed(speed);
	}
}

void ArcadeNetworkEvent::Ans_ProjectileActiveEvent(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int type = m["t"].AsInt32();
	int idx = m["id"].AsInt32();
	bool btn = m["on"].AsBool();

	hos::Vector3 pos(m["px"].AsFloat(), m["py"].AsFloat(), m["pz"].AsFloat());

	m_GameObject->m_Scene->FindGameObjectWithName(L"ProjectileManager")->GetComponent<ProjectileManager>()->ActiveProjectile(idx, type, pos, btn);
}

void ArcadeNetworkEvent::Ans_ToxicActiveEvent(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int idx = m["id"].AsInt32();
	bool btn = m["on"].AsBool();

	hos::Vector3 pos(m["px"].AsFloat(), m["py"].AsFloat(), m["pz"].AsFloat());
	m_GameObject->m_Scene->FindGameObjectWithName(L"ToxicManager")->GetComponent<ToxicManager>()->ActiveToxicFloorPosition(idx, pos, btn);
}

void ArcadeNetworkEvent::Ans_FeverEvent(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int idx = m["Id"].AsInt32();
	float fever = m["Gage"].AsFloat();
	bool on = m["On"].AsBool();

	if (Player->GetComponent<PlayerState>()->GetCharacterId() == idx)
	{
		if (on)
		{
			Player->GetComponent<PlayerState>()->SetNowPlayerAction(PlayerState::PlayerActionList::FeverCast);
			auto action = Player->GetComponent<PlayerAction>();

			action->SetIsInputActive(false);

			if (action->ActionSound)
			{
				action->ActionSound->Stop();
				action->ActionSound->SetAudioClip(L"pc_fevercast");
				action->ActionSound->Play();
			}
		}
		else
		{
			auto state = Player->GetComponent<PlayerState>();
			if (state->GetIsFever())
			{
				state->Weapon1->SetActive(true);
				state->Weapon2->SetActive(false);
				state->SetIsFever(false);
				Player->GetComponent<PlayerState>()->SetNowFeverGage(0);
			}
		}
	}
	else
	{
		if (on)
		{
			auto state = Remote->GetComponent<PlayerState>();
			Remote->GetComponent<PlayerState>()->SetNowPlayerAction(PlayerState::PlayerActionList::FeverCast);

			//auto action = Remote->GetComponent<PlayerAction>();

		}
		else
		{
			auto state = Remote->GetComponent<PlayerState>();
			if (state->GetIsFever())
			{
				state->Weapon1->SetActive(true);
				state->Weapon2->SetActive(false);
				state->SetIsFever(false);
				Remote->GetComponent<PlayerState>()->SetNowFeverGage(0);
			}
		}
	}

}