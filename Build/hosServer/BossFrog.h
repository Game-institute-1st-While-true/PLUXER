#pragma once
#include "GroupSystem.h"
#include "NoteManager.h"
#include "BossFrogState.h"

#include "Gzznet/Random.h"

struct BossArea;
class ProjectileManager;
class Player;
class BossRoom;
using Random = gzz::Random;

const double MAX_MOVE_TIME = 3;
const double MIN_MOVE_TIME = 5;

const double MOVE_ANI_TIME = 1.0;
const double IDLE_ANI_TIME = 1.0;
const double TA_ANI_TIME = 1.16667;
const double JUMP_ANI_TIME = 3.83333;

const double BACK_JUMP_ANI_TIME = 1.0;

const double RANGE_ATT_ANI_TIME = 1.2333;
const double RANGE_REVERSE_ANI_TIME = 0.7333;

const double TONGUE_ATT_READY_TIME = 0.91667;
const double JUMP_ATT_READY_TIME = 0.61111;
const double RUSH_ATT_READY_TIME = 1.1;
const double RANGE_ATT_READY_TIME = 0.68750;

class BossFrog : public hos::com::Script
{
public:
	BossRoom* Room;
	BossArea* Area;
	BossFrogState* State;
	ProjectileManager* Projectile;
	hos::Vector3 PrePos;
	hos::com::CapsuleCollision* Capsule;
	hos::com::NavAgent* m_NavAgent;

private:
	BehaviorTree* Bt;
	NoteManager* m_NoteManager;

private:
	int NowTarget;
	int PreTarget;

	int FirstTarget;

	int FirstAttackId;
	int LastAttackId;

	int AttackRangeSelect;
	Random Rand;

#pragma region BehaviorTree Data
private:
	// ReadyCheck
	bool IsReadyStart;
	bool IsReady;

	double ReadyTimer = 0.0;
	double ReadyTime = 0.0;

	// Dead
	double WaitDead = 5.f;
	double DeadTimer = 0.f;

	// Move
	bool IsMove = false;
	bool IsSetMoveTime = false;
	double MoveTime = 3;
	double MoveTimer = 0.f;
	float MoveSpeed = 2.5f;
	float RotSpeed = 50.f;

	// RotateToTarget
	bool CheckRotateDir = false;
	bool RotateMoveDir = false;
	double RotateMoveTime = 2.5f;
	double RotateMoveTimer = 0.f;

	// BackMove
	bool IsSetBackMoveTime = false;
	double BackMoveTime = 3.0f;
	double BackMoveTimer = 0.f;

	// BackJump
	bool IsSetBackJump = false;
	double BackJumpTime = 0.3f;							// 점프 대기시간
	double BackJumpTimer = 0.f;
	float BackJumpMoveSpeed = 7.f;

	double BackJumpAniTime = BACK_JUMP_ANI_TIME;		// 점프 상태시간
	double BackJumpAniTimer = 0.f;

	// Tongue Attack
	bool IsSetTongueAttack = false;

	double TongueAttackTimer = 0.f;
	double TongueAttackActiveTimer = 0.f;
	float TongueAttackEnable = 0.5f;
	float TongueAttackDisable = 0.666667f;

	// Rush Attack
	bool IsSetRushAttack = false;
	double RushAttackTimer = 0.f;
	double RushAttackTime = 1.5f;
	float RushMoveSpeed = 10.f;

	// Jump Attack
	bool IsSetJumpAttack = false;
	float JumpAttackMove = 0.83f;
	float JumpAttackEnable = 1.333f;
	float JumpAttackDisable = 1.666f;
	double JumpAttackTimer = 0.f;
	float JumpAttackMoveSpeed;
	double JumpAniTimer = 0.f;

	// Range Attack
	bool IsSetRangeAttack = false;
	bool IsReverse = false;
	int RangeAttackCount = 3;
	double RangeAttackAniTimer = 0;
	double RangeAttackAniTime = RANGE_ATT_ANI_TIME;

	// Idle
	float IdleTimer = 0.f;
	float IdleTime = IDLE_ANI_TIME;

	// Setting Target
	//double AggroTimer = 0.f;
	std::chrono::steady_clock::time_point AggroTimer;
	double AggroResetTime = 10.0;
	
#pragma endregion

public:
	BossFrog();
	virtual ~BossFrog();

public:
	enum class ON_AI_TYPE
	{
		ROOT,
		DEC_AI_CHK, // 분기를 나누기 위한 서비스  
		SEL_STATE,
		IDLE, SEL_CLOSE_ATT, SEL_LONG_ATT, SEARCH_TARGET,
		SET_CATT_TYPE, TOUNGUE_ATT, BACK_JUMP, BACK_MOVE, SET_LATT_TYPE, BUBBLE, JUMP_ATT, RUSH_ATT, MOVE, NODE_RATATE_TO_TARGET
	};

	enum class ACTION_RESULT { AVOID = -1, FAILURE = 0, SUCESS = 1, PROCESS = 0xff };

public:
	void SetNoteManager(NoteManager* nm) { m_NoteManager = nm; };

	void GameStart();
	void GameClose();

public:
	void BossRelease();

public:
	void SetAttackId(const int id);
	NoteManager* GetNoteManager() const{ return m_NoteManager; }
	float GetJumpAttackSpeed() { return JumpAttackMoveSpeed; }

private:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void FixedUpdate() override;
	void Update() override;

private:
	void CheckBossCollision();

	bool MapColliderCheck(float& delta);
	bool ObjectColliderCheck(float& delta);
	bool ActionDealy();

	void ClearState();

private:
	int Idle();

	int ChangeCloseRangeAttack();

	int TongueAttack();
	int BackJump();
	int BackMove();

	int ChangeLongRangeAttack();

	int RangeAttack();
	int RushAttack();
	int JumpAttack();
	int RotateToTarget();

	int Move();

	int SetTarget();
	int SearchTarget();
};

