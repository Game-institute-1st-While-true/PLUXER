#include "BossFrogAttackArea.h"

BossFrogAttackArea::BossFrogAttackArea() : hos::com::Script(L"BossFrogAttackArea")
{
	ExecutionOrder = 7;
}

BossFrogAttackArea::~BossFrogAttackArea()
{

}

void BossFrogAttackArea::Reset()
{

}

void BossFrogAttackArea::Awake()
{
	m_BossFrogState = m_GameObject->GetParent()->GetComponent<BossFrogState>();
	AttackCollision = m_GameObject->GetComponent<hos::com::BoxCollision>();
	IsAttack = false;
}

void BossFrogAttackArea::OnTriggerEnter(hos::com::Collision* other)
{
	if (AttackMode == 1) return;

	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		//// Enmey 로 바꾸고 데미지를 주자

		switch (m_BossFrogState->GetNowAction())
		{
		case BossFrogState::BossFrogActionList::JumpAttack:
		{
			AttackDamage = 24;
		}
		break;
		case BossFrogState::BossFrogActionList::RushAttack:
		{
			AttackDamage = 18;
		}
		break;
		case BossFrogState::BossFrogActionList::TongueAttack:
		{
			AttackDamage = 15;
		}
		break;
		default:
		{
			AttackDamage = 0;
		}
		break;
		}

		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::JumpAttack ||
			m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::RushAttack)
		{
			other->m_GameObject->GetComponent<PlayerState>()->GetDamaged(AttackDamage, 1, m_BossFrogState->m_GameObject->transform);
		}
		else
		{
			other->m_GameObject->GetComponent<PlayerState>()->GetDamaged(AttackDamage, 0, m_BossFrogState->m_GameObject->transform);
		}


		IsAttack = true;
	}
}
