#pragma once

#include "hosEngine/COM_Client.h"
#include "PlayerState.h"
#include "NoteManager.h"
#include "UIManager.h"
#include "MeleeFrogState.h"
#include "RangeFrogState.h"
#include "ShieldFrogState.h"

class UIManager;

class PlayerAttack : public hos::com::Script
{
private:
	float Smash1DamageRate = 2.3f;
	float Smash2DamageRate = 2.f;
	float Smash3DamageRate = 1.8f;
	float Smash4DamageRate = 1.6f;

	float AttackRange = 1.f;
	float AttackDamage = 15.f;
	float SmashAccuracyRate = 0.f;
	float SmashBaseRate = 0.6f;
	float SmashSuccessRateIncreaseAmount = 0.3f;
	float ComboDamagedIncrease = 0.f;
	float BaseDamage = 0.f;
	float FeverNormalDamageRate = 0.375f;
	float FeverSmashDamageRate = 0.1f;
	float AccuracyRate = 0.f;
	float NormalAttackDamage = 30.f;
	float NormalAttackDamageRate = 2.f;

	float SmashAttackDamage = 120.f;

	float FeverSmashAttackRate = 2.5f;

	float FeverSmash1Damage = 159.7f;
	float FeverSmash2Damage = 136.9f;
	float FeverSmash3Damage = 171.1f;
	float FeverSmash4Damage = 102.7f;

	float AttackAreaActiveTime = 0.05f;
	float NowAttackAreaActiveTime = 0.0f;

	bool IsSetFeverSmashTime = false;
	bool IsPlayerFeverSmashSound = false;
	float FeverSmashStart = 0.f;
	float FeverSmashEnd = 0.f;
	float FeverSmashTimer = 0.f;

	bool IsSmashStart = false;
	bool IsUserSmashInput = false;
	bool IsNextSmash = false;
	float NextSmashTime = 0;

	int SmashInputNoteCount = 0;
	int NowSmashNoteCount = 0;
	int CurrentSmashNoteCount = 0;
	int UserSmashInput = -1;
	int SmashSuccessInput = 0;

	float ControlSmashTimeScale = 0.2f;
	float NowControlSmashTimeScale = 0.0f;

	static const int SmashNoteMaxCount = 5;

	NoteManager::NoteErrorRangeCheck SmashNoteCheck[SmashNoteMaxCount];

	// 0 : W | 1 : A | 2 : S | 3 : D
	int CurrentSmashNote[SmashNoteMaxCount];

public:
	NoteManager* m_NoteManager;
	PlayerState* m_PlayerState;
	UIManager* m_UIManager;
	hos::com::BoxCollision* AttackArea;
	hos::com::Transform* PlayerTransform;
	hos::com::Transform* AttackAreaTransform;

	hos::com::AudioSource* HitSound;
	hos::com::AudioSource* ActionSound;
	hos::com::AudioSource* CriMissSound;

public:
	PlayerAttack();
	virtual ~PlayerAttack();

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void Render() override;
	void OnTriggerEnter(hos::com::Collision* other) override;

	// GetSet
public:
	bool GetIsSmashStart() const { return IsSmashStart; };
	int GetSmashNoteMaxCount() const { return SmashNoteMaxCount; };
	const int* GetCurrentSmashNote() const { return CurrentSmashNote; };
	int GetCurrentSmashNoteCount() const { return CurrentSmashNoteCount; };
	const NoteManager::NoteErrorRangeCheck* GetSmashNoteCheck() const { return SmashNoteCheck; };
	int GetSmashInputNoteCount() const { return SmashInputNoteCount; };
	int GetNowSmashNoteCount() const { return NowSmashNoteCount; };
	int GetUserSmashInput() const { return UserSmashInput; };

private:
	void NormalAttack();
	void SmashAttack();
	void FeverSmashAttack();
	void CheckSmashAttackInput();
	void SetSmashAttack();
	NoteManager::NoteErrorRangeCheck CheckSmashNoteAccurancyAverage();
	void SetAttackDamage();
};