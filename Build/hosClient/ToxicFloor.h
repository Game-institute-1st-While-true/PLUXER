#pragma once
#include "hosEngine/Com_Client.h"
#include "PlayerState.h"

class ToxicFloor : public hos::com::Script
{
private:
	float ToxicRemoveTime = 10.f;
	float ToxicRemoveTimer = 0.f;

	bool IsPlayerEnter = false;

public:
	PlayerState* m_PlayerState = nullptr;
	hos::com::BoxCollision* SlowArea;
	hos::com::MeshRenderer* ToxicTexture;

public:
	ToxicFloor();
	virtual ~ToxicFloor();

public:
	void Reset() override;
	void Awake() override;
	void Update() override;
	void OnTriggerEnter(hos::com::Collision* other);
	void OnTriggerExit(hos::com::Collision* other);

public:
	void ResetToxicTimer() { ToxicRemoveTimer = 0.f; };

};

