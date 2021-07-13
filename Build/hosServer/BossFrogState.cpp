#include "BossFrogState.h"
#include "PacketDefine.h"

#include "BossRoom.h"


BossFrogState::BossFrogState() : hos::com::Script(L"BossFrogState"), NowAction(BossFrogState::BossFrogActionList::Idle), PrevAction(BossFrogState::BossFrogActionList::Idle), Room(nullptr), IsChangeAction(false), CharacterId(2),
NowDir(hos::Vector3::Zero), PreDir(hos::Vector3::Zero), DirTimePoint(std::chrono::system_clock::now()), MaxHealthPoint(21000.f), NowHealthPoint(12000.f), IsDamaged(false), DamagedTimer(0), DamagedTime(0), IsStop(false)
{
	ExecutionOrder = 6;
}

BossFrogState::~BossFrogState()
{
}

void BossFrogState::SetNowAction(BossFrogActionList action)
{
	PrevAction = NowAction;
	NowAction = action;

	IsChangeAction = true;

	if (PrevAction != NowAction)
	{
		OnStateUpdate();
	}
}

void BossFrogState::SetNextAction(BossFrogActionList action)
{
	NextAction = action;
}

void BossFrogState::PlayNextAction()
{
	SetNowAction(NextAction);
}

void BossFrogState::SetJumpMoveSpeed(const float speed)
{
	JumpMoveSpeed = speed;
}

void BossFrogState::SetDirection(hos::Vector3 dir, bool on_event)
{
	PreDir = NowDir;
	NowDir = dir;

	if (PreDir != NowDir && !on_event)
	{
		auto result = (std::chrono::system_clock::now() - DirTimePoint).count();
		if (result > 0.016)
		{
			OnStateUpdate();
		}
	}
}

void BossFrogState::GetDamaged(float damage, int damagedType)
{
	//if (!IsDamaged)
	//{
	Req_OnDamegedEvent(damage, damagedType, 0);
	//IsDamaged = true;
	NowHealthPoint -= damage;

	if (NowHealthPoint <= 0)
	{
		SetNowAction(BossFrogActionList::Dead);
	}
	//}
}

int BossFrogState::GetAttackDamege()
{
	switch (NowAction)
	{
	case BossFrogState::BossFrogActionList::JumpAttack:
		return 24;
	case BossFrogState::BossFrogActionList::TongueAttack:
		return 18;
	case BossFrogState::BossFrogActionList::RushAttack:
		return 15;
	}
	return 0;
}

void BossFrogState::SetStop(bool btn)
{
	if (IsStop != btn)
	{
		IsStop = btn;
		Req_OnStopEvent();
	}
}

void BossFrogState::Req_OnDamegedEvent(float damage, int damageType, float angle)
{
	// 여기서 Send Message	
	flexbuffers::Builder builder;
	builder.Map([&] {
		// 나중에 타임포인트 추가.
		builder.Int("Id", CharacterId);
		builder.Float("Damage", damage);
		builder.Int("Type", damageType);
		builder.Float("Angle", angle);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::GET_DEMEGED_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	Room->Send(pack);
}

void BossFrogState::Req_OnStopEvent()
{
	// 여기서 Send Message
	if (!Room) return;

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Int("Id", CharacterId);
		builder.Bool("Stop", IsStop);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned short>(PROTOCOL::MOVE_STATE_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	Room->Send(pack);
}

void BossFrogState::Reset()
{
}

void BossFrogState::Awake()
{
	DamagedTime = 0.3f;
	NowAction = PrevAction = BossFrogActionList::Idle;
	NowHealthPoint = MaxHealthPoint;
	JumpMoveSpeed = 1;
}

void BossFrogState::Start()
{
}

void BossFrogState::Update()
{
	//// 피격 받으면 몇초간 데미지 안박히게
	//float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	//if (IsDamaged)
	//{
	//	if (DamagedTime < DamagedTimer)
	//	{
	//		IsDamaged = false;
	//		DamagedTimer = 0.f;
	//	}
	//	else
	//	{
	//		DamagedTimer += delta;
	//	}
	//}
}

void BossFrogState::OnStateUpdate()
{
	// 여기서 Send Message
	if (!Room) return;
	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Int("Id", CharacterId);
		builder.Int("State", static_cast<int>(NowAction));
		builder.Float("RotX", m_GameObject->transform->GetRotation().x);
		builder.Float("RotY", m_GameObject->transform->GetRotation().y);
		builder.Float("RotZ", m_GameObject->transform->GetRotation().z);
		builder.Float("DirX", NowDir.x);
		builder.Float("DirY", NowDir.y);
		builder.Float("DirZ", NowDir.z);
		builder.Float("Hp", NowHealthPoint);
		builder.Int("Next", static_cast<int>(NextAction));
		builder.Int("St", 0);
		builder.Float("Fiber", 0.0);
		builder.Bool("On", IsChangeAction);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned short>(PROTOCOL::STATE_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	Room->Send(pack);
}
