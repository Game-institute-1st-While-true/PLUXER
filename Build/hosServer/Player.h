#pragma once
#include "GroupSystem.h"
#include "PlayerState.h"
#include "PlayerMove.h"

class PlayerState;
class PlayerMove;
class BossRoom;

class Player : public hos::com::Script
{
public:
	// 플레이어가 보유한 컴포넌트, 로비 씬 에서는 딱히 필요 없음.
	PlayerState* State;
	PlayerMove* Move;
	hos::com::CapsuleCollision* Capsule;
	BossRoom* Room;

private:
#pragma region Damage formula
	float Smash1DamageRate = 2.3f;
	float Smash2DamageRate = 2.f;
	float Smash3DamageRate = 1.8f;
	float Smash4DamageRate = 1.6f;

	float MultiSmash1DamageRate = 1.3f;
	float MultiSmash2DamageRate = 1.35f;
	float MultiSmash3DamageRate = 1.4f;
	float MultiSmash4DamageRate = 1.45f;

	float AttackRange = 1.f;
	float AttackDamage = 0.f;
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

	int SmashSuccessInput = 0;
#pragma endregion


private:
	hos::string Id;
	hos::string Pw;
	hos::Vector3 PrePos;

	int Uuid;
	bool Cert;

	bool IsReady;
	bool IsLoad;
	bool IsPlay;

public:
	Player();
	virtual ~Player();

public:
private:
	void Reset() override;
	void Awake() override;
	void Update() override;
	void LateUpdate() override;

public:
	// getter / setter

	hos::string GetId() { return Id; }
	void SetId(std::wstring id) { Id = id; }
	hos::string GetPw() { return Pw; }

	void SetUuid(int id) { Uuid = id; };
	int GetUuid() { return Uuid; }
	void SetPlayerInfo(PlayerState* state, PlayerMove* behaivor);
	void SetReady(bool ready) { IsReady = ready; }
	bool GetReady() { return IsReady; }
	void SetLoad(bool load) { IsLoad = load; }
	bool GetLoad() { return IsLoad; }
	void SetPlay(bool play) { IsPlay = play; }
	bool GetPlay() { return IsPlay; }

public:
	void PlayerComponentsEnable();

	void CheckPlayerCollision();

	bool MapColliderCheck(double& delta);
	bool ObjectColliderCheck(double& delta);

	bool MapAndObjectColliderCheck();
	void ReleaseDirection();
	void SetAttackDamage();

private:
	// ### FROM DBA -> flat buffer
	void DB_Ans_Certification(PACKAGE& package);

	// ### FROM CLIENT -> flex buffer

	// Game Flaga
	void Ans_Certification(PACKAGE& package);
	void Ans_ReadyFlag(PACKAGE& package);
	void Ans_LoadFlag(PACKAGE& package);

	// Game Data
	void Ans_StateUpdate(PACKAGE& package);
	void Ans_TimeScaleUpdate(PACKAGE& package);
	void Ans_OnAttackEvent(PACKAGE& package);

	void Ans_CloseGameInfo(PACKAGE& package);
	void Ans_FeverRequest(PACKAGE& package);
};