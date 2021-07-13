#pragma once
#include "GroupSystem.h"
#include "NoteManager.h"

class ShieldFrogState : public hos::com::Script
{
public:
	enum class ShieldFrogActionList
	{
		Idle, Damaged, Dead
	};

private:
	ShieldFrogActionList NowAction;
	ShieldFrogActionList PrevAction;

	float MaxHealthPoint;
	float HealthPoint;

	bool IsAnimationEnd;

	float DamageTick;

public:
	hos::com::CapsuleCollision* DamagedArea;

public:
	ShieldFrogState();
	virtual ~ShieldFrogState();

	// GetSet
public:
	ShieldFrogState::ShieldFrogActionList GetNowAction() const { return NowAction; }
	void SetNowAction(ShieldFrogState::ShieldFrogActionList val);
	ShieldFrogState::ShieldFrogActionList GetPrevAction() const { return PrevAction; }
	bool GetIsAnimationEnd() const { return IsAnimationEnd; };
	void GetDamaged(float damage);


public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	
};

