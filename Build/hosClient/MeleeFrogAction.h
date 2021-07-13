#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "MeleeFrogState.h"
#include "MeleeFrogAttackArea.h"
#include "BTNode.h"

class MeleeFrogIdle;
class MeleeFrogDamaged;
class MeleeFrogDead;
class MeleeFrogDetect;
class MeleeFrogAttack;
class MeleeFrogMove;

class MeleeFrogAction : public hos::com::Script
{
public:
	NoteManager* m_NoteManager;
	MeleeFrogState* m_MeleeFrogState;
	hos::com::NavAgent* m_NavAgent;
	hos::com::Transform* PlayerTransform;
	hos::com::Transform* MeleeFrogTransform;

	MeleeFrogAttackArea* AttackArea;

	hos::com::AudioSource* ActionSound;

	//////////////////////////////////////////////////////////////////////////

	BTRoot* RootNode;

	BTSelector* StartSelector;
	BTSequence* AttackSequence;
	BTSelector* AttackSelector;

	MeleeFrogIdle* NodeIdle;
	MeleeFrogDamaged* NodeDamaged;
	MeleeFrogDead* NodeDead;
	MeleeFrogDetect* NodeDetect;
	MeleeFrogAttack* NodeAttack;
	MeleeFrogMove* NodeMove;

	//////////////////////////////////////////////////////////////////////////

	bool IsAttackCoolTime;

public:
	MeleeFrogAction();
	virtual ~MeleeFrogAction();

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
private:
	void AudioTimeScale();
};

class MeleeFrogIdle : public BTNode
{
public:
	MeleeFrogIdle(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~MeleeFrogIdle() {};

public:
	virtual SearchResult Invoke() override;
};

class MeleeFrogDamaged : public BTNode
{
public:
	MeleeFrogDamaged(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~MeleeFrogDamaged() {};

public:
	virtual SearchResult Invoke() override;
};

class MeleeFrogDead : public BTNode
{
private:
	float DeadTimer = 0.0f;
	float WaitDead = 2.0f;
	bool IsDead = false;


public:
	MeleeFrogDead(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~MeleeFrogDead() {};

public:
	virtual SearchResult Invoke() override;
};

// 탐지
class MeleeFrogDetect : public BTNode
{
public:
	MeleeFrogDetect(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~MeleeFrogDetect() {};

public:
	virtual SearchResult Invoke() override;
};

// 공격
class MeleeFrogAttack : public BTNode
{
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

public:
	MeleeFrogAttack(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~MeleeFrogAttack() {};

public:
	virtual SearchResult Invoke() override;
};

// 이동
class MeleeFrogMove : public BTNode
{
private:
	float RotateSpeed = 20.f;
	float MoveSpeed = 0.5f;

public:
	MeleeFrogMove(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~MeleeFrogMove() {};

public:
	virtual SearchResult Invoke() override;

private:
	// 이동
	// 회피 이동
	// 공격 후 이동

};