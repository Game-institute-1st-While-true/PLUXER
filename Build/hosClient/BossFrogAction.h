#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "BossFrogState.h"
#include "BossFrogAttackArea.h"
#include "BTNode.h"
#include "ProjectileManager.h"
#include "PlayerState.h"

class BossFrogAction : public hos::com::Script
{
private:
	BehaviorTree* Bt;

private:

	// Dead
	float WaitDead = 5.f;
	float DeadTimer = 0.f;

	// Move
	bool IsMove = false;
	bool IsSetMoveTime = false;
	float MoveTime;
	float MoveTimer = 0.f;
	float MoveSpeed = 2.5f;
	float RotSpeed = 50.f;
	
	// RotateToTarget
	bool CheckRotateDir = false;
	bool RotateMoveDir = false;
	float RotateMoveTime = 2.5f;
	float RotateMoveTimer = 0.f;

	// BackMove
	bool IsSetBackMoveTime = false;
	float BackMoveTime;
	float BackMoveTimer = 0.f;

	// BackJump
	bool IsSetBackJump = false;
	float BackJumpTime = 0.3f;
	float BackJumpTimer = 0.f;
	float BackJumpMoveSpeed = 7.f;

	// Tongue Attack
	bool IsSetTongueAttack = false;
	bool IsTongueAttackReady = false;
	float TongueAttackReadyTime = 0.91667f;
	float TongueAttackReadyTimer = 0.f;
	float TongueAttackTimer = 0.f;
	float TongueAttackEnable = 0.5f;
	float TongueAttackDisable = 0.66667f;

	// Jump Attack
	bool IsSetJumpAttack = false;
	bool IsJumpAttackReady = false;
	float JumpAttackReadyTime = 0.61111f;
	float JumpAttackReadyTimer = 0.f;
	float JumpAttackMove = 0.83f;
	float JumpAttackEnable = 1.333f;
	float JumpAttackDisable = 1.666f;
	float JumpAttackTimer = 0.f;
	float JumpAttackMoveSpeed;

	float JumpAttackShakeTime = 0.5f;
	float JumpAttackShakeValue = 0.f;
	float JumpAttackShakeDistance = 15.f;

	// Rush Attack
	bool IsSetRushAttack = false;
	bool IsRushAttackReady = false;
	float RushAttakReadyTime = 1.1f;
	float RushAttakReadyTimer = 0.f;
	float RushAttackTimer = 0.f;
	float RushAttackTime = 2.5f;
	float RushMoveSpeed = 10.f;

	float RushAttackShakeTime = 0.2f;
	float RushAttackShakeValue = 0.f;
	float RushAttackShakeDistance = 10.f;

	// Range Attack
	bool IsSetRangeAttack = false;
	bool IsReverse = false;
	bool IsRangeAttackReady = false;
	float RangeAttackReadyTime = 0.68750f;
	float RangeAttackReadyTimer = 0.f;
	int RangeAttackCount = 3;
	float RangeAttackTimer = 0.f;
	//float RangeAttackReverse = 0.96667f;
	//float RangeAttackEnd = 1.23333f;

	// Sound
	bool IsSoundChange = false;

public:
	NoteManager* m_NoteManager;
	BossFrogState* m_BossFrogState;
	hos::com::NavAgent* m_NavAgent;
	hos::com::Transform* PlayerTransform;
	PlayerState* m_PlayerState;
	hos::com::Transform* BossFrogTransform;

	BossFrogAttackArea* AttackArea;

	ProjectileManager* m_ProjectileManager;

	hos::com::AudioSource* MelodySound;
	hos::com::AudioSource* ReadySound;

public:
	enum class ON_AI_TYPE { ROOT,
							START_SELECT, // 분기를 나누기 위한 서비스  
							ON_AI_SELECT,
		NODE_DEAD,			ATTACK_SELECT,												MOVE_SELECT,					NODE_IDLE,
	NODE_RUSH_ATT,NODE_RANGE_ATT,NODE_JUMP_ATT,NODE_TONGUE_ATT,		NODE_BACK_JUMP ,NODE_RATATE_TO_TARGET,NODE_BACK_MOVE,NODE_MOVE, END};

	enum class NO_AI_TYPE { DEBUG_SERVICE_NODE = static_cast<int>(ON_AI_TYPE::END),
							NO_AI_SELECT,
							NODE_DEAD, NODE_IDLE, TONGUE_ATTACK, NODE_MOVE};

	enum class ACTION_RESULT { AVOID = -1, FAILURE = 0, SUCESS = 1, PROCESS = 0xff};

public:
	BossFrogAction();
	virtual ~BossFrogAction();

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;

public:
	void BossNetBehaivortree();
	void SetJumpMoveSpeed(float speed) { JumpAttackMoveSpeed = speed; }

private:
	// Node
	int Idle();
	int Dead();
	int Move();
	int RotateToTarget();
	int TongueAttack();
	int JumpAttack();
	int RangeAttack();
	int RushAttack();
	int BackMove();
	int BackJump();
	
	void ChangeState();

	void AudioTimeScale();

	void RotateToPlayer();
};

