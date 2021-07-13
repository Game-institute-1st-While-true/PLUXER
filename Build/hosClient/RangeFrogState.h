#pragma once
#include "hosEngine/Com_Client.h"
#include "PlayerState.h"

class RangeFrogState : public hos::com::Script
{
public:
	enum class RangeFrogActionList 
	{ 
		Idle, Move, Dead, Damaged, Attack, BackMove, Wait, RotateMove
	};

private:
	RangeFrogActionList NowAction = RangeFrogActionList::Idle;
	RangeFrogActionList PrevAction = RangeFrogActionList::Idle;

	float MaxHealthPoint = 580.f;
	float HealthPoint = MaxHealthPoint;
	
	bool IsAnimationEnd = false;

	float AnimationTimeScale = 1.f;

	bool IsInDetect = false;
	bool IsInAttack = false;
	bool IsInAvoid = false;

	bool IsActivation = false;

	float NowHitValue = 0.f;
	float AddDamage = 0.f;
	bool IsDamaged = false;

public:
	hos::com::Animation* RangeFrogAnimation = nullptr;
	hos::com::SphereCollision* DamagedArea = nullptr;
	PlayerState* m_PlayerState = nullptr;
	hos::com::AudioSource* ActionSound = nullptr;

	//////////////////////////////////////////////////////////////////////////
	// UI
	hos::com::GameObject* HPBar = nullptr;
	hos::com::UIImage* HPUI = nullptr;

	hos::com::UIText* DamageUI = nullptr;
	hos::com::UIText* DamageUIBack = nullptr;;

public:
	RangeFrogState();
	virtual ~RangeFrogState();

public:
	RangeFrogState::RangeFrogActionList GetNowAction() const { return NowAction; };
	void SetNowAction(RangeFrogActionList val);
	RangeFrogState::RangeFrogActionList GetPrevAction() const { return PrevAction; };
	bool GetIsAnimationEnd() const { return IsAnimationEnd; };
	float GetAnimationTimeScale() const { return AnimationTimeScale; };
	float GetDamaged(float damage, int damagedType);
	float GetHealthPoint() const { return HealthPoint; };
	bool GetIsInDetect() const { return IsInDetect; };
	bool GetIsInAttack() const { return IsInAttack; };
	bool GetIsInAvoid() const { return IsInAvoid; };
	bool GetIsActivation() const { return IsActivation; };
	void SetIsActivation(bool val) { IsActivation = val; };


public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	void PlayerSmashTime();
	void PlayerFeverTime();
	void PlayerDetect();
	void ShowUI();


};

