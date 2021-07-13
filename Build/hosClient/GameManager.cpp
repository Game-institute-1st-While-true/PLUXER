#include "GameManager.h"

GameManager::GameManager() : hos::com::Script(L"GameManager")
{
	ExecutionOrder = 0;
}

GameManager::~GameManager()
{

}

void GameManager::Awake()
{
	m_EnemyManager = m_GameObject->m_Scene->FindGameObjectWithName(L"EnemyManager")->GetComponent<EnemyManager>();
	m_PlayerState = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerState>();
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();

	IsGameEnd = false;
	EndTime = 3.f;
	Input->SetMouseMode(hos::WindowsInput::eMouseMode::LOCKED);
	Input->SetMouseVisible(false);

	g_Renderer->SetShadowMapCullMode(hos::cg::RendererDesc::ShadowMapCullList::SHADOWMAP_SOLID);

}

void GameManager::Reset()
{

}

void GameManager::Start()
{
	m_NoteManager->MusicStart();
}

void GameManager::Update()
{
	// 게임에 대한 정보를 업데이트

	// 몬스터가 다 죽었나 물어보기

	if (m_EnemyManager->GetIsAllEnemyDead() || m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Dead)
	{
		if (EndTime < EndTimer)
		{
			IsGameEnd = true;

			if (m_NoteManager->GetIsBGMStart())
			{
				m_NoteManager->MusicStart();
				Input->SetMouseMode(hos::WindowsInput::eMouseMode::NONE);
				Input->SetMouseVisible(true);
			}
		}
		else
		{
			EndTimer += (float)Time->DeltaTime();
		}
	}

	if (IsGameEnd)
	{
		if (Input->GetKeyDown(KeyCode::Escape))
		{
			SceneManagement->ChangeScene(0);
		}
	}
}