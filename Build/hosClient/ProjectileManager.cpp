#include "ProjectileManager.h"

ProjectileManager::ProjectileManager() : hos::com::Script(L"ProjectileManager")
{
	ExecutionOrder = 7;
}

ProjectileManager::~ProjectileManager()
{
	while (ProjectileList.size() != 0)
	{
		ProjectileList.pop_back();
	}
}

void ProjectileManager::Reset()
{

}

void ProjectileManager::Awake()
{
	// 자식들 목록 가져오기

	for (int i = 0; i < m_GameObject->transform->GetChildCount(); i++)
	{
		ProjectileList.push_back(m_GameObject->transform->GetChild(i)->m_GameObject->GetComponent<Projectile>());
		ProjectileList[i]->SetActive(false);
	}

}

void ProjectileManager::Update()
{
	// 자식들 상태 확인
}

void ProjectileManager::SetProjectile(hos::com::Transform* TF, int attackType)
{
	if (attackType == 0)
	{
		// 비활성화 된 자식 가져오기
		for (auto _iter = ProjectileList.begin(); _iter != ProjectileList.end(); _iter++)
		{
			if (!(*_iter)->GetActive())
			{
				(*_iter)->m_GameObject->transform->SetPosition(TF->GetPosition() + hos::Vector3(0, 8, 0));
				(*_iter)->m_GameObject->transform->SetRotation(hos::Rotator(-90, 0, 0));
				(*_iter)->m_GameObject->transform->SetScale(hos::Vector3(1, 1, 1));
				(*_iter)->AttackArea->SetActive(true);
				(*_iter)->AttackArea->SetDirection(hos::com::CapsuleCollision::DIRECTION_Z);
				(*_iter)->SetAttackType(0);
				(*_iter)->m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(true);
				(*_iter)->SetActive(true);

				return;
			}
		}
	}


	if (attackType == 1)
	{
		for (auto _iter = ProjectileList.begin(); _iter != ProjectileList.end(); _iter++)
		{
			if (!(*_iter)->GetActive())
			{
				(*_iter)->m_GameObject->transform->SetPosition(TF->GetPosition() + hos::Vector3(0, 1, 0) + TF->GetForward());
				(*_iter)->m_GameObject->transform->SetRotation(hos::Rotator(0, 0, 0));
				(*_iter)->m_GameObject->transform->SetScale(hos::Vector3(0.5f, 0.5f, 0.5f));
				(*_iter)->m_GameObject->transform->SetForward(TF->GetForward());
				(*_iter)->AttackArea->SetActive(true);
				(*_iter)->AttackArea->SetDirection(hos::com::CapsuleCollision::DIRECTION_Z);
				(*_iter)->SetAttackType(1);
				(*_iter)->m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(true);
				(*_iter)->SetActive(true);

				return;
			}
		}
	}

}

void ProjectileManager::ActiveProjectile(int index, int attackType, hos::Vector3 pos, bool btn)
{
	// 다를 경우 셋팅을 바꿔준다.
	if (btn)
	{
		ProjectileList[index]->SetAttackType(attackType);
		ProjectileList[index]->m_GameObject->transform->SetPosition(pos);
		ProjectileList[index]->m_GameObject->transform->SetRotation(hos::Rotator(-90, 0, 0));
		ProjectileList[index]->m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(true);
		ProjectileList[index]->SetActive(true);
	}
	else
	{
		// 오브젝트 비활성화
		ProjectileList[index]->m_GameObject->transform->SetPosition(pos);
		ProjectileList[index]->m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(false);
		ProjectileList[index]->SetActive(false);
	}
}

Projectile* ProjectileManager::GetProjectile(int index)
{
	if (ProjectileList.size() <= index || index < 0)
		return nullptr;
	else
	{
		return ProjectileList[index];
	}
}
