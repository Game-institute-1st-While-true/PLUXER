#include "StageInCheck.h"

StageInCheck::StageInCheck() : hos::com::Script(L"StageInCheck")
{

}

StageInCheck::~StageInCheck()
{
	while (StageEnemyList.size() != 0)
	{
		StageEnemyList.pop_back();
	}
}

void StageInCheck::Reset()
{

}

void StageInCheck::Awake()
{

}

void StageInCheck::Start()
{
	for (auto child : m_GameObject->transform->GetChilds())
	{
		if (child->m_GameObject->GetActive())
		{
			child->m_GameObject->SetActive(false);
		}
	}
}

void StageInCheck::Update()
{

	// �÷��̾ ������ �ߴ°�?
	if (StageEnemyList.size() > 0)
	{
		for (auto _iter = StageEnemyList.begin(); _iter < StageEnemyList.end();)
		{
			if (!(*_iter)->GetActive())
			{
				_iter = StageEnemyList.erase(_iter);
			}
			else
			{
				_iter++;
			}

		}

		// �����ϸ� �ش� ������ �ݸ���, ���� ���� ����, BGM�� ��Ʈ ����
		if (IsPlayerIn)
		{
			for (auto child : m_GameObject->transform->GetChilds())
			{
				if (!child->m_GameObject->GetActive())
				{
					child->m_GameObject->SetActive(true);
				}
			}

			for (auto enemy : StageEnemyList)
			{
				if (enemy->GetActive())
				{
					Component* _temp = enemy->GetComponent<MeleeFrogState>();

					if (_temp != nullptr)
					{
						enemy->GetComponent<MeleeFrogState>()->SetIsActivation(true);
					}

					_temp = enemy->GetComponent<RangeFrogState>();

					if (_temp != nullptr)
					{
						enemy->GetComponent<RangeFrogState>()->SetIsActivation(true);
					}

				}
			}

		}
		else
		{
			for (auto enemy : StageEnemyList)
			{
				if (enemy->GetActive())
				{
					Component* _temp = enemy->GetComponent<MeleeFrogState>();

					if (_temp != nullptr)
					{
						enemy->GetComponent<MeleeFrogState>()->SetNowAction(MeleeFrogState::MeleeFrogActionList::Idle);
						enemy->GetComponent<MeleeFrogState>()->SetIsActivation(false);
					}

					_temp = enemy->GetComponent<RangeFrogState>();

					if (_temp != nullptr)
					{
						enemy->GetComponent<RangeFrogState>()->SetNowAction(RangeFrogState::RangeFrogActionList::Idle);
						enemy->GetComponent<RangeFrogState>()->SetIsActivation(false);
					}

				}
			}
		}

		IsClear = false;
	}
	else
	{
		for (auto child : m_GameObject->transform->GetChilds())
		{
			if (child->m_GameObject->GetActive())
			{
				child->m_GameObject->SetActive(false);
			}
		}

		IsClear = true;
	}

}

void StageInCheck::OnTriggerEnter(hos::com::Collision* other)
{
	// �÷��̾ ���Դ°�?
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		IsPlayerIn = true;
	}

	// ���Ͱ� �ִ°�?
	if (other->m_GameObject->GetTag().compare(L"Enemy") == 0)
	{
		StageEnemyList.push_back(other->m_GameObject);
	}

}

void StageInCheck::AllMonsterDead()
{
	for (auto enemy : StageEnemyList)
	{
		if (enemy->GetActive())
		{
			Component* _temp = enemy->GetComponent<MeleeFrogState>();

			if (_temp != nullptr)
			{
				enemy->GetComponent<MeleeFrogState>()->GetDamaged(999999, 1);
			}

			_temp = enemy->GetComponent<RangeFrogState>();

			if (_temp != nullptr)
			{
				enemy->GetComponent<RangeFrogState>()->GetDamaged(999999, 1);
			}

		}
	}
}
