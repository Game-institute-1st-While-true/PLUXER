#include "MeleeFrog.h"

MeleeFrog::MeleeFrog() : hos::com::Script(L"MeleeFrog")
{
	ExecutionOrder = 6;
	Bt = new BehaviorTree();
}

MeleeFrog::~MeleeFrog()
{
	hos::SafeDelete(Bt);
}

void MeleeFrog::Reset()
{
}

void MeleeFrog::Awake()
{
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();
	m_MeleeFrogState = m_GameObject->GetComponent<MeleeFrogState>();
	m_NavAgent = m_GameObject->GetComponent<hos::com::NavAgent>();
	m_NavAgent->SetNavMeshId(m_GameObject->m_Scene->GetBuildIndex());
	m_NavAgent->RequestPathFind();

	PlayerTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<hos::com::Transform>();
	MeleeFrogTransform = m_GameObject->transform;

	AttackArea = m_GameObject->transform->GetChild(1)->m_GameObject->GetComponent<MeleeFrogAttackArea>();

	SelectNode* start_select = new SelectNode();
	SequenceNode* att_seq = new SequenceNode();
	SelectNode* att_select = new SelectNode();

	BehaviorTask* dead = new BehaviorTask();
	BehaviorTask* demeaged = new BehaviorTask();
	BehaviorTask* idle = new BehaviorTask();
	BehaviorTask* detect = new BehaviorTask();
	BehaviorTask* attack = new BehaviorTask();
	BehaviorTask* move = new BehaviorTask();

	dead->Func = [=]() {return this->Dead(); };
	demeaged->Func = [=]() {return this->Damaged(); };
	idle->Func = [=]() {return this->Idle(); };
	detect->Func = [=]() {return this->Detect(); };
	attack->Func = [=]() {return this->Attack(); };
	move->Func = [=]() {return this->Move(); };

	Bt->Insert(start_select, static_cast<int>(ACTION_TYPE::ROOT));			 // 1

	Bt->Insert(dead, static_cast<int>(ACTION_TYPE::START_SELECT));			 // 2
	Bt->Insert(demeaged, static_cast<int>(ACTION_TYPE::START_SELECT));		 // 3
	Bt->Insert(att_seq, static_cast<int>(ACTION_TYPE::START_SELECT));		 // 4
	Bt->Insert(idle, static_cast<int>(ACTION_TYPE::START_SELECT));			 // 5

	Bt->Insert(detect, static_cast<int>(ACTION_TYPE::ATT_SEQ));				 // 6
	Bt->Insert(att_select, static_cast<int>(ACTION_TYPE::ATT_SEQ));			 // 7

	Bt->Insert(attack, static_cast<int>(ACTION_TYPE::ATT_SELECT));			 // 8
	Bt->Insert(move, static_cast<int>(ACTION_TYPE::ATT_SELECT));			 // 9
}

void MeleeFrog::Start()
{
	AttackArea->AttackCollision->SetActive(false);
}

void MeleeFrog::Update()
{
	if (m_NoteManager->GetIsBGMStart())
	{
		Bt->Run();
	}
}

int MeleeFrog::Idle()
{
	if (m_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Dead)
	{
		m_MeleeFrogState->SetNowAction(MeleeFrogState::MeleeFrogActionList::Idle);
	}
	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int MeleeFrog::Damaged()
{
	if (m_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Damaged)
	{
		if (m_MeleeFrogState->GetIsAnimationEnd())
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
	}
	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int MeleeFrog::Dead()
{
	if (m_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Dead)
	{
		if (DeadTimer > WaitDead)
		{
			m_GameObject->SetActive(false);
		}
		else
		{
			DeadTimer += (float)Time->DeltaTime() * m_MeleeFrogState->GetAnimationTimeScale();
		}
		return static_cast<int>(ACTION_RESULT::SUCESS);
	}
	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int MeleeFrog::Detect()
{
	if (m_MeleeFrogState->GetIsInDetect())
	{
		return static_cast<int>(ACTION_RESULT::SUCESS);
	}
	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int MeleeFrog::Attack()
{
	/// 현재 쿨타임 중인지
	if (IsAttackCoolTime)
	{
		if (AttackCoolTimeTimer > AttackCoolTime)
		{
			IsAttackCoolTime = false;
			NowAttackMotion = -1;
			AttackCoolTimeTimer = 0.f;
			AttackAnimationTimer = 0;
		}
		else
		{
			AttackCoolTimeTimer += (float)Time->DeltaTime() * m_MeleeFrogState->GetAnimationTimeScale();
		}
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	/// 사거리 안에 있는지 & 공격 선택
	if (m_MeleeFrogState->GetIsInAttack())
	{
		// 노트가 바뀌는 시점에 공격
		if (m_NoteManager->GetIsChangeNote())
		{
			if (m_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Attak1 &&
				m_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Attak2)
			{
				AttackArea->AttackCollision->SetActive(false);
				AttackArea->SetIsAttack(false);
				AttackAnimationTimer = 0;

				//std::srand(std::time(nullptr));
				NowAttackMotion = std::rand() / ((RAND_MAX + 1u) / 4);

				if (NowAttackMotion < 2)
				{
					m_MeleeFrogState->SetNowAction(MeleeFrogState::MeleeFrogActionList::Attak1);
				}
				else
				{
					m_MeleeFrogState->SetNowAction(MeleeFrogState::MeleeFrogActionList::Attak2);
				}
			}
		}
	}

	/// 공격
	if (m_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Attak1 ||
		m_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Attak2)
	{
		if (m_MeleeFrogState->GetIsAnimationEnd())
		{
			IsAttackCoolTime = true;
			NowAttackMotion = -1;
			AttackCoolTimeTimer = 0.f;
			AttackAnimationTimer = 0.0f;
			AttackArea->AttackCollision->SetActive(false);
			AttackArea->SetIsAttack(false);
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
		else
		{
			// 공격에 따라 다르게 행동한다.
			if (NowAttackMotion < 2)
			{
				// 꼬리 공격

				// 1 초 후 데미지박스를 전방에 띄우기
				if (AttackAnimationTimer > Attack1DamageEnable)
				{
					if (AttackAnimationTimer > Attack1DamageDisable)
					{
						AttackArea->AttackCollision->SetActive(false);
					}
					else
					{
						if (!AttackArea->GetIsAttack())
						{
							hos::Vector3 _tempPos = MeleeFrogTransform->GetPosition() + (MeleeFrogTransform->GetForward() * 1.75f);
							_tempPos.y += 1;

							AttackArea->m_GameObject->transform->SetPosition(_tempPos);
							AttackArea->m_GameObject->transform->SetRotation(MeleeFrogTransform->GetRotation());
							AttackArea->AttackCollision->SetActive(true);
						}
						else
						{
							AttackArea->AttackCollision->SetActive(false);
						}
					}
				}

			}
			else
			{
				// 몸통 박치기

				// 1 초 후 데미지박스를 띄우기 및 이동
				if (AttackAnimationTimer > Attack2DamageEnable && AttackAnimationTimer < Attack2DamageDisable)
				{
					// 전방으로 돌진
					MeleeFrogTransform->SetPosition(MeleeFrogTransform->GetPosition() + (MeleeFrogTransform->GetForward() * AttackMoveDistance * ((float)Time->DeltaTime() * m_MeleeFrogState->GetAnimationTimeScale() * 4)));

					if (!AttackArea->GetIsAttack())
					{
						hos::Vector3 _tempPos = MeleeFrogTransform->GetPosition();
						_tempPos.y += 1;

						AttackArea->m_GameObject->transform->SetPosition(_tempPos);
						AttackArea->m_GameObject->transform->SetRotation(MeleeFrogTransform->GetRotation());
						AttackArea->AttackCollision->SetActive(true);
					}
					else
					{
						AttackArea->AttackCollision->SetActive(false);
					}
				}
				else
				{
					AttackArea->AttackCollision->SetActive(false);
				}
			}

			AttackAnimationTimer += (float)Time->DeltaTime() * m_MeleeFrogState->GetAnimationTimeScale();
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
	}
	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int MeleeFrog::Move()
{


	if (m_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Dead)
	{
		if (m_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Move)
		{
			m_MeleeFrogState->SetNowAction(MeleeFrogState::MeleeFrogActionList::Move);
		}
	}
	else
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	hos::com::Transform* _MeleeFrogTransform = MeleeFrogTransform;
	hos::com::NavAgent* _NavAgent = m_NavAgent;

	/// 바라보고 있는지 확인 & 회전

	_NavAgent->SetPosition();
	_NavAgent->SetDestination(PlayerTransform->GetPosition());

	hos::Vector3 _RotDir = _MeleeFrogTransform->GetForward().Cross(_NavAgent->GetDir());

	hos::Rotator _Rot = _MeleeFrogTransform->GetRotation();

	float _RotY = 0.0f;

	if (_RotDir.y < 0.f)
	{
		_RotY = _Rot.y;

		_RotY -= (float)Time->DeltaTime() * RotateSpeed * m_MeleeFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		_MeleeFrogTransform->SetRotation(_Rot);
	}

	if (_RotDir.y >= 0.f)
	{
		_RotY = _Rot.y;

		_RotY += (float)Time->DeltaTime() * RotateSpeed * m_MeleeFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		_MeleeFrogTransform->SetRotation(_Rot);
	}

	if (PlayerTransform->GetForward() == _NavAgent->GetDir())
	{
		_RotY -= (float)Time->DeltaTime() * RotateSpeed * m_MeleeFrogState->GetAnimationTimeScale();
	}


	/// 앞으로 이동
	if (IsAttackCoolTime)
	{
		_MeleeFrogTransform->SetPosition(_MeleeFrogTransform->GetPosition() + (-_MeleeFrogTransform->GetForward() * (float)Time->DeltaTime() * MoveSpeed * m_MeleeFrogState->GetAnimationTimeScale()));
	}
	else
	{
		_MeleeFrogTransform->SetPosition(_MeleeFrogTransform->GetPosition() + (_MeleeFrogTransform->GetForward() * (float)Time->DeltaTime() * MoveSpeed * m_MeleeFrogState->GetAnimationTimeScale()));
	}

	return static_cast<int>(ACTION_RESULT::SUCESS);
}
