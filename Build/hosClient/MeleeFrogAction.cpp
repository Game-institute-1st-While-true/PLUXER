#include "MeleeFrogAction.h"

MeleeFrogAction::MeleeFrogAction() : hos::com::Script(L"MeleeFrogAction")
{
	ExecutionOrder = 6;
}

MeleeFrogAction::~MeleeFrogAction()
{
	RootNode->RemoveAllChild();
	delete RootNode;
	RootNode = nullptr;
}

void MeleeFrogAction::Reset()
{

}

void MeleeFrogAction::Awake()
{
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();
	m_MeleeFrogState = m_GameObject->GetComponent<MeleeFrogState>();
	m_NavAgent = m_GameObject->GetComponent<hos::com::NavAgent>();
	m_NavAgent->SetNavMeshId(m_GameObject->m_Scene->GetBuildIndex());
	m_NavAgent->RequestPathFind();

	PlayerTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<hos::com::Transform>();
	MeleeFrogTransform = m_GameObject->transform;

	AttackArea = m_GameObject->transform->GetChild(1)->m_GameObject->GetComponent<MeleeFrogAttackArea>();

	ActionSound = m_GameObject->GetComponent<hos::com::AudioSource>();

	//////////////////////////////////////////////////////////////////////////
	// Audio Source Data Load
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_melee_tailatk_100bpm.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_melee_headatk_100bpm.sclip");
	//g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_walk_combined.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_melee_dead.sclip");


	//////////////////////////////////////////////////////////////////////////

	RootNode = new BTRoot(this);
	StartSelector = new BTSelector(this);
	AttackSequence = new BTSequence(this);
	AttackSelector = new BTSelector(this);

	NodeIdle = new MeleeFrogIdle(this);
	NodeDamaged = new MeleeFrogDamaged(this);
	NodeDead = new MeleeFrogDead(this);
	NodeDetect = new MeleeFrogDetect(this);
	NodeAttack = new MeleeFrogAttack(this);
	NodeMove = new MeleeFrogMove(this);

	//////////////////////////////////////////////////////////////////////////

	RootNode->SetStartNode(StartSelector);

	StartSelector->AddChild(NodeDead);
	StartSelector->AddChild(NodeDamaged);
	StartSelector->AddChild(AttackSequence);
	StartSelector->AddChild(NodeIdle);

	AttackSequence->AddChild(NodeDetect);
	AttackSequence->AddChild(AttackSelector);

	AttackSelector->AddChild(NodeAttack);
	AttackSelector->AddChild(NodeMove);

}

void MeleeFrogAction::Start()
{
	AttackArea->AttackCollision->SetActive(false);
}

void MeleeFrogAction::Update()
{
	if (m_NoteManager->GetIsBGMStart())
	{
		// Behavior Tree 를 돌리자
		RootNode->Invoke();
	}

	AudioTimeScale();

}

void MeleeFrogAction::AudioTimeScale()
{
	if (m_MeleeFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
		m_MeleeFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
		m_MeleeFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
		m_MeleeFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
	{
		float _Pitch = m_MeleeFrogState->GetAnimationTimeScale() - 1.3f;

		ActionSound->SetPitch(_Pitch);
	}
	else
	{
		ActionSound->SetPitch(0.f);
	}

	if (m_MeleeFrogState->m_PlayerState->GetIsFever())
	{
		float _Pitch = m_MeleeFrogState->GetAnimationTimeScale() - 1.3f;

		ActionSound->SetPitch(_Pitch);
	}
}

//////////////////////////////////////////////////////////////////////////
/// 노드들

BTNode::SearchResult MeleeFrogIdle::Invoke()
{
	MeleeFrogAction* _MeleeFrogAction = dynamic_cast<MeleeFrogAction*>(BlackBoard);

	if (!_MeleeFrogAction)
	{
		return SearchResult::Failure;
	}

	if (_MeleeFrogAction->m_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Dead)
	{
		_MeleeFrogAction->m_MeleeFrogState->SetNowAction(MeleeFrogState::MeleeFrogActionList::Idle);
		_MeleeFrogAction->ActionSound->Stop();
	}


	return BTNode::SearchResult::Failure;
}

//////////////////////////////////////////////////////////////////////////

BTNode::SearchResult MeleeFrogDamaged::Invoke()
{
	MeleeFrogAction* _MeleeFrogAction = dynamic_cast<MeleeFrogAction*>(BlackBoard);

	if (!_MeleeFrogAction)
	{
		return SearchResult::Failure;
	}

	MeleeFrogState* _MeleeFrogState = _MeleeFrogAction->m_MeleeFrogState;

	if (_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Damaged)
	{
		if (_MeleeFrogState->GetIsAnimationEnd())
		{
			return BTNode::SearchResult::Failure;
		}
		else
		{
			return BTNode::SearchResult::Success;
		}
	}


	return BTNode::SearchResult::Failure;
}

//////////////////////////////////////////////////////////////////////////

BTNode::SearchResult MeleeFrogDead::Invoke()
{
	MeleeFrogAction* _MeleeFrogAction = dynamic_cast<MeleeFrogAction*>(BlackBoard);

	if (!_MeleeFrogAction)
	{
		return SearchResult::Failure;
	}

	MeleeFrogState* _MeleeFrogState = _MeleeFrogAction->m_MeleeFrogState;

	if (_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Dead)
	{
		if (DeadTimer > WaitDead)
		{
			_MeleeFrogAction->m_GameObject->SetActive(false);
		}
		else
		{
			DeadTimer += (float)Time->DeltaTime() * _MeleeFrogState->GetAnimationTimeScale();
		}

		if (!IsDead)
		{
			IsDead = true;
			_MeleeFrogAction->ActionSound->Stop();
			_MeleeFrogAction->ActionSound->SetLoop(false);
			_MeleeFrogAction->ActionSound->SetAudioClip(L"mob_melee_dead");
			_MeleeFrogAction->ActionSound->Play();
		}

		return BTNode::SearchResult::Success;
	}


	return BTNode::SearchResult::Failure;
}

//////////////////////////////////////////////////////////////////////////

BTNode::SearchResult MeleeFrogDetect::Invoke()
{
	MeleeFrogAction* _MeleeFrogAction = dynamic_cast<MeleeFrogAction*>(BlackBoard);

	if (!_MeleeFrogAction)
	{
		return SearchResult::Failure;
	}

	MeleeFrogState* _MeleeFrogState = _MeleeFrogAction->m_MeleeFrogState;

	if (_MeleeFrogState->GetIsInDetect())
	{
		return BTNode::SearchResult::Success;
	}


	return BTNode::SearchResult::Failure;
}

//////////////////////////////////////////////////////////////////////////

BTNode::SearchResult MeleeFrogAttack::Invoke()
{
	MeleeFrogAction* _MeleeFrogAction = dynamic_cast<MeleeFrogAction*>(BlackBoard);

	if (!_MeleeFrogAction)
	{
		return SearchResult::Failure;
	}

	MeleeFrogState* _MeleeFrogState = _MeleeFrogAction->m_MeleeFrogState;

	/// 현재 쿨타임 중인지
	if (_MeleeFrogAction->IsAttackCoolTime)
	{
		if (AttackCoolTimeTimer > AttackCoolTime)
		{
			_MeleeFrogAction->IsAttackCoolTime = false;
			NowAttackMotion = -1;
			AttackCoolTimeTimer = 0.f;
			AttackAnimationTimer = 0;
		}
		else
		{
			AttackCoolTimeTimer += (float)Time->DeltaTime() * _MeleeFrogState->GetAnimationTimeScale();
		}

		return SearchResult::Failure;
	}



	/// 사거리 안에 있는지 & 공격 선택
	if (_MeleeFrogState->GetIsInAttack())
	{
		// 노트가 바뀌는 시점에 공격
		if (_MeleeFrogAction->m_NoteManager->GetIsChangeNote())
		{
			if (_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Attak1 &&
				_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Attak2)
			{
				_MeleeFrogAction->AttackArea->AttackCollision->SetActive(false);
				_MeleeFrogAction->AttackArea->SetIsAttack(false);
				AttackAnimationTimer = 0;

				//std::srand(std::time(nullptr));
				
				float _SelectMotion = std::rand() / (float)RAND_MAX;
				_SelectMotion *= 100.f;

				if (_SelectMotion < 50.f)
				{
					NowAttackMotion = 0;
					_MeleeFrogState->SetNowAction(MeleeFrogState::MeleeFrogActionList::Attak1);
					_MeleeFrogAction->ActionSound->Stop();
					_MeleeFrogAction->ActionSound->SetLoop(false);
					_MeleeFrogAction->ActionSound->SetAudioClip(L"mob_melee_tailatk_100bpm");
					_MeleeFrogAction->ActionSound->Play();
				}
				else
				{
					NowAttackMotion = 1;
					_MeleeFrogState->SetNowAction(MeleeFrogState::MeleeFrogActionList::Attak2);
					_MeleeFrogAction->ActionSound->Stop();
					_MeleeFrogAction->ActionSound->SetLoop(false);
					_MeleeFrogAction->ActionSound->SetAudioClip(L"mob_melee_headatk_100bpm");
					_MeleeFrogAction->ActionSound->Play();
				}
			}
		}
	}


	/// 공격
	if (_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Attak1 ||
		_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Attak2)
	{
		if (_MeleeFrogState->GetIsAnimationEnd())
		{
			_MeleeFrogAction->IsAttackCoolTime = true;
			NowAttackMotion = -1;
			AttackCoolTimeTimer = 0.f;
			AttackAnimationTimer = 0.0f;
			_MeleeFrogAction->AttackArea->AttackCollision->SetActive(false);
			_MeleeFrogAction->AttackArea->SetIsAttack(false);

			return BTNode::SearchResult::Failure;
		}
		else
		{
			// 공격에 따라 다르게 행동한다.
			if (NowAttackMotion == 0)
			{
				// 꼬리 공격

				// 1 초 후 데미지박스를 전방에 띄우기
				if (AttackAnimationTimer > Attack1DamageEnable)
				{
					if (AttackAnimationTimer > Attack1DamageDisable)
					{
						_MeleeFrogAction->AttackArea->AttackCollision->SetActive(false);
					}
					else
					{
						if (!_MeleeFrogAction->AttackArea->GetIsAttack())
						{
							hos::Vector3 _tempPos = _MeleeFrogAction->MeleeFrogTransform->GetPosition() + (_MeleeFrogAction->MeleeFrogTransform->GetForward() * 1.5f);
							_tempPos.y += 1;

							_MeleeFrogAction->AttackArea->m_GameObject->transform->SetPosition(_tempPos);
							_MeleeFrogAction->AttackArea->m_GameObject->transform->SetRotation(_MeleeFrogAction->MeleeFrogTransform->GetRotation());
							_MeleeFrogAction->AttackArea->AttackCollision->SetActive(true);
						}
						else
						{
							_MeleeFrogAction->AttackArea->AttackCollision->SetActive(false);
						}
					}
				}

			}
			if(NowAttackMotion == 1)
			{
				// 몸통 박치기

				// 1 초 후 데미지박스를 띄우기 및 이동
				if (AttackAnimationTimer > Attack2DamageEnable&&
					AttackAnimationTimer < Attack2DamageDisable)
				{
					// 전방으로 돌진
					_MeleeFrogAction->MeleeFrogTransform->SetPosition(_MeleeFrogAction->MeleeFrogTransform->GetPosition() + (_MeleeFrogAction->MeleeFrogTransform->GetForward() * AttackMoveDistance * ((float)Time->DeltaTime() * _MeleeFrogState->GetAnimationTimeScale() * 4)));

					if (!_MeleeFrogAction->AttackArea->GetIsAttack())
					{
						hos::Vector3 _tempPos = _MeleeFrogAction->MeleeFrogTransform->GetPosition();
						_tempPos.y += 1;

						_MeleeFrogAction->AttackArea->m_GameObject->transform->SetPosition(_tempPos);
						_MeleeFrogAction->AttackArea->m_GameObject->transform->SetRotation(_MeleeFrogAction->MeleeFrogTransform->GetRotation());
						_MeleeFrogAction->AttackArea->AttackCollision->SetActive(true);
					}
					else
					{
						_MeleeFrogAction->AttackArea->AttackCollision->SetActive(false);
					}
				}
				else
				{
					_MeleeFrogAction->AttackArea->AttackCollision->SetActive(false);
				}
			}

			AttackAnimationTimer += (float)Time->DeltaTime() * _MeleeFrogState->GetAnimationTimeScale();

			return BTNode::SearchResult::Success;
		}
	}



	return BTNode::SearchResult::Failure;
}

//////////////////////////////////////////////////////////////////////////

BTNode::SearchResult MeleeFrogMove::Invoke()
{
	MeleeFrogAction* _MeleeFrogAction = dynamic_cast<MeleeFrogAction*>(BlackBoard);

	if (!_MeleeFrogAction)
	{
		return SearchResult::Failure;
	}

	MeleeFrogState* _MeleeFrogState = _MeleeFrogAction->m_MeleeFrogState;

	if (_MeleeFrogAction->m_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Dead)
	{
		if (_MeleeFrogAction->m_MeleeFrogState->GetNowAction() != MeleeFrogState::MeleeFrogActionList::Move)
		{
			_MeleeFrogAction->m_MeleeFrogState->SetNowAction(MeleeFrogState::MeleeFrogActionList::Move);

			//_MeleeFrogAction->ActionSound->Stop();
			//_MeleeFrogAction->ActionSound->SetLoop(true);
			//_MeleeFrogAction->ActionSound->SetAudioClip(L"mob_walk_combined");
			//_MeleeFrogAction->ActionSound->Play();

		}
	}
	else
	{
		return SearchResult::Failure;
	}

	hos::com::Transform* _MeleeFrogTransform = _MeleeFrogAction->MeleeFrogTransform;

	hos::com::NavAgent* _NavAgent = _MeleeFrogAction->m_NavAgent;

	/// 바라보고 있는지 확인 & 회전

	_NavAgent->SetPosition();
	_NavAgent->SetDestination(_MeleeFrogAction->PlayerTransform->GetPosition());

	hos::Vector3 _RotDir = _MeleeFrogTransform->GetForward().Cross(_NavAgent->GetDir());

	hos::Rotator _Rot = _MeleeFrogTransform->GetRotation();

	float _RotY = 0.0f;

	if (_RotDir.y < 0.f)
	{
		_RotY = _Rot.y;

		_RotY -= (float)Time->DeltaTime() * RotateSpeed * _MeleeFrogAction->m_MeleeFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		_MeleeFrogTransform->SetRotation(_Rot);
	}

	if (_RotDir.y >= 0.f)
	{
		_RotY = _Rot.y;

		_RotY += (float)Time->DeltaTime() * RotateSpeed * _MeleeFrogAction->m_MeleeFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		_MeleeFrogTransform->SetRotation(_Rot);
	}

	if (_MeleeFrogAction->PlayerTransform->GetForward() == _NavAgent->GetDir())
	{
		_RotY -= (float)Time->DeltaTime() * RotateSpeed * _MeleeFrogAction->m_MeleeFrogState->GetAnimationTimeScale();
	}


	/// 앞으로 이동
	if (_MeleeFrogAction->IsAttackCoolTime)
	{
		if (_MeleeFrogState->GetPrevAction() == MeleeFrogState::MeleeFrogActionList::Attak1)
		{
			_MeleeFrogTransform->SetPosition(_MeleeFrogTransform->GetPosition() + (-_MeleeFrogTransform->GetForward() * (float)Time->DeltaTime() * MoveSpeed * _MeleeFrogAction->m_MeleeFrogState->GetAnimationTimeScale()));
		}
		else
		{
			_MeleeFrogTransform->SetPosition(_MeleeFrogTransform->GetPosition() + (_MeleeFrogTransform->GetForward() * (float)Time->DeltaTime() * MoveSpeed * _MeleeFrogAction->m_MeleeFrogState->GetAnimationTimeScale()));
		}
	}
	else
	{
		_MeleeFrogTransform->SetPosition(_MeleeFrogTransform->GetPosition() + (_MeleeFrogTransform->GetForward() * (float)Time->DeltaTime() * MoveSpeed * _MeleeFrogAction->m_MeleeFrogState->GetAnimationTimeScale()));
	}

	return BTNode::SearchResult::Success;
}
