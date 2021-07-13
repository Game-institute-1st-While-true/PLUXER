#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "MeleeFrogState.h"
#include "MeleeFrogAttackArea.h"
#include "BTNode.h"

class MeleeFrog : public hos::com::Script
{
private:
	BehaviorTree* Bt;

private:
	float DeadTimer = 0.0f;
	float WaitDead = 1.0f;

private:
	float AttackCoolTime = 4.f;
	float AttackCoolTimeTimer = 0.f;
	int NowAttackMotion = -1;

	float AttackAnimationTimer = 0.f;
	float Attack1DamageEnable = 4.5f;
	float Attack1DamageDisable = 5.0f;
	float Attack2DamageEnable = 4.0f;
	float Attack2DamageDisable = 6.0f;

	float AttackMoveDistance = 0.6f;

private:
	float RotateSpeed = 20.f;
	float MoveSpeed = 0.5f;


public:
	NoteManager* m_NoteManager;
	MeleeFrogState* m_MeleeFrogState;
	hos::com::NavAgent* m_NavAgent;
	hos::com::Transform* PlayerTransform;
	hos::com::Transform* MeleeFrogTransform;

	MeleeFrogAttackArea* AttackArea;

	//////////////////////////////////////////////////////////////////////////

	bool IsAttackCoolTime;

public:
	enum class ACTION_TYPE {ROOT, 
							START_SELECT, NODE_DEAD, NODE_DEMEAGED, ATT_SEQ, IDLE,
																    DETECT, ATT_SELECT,
																	    	    ATTACK, MOVE};

	enum class ACTION_RESULT { AVOID = -1, FAILURE = 0, SUCESS = 1, PROCESS = 0xff};
	
public:
	MeleeFrog();
	virtual ~MeleeFrog();

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	int Idle();
	int Damaged();
	int Dead();
	int Detect();
	int Attack();
	int Move();
};

