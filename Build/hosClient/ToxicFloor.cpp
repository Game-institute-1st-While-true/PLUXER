#include "ToxicFloor.h"

ToxicFloor::ToxicFloor() : hos::com::Script(L"ToxicFloor")
{
	ExecutionOrder = 9;
}

ToxicFloor::~ToxicFloor()
{

}

void ToxicFloor::Reset()
{

}

void ToxicFloor::Awake()
{
	m_PlayerState = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerState>();
	SlowArea = m_GameObject->GetComponent<hos::com::BoxCollision>();

	ToxicTexture = m_GameObject->transform->GetChild(0)->m_GameObject->transform->GetChild(0)->m_GameObject->GetComponent<hos::com::MeshRenderer>();

	ToxicTexture->SetIsEffect(true);

}

void ToxicFloor::Update()
{

	if (m_GameObject->GetActive())
	{
		// 활성화 된다면 몇 초 뒤에 사라지는지
		if (ToxicRemoveTime < ToxicRemoveTimer)
		{
			if (IsPlayerEnter)
			{
				m_PlayerState->SetIsSlow(false);
			}
			SlowArea->SetActive(false);
			m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(false);
			this->SetActive(false);
		}
		else
		{
			SlowArea->SetActive(true);
			ToxicRemoveTimer += (float)Time->DeltaTime();
		}
	}

}

void ToxicFloor::OnTriggerEnter(hos::com::Collision* other)
{
	// 플레이어가 들어오면 느려지게 함
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		if (!IsPlayerEnter)
		{
			other->m_GameObject->GetComponent<PlayerState>()->SetIsSlow(true);
			IsPlayerEnter = true;
		}
	}
}

void ToxicFloor::OnTriggerExit(hos::com::Collision* other)
{
	// 플레이어가 나가면 다시 돌아오게
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		if (IsPlayerEnter)
		{
			other->m_GameObject->GetComponent<PlayerState>()->SetIsSlow(false);
			IsPlayerEnter = false;
		}
	}
}
