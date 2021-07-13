#include "PlayerLookEnemy.h"

PlayerLookEnemy::PlayerLookEnemy() : hos::com::Script(L"PlayerLookEnemy")
{
	ExecutionOrder = 2;
}

PlayerLookEnemy::~PlayerLookEnemy()
{
	while (EnemyPosition.size() != 0)
	{
		EnemyPosition.pop_back();
	}
}

void PlayerLookEnemy::Awake()
{
	PlayerTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->transform;
	LookAtArea = m_GameObject->GetComponent<hos::com::SphereCollision>();
	m_PlayerState = PlayerTransform->m_GameObject->GetComponent<PlayerState>();
}

void PlayerLookEnemy::Update()
{
	RemoveDeadEnmey();

	if (EnemyPosition.size() == 0)
	{
		PlayerForword = hos::Vector3::Zero;
		IsInEnemy = false;
	}
	else if (EnemyPosition.size() == 1)
	{
		PlayerForword = EnemyPosition[0]->transform->GetPosition() - PlayerTransform->GetPosition();
		PlayerForword.y = 0;
		PlayerForword.Normalize();
		IsInEnemy = true;
	}
	else
	{
		hos::Vector3 _temp;

		InEnemyLength = 100.f;

		for (auto _iter = EnemyPosition.begin(); _iter < EnemyPosition.end(); _iter++)
		{
			float _Length = ((*_iter)->transform->GetPosition() - PlayerTransform->GetPosition()).Length();

			if (_Length < InEnemyLength)
			{
				InEnemyLength = _Length;

				_temp = (*_iter)->transform->GetPosition();
			}

		}

		PlayerForword = _temp - PlayerTransform->GetPosition();
		PlayerForword.y = 0;
		PlayerForword.Normalize();

		IsInEnemy = true;

	}

	IncreaseFeverAttackRange();
}

void PlayerLookEnemy::OnTriggerEnter(hos::com::Collision* other)
{
	if (other->m_GameObject->GetTag().compare(L"Enemy") == 0)
	{
		Component* _temp = other->m_GameObject->GetComponent<MeleeFrogState>();

		if (_temp != nullptr)
		{
			if (other->m_GameObject->GetComponent<MeleeFrogState>()->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Dead)
			{
				EnemyPosition.push_back(other->m_GameObject);
			}
		}

		_temp = other->m_GameObject->GetComponent<RangeFrogState>();

		if (_temp != nullptr)
		{
			if (other->m_GameObject->GetComponent<RangeFrogState>()->GetNowAction() != RangeFrogState::RangeFrogActionList::Dead)
			{
				EnemyPosition.push_back(other->m_GameObject);
			}
		}

		_temp = other->m_GameObject->GetComponent<BossFrogState>();

		if (_temp != nullptr)
		{
			if (other->m_GameObject->GetComponent<BossFrogState>()->GetNowAction() != BossFrogState::BossFrogActionList::Dead)
			{
				EnemyPosition.push_back(other->m_GameObject);
			}
		}

	}
}

void PlayerLookEnemy::OnTriggerExit(hos::com::Collision* other)
{
	if (other->m_GameObject->GetTag().compare(L"Enemy") == 0)
	{
		for (auto _iter = EnemyPosition.begin(); _iter < EnemyPosition.end(); _iter++)
		{
			if (other->m_GameObject == (*_iter))
			{
				EnemyPosition.erase(_iter);
				break;
			}
		}
	}
}

void PlayerLookEnemy::IncreaseFeverAttackRange()
{
	// 피버 상태면 공격 범위 늘어나게
	if (m_PlayerState->GetIsFever())
	{
		LookAtRadius = 3.f;
		LookAtArea->SetRadius(LookAtRadius);
	}
	else
	{
		LookAtRadius = 2.f;
		LookAtArea->SetRadius(LookAtRadius);
	}
}

void PlayerLookEnemy::RemoveDeadEnmey()
{
	for (auto _iter = EnemyPosition.begin(); _iter < EnemyPosition.end(); _iter++)
	{
		Component* _temp = (*_iter)->GetComponent<MeleeFrogState>();

		if (_temp != nullptr)
		{
			if ((*_iter)->GetComponent<MeleeFrogState>()->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Dead)
			{
				EnemyPosition.erase(_iter);
				break;
			}
		}

		_temp = (*_iter)->GetComponent<RangeFrogState>();

		if (_temp != nullptr)
		{
			if ((*_iter)->GetComponent<RangeFrogState>()->GetNowAction() == RangeFrogState::RangeFrogActionList::Dead)
			{
				EnemyPosition.erase(_iter);
				break;
			}
		}

		_temp = (*_iter)->GetComponent<BossFrogState>();

		if (_temp != nullptr)
		{
			if ((*_iter)->GetComponent<BossFrogState>()->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
			{
				EnemyPosition.erase(_iter);
				break;
			}
		}

	}
}
