#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "PlayerState.h"
#include "BTNode.h"

class BossFrogState : public hos::com::Script
{
public:
	enum class BossFrogActionList
	{
		Idle, Dead, Move, JumpAttack, BackJump, RangeAttack, TongueAttack, RushAttack, BackMove, RotateToTarget, SearchTarget
	};

	enum class BossFrogTrafficLight
	{
		Red , Green, Yellow
	};

private:	
	BossFrogActionList NowAction;
	BossFrogActionList PrevAction;
	BossFrogActionList NextAction;

	hos::Vector3 NowDir;
	hos::Vector3 PreDir;
	std::chrono::system_clock::time_point DirTimePoint;

	float MaxHealthPoint = 26000.f;
	float HealthPoint;

	float AnimationTimeScale;

	bool IsAnimationEnd;
	bool IsDamaged;

	float DamagedTimer;
	float DamagedTime = 0.001f;

	float NowHitValue = 0.f;

	bool IsInTongueAttack;	

	int AIType; // --> Ai or No Ai ...

	bool IsStop;

	BossFrogTrafficLight TrafficLightColor = BossFrogTrafficLight::Green;

public:
	hos::com::Animation* BossFrogAnimation;
	hos::com::AudioSource* BossFrogDamagedSound;
	PlayerState* m_PlayerState;
	hos::com::SkinnedMeshRenderer* BossTrafficLight;
	hos::com::SkinnedMeshRenderer* BossBody;

public:
	BossFrogState();
	virtual ~BossFrogState();

public:
	BossFrogActionList GetNowAction() const { return NowAction; };
	BossFrogActionList GetNextAction() const { return NextAction; };
	void SetNowAction(BossFrogActionList val);
	void SetNextAction(BossFrogActionList val) { NextAction = val; }
	BossFrogActionList GetPrevAction() const { return PrevAction; };
	bool GetIsAnimationEnd() const { return IsAnimationEnd; };
	float GetAnimationTimeScale() const { return AnimationTimeScale; };
	float GetDamaged(float damage, int damagedType);
	float GetNowHitValue() const { return NowHitValue; };
	bool GetIsDamaged() const { return IsDamaged; };
	float GetMaxHealthPoint() const { return MaxHealthPoint; };
	void SetMaxHealthPoint(float health) { MaxHealthPoint = health; HealthPoint = MaxHealthPoint; };
	float GetHealthPoint() const { return HealthPoint; };
	void SetHealthPoint(float hp) { HealthPoint = hp; };
	bool GetIsInTongueAttack() const { return IsInTongueAttack; };
	void SetDirection(hos::Vector3 dir, bool on_event);
	hos::Vector3 GetDirection() { return NowDir; }
	void SetAIMode(const int type) { AIType = type; };
	int GetAiType() { return AIType; };

	BossFrogState::BossFrogTrafficLight GetTrafficLightColor() const { return TrafficLightColor; }
	void SetTrafficLightColor(BossFrogState::BossFrogTrafficLight val) { TrafficLightColor = val; }

	bool GetStop() const { return IsStop; }
	void SetStop(bool btn);

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	void PlayerSmashTime();
	void PlayerFeverTime();
	void BossFrogTrafficLightColorChange();

private:
	void OnStateUpdate();
};

