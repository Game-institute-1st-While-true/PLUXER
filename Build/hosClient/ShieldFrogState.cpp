#include "ShieldFrogState.h"

ShieldFrogState::ShieldFrogState() : hos::com::Script(L"ShieldFrogState")
{
	ExecutionOrder = 5;
}

ShieldFrogState::~ShieldFrogState()
{

}

void ShieldFrogState::SetNowAction(ShieldFrogActionList val)
{
	PrevAction = NowAction;
	NowAction = val;
}

void ShieldFrogState::GetDamaged(float damage)
{
	
	// 방패를 올리고 있는가?
	// 맞은 방향에 따라 다름

	if (NowAction != ShieldFrogActionList::Damaged)
	{
		HealthPoint -= damage;

		NowAction = ShieldFrogActionList::Damaged;
	}
	// 슈퍼아머 상태 만들기

	if (HealthPoint < 0)
	{
		NowAction = ShieldFrogActionList::Dead;
	}

}

void ShieldFrogState::Reset()
{

}

void ShieldFrogState::Awake()
{
	ShieldFrogAnimation = m_GameObject->transform->GetChild(0)->m_GameObject->GetComponent<hos::com::Animation>();
	// MeleeDetectZone = m_GameObject->transform->GetChild(1)->m_GameObject->GetComponent<ShieldFrogMeleeDetectZone>();
	// RangeDetectZone = m_GameObject->transform->GetChild(2)->m_GameObject->GetComponent<ShieldFrogRangeDetectZone>();

	DamagedArea = m_GameObject->GetComponent<hos::com::CapsuleCollision>();
	m_PlayerState = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerState>();

	MaxHealthPoint = 100;
	HealthPoint = MaxHealthPoint;

	AnimationTimeScale = 1.0f;

	IsAnimationEnd = false;
}

void ShieldFrogState::Start()
{

}

void ShieldFrogState::Update()
{
	// 플레이어 상태가 스매시인지 확인한다.

	if (m_PlayerState->GetIsChangeAction())
	{
		if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
			m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
			m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
			m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
		{
			AnimationTimeScale = ShieldFrogAnimation->GetTimeScale();

			// 스매시이고 TimeScale 이 같은지 확인한다.
			if (AnimationTimeScale != m_PlayerState->GetSmashTimeScale())
			{
				AnimationTimeScale = m_PlayerState->GetSmashTimeScale();
				ShieldFrogAnimation->SetTimeScale(AnimationTimeScale);
			}

		}
		else
		{
			// 스매시가 아니면 TimeScale 을 되돌린다.
			AnimationTimeScale = 1.0f;

			if (ShieldFrogAnimation->GetTimeScale() != AnimationTimeScale)
			{
				ShieldFrogAnimation->SetTimeScale(AnimationTimeScale);
			}

		}
	}


}

void ShieldFrogState::LateUpdate()
{

}
