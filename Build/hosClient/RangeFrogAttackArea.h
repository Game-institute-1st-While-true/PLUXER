#pragma once
#include "hosEngine/Com_Client.h"
#include "RangeFrogState.h"
#include "PlayerState.h"

class RangeFrogAttackArea : public hos::com::Script
{
private:
	int AttackDamage = 12;

public:
	RangeFrogState* m_RangeFrogState;
	hos::com::SphereCollision* AttackCollision;
	hos::com::AudioSource* HitSound;

public:
	RangeFrogAttackArea();
	virtual ~RangeFrogAttackArea();

public:
	void Reset();
	void Awake() override;
	void OnTriggerEnter(hos::com::Collision* other) override;

};

