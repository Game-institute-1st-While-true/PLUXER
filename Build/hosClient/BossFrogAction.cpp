#include "BossFrogAction.h"

BossFrogAction::BossFrogAction() : hos::com::Script(L"BossFrogAction")
{
	ExecutionOrder = 6;
	Bt = new BehaviorTree();
}

BossFrogAction::~BossFrogAction()
{
	hos::SafeDelete(Bt);
}

void BossFrogAction::Reset()
{

}

void BossFrogAction::Awake()
{
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();
	m_BossFrogState = m_GameObject->GetComponent<BossFrogState>();
	m_NavAgent = m_GameObject->GetComponent<hos::com::NavAgent>();
	m_NavAgent->SetNavMeshId(m_GameObject->m_Scene->GetBuildIndex());
	m_NavAgent->RequestPathFind();

	PlayerTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<hos::com::Transform>();
	BossFrogTransform = m_GameObject->transform;
	m_PlayerState = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerState>();

	AttackArea = m_GameObject->transform->GetChild(1)->m_GameObject->GetComponent<BossFrogAttackArea>();

	m_ProjectileManager = m_GameObject->m_Scene->FindGameObjectWithName(L"ProjectileManager")->GetComponent<ProjectileManager>();

	/// ����
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\boss_atk_jump_combined_110bpm.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\boss_atk_rush_110bpm_withmelody.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\boss_atk_tongue_combined_110bpm.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\boss_atk_projectile_1_melodyready_110bpm.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\boss_atk_projectile_2_cast.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\boss_die.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\boss_backjump_110bpm.sclip");
	//g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\boss_walk_combined_110bpm.sclip");

	MelodySound = m_GameObject->GetComponents<hos::com::AudioSource>()[0];
	MelodySound->SetAudioClip(L"boss_atk_jump_combined_110bpm");
	MelodySound->SetPlayOnAwake(false);
	MelodySound->SetLoop(false);
	MelodySound->SetIs3D(true);
	ReadySound = m_GameObject->GetComponents<hos::com::AudioSource>()[1];
	ReadySound->SetAudioClip(L"boss_atk_projectile_2_cast");
	ReadySound->SetPlayOnAwake(false);
	ReadySound->SetLoop(false);
	ReadySound->SetIs3D(true);

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////// SelectNode();

	SelectNode* start_select = new SelectNode();

	SelectNode* on_ai_select = new SelectNode();
	SelectNode* att_select = new SelectNode();
	SelectNode* move_select = new SelectNode();

	BehaviorTask* dead = new BehaviorTask();
	BehaviorTask* idle = new BehaviorTask();
	BehaviorTask* move = new BehaviorTask();
	BehaviorTask* rotate_to_target = new BehaviorTask();
	BehaviorTask* tongue_att = new BehaviorTask();
	BehaviorTask* jump_att = new BehaviorTask();
	BehaviorTask* range_att = new BehaviorTask();
	BehaviorTask* rush_att = new BehaviorTask();
	BehaviorTask* back_move = new BehaviorTask();
	BehaviorTask* back_jump = new BehaviorTask();

	dead->Func = [=]() {return this->Dead(); };
	idle->Func = [=]() {return this->Idle(); };
	move->Func = [=]() {return this->Move(); };
	rotate_to_target->Func = [=]() {return this->RotateToTarget(); };
	tongue_att->Func = [=]() {return this->TongueAttack(); };
	jump_att->Func = [=]() {return this->JumpAttack(); };
	range_att->Func = [=]() {return this->RangeAttack(); };
	rush_att->Func = [=]() {return this->RushAttack(); };
	back_move->Func = [=]() {return this->BackMove(); };
	back_jump->Func = [=]() {return this->BackJump(); };

	Bt->Insert(start_select, static_cast<int>(ON_AI_TYPE::ROOT));					 // 1

	Bt->Insert(on_ai_select, static_cast<int>(ON_AI_TYPE::START_SELECT));			 // 2

	Bt->Insert(dead, static_cast<int>(ON_AI_TYPE::ON_AI_SELECT));					 // 3
	Bt->Insert(att_select, static_cast<int>(ON_AI_TYPE::ON_AI_SELECT));				 // 4
	Bt->Insert(move_select, static_cast<int>(ON_AI_TYPE::ON_AI_SELECT));			 // 5
	Bt->Insert(idle, static_cast<int>(ON_AI_TYPE::ON_AI_SELECT));					 // 6

	Bt->Insert(rush_att, static_cast<int>(ON_AI_TYPE::ATTACK_SELECT));				 // 7
	Bt->Insert(range_att, static_cast<int>(ON_AI_TYPE::ATTACK_SELECT));				 // 8
	Bt->Insert(jump_att, static_cast<int>(ON_AI_TYPE::ATTACK_SELECT));				 // 9
	Bt->Insert(tongue_att, static_cast<int>(ON_AI_TYPE::ATTACK_SELECT));			 // 10

	Bt->Insert(back_jump, static_cast<int>(ON_AI_TYPE::MOVE_SELECT));				 // 11
	Bt->Insert(rotate_to_target, static_cast<int>(ON_AI_TYPE::MOVE_SELECT));		 // 12
	Bt->Insert(back_move, static_cast<int>(ON_AI_TYPE::MOVE_SELECT));				 // 13
	Bt->Insert(move, static_cast<int>(ON_AI_TYPE::MOVE_SELECT));					 // 14


	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	BossNetBehaivortree();
}

void BossFrogAction::Start()
{
	AttackArea->AttackCollision->SetActive(false);
}

void BossFrogAction::Update()
{
	if (m_NoteManager->GetIsBGMStart())
	{
		Bt->Run();
	}

	if (nullptr == m_PlayerState->NetEventer)
	{
		AudioTimeScale();
	}
}

int BossFrogAction::Idle()
{
	if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::Dead)
	{
		m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::Idle);

		if (m_BossFrogState->GetPrevAction() != BossFrogState::BossFrogActionList::Move)
		{

			// ��� ���
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"idle") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"idle");
			}


			// Wait �� ����
			if (!m_BossFrogState->BossFrogAnimation->GetIsAnimationEnd())
			{
				BossFrogTransform->SetPosition(BossFrogTransform->GetPosition());

				return static_cast<int>(ACTION_RESULT::PROCESS);
			}

			MelodySound->Stop();

		}

		// �̵��� �߾����� Ȯ��
		if (!IsMove)
		{

			// �ʹ� �ָ� �̵� ���� 
			float _Length = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length();
			if (_Length <= 30.f)
			{
				// �������� �̵� ����
				m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::Move);

				IsSetMoveTime = false;
			}

			return static_cast<int>(ACTION_RESULT::FAILURE);

		}

		// ���� ��ȯ
		ChangeState();

		// 

		IsMove = false;

		return static_cast<int>(ACTION_RESULT::SUCESS);
	}

	return static_cast<int>(ACTION_RESULT::AVOID);
}

int BossFrogAction::Dead()
{
	if (m_BossFrogState->GetAiType() != 0)
	{
		return static_cast<int>(ACTION_RESULT::SUCESS);
	}

	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"die") != 0)
		{
			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"die", 0, true);

			AttackArea->AttackCollision->SetActive(false);

			m_GameObject->GetComponent<hos::com::CapsuleCollision>()->SetActive(false);

			MelodySound->SetLoop(false);
			MelodySound->Stop();
			MelodySound->SetAudioClip(L"boss_die");
			MelodySound->Play();

		}

		if (WaitDead < DeadTimer)
		{
			m_GameObject->SetActive(false);
		}
		else
		{
			DeadTimer += (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale();
		}

		return static_cast<int>(ACTION_RESULT::SUCESS);
	}
	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int BossFrogAction::Move()
{
	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::Move)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	// �̵��ߴ��� Ȯ��
	if (IsMove)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_forward") != 0)
	{
		m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_forward");
	}

	// �̵� �ð� ���ϱ�
	if (!IsMove && !IsSetMoveTime)
	{
		MoveTime = (float)(std::rand() / ((RAND_MAX + 1u) / 3)) + 2;

		MoveTimer = 0.0f;

		IsSetMoveTime = true;

		//MelodySound->Stop();
		//MelodySound->SetAudioClip(L"boss_walk_combined_110bpm");
		//MelodySound->Play();

	}

	// �ð��� ������ �̵� ������ ����
	if (IsSetMoveTime)
	{
		MoveTimer += (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale();
	}

	if (MoveTime < MoveTimer)
	{
		IsMove = true;
	}

	m_NavAgent->SetPosition();
	m_NavAgent->SetDestination(PlayerTransform->GetPosition());


	hos::Vector3 _RotDir = BossFrogTransform->GetForward().Cross(m_NavAgent->GetDir());

	hos::Rotator _Rot = BossFrogTransform->GetRotation();

	float _RotY = 0.0f;

	if (_RotDir.y < -0.01f)
	{
		_RotY = _Rot.y;

		_RotY -= (float)Time->DeltaTime() * RotSpeed * m_BossFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		BossFrogTransform->SetRotation(_Rot);
	}

	if (_RotDir.y > 0.01f)
	{
		_RotY = _Rot.y;

		_RotY += (float)Time->DeltaTime() * RotSpeed * m_BossFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		BossFrogTransform->SetRotation(_Rot);
	}

	hos::Vector3 dir = BossFrogTransform->GetForward();

	m_BossFrogState->SetDirection(dir, true);
	BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (dir * (float)Time->DeltaTime() * MoveSpeed * m_BossFrogState->GetAnimationTimeScale()));

	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrogAction::RotateToTarget()
{
	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::RotateToTarget)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	// �÷��̾ ���ʿ� �ֳ� �����ʿ� �ֳ� Ȯ��
	if (!CheckRotateDir)
	{
		CheckRotateDir = true;

		hos::Vector3 _RotDir = BossFrogTransform->GetForward().Cross(PlayerTransform->GetForward());

		if (_RotDir.y < 0.f)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_left") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_left");
			}

			RotateMoveDir = true;
		}
		else
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_right") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_right");
			}

			RotateMoveDir = false;
		}

		//MelodySound->Stop();
		//MelodySound->SetAudioClip(L"boss_walk_combined_110bpm");
		//MelodySound->Play();

	}

	// �ش� �������� 2.5�� ���� �̵�
	// �÷��̾ �ٶ󺸸� ȸ���� �Ѵ�

	hos::Rotator _Rot = BossFrogTransform->GetRotation();

	float _RotY = 0.0f;

	if (RotateMoveTime > RotateMoveTimer)
	{
		if (RotateMoveDir)
		{
			_RotY = _Rot.y;

			_RotY -= (float)Time->DeltaTime() * (RotSpeed * 0.5f) * m_BossFrogState->GetAnimationTimeScale();

			_Rot.y = _RotY;

			BossFrogTransform->SetRotation(_Rot);

			BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (BossFrogTransform->GetLeft() * MoveSpeed * (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale()));


		}
		else
		{
			_RotY = _Rot.y;

			_RotY += (float)Time->DeltaTime() * (RotSpeed * 0.5f) * m_BossFrogState->GetAnimationTimeScale();

			_Rot.y = _RotY;

			BossFrogTransform->SetRotation(_Rot);

			BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (-BossFrogTransform->GetLeft() * MoveSpeed * (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale()));
		}

		RotateMoveTimer += (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale();

	}
	else
	{
		CheckRotateDir = false;

		RotateMoveTimer = 0;

		//m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::Idle);

		return static_cast<int>(ACTION_RESULT::FAILURE);
	}


	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrogAction::TongueAttack()
{
	/// ���߿� �����غ���
	// ���� ���°� ȸ�� �̵��� �ƴϸ� ȸ�� �̵� ���·� ��ȯ

	// ���� ���°� ȸ�� �̵��̸� ����

	//////////////////////////////////////////////////////////////////////////

	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}

	else if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::TongueAttack)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	// ���� ���� ��� �� �ִϸ��̼�

	// ���� ���
	// 2 �ʰ� ��ε� ���� ���, ����� Idle ��
	// return static_cast<int>(ACTION_RESULT::Process);


	if (TongueAttackReadyTimer < TongueAttackReadyTime)
	{
		if (!IsTongueAttackReady)
		{
			IsTongueAttackReady = true;

			// ȸ������ �����غ���

			//if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"idle") != 0)
			//{
			//	m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"idle", 0);
			//}

			MelodySound->Stop();
			MelodySound->SetAudioClip(L"boss_atk_tongue_combined_110bpm");
			MelodySound->Play();



			m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Yellow);

		}

		RotateToPlayer();

		TongueAttackReadyTimer += (float)Time->DeltaTime();

		return static_cast<int>(ACTION_RESULT::PROCESS);
	}


	// ���� ����

	if (!IsSetTongueAttack)
	{
		IsSetTongueAttack = true;

		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk2_tongue_combined") != 0)
		{
			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk2_tongue_combined", 0);
		}

		m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);

		AttackArea->AttackCollision->SetActive(false);

		AttackArea->m_GameObject->transform->SetPosition(BossFrogTransform->GetPosition() + (BossFrogTransform->GetForward() * 4));

		hos::Vector3 temp(0, 2, 0.5);
		AttackArea->AttackCollision->SetCenter(temp);

		temp = hos::Vector3(7, 4, 5);
		AttackArea->AttackCollision->SetSize(temp);

		TongueAttackTimer = 0.f;
	}

	if (m_BossFrogState->BossFrogAnimation->GetIsAnimationEnd())
	{
		AttackArea->AttackCollision->SetActive(false);

		IsSetTongueAttack = false;

		TongueAttackReadyTimer = 0.f;
		IsTongueAttackReady = false;

		m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Green);

		ChangeState();


		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		if (TongueAttackTimer > TongueAttackEnable)
		{
			// ���� ���� Ȱ��ȭ
			AttackArea->AttackCollision->SetActive(true);
		}

		if (TongueAttackTimer > TongueAttackDisable)
		{
			// ���� ���� ��Ȱ��ȭ
			AttackArea->AttackCollision->SetActive(false);
		}

		TongueAttackTimer += (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale();

		return static_cast<int>(ACTION_RESULT::PROCESS);

	}
	return static_cast<int>(ACTION_RESULT::FAILURE);

}

int BossFrogAction::JumpAttack()
{
	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::JumpAttack)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	hos::Vector3 _temp = BossFrogTransform->GetPosition();

	_temp.y = _temp.y + 1;

	// ���ʿ� ���� �ִ��� Ȯ��
	if (g_Px)
	{
		if (m_GameObject->m_Scene->RayCast(_temp, BossFrogTransform->GetForward(), 4.f))
		{
			// ���� ĳ��Ʈ�� �Ÿ��� ª���� ���Ÿ� �������� ��ȯ
			if (!IsSetJumpAttack)
			{
				JumpAttackTimer = 0.f;

				m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::RangeAttack);

				return static_cast<int>(ACTION_RESULT::AVOID);
			}
		}
	}

	/// �ϴ� ��ŵ
	// �ƴϸ� ȸ�� �̵� ���·� ��ȯ
	// ȸ�� �̵��� �����ٸ� ���� ���� ����

	if (JumpAttackReadyTimer < JumpAttackReadyTime)
	{
		if (!IsJumpAttackReady)
		{
			IsJumpAttackReady = true;

			// ȸ������ �����غ���

			//if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"idle") != 0)
			//{
			//	m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"idle", 0);
			//}

			MelodySound->Stop();
			MelodySound->SetAudioClip(L"boss_atk_jump_combined_110bpm");
			MelodySound->Play();

			m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Yellow);

		}

		RotateToPlayer();

		JumpAttackReadyTimer += (float)Time->DeltaTime();

		return static_cast<int>(ACTION_RESULT::PROCESS);
	}

	// �÷��̾�� ������ ��ŭ�� �ӵ��� �̵�
	if (!IsSetJumpAttack)
	{
		IsSetJumpAttack = true;

		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk1_jump_combined") != 0)
		{
			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk1_jump_combined", 0);
		}

		m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);

		AttackArea->AttackCollision->SetActive(false);

		AttackArea->m_GameObject->transform->SetLocalPosition(hos::Vector3::Zero);

		hos::Vector3 temp(0, 2, 0);
		AttackArea->AttackCollision->SetCenter(temp);

		temp = hos::Vector3(7, 4, 7);
		AttackArea->AttackCollision->SetSize(temp);

		JumpAttackTimer = 0.f;

		JumpAttackMoveSpeed = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length() * 2;

	}

	if (m_BossFrogState->BossFrogAnimation->GetIsAnimationEnd())
	{
		AttackArea->AttackCollision->SetActive(false);

		IsSetJumpAttack = false;

		JumpAttackTimer = 0.f;

		IsJumpAttackReady = false;

		m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Green);

		ChangeState();

		JumpAttackReadyTimer = 0.f;

		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		if (JumpAttackTimer > JumpAttackMove && JumpAttackTimer < JumpAttackEnable)
		{
			m_BossFrogState->m_GameObject->GetComponent<hos::com::CapsuleCollision>()->SetActive(false);

			if (!m_GameObject->m_Scene->RayCast(_temp, BossFrogTransform->GetForward(), 1.f))
			{
				BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (BossFrogTransform->GetForward() * JumpAttackMoveSpeed * (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale()));
			}
			else
			{
				BossFrogTransform->SetPosition(BossFrogTransform->GetPosition());
			}
		}

		if (JumpAttackTimer > JumpAttackEnable && JumpAttackTimer < JumpAttackDisable)
		{
			AttackArea->AttackCollision->SetActive(true);
			BossFrogTransform->SetPosition(BossFrogTransform->GetPosition());

			float _Distance = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length();

			if (_Distance < JumpAttackShakeDistance)
			{
				PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(true);
				PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeTime(JumpAttackShakeTime);

				if (_Distance <= 2)
				{
					_Distance = 2.f;
				}

				JumpAttackShakeValue = 1 / _Distance;

				PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeValue(JumpAttackShakeValue);
			}

		}

		if (JumpAttackTimer > JumpAttackDisable)
		{
			AttackArea->AttackCollision->SetActive(false);
			m_BossFrogState->m_GameObject->GetComponent<hos::com::CapsuleCollision>()->SetActive(true);
			BossFrogTransform->SetPosition(BossFrogTransform->GetPosition());
		}

		JumpAttackTimer += (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale();
		return static_cast<int>(ACTION_RESULT::PROCESS);
	}
}

int BossFrogAction::RangeAttack()
{
	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::RangeAttack)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	if (RangeAttackReadyTimer < RangeAttackReadyTime)
	{
		if (!IsRangeAttackReady)
		{
			IsRangeAttackReady = true;

			// ȸ������ �����غ���

			//if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"idle") != 0)
			//{
			//	m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"idle", 0);
			//}

			MelodySound->Stop();
			MelodySound->SetLoop(false);
			MelodySound->SetAudioClip(L"boss_atk_projectile_1_melodyready_110bpm");
			MelodySound->Play();

			m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Yellow);

		}
		RotateToPlayer();

		RangeAttackReadyTimer += (float)Time->DeltaTime();

		return static_cast<int>(ACTION_RESULT::PROCESS);
	}

	if (!IsSetRangeAttack)
	{
		// Cast Animation ����

		IsSetRangeAttack = true;

		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk0_projectile_combined") != 0)
		{
			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk0_projectile_combined", 0, true);
		}

		m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);

		RangeAttackCount = 3;
	}

	if (m_BossFrogState->BossFrogAnimation->GetIsAnimationEnd())
	{
		if (RangeAttackCount != 0)
		{
			if (!IsReverse)
			{
				// �߻�

				m_ProjectileManager->SetProjectile(PlayerTransform, 0);

				ReadySound->Stop();
				ReadySound->SetAudioClip(L"boss_atk_projectile_2_cast");
				ReadySound->Play();

				IsReverse = true;
				RangeAttackCount--;

				if (RangeAttackCount <= 0)
				{
					m_BossFrogState->BossFrogAnimation->SetBlendAnimation(L"boss", L"atk0_projectile_recover", L"atk0_projectile_combined", 1.2f, 0.15f, true);
				}
				else
				{
					m_BossFrogState->BossFrogAnimation->SetReverseAnimation(L"boss", L"atk0_projectile_combined", 100.f, 69.f, true);
				}

			}
			else
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk0_projectile_combined", 69.f, true);
				IsReverse = false;
			}
		}
		else
		{
			RangeAttackCount = 3;

			IsSetRangeAttack = false;

			m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Green);

			IsRangeAttackReady = false;

			RangeAttackReadyTimer = 0.f;

			MelodySound->SetLoop(true);

			ChangeState();

			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	}

	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrogAction::RushAttack()
{
	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::RushAttack)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	if (RushAttakReadyTimer < RushAttakReadyTime)
	{
		if (!IsRushAttackReady)
		{
			IsRushAttackReady = true;

			// ȸ������ �����غ���

			//if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"idle") != 0)
			//{
			//	m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"idle", 0);
			//}

			MelodySound->Stop();
			MelodySound->SetAudioClip(L"boss_atk_rush_110bpm_withmelody");
			MelodySound->Play();

			m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Yellow);

		}
		RotateToPlayer();

		RushAttakReadyTimer += (float)Time->DeltaTime();

		return static_cast<int>(ACTION_RESULT::PROCESS);
	}


	if (!IsSetRushAttack)
	{
		IsSetRushAttack = true;

		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk3_rush") != 0)
		{
			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk3_rush", 0);
		}

		AttackArea->AttackCollision->SetActive(false);

		AttackArea->m_GameObject->transform->SetLocalPosition(hos::Vector3::Zero);

		hos::Vector3 temp(0, 2, 0);
		AttackArea->AttackCollision->SetCenter(temp);

		temp = hos::Vector3(5, 4, 6);
		AttackArea->AttackCollision->SetSize(temp);

		m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);

	}

	// Move �ִϸ��̼��� ������ ������ �� (�������� ������ֽ�)
	// ���� ������� ������ �̵�

	hos::Vector3 _temp = BossFrogTransform->GetPosition();

	_temp.y = _temp.y + 1;

	if (!m_GameObject->m_Scene->RayCast(_temp, BossFrogTransform->GetForward(), 3.f))
	{
		if (RushAttackTimer < RushAttackTime)
		{
			m_BossFrogState->m_GameObject->GetComponent<hos::com::CapsuleCollision>()->SetActive(false);

			BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (BossFrogTransform->GetForward() * RushMoveSpeed * (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale()));

			AttackArea->AttackCollision->SetActive(true);

			RushAttackTimer += (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale();


			float _Distance = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length();

			if (_Distance < RushAttackShakeDistance)
			{
				PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(true);
				PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeTime(RushAttackShakeTime);

				if (_Distance <= 2)
				{
					_Distance = 2.f;
				}

				RushAttackShakeValue = 1 / _Distance;

				PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeValue(RushAttackShakeValue);
			}

			return static_cast<int>(ACTION_RESULT::PROCESS);
		}
		else
		{
			m_BossFrogState->m_GameObject->GetComponent<hos::com::CapsuleCollision>()->SetActive(true);

			RushAttackTimer = 0.f;

			IsSetRushAttack = false;

			AttackArea->AttackCollision->SetActive(false);

			BossFrogTransform->SetPosition(BossFrogTransform->GetPosition());

			IsRushAttackReady = false;

			RushAttakReadyTimer = 0.f;

			MelodySound->Stop();

			m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Green);

			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	}
	else
	{
		// ��������� ���� �� ���� ����
		m_BossFrogState->m_GameObject->GetComponent<hos::com::CapsuleCollision>()->SetActive(true);

		RushAttackTimer = 0.f;

		IsSetRushAttack = false;

		AttackArea->AttackCollision->SetActive(false);

		BossFrogTransform->SetPosition(BossFrogTransform->GetPosition());

		m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Green);

		MelodySound->Stop();

		IsRushAttackReady = false;

		RushAttakReadyTimer = 0.f;

		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int BossFrogAction::BackMove()
{
	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::BackMove)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	if (!IsSetBackMoveTime)
	{
		IsSetBackMoveTime = true;

		BackMoveTime = (float)(std::rand() / ((RAND_MAX + 1u) / 2)) + 2;

		BackMoveTimer = 0.f;

		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_forward") != 0)
		{
			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_forward");
		}

		//MelodySound->Stop();
		//MelodySound->SetAudioClip(L"boss_walk_combined_110bpm");
		//MelodySound->Play();

	}

	// 2 ~ 3 �� ������ �ð����� �ڷ� �̵�
	if (BackMoveTime > BackMoveTimer)
	{
		BackMoveTimer += (float)Time->DeltaTime() * m_BossFrogState->GetAnimationTimeScale();

		// �÷��̾� �������� ȸ���� �ؾ� ��
		m_NavAgent->SetPosition();
		m_NavAgent->SetDestination(PlayerTransform->GetPosition());

		hos::Vector3 _RotDir = BossFrogTransform->GetForward().Cross(m_NavAgent->GetDir());

		hos::Rotator _Rot = BossFrogTransform->GetRotation();

		float _RotY = 0.0f;

		if (_RotDir.y < -0.01f)
		{
			_RotY = _Rot.y;

			_RotY -= (float)Time->DeltaTime() * RotSpeed * m_BossFrogState->GetAnimationTimeScale();

			_Rot.y = _RotY;

			BossFrogTransform->SetRotation(_Rot);
		}

		if (_RotDir.y > 0.01f)
		{
			_RotY = _Rot.y;

			_RotY += (float)Time->DeltaTime() * RotSpeed * m_BossFrogState->GetAnimationTimeScale();

			_Rot.y = _RotY;

			BossFrogTransform->SetRotation(_Rot);
		}

		BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (-BossFrogTransform->GetForward() * (float)Time->DeltaTime() * MoveSpeed * m_BossFrogState->GetAnimationTimeScale()));
	}
	else
	{
		BackMoveTimer = 0.f;
		IsSetBackMoveTime = false;

		ChangeState();

		return static_cast<int>(ACTION_RESULT::FAILURE);
	}


	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrogAction::BackJump()
{
	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::BackJump)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	MelodySound->Stop();

	hos::Vector3 _temp = BossFrogTransform->GetPosition();

	_temp.y = _temp.y + 1;

	// Raycast �� �ڿ� ���� �ֳ� Ȯ��
	if (g_Px)
	{
		if (m_GameObject->m_Scene->RayCast(_temp, -BossFrogTransform->GetForward(), 4.f))
		{
			if (!IsSetBackJump)
			{
				BackJumpTimer = 0.f;

				return static_cast<int>(ACTION_RESULT::FAILURE);
			}
		}
	}

	if (!IsSetBackJump)
	{
		IsSetBackJump = true;

		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"backjump_xy0") != 0)
		{
			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"backjump_xy0");
		}

		MelodySound->Stop();
		MelodySound->SetAudioClip(L"boss_backjump_110bpm");
		MelodySound->Play();

		m_BossFrogState->m_GameObject->GetComponent<hos::com::CapsuleCollision>()->SetActive(false);

	}


	if (BackJumpTime < BackJumpTimer)
	{
		// �̵� �� ���� ������ �̵����� ���ϰ� �ϱ� (���� ��� ���� Ȯ�� => �÷��̾� �뽬 ���� �Ͱ� ����) 
		if (!m_GameObject->m_Scene->RayCast(_temp, -BossFrogTransform->GetForward(), 3.f))
		{
			// �ָ� 1 �ʰ� 5 �� �ӵ��� �̵�
			BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (-BossFrogTransform->GetForward() * (float)Time->DeltaTime() * BackJumpMoveSpeed * m_BossFrogState->GetAnimationTimeScale()));
		}
		else
		{
			BossFrogTransform->SetPosition(BossFrogTransform->GetPosition());
		}
	}
	else
	{
		BackJumpTimer += (float)Time->DeltaTime();
	}


	if (m_BossFrogState->BossFrogAnimation->GetIsAnimationEnd())
	{
		IsSetBackJump = false;
		BackJumpTimer = 0.f;
		m_BossFrogState->m_GameObject->GetComponent<hos::com::CapsuleCollision>()->SetActive(true);

		ChangeState();

		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		return static_cast<int>(ACTION_RESULT::PROCESS);
	}


}

void BossFrogAction::ChangeState()
{
	float _Length = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length();

	if (_Length <= 3.f)
	{
		// ���� �̱� TongueAttack, BackJump, BackMove
		double _SelectState = std::rand() / (double)RAND_MAX;
		_SelectState *= 100.f;

		if (_SelectState <= 5.f)
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::BackMove);
		}
		else if (_SelectState <= 10.f)
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::BackJump);
		}
		else if (_SelectState <= 40.f)
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::JumpAttack);
		}
		else if (_SelectState <= 70.f)
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::RushAttack);
		}
		else
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::TongueAttack);
		}

	}
	else if (_Length > 30.f)
	{
		m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::Idle);
	}
	else
	{
		// ���� �̱� RotateToTarget, JumpAttack, RangeAttack
		double _SelectState = std::rand() / (double)RAND_MAX;
		_SelectState *= 100.f;

		if (_SelectState <= 35.f)
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::RangeAttack);
		}
		else if (_SelectState <= 65.f)
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::JumpAttack);
		}
		else if (_SelectState <= 95.f)
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::RushAttack);
		}
		else
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::RotateToTarget);
		}

	}
}

void BossFrogAction::AudioTimeScale()
{

	if (m_BossFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
		m_BossFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
		m_BossFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
		m_BossFrogState->m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
	{
		float _Pitch = m_BossFrogState->GetAnimationTimeScale() - 1.3f;

		MelodySound->SetPitch(_Pitch);

		if (ReadySound->GetAudioClip())
		{
			ReadySound->SetPitch(_Pitch);
		}
	}
	else
	{
		MelodySound->SetPitch(0.f);

		if (ReadySound->GetAudioClip())
		{
			ReadySound->SetPitch(0.f);
		}
	}

	if (m_BossFrogState->m_PlayerState->GetIsFever())
	{
		float _Pitch = m_BossFrogState->GetAnimationTimeScale() - 1.3f;

		MelodySound->SetPitch(_Pitch);

		if (ReadySound->GetAudioClip())
		{
			ReadySound->SetPitch(_Pitch);
		}
	}

}

void BossFrogAction::RotateToPlayer()
{
	m_NavAgent->SetPosition();
	m_NavAgent->SetDestination(PlayerTransform->GetPosition());

	hos::Vector3 _RotDir = BossFrogTransform->GetForward().Cross(m_NavAgent->GetDir());

	hos::Rotator _Rot = BossFrogTransform->GetRotation();

	float _RotY = 0.0f;


	if (_RotDir.y < -0.1f)
	{
		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_left") != 0)
		{
			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_left");
		}

		_RotY = _Rot.y;

		_RotY -= (float)Time->DeltaTime() * (RotSpeed * 2.5f) * m_BossFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		BossFrogTransform->SetRotation(_Rot);
	}

	if (_RotDir.y > 0.1f)
	{
		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_right") != 0)
		{
			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_right");
		}

		_RotY = _Rot.y;

		_RotY += (float)Time->DeltaTime() * (RotSpeed * 2.5f) * m_BossFrogState->GetAnimationTimeScale();

		_Rot.y = _RotY;

		BossFrogTransform->SetRotation(_Rot);
	}

}