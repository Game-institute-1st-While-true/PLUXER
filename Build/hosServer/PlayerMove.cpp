#include "PlayerMove.h"

PlayerMove::PlayerMove() : hos::com::Script(L"PlayerMove")
{
	ExecutionOrder = 5;
}

PlayerMove::~PlayerMove()
{

}

void PlayerMove::ToPreviousLocation()
{
	std::scoped_lock<std::mutex> lock(PositionMutex);
	m_GameObject->transform->SetPosition(PrePos);
}

void PlayerMove::Reset()
{

}

void PlayerMove::Awake()
{
	PlayerTransform = m_GameObject->transform;

	PlayerMoveSpeed = 4;

	DashSpeed = 20.0;

	IsDashStart = false;

	NormalAttackSpeed = 3.f;
	NormalAttackTime = 0;
	NormalAttackEndTime = 0.1f;
	IsNormalAttackStart = false;
	DashAttackSpeed = 5.0;

	DamagedTime = 0.5f;
	DamagedTimer = 0.f;
	DamagedDistance = 6.f;
}

void PlayerMove::Start()
{
}

void PlayerMove::Update()
{
	std::scoped_lock<std::mutex, std::mutex> lock(PositionMutex, m_PlayerState->StateMutex);
	PrePos = m_GameObject->transform->GetPosition();

	if (m_PlayerState->GetSlow())
	{
		PlayerMoveSpeed = 2.f;
		Move();
	}
	else
	{
		PlayerMoveSpeed = 4.f;

		Move();
		Dash();
		AttackMove();
		DashAttackMove();
		DamagedMove();
	}
}

void PlayerMove::Render()
{
}

void PlayerMove::Move()
{
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Move)
	{
		if (!m_PlayerState->GetStop())
		{
			hos::Vector3 MoveVector = m_PlayerState->GetDirection();
			PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (MoveVector * PlayerMoveSpeed * delta));
		}
	}
}

void PlayerMove::Dash()
{
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Dash)
	{
		if (!m_PlayerState->GetStop())
		{
			// 플레이어가 바라보고 있는 방향으로 이동
			PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (PlayerTransform->GetForward() * DashSpeed * delta));
		}
	}
}


void PlayerMove::AttackMove()
{
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3)
	{
		if (m_PlayerState->GetIsChangeAction())
		{
			IsNormalAttackStart = true;
			PrePos = m_GameObject->transform->GetPosition();
			NormalAttackTime = 0.0f;
		}

		if (IsNormalAttackStart)
		{
			if (NormalAttackTime > NormalAttackEndTime)
			{
				IsNormalAttackStart = false;
			}
			else
			{
				if (!m_PlayerState->GetStop())
				{
					PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (PlayerTransform->GetForward() * delta));
					NormalAttackTime += delta;
				}
			}
		}
	}

	//if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1 && IsNormalAttackStart == false)
	//{
	//	IsNormalAttackStart = true;
	//	NormalAttackTime = 0.0f;
	//}

	//if ( m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2)
	//{
	//	if (m_PlayerState->GetPrevPlayerAction() == PlayerState::PlayerActionList::Attack1 && IsNormalAttackStart == true)
	//	{
	//		NormalAttackTime = 0.0f;
	//	}
	//}

	//if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3)
	//{
	//	if (m_PlayerState->GetPrevPlayerAction() == PlayerState::PlayerActionList::Attack2 && IsNormalAttackStart == true)
	//	{
	//		NormalAttackTime = 0.0f;
	//	}
	//}

	//if (IsNormalAttackStart)
	//{
	//	if (NormalAttackTime >= NormalAttackEndTime)
	//	{
	//		IsNormalAttackStart = false;
	//		NormalAttackTime = 0.0f;
	//	}
	//	else
	//	{
	//		PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (PlayerTransform->GetForward() * NormalAttackSpeed * delta));
	//		NormalAttackTime += delta;
	//	}
	//}
}

void PlayerMove::DashAttackMove()
{
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	// 대쉬 상태일 때 이동
	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::DashAttack)
	{
		if (!m_PlayerState->GetStop())
		{
			PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (PlayerTransform->GetForward() * DashAttackSpeed * delta));
		}
	}
}


void PlayerMove::DamagedMove()
{
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Damaged)
	{
		if (m_PlayerState->GetDamagedType() == 1)
		{
			if (DamagedTimer < DamagedTime)
			{
				hos::Vector3 _temp = PlayerTransform->GetForward();
				_temp.y = 0;

				if (!m_PlayerState->GetStop())
				{
					PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (-_temp * DamagedDistance * delta));
				}
				DamagedTimer += delta;
			}
		}
	}
	else
	{
		DamagedTimer = 0.f;
	}
}