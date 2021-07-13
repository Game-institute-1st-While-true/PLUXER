#pragma once

#include "hosEngine/COM_Client.h"
#include "MeleeFrogState.h"
#include "PlayerState.h"

class MeleeFrogAttackArea : public hos::com::Script
{
private:
	int Attak0Damage = 5;
	int Attak1Damage = 7;
	

public:
	MeleeFrogState* m_MeleeFrogState;
	hos::com::BoxCollision* AttackCollision;
	bool IsAttack;
	hos::com::AudioSource* HitSound;
public:
	MeleeFrogAttackArea();
	virtual ~MeleeFrogAttackArea();

public:
	bool GetIsAttack() const { return IsAttack; };
	void SetIsAttack(bool isAttack) { IsAttack = isAttack; };

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void OnTriggerEnter(hos::com::Collision* other) override;
	void Render() override;

};

