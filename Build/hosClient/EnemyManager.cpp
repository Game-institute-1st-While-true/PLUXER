#include "EnemyManager.h"

using namespace hos;
using namespace com;

EnemyManager::EnemyManager() : hos::com::Script(L"EnemyManager")
{

}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Reset()
{

}

void EnemyManager::Awake()
{
	IsClear = false;
	IsEnemiesDead = false;

	if (!m_GameObject->transform->GetChild(0))
	{
		IsClear = true;
		return;
	}

	Scene* scene = m_GameObject->m_Scene;

	if (scene)
	{
		Portal = scene->FindGameObjectWithName(L"GotoBoss");
		if (Portal)
		{
			Portal->SetActive(false);
		}

		GameObject* tmp = nullptr;
		for (int i = 0; i < m_GameObject->transform->GetChilds().size(); i++)
		{
			tmp = m_GameObject->transform->GetChild(i)->m_GameObject;
			if (tmp)
			{
				if (!Boss)
				{
					if (tmp->GetName() == L"BossFrog")
					{
						Boss = tmp;
					}
				}
				else
				{
					Enemies.push_back(tmp);
				}
			}
		}
		hos::com::GameObject* BossUI = scene->FindGameObjectWithName(L"BossUI");
		if (Enemies.empty())
		{
			IsEnemiesDead = true;

			if (Boss)
			{
				if (Boss->GetActive())
				{
					if (BossUI)
					{
						BossUI->SetActive(true);
					}
				}
			}
		}
		else
		{
			if (BossUI)
			{
				BossUI->SetActive(false);
			}
		}
	}

	if (IsEnemiesDead && nullptr == Boss)
	{
		IsClear = true;
		return;
	}
	g_Renderer->SetFogOption(0.24f, 0.f, 0.29f, 0.05f);
}

void EnemyManager::Update()
{
	// 하위에 있는 오브젝트들이 활성화 됐는지 확인

	if (!IsClear)
	{
		if (!IsEnemiesDead)
		{
			bool checkAlive = false;
			for (GameObject* enemy : Enemies)
			{
				if (enemy->GetActive())
				{
					checkAlive = true;
					break;
				}
			}

			IsEnemiesDead = !checkAlive;

			if (IsEnemiesDead)
			{
				if (Portal)
				{
					Portal->SetActive(true);
				}
			}
		}
		else
		{
			if (Boss)
			{
				if (Boss->GetActive() == false)
				{
					IsClear = true;
				}
			}
			else
			{
				IsClear = true;
			}
		}
	}

	// 몬스터 재생성 할 수 있게 하기?

}
