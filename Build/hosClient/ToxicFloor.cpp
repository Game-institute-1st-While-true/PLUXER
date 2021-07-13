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
		// Ȱ��ȭ �ȴٸ� �� �� �ڿ� ���������
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
	// �÷��̾ ������ �������� ��
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
	// �÷��̾ ������ �ٽ� ���ƿ���
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		if (IsPlayerEnter)
		{
			other->m_GameObject->GetComponent<PlayerState>()->SetIsSlow(false);
			IsPlayerEnter = false;
		}
	}
}
