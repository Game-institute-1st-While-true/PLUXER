#include "PlayerState.h"
#include "PacketDefine.h"

PlayerState::PlayerState() : hos::com::Script(L"PlayerState")
{
	ExecutionOrder = 4;
}

PlayerState::~PlayerState()
{

}

void PlayerState::SetNowPlayerAction(PlayerActionList playerAction)
{
	std::lock_guard<std::mutex> lock(StateMutex);

	PrevAction = NowAction;
	NowAction = playerAction;
	IsChangeAction = true;

	if (playerAction == PlayerState::PlayerActionList::Dead)
	{
		OnStateUpdate();
	}
}

PlayerState::PlayerActionList PlayerState::GetNowPlayerAction()
{
	return NowAction;
}

void PlayerState::SetDirection(hos::Vector3 dir)
{
	if (NowAction == PlayerActionList::Move || NowAction == PlayerActionList::Idle)
	{
		PreDir = NowDir;
		NowDir = dir;
	}
}

void PlayerState::SetSmashTimeScale(float timeScale)
{
	if (NowAction == PlayerActionList::Smash ||
		NowAction == PlayerActionList::Smash1 ||
		NowAction == PlayerActionList::Smash2 ||
		NowAction == PlayerActionList::Smash3)
	{
	}
}

void PlayerState::GetDamaged(int damage, int damageType, hos::com::Transform* tr)
{
	if (NowAction == PlayerActionList::Dash || IsDamaged)
	{
		return;
	}
	else
	{
		if (NowHealthPoint > 0)
		{
			// 차지 상태면 데미지만 닳게 함
			if (NowAction == PlayerActionList::ChargeStart ||
				NowAction == PlayerActionList::ChargeIdle ||
				NowAction == PlayerActionList::ChargeEnd ||
				NowAction == PlayerActionList::Smash ||
				NowAction == PlayerActionList::Smash1 ||
				NowAction == PlayerActionList::Smash2 ||
				NowAction == PlayerActionList::Smash3 ||
				NowAction == PlayerActionList::FeverCast)
			{
				if (!IsSuperArmor)
				{
					IsSuperArmor = true;

					NowHealthPoint -= damage;
					Req_OnDamegedEvent(damage, damageType, 0);
				}
				return;
			}

			NowHealthPoint -= damage;
			float angle = m_GameObject->transform->GetForward().Dot(tr->GetForward());
			DamageType = damageType;

			if (DamageType == 0)
			{
				IsDamaged = true;
				DamagedTime = STADING_DAMAGED_ANI_TIME;
				angle = hos::RadToDegree(angle);

				if (angle < 0)
				{
					angle = 360 + angle;
				}

				if (angle >= 45.f && angle < 135.f)
				{
					// 좌
					DamagedDir = 3;
				}
				else if (angle >= 225.f && angle < 315.f)
				{
					// 우
					DamagedDir = 2;
				}
				else if (angle >= 135.f && angle < 225.f)
				{
					// 앞
					DamagedDir = 0;
				}
				else if (angle >= 315.f || angle < 45.f)
				{
					// 뒤
					DamagedDir = 1;
				}
			}

			if (DamageType == 1)
			{
				IsDamaged = true;
				DamagedTime = 1.733;
				hos::Vector3 _temp = tr->GetPosition() - m_GameObject->transform->GetPosition();
				_temp.Normalize();
				m_GameObject->transform->SetForward(_temp);
			}

			Req_OnDamegedEvent(damage, damageType, angle);

			if (NowHealthPoint > 0)
			{
				SetNowPlayerAction(PlayerActionList::Damaged);
			}
			else
			{
				SetNowPlayerAction(PlayerActionList::Dead);
			}
		}
	}
}

void PlayerState::GetDamaged(int damage, int damageType, float angle)
{
	if (NowAction == PlayerActionList::Dash || IsDamaged) 
	{
		return;
	}
	else
	{
		if (NowHealthPoint > 0)
		{
			// 차지 상태면 데미지만 닳게 함
			if (NowAction == PlayerActionList::ChargeStart ||
				NowAction == PlayerActionList::ChargeIdle ||
				NowAction == PlayerActionList::ChargeEnd ||
				NowAction == PlayerActionList::Smash ||
				NowAction == PlayerActionList::Smash1 ||
				NowAction == PlayerActionList::Smash2 ||
				NowAction == PlayerActionList::Smash3 ||
				NowAction == PlayerActionList::FeverCast)
			{
				if (!IsSuperArmor)
				{
					IsSuperArmor = true;

					NowHealthPoint -= damage;
					Req_OnDamegedEvent(damage, damageType, 0);
				}
				return;
			}

			NowHealthPoint -= damage;
			DamageType = damageType;

			if (DamageType == 0)
			{
				IsDamaged = true;
				DamagedTime = 0.5;
				angle = hos::RadToDegree(angle);

				if (angle < 0)
				{
					angle = 360 + angle;
				}

				if (angle >= 45.f && angle < 135.f)
				{
					// 좌
					DamagedDir = 3;
				}
				else if (angle >= 225.f && angle < 315.f)
				{
					// 우
					DamagedDir = 2;
				}
				else if (angle >= 135.f && angle < 225.f)
				{
					// 앞
					DamagedDir = 0;
				}
				else if (angle >= 315.f || angle < 45.f)
				{
					// 뒤
					DamagedDir = 1;
				}
			}

			Req_OnDamegedEvent(damage, damageType, angle);
			if (NowHealthPoint > 0)
			{
				SetNowPlayerAction(PlayerActionList::Damaged);
			}
			else
			{
				SetNowPlayerAction(PlayerActionList::Dead);
			}
		}
	}
}

double PlayerState::CalcAggrol()
{
	if (!m_GameObject->network->IsConnection() || NowAction == PlayerActionList::Dead)
	{
		return -1;
	}
	return (DamageDealt * 1.f) + (MaxHealthPoint - NowHealthPoint * 1.5f) + FirstTargetRate;
}

void PlayerState::Req_OnDamegedEvent(int damage, int damageType, float angle)
{
	// 여기서 Send Message	
	if (!NetEventer) return;
	if (!m_GameObject->network->GetToken()) return;

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Int("Id", CharacterId);
		builder.Int("Damage", damage);
		builder.Int("Type", damageType);
		builder.Int("hp", NowHealthPoint);
		builder.Float("Angle", angle);
		builder.Float("Fx", m_GameObject->transform->GetForward().x);
		builder.Float("Fy", m_GameObject->transform->GetForward().y);
		builder.Float("Fz", m_GameObject->transform->GetForward().z);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKAGE package;
	package.id = m_GameObject->network->GetToken()->GetSessionId();
	package.packet.head.size = PACKET_HEADER_SIZE;
	package.packet.head.protocol = static_cast<unsigned short>(PROTOCOL::GET_DEMEGED_EVENT);
	package.packet.Make(package.packet, package.packet.head, buffer, static_cast<int>(builder.GetSize()));
	m_GameObject->network->Call(static_cast<int>(PROTOCOL::GROUPSEND), package);
}

void PlayerState::DamegedUpdate()
{
	if (IsDamaged)
	{
		if (DamagedTimer >= DamagedTime)
		{
			IsDamaged = false;
			DamagedTimer = 0;
		}
		else
		{
			double delta = m_GameObject->m_Scene->GetTimer()->DeltaTime();
			DamagedTimer += delta;
		}
	}
}

void PlayerState::Req_MoveStateEvent()
{
	// 여기서 Send Message	
	if (!NetEventer) return;
	if (!m_GameObject->network->GetToken()) return;

	flexbuffers::Builder builder;

	builder.Map([&] {
		builder.Int("Id", CharacterId);
		builder.Bool("Stop", IsStop);
		builder.Bool("Slow", IsSlow);
		builder.Float("Px", m_GameObject->transform->GetPosition().x);
		builder.Float("Py", m_GameObject->transform->GetPosition().y);
		builder.Float("Pz", m_GameObject->transform->GetPosition().z);
		builder.Float("Rx", m_GameObject->transform->GetRotation().x);
		builder.Float("Ry", m_GameObject->transform->GetRotation().y);
		builder.Float("Rz", m_GameObject->transform->GetRotation().z);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKAGE package;
	package.id = m_GameObject->network->GetToken()->GetSessionId();
	package.packet.head.size = PACKET_HEADER_SIZE;
	package.packet.head.protocol = static_cast<unsigned short>(PROTOCOL::MOVE_STATE_EVENT);
	package.packet.Make(package.packet, package.packet.head, buffer, static_cast<int>(builder.GetSize()));
	m_GameObject->network->Call(static_cast<int>(PROTOCOL::GROUPSEND), package);
}

void PlayerState::SetStop(bool btn)
{
	if (IsStop != btn)
	{
		IsStop = btn;
		Req_MoveStateEvent();
	}
}

void PlayerState::SetSlow(bool btn)
{
	if (IsSlow != btn)
	{
		IsSlow = btn;
		Req_MoveStateEvent();
	}
}


void PlayerState::Reset()
{

}

void PlayerState::Awake()
{
	// 방향정보 초기화
	NowDir = hos::Vector3::Zero;
	PreDir = NowDir;
	DirTimePoint = std::chrono::system_clock::now();

	DamagedTime = STADING_DAMAGED_ANI_TIME;

	FirstTargetRate = 1.0;

	// 게임 스탯 초기화
	MaxHealthPoint = 100;
	NowHealthPoint = MaxHealthPoint;

	MaxStamina = 12;
	NowStamina = MaxStamina;
}

void PlayerState::Start()
{
	NowAction = PlayerActionList::Idle;
	PrevAction = PlayerActionList::Idle;
}

void PlayerState::Update()
{
	IsChangeAction = false;
	FeverState();
	DamegedUpdate();
}

void PlayerState::LateUpdate()
{
}

void PlayerState::Render()
{
}

void PlayerState::OnCollisionEnter(hos::com::Collision* other)
{
	int a = 0;
}

void PlayerState::OnStateUpdate()
{
	// 여기서 Send Message
	if (!NetEventer) return;
	if (NowAction != PlayerState::PlayerActionList::Dead) return;

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
		builder.Float("Hp", 0);
		builder.Int("St", NowStamina);
		builder.Float("Fiber", NowFeverGage);
		builder.Bool("On", IsChangeAction);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKAGE package;
	auto token = m_GameObject->network->GetToken();

	if (token)
	{
		package.id = token->GetSessionId();
		package.packet.head.size = PACKET_HEADER_SIZE;
		package.packet.head.protocol = static_cast<unsigned short>(PROTOCOL::STATE_EVENT);
		package.packet.Make(package.packet, package.packet.head, buffer, static_cast<int>(builder.GetSize()));
		m_GameObject->network->Call(static_cast<int>(PROTOCOL::GROUPSEND), package);
	}
}

void PlayerState::OnDirectionUpdate()
{
	// 여기서 Send Message	
	if (!NetEventer) return;

	// Time Rate
	auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - DirTimePoint);

	if (milli.count() > 16)
	{
		flexbuffers::Builder builder;
		builder.Map([&] {
			// 나중에 타임포인트 추가.
			builder.Int("Id", CharacterId);
			builder.Float("DirX", NowDir.x);
			builder.Float("DirY", NowDir.y);
			builder.Float("DirZ", NowDir.z);
			builder.Float("RotX", m_GameObject->transform->GetRotation().x);
			builder.Float("RotY", m_GameObject->transform->GetRotation().y);
			builder.Float("RotZ", m_GameObject->transform->GetRotation().z);
			});

		builder.Finish();
		const uint8_t* buffer = builder.GetBuffer().data();

		PACKET pack;
		pack.head.size = PACKET_HEADER_SIZE;
		pack.head.protocol = static_cast<unsigned short>(PROTOCOL::MOVE);
		pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
		NetEventer->network->SendMsg(pack);
		DirTimePoint = std::chrono::system_clock::now();
	}
}

void PlayerState::AddScore(float attackDamage)
{
	int _NowAttackDamage = 0;
	float _CheckScore = 0;
	int _BonusScore = 0;
	bool IsSmash = false;

	if (NowAction == PlayerActionList::Smash ||
		NowAction == PlayerActionList::Smash1 ||
		NowAction == PlayerActionList::Smash2 ||
		NowAction == PlayerActionList::Smash3)
	{
		IsSmash = true;
	}

	_NowAttackDamage = static_cast<int>(floor(attackDamage + 0.5f));

	switch (Accuracy)
	{
	case NoteManager::NoteErrorRangeCheck::CP:
	{
		// 현재 판정에 따른 점수
		//_AddScore = ScoreCP;

		// 현재 판정에 따른 보너스 점수
		ComboBonus += ComboBonusCP;

		if (ComboBonus > 100)
		{
			ComboBonus = 100;
		}

		if (ComboBonus < 0)
		{
			ComboBonus = 0;
		}

		if (IsSmash)
		{
			_CheckScore = _NowAttackDamage * SmashBonusCP + (BonusCP * (ComboBonus * 0.01f));
		}
		else
		{
			_CheckScore = _NowAttackDamage * NormalBonusCP + (BonusCP * (ComboBonus * 0.01f));
		}

		_NowAttackDamage = static_cast<int>(floor(_CheckScore + 0.5f));

		/*
		_CheckScore = BonusCP * (ComboBonus * 0.01f);

		// 현재 콤보 보너스
		_BonusScore = (int)(floor(_CheckScore + 0.5f));

		_NowAttackDamage += _BonusScore;

		if (IsSmash)
		{
			float _SmashScore = _NowAttackDamage * SmashBonusCP;
			_NowAttackDamage = (int)(floor(_SmashScore + 0.5f));
		}
		*/

	}
	break;
	case NoteManager::NoteErrorRangeCheck::P:
	{
		//_AddScore = ScoreP;
		ComboBonus += ComboBonusP;

		if (ComboBonus > 100)
		{
			ComboBonus = 100;
		}

		if (ComboBonus < 0)
		{
			ComboBonus = 0;
		}

		if (IsSmash)
		{
			_CheckScore = _NowAttackDamage * SmashBonusCP + (BonusP * (ComboBonus * 0.01f));
		}
		else
		{
			_CheckScore = _NowAttackDamage * NormalBonusCP + (BonusP * (ComboBonus * 0.01f));
		}

		_NowAttackDamage = static_cast<int>(floor(_CheckScore + 0.5f));

		/*
		_CheckScore = BonusP * (ComboBonus * 0.01f);

		_BonusScore = (int)(floor(_CheckScore + 0.5f));

		_NowAttackDamage += _BonusScore;

		if (IsSmash)
		{
			float _SmashScore = _NowAttackDamage * SmashBonusP;
			_NowAttackDamage = (int)(floor(_SmashScore + 0.5f));
		}
		*/
	}
	break;
	case NoteManager::NoteErrorRangeCheck::C:
	{
		//_AddScore = ScoreC;
		ComboBonus += ComboBonusC;

		if (ComboBonus > 100)
		{
			ComboBonus = 100;
		}

		if (ComboBonus < 0)
		{
			ComboBonus = 0;
		}

		if (IsSmash)
		{
			_CheckScore = _NowAttackDamage * SmashBonusCP + (BonusC * (ComboBonus * 0.01f));
		}
		else
		{
			_CheckScore = _NowAttackDamage * NormalBonusCP + (BonusC * (ComboBonus * 0.01f));
		}

		_NowAttackDamage = static_cast<int>(floor(_CheckScore + 0.5f));

		/*_CheckScore = BonusC * (ComboBonus * 0.01f);

		_BonusScore = (int)(floor(_CheckScore + 0.5f));

		_NowAttackDamage += _BonusScore;

		if (IsSmash)
		{
			float _SmashScore = _NowAttackDamage * SmashBonusC;
			_NowAttackDamage = (int)(floor(_SmashScore + 0.5f));
		}*/

	}
	break;
	case NoteManager::NoteErrorRangeCheck::N:
	{
		//_AddScore = ScoreN;
		ComboBonus += ComboBonusN;

		if (ComboBonus > 100)
		{
			ComboBonus = 100;
		}

		if (ComboBonus < 0)
		{
			ComboBonus = 0;
		}

		if (IsSmash)
		{
			_CheckScore = _NowAttackDamage * SmashBonusCP + (BonusN * (ComboBonus * 0.01f));
		}
		else
		{
			_CheckScore = _NowAttackDamage * NormalBonusCP + (BonusN * (ComboBonus * 0.01f));
		}

		_NowAttackDamage = static_cast<int>(floor(_CheckScore + 0.5f));

	}
	break;
	case NoteManager::NoteErrorRangeCheck::M:
	{
		//_AddScore = ScoreM;
		ComboBonus += ComboBonusM;

		if (ComboBonus > 100)
		{
			ComboBonus = 100;
		}

		if (ComboBonus < 0)
		{
			ComboBonus = 0;
		}

		if (IsSmash)
		{
			_CheckScore = _NowAttackDamage * SmashBonusM + (BonusM * (ComboBonus * 0.01f));
		}
		else
		{
			_CheckScore = _NowAttackDamage * NormalBonusM + (BonusM * (ComboBonus * 0.01f));
		}
		_NowAttackDamage = static_cast<int>(floor(_CheckScore + 0.5f));
	}
	break;
	}

	NowScore += _NowAttackDamage;
}

void PlayerState::AddNowFeverGage()
{
	if (!IsFever)
	{
		if (NowAction == PlayerActionList::Attack1 ||
			NowAction == PlayerActionList::Attack2 ||
			NowAction == PlayerActionList::Attack3 ||
			NowAction == PlayerActionList::ChargeEnd)
		{
			switch (Accuracy)
			{
			case NoteManager::NoteErrorRangeCheck::CP:
			{
				NowFeverGage += NormalFeverCP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::P:
			{
				NowFeverGage += NormalFeverP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::C:
			{
				NowFeverGage += NormalFeverC;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::N:
			{
				NowFeverGage += NormalFeverN;
			}
			break;
			default:
				break;
			}
		}
		else
		{
			int _NowSmash = 0;

			switch (NowAction)
			{

			case PlayerState::PlayerActionList::Smash:
				_NowSmash = 2;
				break;
			case PlayerState::PlayerActionList::Smash1:
				_NowSmash = 3;
				break;
			case PlayerState::PlayerActionList::Smash2:
				_NowSmash = 4;
				break;
			case PlayerState::PlayerActionList::Smash3:
				_NowSmash = 5;
				break;
			}

			switch (Accuracy)
			{
			case NoteManager::NoteErrorRangeCheck::CP:
			{
				SmashFeverCP = NormalFeverCP * _NowSmash * 1.3f;

				NowFeverGage += SmashFeverCP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::P:
			{
				SmashFeverP = NormalFeverP * _NowSmash * 1.3f;

				NowFeverGage += SmashFeverP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::C:
			{
				SmashFeverC = NormalFeverC * _NowSmash * 1.3f;

				NowFeverGage += SmashFeverC;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::N:
			{
				SmashFeverN = NormalFeverN * _NowSmash * 1.3f;

				NowFeverGage += SmashFeverN;
			}
			break;
			default:
				break;
			}
		}

		if (NowFeverGage > MaxFeverGage)
		{
			NowFeverGage = MaxFeverGage;
		}
	}
}

void PlayerState::FeverState()
{
	double delta = m_GameObject->m_Scene->GetTimer()->DeltaTime();
	if (IsFever)
	{
		if (GageRemoveTimer > GageRemoveTime)
		{
			if (NowFeverGage >= 0)
			{
				NowFeverGage -= FeverGageDecreaseAmount;
			}

			GageRemoveTimer = 0.0;
		}
		else
		{
			GageRemoveTimer += delta;
		}

		if (NowFeverGage <= 0)
		{
			if (FeverAniDelayTimer >= FeverAniDelayTime)
			{
				Req_FeverResult(false);
				NowFeverGage = 0;
				FeverAniDelayTimer = 0;
			}
			else
			{
				FeverAniDelayTimer += delta;
			}
		}
	}

	if (NowFeverGage >= MaxFeverGage)
	{
		IsGageFull = true;
	}
	else
	{
		IsGageFull = false;
	}
}

void PlayerState::Req_FeverResult(bool btn)
{
	flexbuffers::Builder builder;

	if (btn)
	{
		IsFever = true;
	}
	else
	{
		IsFever = false;
	}

	builder.Map([&]
		{
			builder.Int("Id", GetCharacterId());
			builder.Float("Gage", GetNowFeverGage());
			builder.Bool("On", btn);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET packet;
	packet.head.size = PACKET_HEADER_SIZE;
	packet.head.protocol = static_cast<unsigned short>(PROTOCOL::FEVER_EVENT);
	packet.Make(packet, packet.head, buffer, static_cast<int>(builder.GetSize()));
	m_GameObject->network->SendMsg(packet);
}
