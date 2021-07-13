#pragma once
#include "hosEngine/Com_Client.h"
#include "BossFrogState.h"
#include "PlayerState.h"

class BossFrogAttackArea : public hos::com::Script
{
private:
	int AttackDamage = 10;
	int AttackMode = 0; // 0 normal 1, debug
public:
	BossFrogState* m_BossFrogState;
	hos::com::BoxCollision* AttackCollision;
	bool IsAttack;

public:
	BossFrogAttackArea();
	virtual ~BossFrogAttackArea();

public:
	bool GetIsAttack() const { return IsAttack; };
	void SetIsAttack(bool isAttack) { IsAttack = isAttack; };
	void SetAttackMode(int mode) { AttackMode = mode; }

public:
	void Reset() override;
	void Awake() override;
	void OnTriggerEnter(hos::com::Collision* other) override;
};

