#pragma once
#include "hosEngine/Com_Client.h"
#include "PlayerState.h"
#include "ToxicManager.h"

class Projectile : public hos::com::Script
{

private:
	int AttackType = 0;
	int BossAttackDamage = 12;
	int AttackDamage = 5;
	float MoveSpeed = 8.f;
	float AnimationTimeScale = 1.f;
	float ProjectileDisableTime = 10.f;
	float ProjectileDisableTimer = 0.f;
public:
	hos::com::CapsuleCollision* AttackArea = nullptr;
	ToxicManager* m_ToxicManager = nullptr;
	PlayerState* m_PlayerState = nullptr;

public:
	Projectile();
	virtual ~Projectile();

public:
	void SetAttackType(int attackType) { AttackType = attackType; };

public:
	void Reset() override;
	void Awake() override;
	void Update() override;
	void LateUpdate() override;
	void OnTriggerEnter(hos::com::Collision* other);

};

