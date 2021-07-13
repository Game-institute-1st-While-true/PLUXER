#pragma once
#include "hosEngine/COM_Client.h"
#include "CameraMove.h"
#include "PlayerState.h"

class PlayerMove : public hos::com::Script
{
private:
	double PlayerMoveSpeed = 4.f;

	double DashDistance = 20.f;
	bool IsDashStart = false;

	double NormalAttackDistance = 3.f;
	double NormalAttackTime = 0.f;
	double NormalAttackEndTime = 0.1f;
	bool IsNormalAttackStart = false;

	double DashAttackDistance = 5.f;

	bool MoveSendOn = false;

	float DamagedTime = 0.5f;
	float DamagedTimer = 0.f;
	float DamagedDistance = 6.f;

	bool IsInputCheat = false;

public:
	hos::com::Transform* PlayerTransform;
	hos::com::Transform* CameraTransform;
	hos::com::Transform* NoteManagerTransform;
	PlayerState* m_PlayerState;

public:
	PlayerMove();
	virtual ~PlayerMove();

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void Render() override;

private:
	// new move
	void Move();
	void Dash();
	void AttackMove();
	void DashAttackMove();
	void DamagedMove();
	void PlayerSlow();

	void GoToBoss();
};

