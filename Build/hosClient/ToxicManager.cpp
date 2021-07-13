#include "ToxicManager.h"

ToxicManager::ToxicManager() : hos::com::Script(L"ToxicManager")
{
	ExecutionOrder = 8;
}

ToxicManager::~ToxicManager()
{
	while (ToxicFloorList.size() != 0)
	{
		ToxicFloorList.pop_back();
	}
}

void ToxicManager::Reset()
{

}

void ToxicManager::Awake()
{
	for (int i = 0; i < m_GameObject->transform->GetChildCount(); i++)
	{
		ToxicFloorList.push_back(m_GameObject->transform->GetChild(i)->m_GameObject->GetComponent<ToxicFloor>());
		ToxicFloorList[i]->SetActive(false);
	}
}

void ToxicManager::Update()
{

}

void ToxicManager::SetToxicFloorPosition(hos::Vector3 pos)
{
	for (auto _iter = ToxicFloorList.begin(); _iter != ToxicFloorList.end(); _iter++)
	{
		if (!(*_iter)->GetActive())
		{
			// 해당하는 위치로 옮기고 활성화
			(*_iter)->ResetToxicTimer();
			(*_iter)->m_GameObject->transform->SetPosition(pos);
			//(*_iter)->SlowArea->SetActive(true);
			(*_iter)->m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(true);
			(*_iter)->SetActive(true);
			return;
		}
	}
}

void ToxicManager::ActiveToxicFloorPosition(int index, hos::Vector3 pos, bool btn)
{
	if (btn)
	{
		ToxicFloorList[index]->ResetToxicTimer();
		ToxicFloorList[index]->m_GameObject->transform->SetPosition(pos);
		ToxicFloorList[index]->SetActive(true);
		ToxicFloorList[index]->m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(true);
	}
	else
	{
		ToxicFloorList[index]->ResetToxicTimer();
		ToxicFloorList[index]->m_GameObject->transform->SetPosition(pos);
		ToxicFloorList[index]->SetActive(false);
		ToxicFloorList[index]->m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(false);
	}
}
