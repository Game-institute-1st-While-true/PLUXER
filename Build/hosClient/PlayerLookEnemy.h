#pragma once
#include "hosEngine/Com_Client.h"
#include "PlayerState.h"
#include "MeleeFrogState.h"
#include "RangeFrogState.h"
#include "BossFrogState.h"

class PlayerLookEnemy : public hos::com::Script
{
private:
	std::vector <hos::com::GameObject*> EnemyPosition;
	hos::Vector3 PlayerForword = hos::Vector3::Zero;
	hos::com::Transform* PlayerTransform;
	bool IsInEnemy = false;
	float InEnemyLength = 100.f;

	PlayerState* m_PlayerState;
	hos::com::SphereCollision* LookAtArea;
	float LookAtRadius = 2.f;

public:
	PlayerLookEnemy();
	virtual ~PlayerLookEnemy();

public:
	void Awake()override;
	void Update()override;
	void OnTriggerEnter(hos::com::Collision* other);
	void OnTriggerExit(hos::com::Collision* other);

public:
	hos::Vector3 GetPlayerForword() { return PlayerForword; };
	bool GetIsInEnemy() { return IsInEnemy; };

private:
	void IncreaseFeverAttackRange();
	void RemoveDeadEnmey();

};

