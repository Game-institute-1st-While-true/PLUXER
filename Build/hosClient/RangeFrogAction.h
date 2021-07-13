#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "RangeFrogState.h"
#include "RangeFrogAttackArea.h"
#include "ProjectileManager.h"
#include "BTNode.h"

class RangeFrogAction : public hos::com::Script
{
private:
	BehaviorTree* Bt;

private:
	float MoveSpeed = 2.5f;
	float BackMoveSpeed = 6.f;
	float RotateSpeed = 60.f;

	bool IsSelect = false;

	bool IsWait = false;
	float WaitTimer = 0.f;
	float WaitTime = 1.5f;

	bool IsBackMove = false;
	float BackMoveTimer = 0.f;
	float BackMoveTime = 2.0f;

	bool IsRotateMove = false;
	bool IsSelectRotate = false;
	bool IsRight = false;
	bool IsRotateEnd = false;
	float RotateValue = 0.f;
	float RotateTimer = 0.f;
	float RotateTime = 1.f;
	float RotateMoveTime = 2.5f;

	bool IsAttack = false;
	bool IsFire = false;
	bool IsAttackMove = false;
	bool IsAttackAvoid = false;
	float AttackMoveTimer = 0.f;
	float AttackMoveTime = 1.f;
	float AttackTimer = 0.f;
	float AttackTime = 1.f;
	float AttackBackMoveTimer = 0.f;
	float AttackBackMoveTime = 0.15f;

	bool IsDeadMove = false;
	float DeadTimer = 0.f;
	float DeadMoveTime = 2.5f;
	float DeadAttackTime = 0.2f;
	float RemoveDeadTime = 2.f;

public:
	NoteManager* m_NoteManager;
	ProjectileManager* m_ProjectileManager;
	RangeFrogState* m_RangeFrogState;
	hos::com::NavAgent* m_NavAgent;
	hos::com::Transform* PlayerTransform;
	hos::com::Transform* RangeFrogTransform;

	RangeFrogAttackArea* AttackArea;

	hos::com::AudioSource* ActionSound;

public:
	enum class ACTION_TYPE {ROOT, 
							START_SELECT, NODE_DEAD, NODE_DAMAGED, ATT_SEQ, IDLE,
															DETECT, ATT_SELECT,
																	ATTACK, MOVE};

	enum class ACTION_RESULT { ABORT = -1, FAILURE = 0, SUCCESS = 1, CONTINUE = 0Xff };

public:
	RangeFrogAction();
	virtual ~RangeFrogAction();

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	int Idle();
	int Dead();
	int Attack();
	int Damaged();

	void MoveFrog(bool val);
	void SelectState();
	void StateAttack();
	void StateBackMove();
	void StateWait();
	void StateRotateMove();
	void ResetState();

private:
	void AudioTimeScale();
};

