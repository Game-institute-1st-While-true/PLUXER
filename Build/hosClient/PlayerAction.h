#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "PlayerState.h"
#include "PlayerLookEnemy.h"

class PlayerAction : public hos::com::Script
{
private: // Camera Info

	float CameraThetaY = 0.f;
	float CameraPosY = 3.f;
	float CameraDistance = 3.f;

	float CameraMoveSpeedX = 5.f;
	float CameraMoveSpeedY = 10.f;

	float PrevCameraMouseMoveX = 0.f;
	float PrevCameraMouseMoveY = 0.f;
	float NowCameraMouseMoveX = 0.f;
	float NowCameraMouseMoveY = 0.f;

	float MaxCameraPosY = 5.f;
	float MinCameraPosY = 0.5f;

	float FollowCameraTargetPosY = 1.f;

private:
	float InputDelay = 0.0f;
	bool IsInputActive = true;

public:
	// 판정 체크를 위한 노트 매니저
	NoteManager* m_NoteManager;
	PlayerState* m_PlayerState;
	
	// 범위 내에 몬스터가 있는 지 판단
	PlayerLookEnemy* m_PlayerLookEnemy;

	// 방향 적용을 위한 카메라 / 플레이어 트랜스폼
	hos::com::Transform* CameraTransform;
	hos::com::Transform* PlayerTransform;
	hos::com::Rigidbody* PlayerRig;

	hos::com::AudioSource* ActionSound;
	hos::com::AudioSource* CriMissSound;


public:
	PlayerAction();
	virtual ~PlayerAction();

public:
	bool GetIsInputActive() const { return IsInputActive; };
	void SetIsInputActive(bool btn) { IsInputActive = btn; }
public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void Render() override;

private:
	void CameraUpdate();
	void InputDirection();

	void InputCharge();
	void InputDash();
	void InputParrying();
	void InputSmash();
	void InputAttack();
	void InputFeverCast();
	void NoteManagerSetTransform();  

	void CameraBulrActive();

};

