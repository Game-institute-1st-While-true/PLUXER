#include "RangeFrogAction.h"

RangeFrogAction::RangeFrogAction() : hos::com::Script(L"RangeFrogAction")
{
	ExecutionOrder = 6;
	Bt = new BehaviorTree();
}

RangeFrogAction::~RangeFrogAction()
{
	hos::SafeDelete(Bt);
}

void RangeFrogAction::Reset()
{

}

void RangeFrogAction::Awake()
{
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();
	m_ProjectileManager = m_GameObject->m_Scene->FindGameObjectWithName(L"ProjectileManager")->GetComponent<ProjectileManager>();
	m_RangeFrogState = m_GameObject->GetComponent<RangeFrogState>();
	m_NavAgent = m_GameObject->GetComponent<hos::com::NavAgent>();
	m_NavAgent->SetNavMeshId(m_GameObject->m_Scene->GetBuildIndex());
	m_NavAgent->RequestPathFind();

	PlayerTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<hos::com::Transform>();
	RangeFrogTransform = m_GameObject->transform;

	AttackArea = m_GameObject->transform->GetChild(1)->m_GameObject->GetComponent<RangeFrogAttackArea>();

	ActionSound = m_GameObject->GetComponent<hos::com::AudioSource>();

	//////////////////////////////////////////////////////////////////////////
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_range_die_100bpm.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_range_attack0_100bpm.sclip");

	//////////////////////////////////////////////////////////////////////////

	SelectNode* start_select = new SelectNode();

	BehaviorTask* dead = new BehaviorTask();
	BehaviorTask* damaged = new BehaviorTask();
	BehaviorTask* idle = new BehaviorTask();
	BehaviorTask* attack = new BehaviorTask();

	dead->Func = [=]() {return this->Dead(); };
	damaged->Func = [=]() {return this->Damaged(); };
	idle->Func = [=]() {return this->Idle(); };
	attack->Func = [=]() {return this->Attack(); };

	Bt->Insert(start_select, static_cast<int>(ACTION_TYPE::ROOT));			 // 1

	Bt->Insert(dead, static_cast<int>(ACTION_TYPE::START_SELECT));			 // 2
	Bt->Insert(damaged, static_cast<int>(ACTION_TYPE::START_SELECT));		 // 3
	Bt->Insert(attack, static_cast<int>(ACTION_TYPE::START_SELECT));			 // 8
	Bt->Insert(idle, static_cast<int>(ACTION_TYPE::START_SELECT));			 // 5


}

void RangeFrogAction::Start()
{
	AttackArea->AttackCollision->SetActive(false);
}

void RangeFrogAction::Update()
{
	if (m_NoteManager->GetIsBGMStart())
	{
		Bt->Run();
	}

	AudioTimeScale();

}

int RangeFrogAction::Idle()
{
	if (m_RangeFrogState->GetNowAction() != RangeFrogState::RangeFrogActionList::Dead)
	{
		if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"idle") != 0)
		{
			m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"idle");
		}

		m_RangeFrogState->SetNowAction(RangeFrogState::RangeFrogActionList::Idle);
	}

	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int RangeFrogAction::Dead()
{
	if (m_RangeFrogState->GetNowAction() == RangeFrogState::RangeFrogActionList::Dead)
	{

		// 죽으면 3초 동안 플레이어를 향해 뛰어감
		if (!IsDeadMove)
		{
			if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"attack1") != 0)
			{
				m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"attack1");

				ActionSound->Stop();
				ActionSound->SetLoop(false);
				ActionSound->SetAudioClip(L"mob_range_die_100bpm");
				ActionSound->Play();

			}

			if (DeadTimer < DeadMoveTime)
			{
				RotateSpeed = 120;
				MoveSpeed = 4;

				MoveFrog(true);

				DeadTimer += (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale();

				return static_cast<int>(ACTION_RESULT::CONTINUE);
			}
			else
			{
				DeadTimer = 0.f;
				IsDeadMove = true;
				m_RangeFrogState->m_GameObject->GetComponent<hos::com::Rigidbody>()->SetUseGravity(false);
				m_RangeFrogState->DamagedArea->SetActive(false);
			}
		}
		else
		{
			if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"dead") != 0)
			{
				m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"dead", 0, true);
			}

			// 죽으면서 데미지를 줌
			if (DeadTimer < DeadAttackTime)
			{
				AttackArea->AttackCollision->SetActive(true);
			}
			else
			{
				AttackArea->AttackCollision->SetActive(false);

				if (DeadTimer > RemoveDeadTime)
				{
					m_GameObject->SetActive(false);
				}
			}

			DeadTimer += (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale();

		}

		return static_cast<int>(ACTION_RESULT::SUCCESS);
	}



	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int RangeFrogAction::Attack()
{
	// 죽었는가
	if (m_RangeFrogState->GetNowAction() == RangeFrogState::RangeFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::ABORT);
	}

	if (m_RangeFrogState->GetNowAction() == RangeFrogState::RangeFrogActionList::Damaged)
	{
		ResetState();

		return static_cast<int>(ACTION_RESULT::ABORT);
	}


	if (m_RangeFrogState->GetIsInAttack() || IsSelect)
	{
		// 상태에 따라 다르게 움직임
		SelectState();

		// 공격
		StateAttack();

		// 1.5초 대기
		StateWait();

		// 좌 우 랜덤 선택 후 이동
		StateRotateMove();

		// 뒤로 이동
		StateBackMove();

		return static_cast<int>(ACTION_RESULT::CONTINUE);

	}

	// 공격 범위 안에 없으면 따라가기
	if (m_RangeFrogState->GetIsInDetect() && !m_RangeFrogState->GetIsInAttack())
	{
		if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"move") != 0)
		{
			m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"move");
			m_RangeFrogState->SetNowAction(RangeFrogState::RangeFrogActionList::Move);
		}

		MoveFrog(true);

		return static_cast<int>(ACTION_RESULT::CONTINUE);

	}

	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int RangeFrogAction::Damaged()
{
	if (m_RangeFrogState->GetNowAction() == RangeFrogState::RangeFrogActionList::Damaged)
	{
		if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"damaged") != 0)
		{
			m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"damaged");
		}

		if (m_RangeFrogState->GetIsAnimationEnd())
		{
			m_RangeFrogState->SetNowAction(RangeFrogState::RangeFrogActionList::Idle);

			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::SUCCESS);
		}
	}

	return static_cast<int>(ACTION_RESULT::FAILURE);
}

void RangeFrogAction::MoveFrog(bool val)
{
	m_NavAgent->SetPosition();
	m_NavAgent->SetDestination(PlayerTransform->GetPosition());

	hos::Vector3 _RotDir = RangeFrogTransform->GetForward().Cross(m_NavAgent->GetDir());

	hos::Rotator _Rot = RangeFrogTransform->GetRotation();

	float _RotY = 0.0f;

	if (_RotDir.y < 0.f)
	{
		_RotY = _Rot.y;

		_RotY -= (float)Time->DeltaTime() * RotateSpeed * m_RangeFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		RangeFrogTransform->SetRotation(_Rot);
	}

	if (_RotDir.y >= 0.f)
	{
		_RotY = _Rot.y;

		_RotY += (float)Time->DeltaTime() * RotateSpeed * m_RangeFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		RangeFrogTransform->SetRotation(_Rot);
	}

	if (PlayerTransform->GetForward() == m_NavAgent->GetDir())
	{
		_RotY -= (float)Time->DeltaTime() * RotateSpeed * m_RangeFrogState->GetAnimationTimeScale();
	}

	if (val)
	{
		RangeFrogTransform->SetPosition(RangeFrogTransform->GetPosition() + (RangeFrogTransform->GetForward() * (float)Time->DeltaTime() * MoveSpeed * m_RangeFrogState->GetAnimationTimeScale()));

	}
	else
	{
		RangeFrogTransform->SetPosition(RangeFrogTransform->GetPosition() + (-RangeFrogTransform->GetForward() * (float)Time->DeltaTime() * MoveSpeed * m_RangeFrogState->GetAnimationTimeScale()));
	}
}

void RangeFrogAction::SelectState()
{
	if (!IsSelect)
	{
		if (m_NoteManager->GetIsChangeNote())
		{
			IsSelect = true;

			double _SelectMove = std::rand() / (double)RAND_MAX;
			_SelectMove *= 100.f;

			if (_SelectMove <= 10.f)
			{
				IsWait = true;
			}
			else if (_SelectMove <= 20.f)
			{
				IsBackMove = true;
			}
			else if (_SelectMove <= 30.f)
			{
				IsRotateMove = true;
			}
			else
			{
				IsAttack = true;
			}
		}
		else
		{
			if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"idle") != 0)
			{
				m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"idle");
			}
		}
	}

}

void RangeFrogAction::StateAttack()
{
	if (IsAttack)
	{
		if (m_RangeFrogState->GetNowAction() != RangeFrogState::RangeFrogActionList::Attack)
		{
			m_RangeFrogState->SetNowAction(RangeFrogState::RangeFrogActionList::Attack);
		}

		if (!IsAttackMove)
		{
			if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"move") != 0)
			{
				m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"move");

				if (m_RangeFrogState->GetIsInAvoid())
				{
					IsAttackAvoid = true;
				}

			}

			// 1 초간 플레이어 방향으로 이동
			if (AttackMoveTimer < AttackMoveTime)
			{
				if (IsAttackAvoid)
				{
					MoveFrog(false);
				}
				else
				{
					MoveFrog(true);
				}


				AttackMoveTimer += (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale();

				return;
			}
			else
			{
				IsAttackMove = true;
				IsAttackAvoid = false;
			}
		}
		else
		{
			// 멜로디
			// 발사

			if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"attack0") != 0)
			{
				m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"attack0");

				ActionSound->Stop();
				ActionSound->SetAudioClip(L"mob_range_attack0_100bpm");
				ActionSound->Play();

			}

			if (AttackTimer > AttackTime)
			{
				if (!IsFire)
				{
					IsFire = true;

					m_ProjectileManager->SetProjectile(RangeFrogTransform, 1);
				}
			}
			else
			{
				AttackTimer += (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale();
			}

			// 뒤로 밀려남
			if (IsFire)
			{
				if (AttackBackMoveTimer < AttackBackMoveTime)
				{
					RangeFrogTransform->SetPosition(RangeFrogTransform->GetPosition() + (-RangeFrogTransform->GetForward() * (float)Time->DeltaTime() * BackMoveSpeed * m_RangeFrogState->GetAnimationTimeScale()));

					AttackBackMoveTimer += (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale();
				}

			}

			if (m_RangeFrogState->GetIsAnimationEnd())
			{
				IsSelect = false;
				IsAttack = false;
				IsFire = false;
				IsAttackMove = false;
				AttackTimer = 0.f;
				AttackMoveTimer = 0.f;
				AttackBackMoveTimer = 0.f;
			}
		}
	}
}

void RangeFrogAction::StateBackMove()
{
	if (IsBackMove)
	{
		if (m_RangeFrogState->GetNowAction() != RangeFrogState::RangeFrogActionList::BackMove)
		{
			m_RangeFrogState->SetNowAction(RangeFrogState::RangeFrogActionList::BackMove);
		}

		if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"move") != 0)
		{
			m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"move");
		}

		if (BackMoveTimer < BackMoveTime)
		{
			MoveFrog(false);

			BackMoveTimer += (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale();
		}
		else
		{
			IsSelect = false;
			IsBackMove = false;
			BackMoveTimer = 0.f;
		}

	}
}

void RangeFrogAction::StateWait()
{
	if (IsWait)
	{

		if (m_RangeFrogState->GetNowAction() != RangeFrogState::RangeFrogActionList::Wait)
		{
			m_RangeFrogState->SetNowAction(RangeFrogState::RangeFrogActionList::Wait);
		}

		if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"idle") != 0)
		{
			m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"idle");
		}

		if (WaitTimer > WaitTime)
		{
			WaitTimer = 0.f;
			IsWait = false;
			IsSelect = false;
		}
		else
		{
			WaitTimer += (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale();

		}
	}
}

void RangeFrogAction::StateRotateMove()
{
	if (IsRotateMove)
	{
		if (m_RangeFrogState->GetNowAction() != RangeFrogState::RangeFrogActionList::RotateMove)
		{
			m_RangeFrogState->SetNowAction(RangeFrogState::RangeFrogActionList::RotateMove);
		}

		if (m_RangeFrogState->RangeFrogAnimation->GetAniClipName().compare(L"move") != 0)
		{
			m_RangeFrogState->RangeFrogAnimation->SetAnimation(L"rangefrog", L"move");
		}

		if (!IsSelectRotate)
		{
			IsSelectRotate = true;

			double _SelectMove = std::rand() / (double)RAND_MAX;
			_SelectMove *= 100.f;

			if (_SelectMove <= 50.f)
			{
				IsRight = true;
				RotateValue = RangeFrogTransform->GetRotation().y + 90;

			}
			else
			{
				RotateValue = RangeFrogTransform->GetRotation().y - 90;

				IsRight = false;
			}
		}

		if (!IsRotateEnd)
		{
			if (RotateTimer < RotateTime)
			{
				if (IsRight)
				{
					RangeFrogTransform->SetRotation(RangeFrogTransform->GetRotation() + hos::Rotator(0, RotateSpeed * 2 * (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale(), 0));
				}
				else
				{
					RangeFrogTransform->SetRotation(RangeFrogTransform->GetRotation() - hos::Rotator(0, RotateSpeed * 2 * (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale(), 0));
				}

				RotateTimer += (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale();

				return;

			}
			else
			{
				IsRotateEnd = true;
				RotateTimer = 0.f;
			}

		}

		if (RotateTimer < RotateMoveTime)
		{
			RangeFrogTransform->SetPosition(RangeFrogTransform->GetPosition() + (RangeFrogTransform->GetForward() * MoveSpeed * (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale()));

			RotateTimer += (float)Time->DeltaTime() * m_RangeFrogState->GetAnimationTimeScale();
		}
		else
		{
			IsSelect = false;
			IsRotateMove = false;
			IsSelectRotate = false;
			IsRotateEnd = false;
			RotateTimer = 0.f;
		}

	}
}

void RangeFrogAction::ResetState()
{
	IsSelect = false;

	IsWait = false;
	WaitTimer = 0.f;

	IsBackMove = false;
	BackMoveTimer = 0.f;

	IsRotateMove = false;
	IsSelectRotate = false;
	IsRight = false;
	IsRotateEnd = false;
	RotateValue = 0.f;
	RotateTimer = 0.f;

	IsAttack = false;
	IsFire = false;
	IsAttackMove = false;
	AttackMoveTimer = 0.f;
	AttackTimer = 0.f;
	AttackBackMoveTimer = 0.f;

}

void RangeFrogAction::AudioTimeScale()
{
	if (m_RangeFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
		m_RangeFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
		m_RangeFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
		m_RangeFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
	{
		float _Pitch = m_RangeFrogState->GetAnimationTimeScale() - 1.3f;

		ActionSound->SetPitch(_Pitch);
	}
	else
	{
		ActionSound->SetPitch(0.f);
	}

	if (m_RangeFrogState->m_PlayerState->GetIsFever())
	{
		float _Pitch = m_RangeFrogState->GetAnimationTimeScale() - 1.3f;

		ActionSound->SetPitch(_Pitch);
	}
}
