#ifndef _CAMERA_MOVE_H
#define _CAMERA_MOVE_H

#include "hosEngine/COM_Client.h"

// [2021/01/25 노종원]
// 카메라 이동에 관한 스크립트

class CameraMove : public hos::com::Script
{
private:
	//float ThetaY;
	//float PosY;
	//float Distance;
	//float CameraYPos;
	//float MoveSpeed;

	//float PrevMouseMove;
	//float NowMouseMove;

	bool IsDebugCamera = false;

public:
	hos::com::Transform* CameraTransform = nullptr;
	//hos::com::Transform* PlayerTransform;
	//hos::com::Camera* PlayerViewCamera;

public:
	CameraMove() :hos::com::Script(L"CameraMove") {};

	bool GetIsDebugCamera() const { return IsDebugCamera; };

public:
	void Awake()override;
	void Reset()override;
	void Update()override;
	void Render()override;

};

#endif // !_CAMERA_MOVE_H