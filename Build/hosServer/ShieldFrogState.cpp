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
	if (NowAction != ShieldFrogActionList::Damaged)
	{
		HealthPoint -= damage;

		NowAction = ShieldFrogActionList::Damaged;
	}

	// 슈퍼아머 상태 만들기

	if (HealthPoint <= 0)
	{
		NowAction = ShieldFrogActionList::Dead;
	}

}

void ShieldFrogState::Reset()
{

}

void ShieldFrogState::Awake()
{
	DamagedArea = m_GameObject->GetComponent<hos::com::CapsuleCollision>();

	MaxHealthPoint = 100;
	HealthPoint = 100;

	IsAnimationEnd = false;
}

void ShieldFrogState::Start()
{

}

void ShieldFrogState::Update()
{


}

void ShieldFrogState::LateUpdate()
{
	
}
