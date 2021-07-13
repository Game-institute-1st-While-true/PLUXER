#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "PlayerState.h"

class MeleeFrogState : public hos::com::Script
{
public:
	enum class MeleeFrogActionList
	{
		Idle, Damaged, Dead, Attak1, Attak2, Move
	};
private:
	MeleeFrogActionList NowAction = MeleeFrogActionList::Idle;
	MeleeFrogActionList PrevAction = MeleeFrogActionList::Idle;

	float MaxHealthPoint = 690.f;
	float HealthPoint = MaxHealthPoint;

	bool IsAnimationEnd = false;

	float AnimationTimeScale = 1.f;

	bool IsInDetect = false;
	bool IsInAttack = false;

	bool IsActivation = false;

	float NowHitValue = 0.f;
	float AddDamage = 0.f;
	bool IsDamaged = false;

public:
	hos::com::Animation* MeleeFrogAnimation = nullptr;
	hos::com::SphereCollision* DamagedArea = nullptr;
	PlayerState* m_PlayerState = nullptr;
	hos::com::AudioSource* ActionSound = nullptr;

	//////////////////////////////////////////////////////////////////////////
	// UI
	hos::com::GameObject* HPBar = nullptr;
	hos::com::UIImage* HPUI = nullptr;

	hos::com::UIText* DamageUI = nullptr;
	hos::com::UIText* DamageUIBack = nullptr;

public:
	MeleeFrogState();
	virtual ~MeleeFrogState();

public:
	MeleeFrogState::MeleeFrogActionList GetNowAction() const { return NowAction; };
	void SetNowAction(MeleeFrogActionList val);
	MeleeFrogState::MeleeFrogActionList GetPrevAction() const { return PrevAction; };
	bool GetIsAnimationEnd() const { return IsAnimationEnd; };
	float GetAnimationTimeScale() const { return AnimationTimeScale; };
	float GetDamaged(float damage, int damagedType);
	float GetHealthPoint() const { return HealthPoint; };
	bool GetIsInDetect() const { return IsInDetect; };
	bool GetIsInAttack() const { return IsInAttack; };
	bool GetIsActivation() const { return IsActivation; };
	void SetIsActivation(bool val) { IsActivation = val; };

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnEnable() override;
	void Render() override;

private:
	void PlayerSmashTime();
	void PlayerFeverTime();
	void PlayerDetect();

	void ShowUI();

};

