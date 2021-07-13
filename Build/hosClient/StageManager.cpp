#include "StageManager.h"

StageManager::StageManager() : hos::com::Script(L"StageManager")
{

}

StageManager::~StageManager()
{
	while (Stages.size() != 0)
	{
		Stages.pop_back();
	}
}

void StageManager::Reset()
{

}

void StageManager::Awake()
{
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();
	BossPortal = m_GameObject->m_Scene->FindGameObjectWithName(L"GotoBoss")->GetComponent<Portal>();

	// 스테이지 영역들
	for (int i = 0; i < m_GameObject->transform->GetChildCount(); i++)
	{
		Stages.push_back(m_GameObject->transform->GetChild(i)->m_GameObject->GetComponent<StageInCheck>());
	}

	BossGate = m_GameObject->m_Scene->FindGameObjectWithName(L"BossGate");

}

void StageManager::Update()
{

	// 플레이어가 진입을 했는가?
	for (auto stage : Stages)
	{
		if (stage->GetIsPlayerIn() && !stage->GetIsClear())
		{
			IsPlayerBattle = true;
			break;
		}
		else
		{
			IsPlayerBattle = false;
		}

	}

	if (Stages[Stages.size() - 1]->GetIsClear())
	{
		BossGate->SetActive(false);
	}
	else
	{
		BossGate->SetActive(true);
	}

	if (BossPortal->GetIsPlayerIn())
	{
		IsInBossStage = true;
	}


	if (IsInBossStage)
	{
		if (m_NoteManager->GetNowBGM() != NoteManager::BGMList::BossZone)
		{
			m_NoteManager->ChangeBGM(NoteManager::BGMList::BossZone);
		}
	}
	else if (IsPlayerBattle)
	{
		if (m_NoteManager->GetNowBGM() != NoteManager::BGMList::CombatZone)
		{
			m_NoteManager->ChangeBGM(NoteManager::BGMList::CombatZone);
		}
	}
	else
	{
		if (m_NoteManager->GetNowBGM() != NoteManager::BGMList::PeaceZone)
		{
			m_NoteManager->ChangeBGM(NoteManager::BGMList::PeaceZone);
		}
	}


	AllMonsterDead();

}

void StageManager::AllMonsterDead()
{
	if (bDebug)
	{
		if (!IsInputCheat)
		{
			if (Input->GetKeyDown(KeyCode::Multiply))
			{
				for (auto stage : Stages)
				{
					stage->AllMonsterDead();
				}

				IsInputCheat = true;
			}
		}
	}
}
