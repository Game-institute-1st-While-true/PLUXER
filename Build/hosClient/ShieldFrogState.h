#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "PlayerState.h"
#include "ShieldFrogMeleeDetectZone.h"
#include "ShieldFrogRangeDetectZone.h"

class ShieldFrogState : public hos::com::Script
{
public:
	enum class ShieldFrogActionList
	{
		Idle, Damaged, Dead, Move
	};

private:
	ShieldFrogActionList NowAction;
	ShieldFrogActionList PrevAction;

	float MaxHealthPoint;
	float HealthPoint;

	bool IsAnimationEnd;

	float DamageTick;

	float AnimationTimeScale;

public:
	hos::com::Animation* ShieldFrogAnimation;
	hos::com::CapsuleCollision* DamagedArea;
	PlayerState* m_PlayerState;
	ShieldFrogMeleeDetectZone* MeleeDetectZone;
	ShieldFrogRangeDetectZone* RangeDetectZone;

public:
	ShieldFrogState();
	virtual ~ShieldFrogState();

	// GetSet
public:
	ShieldFrogState::ShieldFrogActionList GetNowAction() const { return NowAction; }
	void SetNowAction(ShieldFrogState::ShieldFrogActionList val);
	ShieldFrogState::ShieldFrogActionList GetPrevAction() const { return PrevAction; }
	bool GetIsAnimationEnd() const { return IsAnimationEnd; };
	float GetAnimationTimeScale() const { return AnimationTimeScale; };
	void GetDamaged(float damage);
	float GetHealthPoint() const { return HealthPoint; };


public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;


};

