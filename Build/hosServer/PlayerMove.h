#pragma once
#include "GroupSystem.h"
#include "PlayerState.h"

#include <mutex>

class PlayerMove : public hos::com::Script
{
public:
	std::mutex PositionMutex;

private:
	hos::Vector3 PrePos;
	float PlayerMoveSpeed;

	float DashSpeed;
	bool IsDashStart;

	float NormalAttackSpeed;
	double NormalAttackTime;
	double NormalAttackEndTime;
	bool IsNormalAttackStart;

	float DashAttackSpeed;

	double DamagedTime;
	double DamagedTimer;
	float DamagedDistance;
public:
	hos::com::Transform* PlayerTransform;
	PlayerState* m_PlayerState;

public:
	PlayerMove();
	virtual ~PlayerMove();

public:
	void SetPlayerState(PlayerState* state) { m_PlayerState = state; }
	const hos::Vector3 GetPrePos() { return PrePos; }
	void ToPreviousLocation();

	const double GetMoveSpeed() { return PlayerMoveSpeed; }
	const double GetDashSpeed() { return DashSpeed; }
	const double GetDashAttackSpeed() { return DashAttackSpeed; }
	const double GetNormalAttackSpeed() { return NormalAttackSpeed; }
	const double GetDamagedDistance() { return DamagedDistance; }

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void Render() override;

private:
	void Move();
	void Dash();
	void AttackMove();
	void DashAttackMove();
	void DamagedMove();
};

