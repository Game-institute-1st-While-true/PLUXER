#pragma once
#include "hosEngine/Com_Client.h"
#include "MeleeFrogState.h"
#include "RangeFrogState.h"

class StageInCheck : public hos::com::Script
{
private:
	bool IsPlayerIn = false;
	bool IsClear = false;

	std::vector<hos::com::GameObject*> StageEnemyList;

public:
	StageInCheck();
	virtual ~StageInCheck();

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerEnter(hos::com::Collision* other);

public:
	bool GetIsPlayerIn() const { return IsPlayerIn; };
	bool GetIsClear() const { return IsClear; };

public:
	void AllMonsterDead();


};

