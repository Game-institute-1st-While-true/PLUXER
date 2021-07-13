#include "PlayerMove.h"

PlayerMove::PlayerMove() : hos::com::Script(L"PlayerMove")
{
	ExecutionOrder = 3;
}

PlayerMove::~PlayerMove()
{

}

void PlayerMove::Reset()
{

}

void PlayerMove::Awake()
{
	m_PlayerState = m_GameObject->GetComponent<PlayerState>();
	PlayerTransform = m_GameObject->transform;

	CameraTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"MainCamera")->transform;
	NoteManagerTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->transform;

}

void PlayerMove::Start()
{
}

void PlayerMove::Update()
{
	PlayerSlow();
	Move();
	Dash();
	AttackMove();
	DashAttackMove();
	DamagedMove();

	GoToBoss();
}

void PlayerMove::Render()
{
}

void PlayerMove::Move()
{
	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Move)
	{
		hos::Vector3 MoveVector = m_PlayerState->GetDirection();

		if (!m_PlayerState->GetStop())
		{
			PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (MoveVector * (float)PlayerMoveSpeed * (float)Time->DeltaTime()));
		}
	}
}

void PlayerMove::Dash()
{
	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Dash)
	{
		if (!m_PlayerState->NetEventer)
		{
			if (!m_GameObject->m_Scene->RayCast(PlayerTransform->GetPosition() + hos::Vector3(0, 1, 0), PlayerTransform->GetForward(), 0.5f))
			{
				// 플레이어가 바라보고 있는 방향으로 이동
				PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (PlayerTransform->GetForward() * (float)DashDistance * (float)Time->DeltaTime()));
			}
		}
		else
		{
			if (!m_PlayerState->GetStop())
			{
				// 플레이어가 바라보고 있는 방향으로 이동
				PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (PlayerTransform->GetForward() * (float)DashDistance * (float)Time->DeltaTime()));
			}

		}
	}
}

void PlayerMove::AttackMove()
{
	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::ChargeEnd)
	{
		if (m_PlayerState->GetIsChangeAction())
		{
			IsNormalAttackStart = true;
			NormalAttackTime = 0.0f;

			if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::ChargeEnd)
			{
				NormalAttackDistance = 12.f;
			}
			else
			{
				NormalAttackDistance = 3.f;
			}

		}

		if (IsNormalAttackStart)
		{
			if (NormalAttackTime > NormalAttackEndTime)
			{
				IsNormalAttackStart = false;
			}
			else
			{
				hos::Vector3 _temp = PlayerTransform->GetForward();
				_temp.y = 0;

				if (!m_PlayerState->GetStop())
				{
					PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (_temp * (float)NormalAttackDistance * (float)Time->DeltaTime()));
				}
				NormalAttackTime += Time->DeltaTime();
			}
		}
	}
}

void PlayerMove::DashAttackMove()
{
	// 대쉬 상태일 때 이동
	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::DashAttack)
	{
		hos::Vector3 _temp = PlayerTransform->GetForward();
		_temp.y = 0;
		if (!m_PlayerState->GetStop())
		{

			PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (_temp * (float)DashAttackDistance * (float)Time->DeltaTime()));
		}
	}
}

void PlayerMove::DamagedMove()
{
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
					PlayerTransform->SetPosition(PlayerTransform->GetPosition() + (-_temp * DamagedDistance * (float)Time->DeltaTime()));
				}
				DamagedTimer += (float)Time->DeltaTime();
			}
		}
	}
	else
	{
		DamagedTimer = 0.f;
	}
}

void PlayerMove::PlayerSlow()
{
	if (m_PlayerState->GetIsSlow())
	{
		PlayerMoveSpeed = 2.f;
	}
	else
	{
		PlayerMoveSpeed = 4.f;
	}
}

void PlayerMove::GoToBoss()
{
	if (bDebug)
	{
		if (!IsInputCheat)
		{
			if (Input->GetKeyDown(KeyCode::Multiply))
			{
				//-10 -150
				PlayerTransform->SetPosition(hos::Vector3(-10, 0, -150));
				IsInputCheat = true;
			}
		}
	}
}
