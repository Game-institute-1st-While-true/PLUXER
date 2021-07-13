#include "BossFrog.h"
#include "BossArea.h"
#include "ProjectileManager.h"
#include "BossRoom.h"

BossFrog::BossFrog()
	:Area(nullptr), State(nullptr), Bt(nullptr), m_NavAgent(nullptr), Room(nullptr), AttackRangeSelect(0), Projectile(nullptr)
{
	ExecutionOrder = 5;
	Area = new BossArea(this);
	Projectile = new ProjectileManager(this, 3);
	Bt = new BehaviorTree();

#pragma region Behavior Tree Create
	DecoratorNode* dec_ai_chk = new DecoratorNode();

	SelectNode* sel_state_set = new SelectNode();

	BehaviorTask* task_idle = new BehaviorTask();

	SelectNode* sel_close_range_att_sel = new SelectNode();

	BehaviorTask* task_set_close_type = new BehaviorTask();
	BehaviorTask* task_toungue_att = new BehaviorTask();
	BehaviorTask* task_back_jump = new BehaviorTask();
	BehaviorTask* task_back_move = new BehaviorTask();

	SelectNode* sel_long_range_att_sel = new SelectNode();

	BehaviorTask* task_set_long_type = new BehaviorTask();
	BehaviorTask* task_bubble = new BehaviorTask();
	BehaviorTask* task_jump_att = new BehaviorTask();
	BehaviorTask* task_rush_att = new BehaviorTask();
	BehaviorTask* task_rotate_to_target = new BehaviorTask();

	BehaviorTask* task_move = new BehaviorTask();

	BehaviorTask* task_search_target = new BehaviorTask();
#pragma endregion

#pragma region Behavior Tree Set Function
	dec_ai_chk->Func = [&]()
	{
		if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
	};

	task_idle->Func = [&]()
	{
		return Idle();
	};

	task_set_close_type->Func = [&]()
	{
		return ChangeCloseRangeAttack();
	};

	task_toungue_att->Func = [&]()
	{
		return TongueAttack();
	};

	task_back_jump->Func = [&]()
	{
		return BackJump();
	};

	task_back_move->Func = [&]()
	{
		return BackMove();
	};

	task_set_long_type->Func = [&]()
	{
		return ChangeLongRangeAttack();
	};

	task_bubble->Func = [&]()
	{
		return RangeAttack();
	};

	task_jump_att->Func = [&]()
	{
		return JumpAttack();
	};

	task_rush_att->Func = [&]()
	{
		return RushAttack();
	};

	task_rotate_to_target->Func = [&]()
	{
		return RotateToTarget();
	};

	task_move->Func = [&]()
	{
		return Move();
	};

	task_search_target->Func = [&]()
	{
		return SearchTarget();
	};

#pragma endregion

#pragma region Behavior Tree Add Node
	Bt->Insert(dec_ai_chk, static_cast<int>(ON_AI_TYPE::ROOT));								// 1

	Bt->Insert(sel_state_set, static_cast<int>(ON_AI_TYPE::DEC_AI_CHK));					// 2

	Bt->Insert(task_idle, static_cast<int>(ON_AI_TYPE::SEL_STATE));							// 3
	Bt->Insert(sel_close_range_att_sel, static_cast<int>(ON_AI_TYPE::SEL_STATE));			// 4
	Bt->Insert(sel_long_range_att_sel, static_cast<int>(ON_AI_TYPE::SEL_STATE));			// 5
	Bt->Insert(task_search_target, static_cast<int>(ON_AI_TYPE::SEL_STATE));				// 6

	Bt->Insert(task_set_close_type, static_cast<int>(ON_AI_TYPE::SEL_CLOSE_ATT));			// 7
	Bt->Insert(task_toungue_att, static_cast<int>(ON_AI_TYPE::SEL_CLOSE_ATT));				// 8
	Bt->Insert(task_back_jump, static_cast<int>(ON_AI_TYPE::SEL_CLOSE_ATT));				// 9
	Bt->Insert(task_back_move, static_cast<int>(ON_AI_TYPE::SEL_CLOSE_ATT));				// 10 

	Bt->Insert(task_set_long_type, static_cast<int>(ON_AI_TYPE::SEL_LONG_ATT));				// 11
	Bt->Insert(task_bubble, static_cast<int>(ON_AI_TYPE::SEL_LONG_ATT));					// 12
	Bt->Insert(task_jump_att, static_cast<int>(ON_AI_TYPE::SEL_LONG_ATT));					// 13
	Bt->Insert(task_rush_att, static_cast<int>(ON_AI_TYPE::SEL_LONG_ATT));					// 14
	Bt->Insert(task_rotate_to_target, static_cast<int>(ON_AI_TYPE::SEL_LONG_ATT));			// 15
	Bt->Insert(task_move, static_cast<int>(ON_AI_TYPE::SEL_LONG_ATT));						// 16
#pragma endregion
}

BossFrog::~BossFrog()
{
	hos::SafeDelete(Bt);
	hos::SafeDelete(Area);
	hos::SafeDelete(Projectile);
}

void BossFrog::GameStart()
{
	try
	{
		if (m_NavAgent)
		{
			m_NavAgent->RequestPathFind();
			throw - 1;
		}

		FirstTarget = Rand.GetRand(0, 1);
		Room->Players[FirstTarget]->State->SetFirstTargetRate(Rand.GetRand(0.15, 0.25, 100));

	}
	catch (int errcode)
	{
		switch (errcode)
		{
		case -1:
			NetLogger::GetIns()->Write(L"BossFrog", L"Request NaviAgent Is Null");
			break;
		default:
			break;
		}
	}
}

void BossFrog::GameClose()
{
}

void BossFrog::BossRelease()
{
	Area->OnTongueCollision = false;
}

void BossFrog::SetAttackId(const int id)
{
	if (FirstAttackId < 0)
		FirstAttackId = id;

	LastAttackId = id;
}

void BossFrog::Reset()
{
}

void BossFrog::Awake()
{
	m_NavAgent = m_GameObject->GetComponent<hos::com::NavAgent>();
	m_NavAgent->SetNavMeshId(m_GameObject->m_Scene->GetBuildIndex());
	Room = m_GameObject->m_Scene->FindGameObjectWithName(L"BossScene")->GetComponent<BossRoom>();
	Capsule = m_GameObject->GetComponent<hos::com::CapsuleCollision>();

	Area->TongueArea.Center = hos::Vector2(0, -2);
	Area->TongueArea.Radius = 5.0;

	State = m_GameObject->GetComponent<BossFrogState>();

	State->SetRoom(Room);
};

void BossFrog::Start()
{

}

void BossFrog::FixedUpdate()
{
	CheckBossCollision();

	for (auto player : Room->Players)
	{
		Projectile->CollisionCheck(*player);
	}
}

void BossFrog::Update()
{
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	Projectile->ProjectileUpdate(delta);

	if (m_NoteManager->GetIsBGMStart())
	{
		PrePos = m_GameObject->transform->GetPosition();
		Bt->Run();
	}
}

void BossFrog::CheckBossCollision()
{
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (MapColliderCheck(delta) || ObjectColliderCheck(delta))
	{
		if (!State->GetStop())
		{
			State->SetStop(true);
		}
	}
	else
	{
		State->SetStop(false);
	}
}

bool BossFrog::MapColliderCheck(float& delta)
{
	hos::Vector3 dir = m_GameObject->transform->GetForward();
	hos::Vector3 next_pos;
	hos::Vector2 v2pos;

	hos::Vector2 collider_size;
	hos::Vector2 center(m_GameObject->transform->GetPosition().x, m_GameObject->transform->GetPosition().z);

	bool ret = false;

	switch (State->GetNowAction())
	{
	case BossFrogState::BossFrogActionList::Move:
		next_pos = m_GameObject->transform->GetPosition() + (dir * delta * MoveSpeed);
		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;
		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	case BossFrogState::BossFrogActionList::BackMove:
		next_pos = m_GameObject->transform->GetPosition() + (-dir * delta * MoveSpeed);
		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;
		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	case BossFrogState::BossFrogActionList::JumpAttack:
		next_pos = m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetForward() * JumpAttackMoveSpeed * delta);
		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;
		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	case BossFrogState::BossFrogActionList::BackJump:
		collider_size.x = m_GameObject->transform->GetPosition().x;
		collider_size.y = m_GameObject->transform->GetPosition().z;

		Area->BackJumpBox.SetSizeCenter(collider_size, center);

		if (Room->MapCollider->CheckCollisionMap(Area->BackJumpBox))
		{
			Area->BackJumpAble = false;
		}
		else
		{
			Area->BackJumpAble = true;
		}

		next_pos = m_GameObject->transform->GetPosition() + (-m_GameObject->transform->GetForward() * delta * BackJumpMoveSpeed);

		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;

		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	case BossFrogState::BossFrogActionList::RushAttack:
		next_pos = m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetForward() * RushMoveSpeed * delta);

		v2pos.x = next_pos.x;
		v2pos.y = next_pos.z;
		if (Room->MapCollider->CheckCollisionMap(v2pos))
		{
			ret = true;
		}
		break;
	}
	return ret;
}

bool BossFrog::ObjectColliderCheck(float& delta)
{
	using namespace DirectX::SimpleMath;
	using namespace collider2;

#pragma region AttackCollision
	if (Area->OnTongueCollision)
	{
		for (auto player : Room->Players)
		{
			if (player)
			{
				int ret = Area->CheckTongue(player);
				if (ret > -1)
				{
					player->State->GetDamaged(State->GetAttackDamege(), ret, m_GameObject->transform);
				}
			}
		}
	}

	if (Area->OnJumpCollision)
	{
		for (auto player : Room->Players)
		{
			if (player)
			{
				int ret = Area->CheckJump(player);
				if (ret > -1)
				{
					player->State->GetDamaged(State->GetAttackDamege(), ret, m_GameObject->transform);
				}
			}
		}
	}

	if (State->GetNowAction() == BossFrogState::BossFrogActionList::RushAttack)
	{
		for (auto player : Room->Players)
		{
			if (player)
			{
				int ret = Area->CheckRush(player);
				if (ret > -1)
				{
					player->State->GetDamaged(State->GetAttackDamege(), ret, m_GameObject->transform);
				}
			}
		}
	}
#pragma endregion

	Circle player_circle;

	Circle boss_circle;
	boss_circle.Radius = Capsule->GetRadius();

	Vector3 next_pos;
	Vector2 v2pos;

	switch (State->GetNowAction())
	{
	case BossFrogState::BossFrogActionList::Move:
		for (auto player : Room->Players)
		{
			next_pos = m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetForward() * delta * MoveSpeed);

			boss_circle.Center.x = next_pos.x;
			boss_circle.Center.y = next_pos.z;

			player_circle.Center.x = player->m_GameObject->transform->GetPosition().x;
			player_circle.Center.y = player->m_GameObject->transform->GetPosition().z;
			player_circle.Radius = player->Capsule->GetRadius();

			if (CC(boss_circle, player_circle))
			{
				return true;
			}
		}
		break;

	default:
		return false;
	}
	return false;
}

bool BossFrog::ActionDealy()
{
	switch (State->GetNextAction())
	{
		// 이 행동들엔 Ready State 가 필요 없다.
	case BossFrogState::BossFrogActionList::Idle:
	case BossFrogState::BossFrogActionList::Dead:
	case BossFrogState::BossFrogActionList::Move:
	case BossFrogState::BossFrogActionList::BackJump:
	case BossFrogState::BossFrogActionList::RotateToTarget:
		return false;
		break;

	case BossFrogState::BossFrogActionList::JumpAttack:
		ReadyTimer = 0;
		ReadyTime = JUMP_ATT_READY_TIME;
		break;
	case BossFrogState::BossFrogActionList::RangeAttack:
		ReadyTimer = 0;
		ReadyTime = RANGE_ATT_READY_TIME;
		break;
	case BossFrogState::BossFrogActionList::TongueAttack:
		ReadyTimer = 0;
		ReadyTime = TONGUE_ATT_READY_TIME;
		break;
	case BossFrogState::BossFrogActionList::RushAttack:
		ReadyTimer = 0;
		ReadyTime = RUSH_ATT_READY_TIME;
		break;
	default:
		break;
	}
	return true;
}

int BossFrog::Idle()
{
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::SUCESS);
	}

	if (State->GetNowAction() != BossFrogState::BossFrogActionList::Idle || AttackRangeSelect != 0)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	// idle process
	if (IdleTimer >= IdleTime)
	{
		IdleTimer = 0;
		//IdleTime = static_cast<float>(Rand.GetRand(0, IDLE_ANI_TIME, 100));

		// get range
		float _Length = (m_GameObject->transform->GetPosition() - Room->Players[NowTarget]->m_GameObject->transform->GetPosition()).Length();

		if (_Length < 4.f)
		{
			// 근거리 공격
			AttackRangeSelect = 1;
		}
		else
		{
			// 원거리 공격
			AttackRangeSelect = 2;
		}
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		IdleTimer += delta;
	}
	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrog::ChangeCloseRangeAttack()
{
	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (AttackRangeSelect != 1)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	if (State->GetNowAction() == BossFrogState::BossFrogActionList::TongueAttack ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::BackMove ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::BackJump ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::JumpAttack ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::RushAttack ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::SearchTarget)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		double rand = Rand.GetRand(0.01, 1.0, 1000);

		if (rand <= 0.075)
		{
			State->SetNextAction(BossFrogState::BossFrogActionList::BackMove);
		}
		else if (rand <= 0.15)
		{
			State->SetNextAction(BossFrogState::BossFrogActionList::BackJump);
		}
		else if (rand <= 0.55)
		{
			State->SetNextAction(BossFrogState::BossFrogActionList::JumpAttack);
		}
		else if (rand <= 0.85)
		{
			State->SetNextAction(BossFrogState::BossFrogActionList::RushAttack);
		}
		else
		{
			State->SetNextAction(BossFrogState::BossFrogActionList::TongueAttack);
		}
	}

	if (ActionDealy())
	{
		State->SetNowAction(BossFrogState::BossFrogActionList::SearchTarget);
	}
	else
	{
		State->PlayNextAction();
		State->SetNextAction(BossFrogState::BossFrogActionList::Idle);
	}

	return static_cast<int>(ACTION_RESULT::SUCESS);
}

int BossFrog::TongueAttack()
{
	double delta = m_GameObject->m_Scene->GetTimer()->DeltaTime();

	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (State->GetNowAction() != BossFrogState::BossFrogActionList::TongueAttack)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	// 공격 시전
	if (!IsSetTongueAttack)
	{
		IsSetTongueAttack = true;

		Area->OnTongueCollision = false;
		TongueAttackActiveTimer = 0.f;
		TongueAttackTimer = 0.f;
	}

	if (TongueAttackTimer >= TA_ANI_TIME)
	{
		Area->OnTongueCollision = false;
		IsSetTongueAttack = false;

		ClearState();
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		if (TongueAttackActiveTimer > TongueAttackEnable)
		{
			// 공격 영역 활성화
			Area->OnTongueCollision = true;
		}

		if (TongueAttackActiveTimer > TongueAttackDisable)
		{
			// 공격 영역 비활성화
			Area->OnTongueCollision = false;
		}

		TongueAttackActiveTimer += delta;
		TongueAttackTimer += delta;
	}
	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrog::BackJump()
{
	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (State->GetNowAction() != BossFrogState::BossFrogActionList::BackJump)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	if (!Area->BackJumpAble)
	{
		if (!IsSetBackJump)
		{
			BackJumpTimer = 0.f;
			BackJumpAniTimer = 0.f;
			ClearState();
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	}

	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (!IsSetBackJump)
	{
		IsSetBackJump = true;

		BackJumpTimer = 0.f;
		BackJumpAniTimer = 0.f;
	}

	if (BackJumpTime < BackJumpTimer)
	{
		if (!State->GetStop())
		{
			// 멀면 1 초간 5 의 속도로 이동
			m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition() + (-m_GameObject->transform->GetForward() * delta * BackJumpMoveSpeed));
		}
	}
	else
	{
		BackJumpTimer += delta;
	}

	if (BackJumpAniTimer >= BackJumpAniTime)
	{
		IsSetBackJump = false;
		BackJumpTimer = 0.f;
		ClearState();
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		BackJumpAniTimer += delta;
		return static_cast<int>(ACTION_RESULT::PROCESS);
	}
}

int BossFrog::BackMove()
{
	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}

	else if (State->GetNowAction() != BossFrogState::BossFrogActionList::BackMove)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	if (!IsSetBackMoveTime)
	{
		IsSetBackMoveTime = true;

		BackMoveTime = Rand.GetRand(2, 3);

		BackMoveTimer = 0.f;

	}

	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	// 2 ~ 3 초 사이의 시간동안 뒤로 이동
	if (BackMoveTimer >= BackMoveTime)
	{
		BackMoveTimer = 0.f;
		IsSetBackMoveTime = false;
		ClearState();
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		BackMoveTimer += delta;

		// 플레이어 방향으로 회전도 해야 함
		m_NavAgent->SetPosition();
		m_NavAgent->SetDestination(Room->Players[NowTarget]->m_GameObject->transform->GetPosition());

		hos::Vector3 _RotDir = m_GameObject->transform->GetForward().Cross(m_NavAgent->GetDir());
		hos::Rotator _Rot = m_GameObject->transform->GetRotation();
		float _RotY = 0.0f;

		if (_RotDir.y < -0.01f)
		{
			_RotY = _Rot.y;
			_RotY -= delta * RotSpeed;
			_Rot.y = _RotY;
			m_GameObject->transform->SetRotation(_Rot);
		}

		if (_RotDir.y > 0.01f)
		{
			_RotY = _Rot.y;
			_RotY += delta * RotSpeed;
			_Rot.y = _RotY;
			m_GameObject->transform->SetRotation(_Rot);
		}

		hos::Vector3 dir = m_GameObject->transform->GetForward();
		State->SetDirection(dir, true);

		if (!State->GetStop())
		{
			m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition() + (-m_GameObject->transform->GetForward() * delta * MoveSpeed));
		}
	}
	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrog::ChangeLongRangeAttack()
{
	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (AttackRangeSelect != 2)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	if (State->GetNowAction() == BossFrogState::BossFrogActionList::RangeAttack ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::RushAttack ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::RotateToTarget ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::JumpAttack ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::Move ||
		State->GetNowAction() == BossFrogState::BossFrogActionList::SearchTarget)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		double rand = Rand.GetRand(0.01, 1.0, 1000);

		if (rand <= 0.15)
		{
			State->SetNextAction(BossFrogState::BossFrogActionList::Move);
		}
		else if (rand <= 0.50)
		{
			if (Projectile->GetCurrentActiveObject() != 0)
			{
				State->SetNextAction(BossFrogState::BossFrogActionList::RushAttack);
			}
			else
			{
				State->SetNextAction(BossFrogState::BossFrogActionList::RangeAttack);
			}
		}
		else if (rand <= 0.75)
		{
			State->SetNextAction(BossFrogState::BossFrogActionList::JumpAttack);
		}
		else if (rand <= 0.95)
		{
			State->SetNextAction(BossFrogState::BossFrogActionList::RushAttack);
		}
		else
		{
			State->SetNextAction(BossFrogState::BossFrogActionList::Move);
		}
	}
	if (ActionDealy())
	{
		State->SetNowAction(BossFrogState::BossFrogActionList::SearchTarget);
	}
	else
	{
		State->PlayNextAction();
		State->SetNextAction(BossFrogState::BossFrogActionList::Idle);
	}
	return static_cast<int>(ACTION_RESULT::SUCESS);
}

int BossFrog::RangeAttack()
{
	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (State->GetNowAction() != BossFrogState::BossFrogActionList::RangeAttack)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	double delta = m_GameObject->m_Scene->GetTimer()->DeltaTime();

	if (!IsSetRangeAttack)
	{
		// Cast Animation 실행
		IsSetRangeAttack = true;
		RangeAttackCount = 3;
		RangeAttackAniTimer = 0.f;
		RangeAttackAniTime = RANGE_ATT_ANI_TIME;
	}

	if (RangeAttackAniTimer >= RangeAttackAniTime)
	{
		if (RangeAttackCount != 0)
		{
			if (!IsReverse)
			{
				// 발사
				Projectile->CreateProjectile(Room->Players[NowTarget]->m_GameObject->transform, 0);

				IsReverse = true;
				RangeAttackCount--;

				if (RangeAttackCount <= 0)
				{
					RangeAttackAniTime = RANGE_REVERSE_ANI_TIME;
				}
				else
				{
					RangeAttackAniTime = RANGE_ATT_ANI_TIME * 0.31;
				}
			}
			else
			{
				RangeAttackAniTime = RANGE_ATT_ANI_TIME * 0.31;
				IsReverse = false;
			}
			RangeAttackAniTimer = 0;
		}
		else
		{
			RangeAttackCount = 3;
			IsSetRangeAttack = false;
			RangeAttackAniTimer = 0;
			RangeAttackAniTime = RANGE_ATT_ANI_TIME;

			ClearState();
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	}
	else
	{
		RangeAttackAniTimer += delta;
	}

	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrog::RushAttack()
{
	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (State->GetNowAction() != BossFrogState::BossFrogActionList::RushAttack)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	if (!IsSetRushAttack)
	{
		IsSetRushAttack = true;
	}

	// Move 애니메이션을 빠르게 돌려야 함 (익진님이 만들어주심)
	// 벽에 가까워질 때까지 이동

	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (!State->GetStop())
	{
		if (RushAttackTimer < RushAttackTime)
		{
			m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetForward() * RushMoveSpeed * delta));
			RushAttackTimer += delta;
			return static_cast<int>(ACTION_RESULT::PROCESS);
		}
		else
		{
			RushAttackTimer = 0.f;
			IsSetRushAttack = false;
			ClearState();
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	}
	else
	{
		// 가까워지면 정지 후 다음 상태
		RushAttackTimer = 0.f;
		IsSetRushAttack = false;
		m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition());
		ClearState();
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	return static_cast<int>(ACTION_RESULT::FAILURE);
}

int BossFrog::JumpAttack()
{
	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (State->GetNowAction() != BossFrogState::BossFrogActionList::JumpAttack)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	/// 일단 스킵
	// 아니면 회전 이동 상태로 전환
	// 회전 이동이 끝났다면 점프 공격 진행

	// 플레이어와 떨어진 만큼의 속도로 이동

	if (!IsSetJumpAttack)
	{
		// 앞쪽에 벽이 있는지 확인
		//if (State->GetStop())
		//{
		//	JumpAttackTimer = 0.f;
		//	State->SetNowAction(BossFrogState::BossFrogActionList::Move);
		//	return static_cast<int>(ACTION_RESULT::AVOID);
		//}

		IsSetJumpAttack = true;
		Area->OnJumpCollision = false;
		JumpAttackTimer = 0.f;
		JumpAttackMoveSpeed = (m_GameObject->transform->GetPosition() - Room->Players[NowTarget]->m_GameObject->transform->GetPosition()).Length() * 2;
		Room->Req_GameInfo(*this);
		State->SetJumpMoveSpeed(JumpAttackMoveSpeed);
	}

	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (JumpAniTimer >= JUMP_ANI_TIME)
	{
		Area->OnJumpCollision = false;
		IsSetJumpAttack = false;
		JumpAttackTimer = 0.f;
		JumpAniTimer = 0.f;
		ClearState();
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}
	else
	{
		if (JumpAttackTimer > JumpAttackMove && JumpAttackTimer < JumpAttackEnable)
		{
			if (!State->GetStop())
			{
				m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetForward() * JumpAttackMoveSpeed * delta));
			}
		}

		if (JumpAttackTimer > JumpAttackEnable && JumpAttackTimer < JumpAttackDisable)
		{
			Area->OnJumpCollision = true;
		}

		if (JumpAttackTimer > JumpAttackDisable)
		{
			Area->OnJumpCollision = false;
		}

		JumpAttackTimer += delta;
		JumpAniTimer += delta;

		return static_cast<int>(ACTION_RESULT::PROCESS);
	}
}

int BossFrog::RotateToTarget()
{
	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (State->GetNowAction() != BossFrogState::BossFrogActionList::RotateToTarget)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	// 플레이어가 왼쪽에 있나 오른쪽에 있나 확인
	if (!CheckRotateDir)
	{
		CheckRotateDir = true;
		hos::Vector3 _RotDir = m_GameObject->transform->GetForward().Cross(Room->Players[NowTarget]->m_GameObject->transform->GetForward());

		if (_RotDir.y < 0.f)
		{
			RotateMoveDir = true;
		}
		else
		{
			RotateMoveDir = false;
		}
	}

	// 해당 방향으로 2.5초 동안 이동
	// 플레이어를 바라보며 회전도 한다

	hos::Rotator _Rot = m_GameObject->transform->GetRotation();

	float _RotY = 0.0f;
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (RotateMoveTime > RotateMoveTimer)
	{
		if (RotateMoveDir)
		{
			_RotY = _Rot.y;
			_RotY -= delta * RotSpeed;
			_Rot.y = _RotY;
			m_GameObject->transform->SetRotation(_Rot);
			m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetLeft() * MoveSpeed * delta));
		}
		else
		{
			_RotY = _Rot.y;
			_RotY += delta * RotSpeed;
			_Rot.y = _RotY;
			m_GameObject->transform->SetRotation(_Rot);
			m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition() + (-m_GameObject->transform->GetLeft() * MoveSpeed * delta));
		}
		RotateMoveTimer += delta;
	}
	else
	{
		CheckRotateDir = false;
		RotateMoveTimer = 0;
		ClearState();
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrog::Move()
{
	float delta = static_cast<float>(m_GameObject->m_Scene->GetTimer()->DeltaTime());

	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}
	else if (State->GetNowAction() != BossFrogState::BossFrogActionList::Move)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	// setting target
	int target = 0;
	int idx_cnt = 0;

	for (auto player : Room->Players)
	{
		target = (player->State->CalcAggrol() > Room->Players[NowTarget]->State->CalcAggrol()) ? idx_cnt : NowTarget;
		++idx_cnt;
	}

	// move process
	if (MoveTimer >= MoveTime)
	{
		MoveTimer = 0;
		MoveTime = Rand.GetRand(2, 3);
		ClearState();
		return static_cast<int>(ACTION_RESULT::AVOID);
	}

	else
	{
		// get range
		float _Length = (m_GameObject->transform->GetPosition() - Room->Players[NowTarget]->m_GameObject->transform->GetPosition()).Length();

		if (_Length < 4.f)
		{
			// 근거리 공격
			ClearState();
			return static_cast<int>(ACTION_RESULT::AVOID);
		}
		MoveTimer += delta;
	}

	m_NavAgent->SetPosition();
	m_NavAgent->SetDestination(Room->Players[NowTarget]->m_GameObject->transform->GetPosition());

	hos::Vector3 _RotDir = m_GameObject->transform->GetForward().Cross(m_NavAgent->GetDir());
	hos::Rotator _Rot = m_GameObject->transform->GetRotation();

	float _RotY = 0.0f;

	if (_RotDir.y < -0.01f)
	{
		_RotY = _Rot.y;
		_RotY -= delta * RotSpeed;
		_Rot.y = _RotY;
		m_GameObject->transform->SetRotation(_Rot);
	}

	if (_RotDir.y > 0.01f)
	{
		_RotY = _Rot.y;
		_RotY += delta * RotSpeed;
		_Rot.y = _RotY;
		m_GameObject->transform->SetRotation(_Rot);
	}

	hos::Vector3 dir = m_GameObject->transform->GetForward();
	State->SetDirection(dir, true);

	if (!State->GetStop())
	{
		m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition() + (dir * delta * MoveSpeed));
	}
	return static_cast<int>(ACTION_RESULT::PROCESS);
}

int BossFrog::SetTarget()
{
	// setting target
	int target = 0;
	int idx_cnt = 0;

	double delta = m_GameObject->m_Scene->GetTimer()->DeltaTime();

	for (auto player : Room->Players)
	{
		target = (player->State->CalcAggrol() > Room->Players[NowTarget]->State->CalcAggrol()) ? idx_cnt : NowTarget;
		++idx_cnt;
	}

	PreTarget = NowTarget;
	NowTarget = target;

	if (PreTarget == NowTarget)
	{
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<double> time = (now - AggroTimer);

		if (time.count() >= AggroResetTime)
		{
			for (auto player : Room->Players)
			{
				player->State->AggroReset();
			}
	
			NowTarget = Rand.GetRand(0, 1);

			if (Room->Players[NowTarget]->State->GetNowPlayerAction() == PlayerState::PlayerActionList::Dead)
			{
				switch (NowTarget)
				{
				case 0:
					NowTarget = 1;
				case 1:
					NowTarget = 0;
				}
			}
			AggroTimer = now;
		}
	}
	return static_cast<int>(ACTION_RESULT::SUCESS);
}

int BossFrog::SearchTarget()
{
	if (State->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
	{
		return static_cast<int>(ACTION_RESULT::AVOID);
	}

	else if (State->GetNowAction() != BossFrogState::BossFrogActionList::SearchTarget)
	{
		return static_cast<int>(ACTION_RESULT::FAILURE);
	}

	double delta = m_GameObject->m_Scene->GetTimer()->DeltaTime();

	if (ReadyTimer >= ReadyTime)
	{
		State->PlayNextAction();
		State->SetNextAction(BossFrogState::BossFrogActionList::Idle);
		return static_cast<int>(ACTION_RESULT::SUCESS);
	}
	else
	{
		m_NavAgent->SetPosition();
		m_NavAgent->SetDestination(Room->Players[NowTarget]->m_GameObject->transform->GetPosition());

		hos::Vector3 _RotDir = m_GameObject->transform->GetForward().Cross(m_NavAgent->GetDir());
		hos::Rotator _Rot = m_GameObject->transform->GetRotation();

		float _RotY = 0.0f;
		float _RotAxis = m_GameObject->transform->GetForward().Dot(m_NavAgent->GetDir());

		if (_RotDir.y < -0.1f)
		{
			_RotY = _Rot.y;
			_RotY -= static_cast<float>(delta) * (RotSpeed * 2.5f);
			_Rot.y = _RotY;
		}

		if (_RotDir.y > 0.1f)
		{
			_RotY = _Rot.y;
			_RotY += static_cast<float>(delta) * (RotSpeed * 2.5f);
			_Rot.y = _RotY;
		}

		ReadyTimer += delta;
		m_GameObject->transform->SetRotation(_Rot);
		return static_cast<int>(ACTION_RESULT::PROCESS);
	}

	return static_cast<int>(ACTION_RESULT::FAILURE);
}

void BossFrog::ClearState()
{
	State->SetNowAction(BossFrogState::BossFrogActionList::Idle);
	SetTarget();

	// Idle (초기 선택)
	AttackRangeSelect = 0;
}