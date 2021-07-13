#include "Player.h"
#include "hosEngine/Object.h"

#include "hosUtility/hosUtility.h"
#include "hosEngine/NetworkManager.h"
#include "PacketDefine.h"
#include "DB_Connection.h"

#include "BossRoom.h"

Player::Player()
	: hos::com::Script(L"Player"), Id(L""), Pw(L""), Cert(false), Uuid(-1), IsReady(false), State(nullptr), Move(nullptr), IsLoad(false), IsPlay(false), Capsule(nullptr), Room(nullptr)
{
	ExecutionOrder = 3;
}

Player::~Player()
{
}

void Player::Reset()
{
}

void Player::Awake()
{
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::READY), &Player::Ans_ReadyFlag);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::LOAD), &Player::Ans_LoadFlag);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::CERTIFICATION), &Player::Ans_Certification);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::STATE_EVENT), &Player::Ans_StateUpdate);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::ON_ATTACK_EVENT), &Player::Ans_OnAttackEvent);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::TIME_SCALE_EVENT), &Player::Ans_TimeScaleUpdate);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::FEVER_EVENT), &Player::Ans_FeverRequest);

	// DB MAPPING
	m_GameObject->network->Mapping(System, static_cast<int>(PROTOCOL::ROOM_ENABLE), &hos::com::GroupSystem::Ans_ActiveRoom);
	m_GameObject->network->Mapping(System, static_cast<int>(PROTOCOL::ROOM_IN), &hos::com::GroupSystem::Ans_UserInRoom);
	m_GameObject->network->Mapping(System, static_cast<int>(PROTOCOL::DB_ROOM_IN), &hos::com::GroupSystem::DB_Ans_UserInRoom);
	m_GameObject->network->Mapping(System, static_cast<int>(PROTOCOL::DB_ROOM_ENABLE), &hos::com::GroupSystem::DB_Ans_ActiveRoom);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::DB_GAME_CLOSE_INFO), &Player::Ans_CloseGameInfo);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::DB_CERTIFICATION), &Player::DB_Ans_Certification);
}

void Player::Update()
{
	CheckPlayerCollision();
}

void Player::LateUpdate()
{
}

void Player::SetPlayerInfo(PlayerState* state, PlayerMove* behaivor)
{
}

void Player::PlayerComponentsEnable()
{
	if (State && Move)
	{
		State->OnEnable();
		Move->OnEnable();
	}
}

void Player::CheckPlayerCollision()
{
	double delta = m_GameObject->m_Scene->GetTimer()->DeltaTime();
	if (MapColliderCheck(delta) || ObjectColliderCheck(delta))
	{
		State->SetStop(true);
	}
	else
	{
		State->SetStop(false);
	}
}

bool Player::MapColliderCheck(double& delta)
{
	hos::Vector3 dir = m_GameObject->transform->GetForward();
	hos::Vector3 next_pos;
	hos::Vector2 v2pos;

	hos::Vector2 collider_size;
	hos::Vector2 center(m_GameObject->transform->GetPosition().x, m_GameObject->transform->GetPosition().z);

	bool ret = false;

	switch (State->GetNowPlayerAction())
	{
	case PlayerState::PlayerActionList::Move:
		next_pos = m_GameObject->transform->GetPosition() + (dir * static_cast<float>(delta) * static_cast<float>(Move->GetMoveSpeed()));
		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;
		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	case PlayerState::PlayerActionList::Damaged:
		next_pos = m_GameObject->transform->GetPosition() + (dir * static_cast<float>(delta) * static_cast<float>(Move->GetDamagedDistance()));
		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;
		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	case PlayerState::PlayerActionList::Dash:
		next_pos = m_GameObject->transform->GetPosition() + (dir * static_cast<float>(delta) * static_cast<float>(Move->GetDashSpeed()));
		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;
		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	case PlayerState::PlayerActionList::DashAttack:
		next_pos = m_GameObject->transform->GetPosition() + (dir * static_cast<float>(delta) * static_cast<float>(Move->GetDashAttackSpeed()));
		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;
		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	case PlayerState::PlayerActionList::Attack1:
	case PlayerState::PlayerActionList::Attack2:
	case PlayerState::PlayerActionList::Attack3:
		next_pos = m_GameObject->transform->GetPosition() + (dir * static_cast<float>(delta) * static_cast<float>(Move->GetNormalAttackSpeed()));
		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;
		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	default:
		ret = false;
		break;
	}
	return ret;
}

bool Player::ObjectColliderCheck(double& delta)
{
	using namespace DirectX::SimpleMath;
	using namespace collider2;

	Circle player_circle;
	player_circle.Radius = Capsule->GetRadius();
	player_circle.Center.x = m_GameObject->transform->GetPosition().x;
	player_circle.Center.y = m_GameObject->transform->GetPosition().z;

	Circle other_circle;
	Vector3 next_pos;
	Vector2 v2pos;

	switch (State->GetNowPlayerAction())
	{
	case PlayerState::PlayerActionList::Dash:
		break;
	case PlayerState::PlayerActionList::DashAttack:
		break;
	case PlayerState::PlayerActionList::Attack1:
		break;
	case PlayerState::PlayerActionList::Attack2:
		break;
	case PlayerState::PlayerActionList::Attack3:
		break;
	case PlayerState::PlayerActionList::Move:
		for (auto other : Room->Players)
		{
			if (other != this && other != nullptr)
			{
				other_circle.Center.x = other->m_GameObject->transform->GetPosition().x;
				other_circle.Center.y = other->m_GameObject->transform->GetPosition().z;
				other_circle.Radius = other->Capsule->GetRadius();

				if (CC(player_circle, other_circle))
				{
					return true;
				}
			}
		}
		other_circle.Center.x = Room->Boss->m_GameObject->transform->GetPosition().x;
		other_circle.Center.y = Room->Boss->m_GameObject->transform->GetPosition().z;
		other_circle.Radius = Room->Boss->Capsule->GetRadius();

		if (CC(player_circle, other_circle))
		{
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

bool Player::MapAndObjectColliderCheck()
{
	using namespace DirectX::SimpleMath;
	using namespace collider2;

	float speed = 1.0;

	switch (State->GetNowPlayerAction())
	{
	case PlayerState::PlayerActionList::Dash:
		speed = static_cast<float>(Move->GetDashSpeed());
		break;
	case PlayerState::PlayerActionList::DashAttack:
		speed = static_cast<float>(Move->GetDashAttackSpeed());
		break;
	case PlayerState::PlayerActionList::Move:
		speed = static_cast<float>(Move->GetMoveSpeed());
		break;
	case PlayerState::PlayerActionList::Attack1:
	case PlayerState::PlayerActionList::Attack2:
	case PlayerState::PlayerActionList::Attack3:
		speed = static_cast<float>(Move->GetNormalAttackSpeed());
		break;
	default:
		return false;
	}

	if (!State->GetStop())
	{
		if (State->GetNowPlayerAction() == PlayerState::PlayerActionList::Dash ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Move ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1 ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2 ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3 ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::DashAttack)
		{
			float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());
			auto next_pos = m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetForward() * speed * delta);

			Vector2 v2pos(next_pos.x, next_pos.z);

			v2pos *= Capsule->GetRadius();

			if (Room->MapCollider->CheckCollisionMap(v2pos))
			{
				return true;
			}
#pragma region Object Collision
			Circle player_circle;
			Circle other_circle;

			player_circle.Radius = Capsule->GetRadius();
			player_circle.Center = v2pos;

			other_circle.Radius = Room->Boss->Capsule->GetRadius();
			other_circle.Center.x = Room->Boss->m_GameObject->transform->GetPosition().x;
			other_circle.Center.y = Room->Boss->m_GameObject->transform->GetPosition().z;

			if (CC(player_circle, other_circle))
			{
				return true;
			}

			for (auto player : Room->Players)
			{
				if (player != this)
				{
					other_circle.Radius = player->Capsule->GetRadius();
					other_circle.Center.x = player->m_GameObject->transform->GetPosition().x;
					other_circle.Center.y = player->m_GameObject->transform->GetPosition().z;

					if (CC(player_circle, other_circle))
					{
						return true;
					}
				}
			}
#pragma endregion
		}
	}
	else
	{
		if (State->GetNowPlayerAction() == PlayerState::PlayerActionList::Dash ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Move ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1 ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2 ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3 ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::DashAttack)
		{
			float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());
			auto next_pos = m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetForward() * speed * delta);

			Vector2 v2pos(next_pos.x, next_pos.z);

			if (!Room->MapCollider->CheckCollisionMap(v2pos))
			{
				return false;
			}

#pragma region Object Collision
			Circle player_circle;
			Circle other_circle;

			player_circle.Radius = Capsule->GetRadius();
			player_circle.Center = v2pos;

			other_circle.Radius = Room->Boss->Capsule->GetRadius();
			other_circle.Center.x = Room->Boss->m_GameObject->transform->GetPosition().x;
			other_circle.Center.y = Room->Boss->m_GameObject->transform->GetPosition().z;

			if (!CC(player_circle, other_circle))
			{
				return false;
			}

			for (auto player : Room->Players)
			{
				if (player != this)
				{
					other_circle.Radius = player->Capsule->GetRadius();
					other_circle.Center.x = player->m_GameObject->transform->GetPosition().x;
					other_circle.Center.y = player->m_GameObject->transform->GetPosition().z;

					if (!CC(player_circle, other_circle))
					{
						return false;
					}
				}
			}
#pragma endregion
		}
	}
	return false;
}

void Player::ReleaseDirection()
{
	State->SetDirection(hos::Vector3::Zero);
}

void Player::DB_Ans_Certification(PACKAGE& package)
{
	auto cert = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);
	int ret_code = cert->uuid();

	if (ret_code < 0)
	{
		NetLogger::GetIns()->Write(L"net", L"인증에 실패했습니다.  ID : ", Id);
		Cert = false;
	}
	else
	{
		Id = hos::ut::AnsiToUTF16(cert->id()->c_str());
		Pw = hos::ut::AnsiToUTF16(cert->pw()->c_str());
		Uuid = ret_code;
		Cert = true;
	}

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.String("Id", cert->id()->c_str());
		builder.String("Pw", cert->pw()->c_str());
		builder.Int("UUID", cert->uuid());
		});
	builder.Finish();

	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::CERTIFICATION);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	m_GameObject->network->SendMsg(pack);
}

void Player::Ans_Certification(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;

	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();
	hos::mbstring id = m["Id"].AsString().str();
	hos::mbstring pw = m["Pw"].AsString().str();
	Uuid = m["UUID"].AsInt32();

	Id = hos::ut::AnsiToUTF16(id.c_str());
	Pw = hos::ut::AnsiToUTF16(pw.c_str());
	NetLogger::GetIns()->Write(L"net", L"인증을 시도합니다. ID : ", Id);
	DB_Connection::DBC->Req_Certification(m_GameObject->network->GetToken()->GetSessionId(), id.c_str());
}

void Player::Ans_ReadyFlag(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	hos::mbstring id = m["Id"].AsString().str();
	IsReady = m["IsReady"].AsBool();

	m_GameObject->network->Call(static_cast<int>(PROTOCOL::GROUPSEND), package);
}

void Player::Ans_LoadFlag(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	hos::mbstring id = m["Id"].AsString().str();
	IsLoad = m["IsLoad"].AsBool();

	m_GameObject->network->Call(static_cast<int>(PROTOCOL::GROUPSEND), package);
}

void Player::Ans_StateUpdate(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	hos::Rotator rot;
	hos::Vector3 dir;

	int st = m["State"].AsInt32();

	rot.x = m["RotX"].AsFloat();
	rot.y = m["RotY"].AsFloat();
	rot.z = m["RotZ"].AsFloat();
	dir.x = m["DirX"].AsFloat();
	dir.y = m["DirY"].AsFloat();
	dir.z = m["DirZ"].AsFloat();

	bool action = m["On"].AsBool();

	if (Move && State)
	{
		if (State->GetNowPlayerAction() == PlayerState::PlayerActionList::Dead)
		{
			return;
		}

		std::lock_guard<std::mutex> lock(Move->PositionMutex);

		State->SetDirection(dir);
		State->SetIsChangeAction(action);
		m_GameObject->transform->SetRotation(rot);

		if (State->GetNowPlayerAction() != PlayerState::PlayerActionList::Damaged ||
			State->GetNowPlayerAction() != PlayerState::PlayerActionList::Dead)
		{
			State->SetNowPlayerAction(static_cast<PlayerState::PlayerActionList>(st));
			m_GameObject->network->Call(static_cast<int>(PROTOCOL::EXPORT_ME_GROUPSEND), package);
		}
	}
}

void Player::Ans_TimeScaleUpdate(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	m_GameObject->network->Call(static_cast<int>(PROTOCOL::EXPORT_ME_GROUPSEND), package);
}

void Player::Ans_OnAttackEvent(PACKAGE& package)
{
	std::lock_guard<std::mutex> lock(Move->PositionMutex);

	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	int smash_cnt = m["sm_cnt"].AsInt32();
	int type = m["Type"].AsInt32();
	int acc = m["Acc"].AsInt32();
	int combo = m["Combo"].AsInt32();

	hos::Vector3 boss_pos = Room->Boss->m_GameObject->transform->GetPosition();
	hos::Vector3 my_pos = m_GameObject->transform->GetPosition();
	hos::Vector3 attackPos = my_pos + (m_GameObject->transform->GetForward() * 1.5);	// attack offset = 1.5


	if ((boss_pos - attackPos).Length() <= 1.5 &&
		Room->Boss->State->GetNowAction() != BossFrogState::BossFrogActionList::Dead)
	{
		State->SetAccuracy(static_cast<NoteManager::NoteErrorRangeCheck>(acc));
		State->SetCombo(combo);
		State->AddNowFeverGage();

		SetAttackDamage();

		State->AddScore(AttackDamage);
		State->AddDamageDealt(AttackDamage);

		Room->Boss->State->GetDamaged(AttackDamage, type);
		Room->Boss->SetAttackId(State->GetCharacterId());

		Room->Req_GameInfo(*this);
		return;
	}
}

void Player::Ans_CloseGameInfo(PACKAGE& package)
{
	auto gameinfo = GetGameInfo(package.packet.memory + PACKET_HEADER_SIZE);

	int mine = gameinfo->myscore();
	int best = gameinfo->bestscore();
	int sid = gameinfo->session_id();

	Room->SetBestScore(best);
	Room->Req_GameInfo(*this);

	if (Room->CheckGameEnd())
	{
		Room->AddGameCloseCount();
	}
}

void Player::Ans_FeverRequest(PACKAGE& package)
{
	bool btn = false;

	if (State->GetIsGageFull())
	{
		btn = true;
	}
	State->Req_FeverResult(btn);
}

void Player::SetAttackDamage()
{
	auto onebittime = Room->Boss->GetNoteManager()->GetOneBeatTime();

	// 콤보 계수
	if (State->GetNowCombo() >= 160)
	{
		ComboDamagedIncrease = 0.5f;
	}
	else if (State->GetNowCombo() >= 150)
	{
		ComboDamagedIncrease = 0.49f;
	}
	else if (State->GetNowCombo() >= 140)
	{
		ComboDamagedIncrease = 0.48f;
	}
	else if (State->GetNowCombo() >= 130)
	{
		ComboDamagedIncrease = 0.475f;
	}
	else if (State->GetNowCombo() >= 120)
	{
		ComboDamagedIncrease = 0.47f;
	}
	else if (State->GetNowCombo() >= 110)
	{
		ComboDamagedIncrease = 0.44f;
	}
	else if (State->GetNowCombo() >= 100)
	{
		ComboDamagedIncrease = 0.42f;
	}
	else if (State->GetNowCombo() >= 90)
	{
		ComboDamagedIncrease = 0.4f;
	}
	else if (State->GetNowCombo() >= 80)
	{
		ComboDamagedIncrease = 0.38f;
	}
	else if (State->GetNowCombo() >= 70)
	{
		ComboDamagedIncrease = 0.34f;
	}
	else if (State->GetNowCombo() >= 60)
	{
		ComboDamagedIncrease = 0.32f;
	}
	else if (State->GetNowCombo() >= 50)
	{
		ComboDamagedIncrease = 0.3f;
	}
	else if (State->GetNowCombo() >= 40)
	{
		ComboDamagedIncrease = 0.25f;
	}
	else if (State->GetNowCombo() >= 30)
	{
		ComboDamagedIncrease = 0.2f;
	}
	else if (State->GetNowCombo() >= 20)
	{
		ComboDamagedIncrease = 0.15f;
	}
	else if (State->GetNowCombo() >= 10)
	{
		ComboDamagedIncrease = 0.1f;
	}
	else
	{
		ComboDamagedIncrease = 0.0f;
	}

	// 판정 계수
	switch (State->GetAccuracy())
	{
	case NoteManager::NoteErrorRangeCheck::CP:
		AccuracyRate = 2.f;
		break;
	case NoteManager::NoteErrorRangeCheck::P:
	{
		AccuracyRate = 1.5f;
	}
	break;
	case NoteManager::NoteErrorRangeCheck::C:
	{
		AccuracyRate = 1.25f;
	}
	break;
	case NoteManager::NoteErrorRangeCheck::N:
	{
		AccuracyRate = 1;
	}
	break;
	case NoteManager::NoteErrorRangeCheck::M:
	{
		AccuracyRate = 0.8f;
	}
	break;
	default:
		break;
	}


	// 베이스 데미지
	switch (State->GetNowPlayerAction())
	{
	case PlayerState::PlayerActionList::Attack1:
	{
		BaseDamage = (NormalAttackDamage + (float)(1 / onebittime)) * NormalAttackDamageRate * AccuracyRate;
	}
	break;
	case PlayerState::PlayerActionList::Attack2:
	{
		BaseDamage = (NormalAttackDamage + (float)(2 / onebittime)) * NormalAttackDamageRate * AccuracyRate;
	}
	break;
	case PlayerState::PlayerActionList::Attack3:
	{
		BaseDamage = (NormalAttackDamage + (float)(3 / onebittime)) * NormalAttackDamageRate * AccuracyRate;

	}
	break;
	case PlayerState::PlayerActionList::ChargeEnd:
	{
		BaseDamage = (NormalAttackDamage + (float)(1 / onebittime)) * NormalAttackDamageRate * AccuracyRate;
	}
	break;
	case PlayerState::PlayerActionList::Smash:
	{
		// 스매시 데미지
		if (State->GetIsFever())
		{
			BaseDamage = FeverSmash1Damage * (FeverSmashAttackRate + (FeverSmashDamageRate * 1));
		}
		else
		{
			BaseDamage = FeverSmash1Damage * MultiSmash1DamageRate;

			BaseDamage *= AccuracyRate;
		}
	}
	break;
	case PlayerState::PlayerActionList::Smash1:
	{
		if (State->GetIsFever())
		{
			BaseDamage = FeverSmash2Damage * (FeverSmashAttackRate + (FeverSmashDamageRate * 2));
		}
		else
		{
			BaseDamage = FeverSmash2Damage * MultiSmash2DamageRate;

			BaseDamage *= AccuracyRate;
		}
	}
	break;
	case PlayerState::PlayerActionList::Smash2:
	{
		if (State->GetIsFever())
		{
			BaseDamage = FeverSmash3Damage * (FeverSmashAttackRate + (FeverSmashDamageRate * 3));
		}
		else
		{
			BaseDamage = FeverSmash3Damage * MultiSmash3DamageRate;

			BaseDamage *= AccuracyRate;
		}
	}
	break;
	case PlayerState::PlayerActionList::Smash3:
	{
		if (State->GetIsFever())
		{
			BaseDamage = FeverSmash4Damage * (FeverSmashAttackRate + (FeverSmashDamageRate * 4));
		}
		else
		{
			BaseDamage = FeverSmash4Damage * MultiSmash4DamageRate;

			BaseDamage *= AccuracyRate;
		}
	}
	break;
	}

	// 콤보 데미지 증가율 적용
	BaseDamage += (BaseDamage * ComboDamagedIncrease);

	// 피버 적용 여부
	if (State->GetIsFever())
	{
		if (State->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1 ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2 ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3 ||
			State->GetNowPlayerAction() == PlayerState::PlayerActionList::ChargeEnd)
		{
			AttackDamage = BaseDamage * FeverNormalDamageRate;
		}
		// 피버일 때 스매시
		else
		{
			AttackDamage = BaseDamage;
		}
	}
	else
	{
		AttackDamage = BaseDamage;
	}
}

