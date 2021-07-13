#pragma once

#include "hosEngine/Com_Client.h"

class EnemyManager : public hos::com::Script
{
private:
	bool IsClear;
	hos::com::GameObject* Portal;
	hos::com::GameObject* Boss;
	bool IsEnemiesDead;
	std::vector<hos::com::GameObject*> Enemies;

public:
	EnemyManager();
	virtual ~EnemyManager();

public:
	void Reset() override;
	void Awake() override;
	void Update() override;

public:
	bool GetIsAllEnemyDead() const { return IsClear; };

};