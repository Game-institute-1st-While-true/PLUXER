#pragma once
#include "hosEngine/Com_Client.h"
#include "StageInCheck.h"
#include "NoteManager.h"
#include "Portal.h"

class StageManager : public hos::com::Script
{
private:
	//int StageCount = 0; // 스테이지 갯수
	std::vector<StageInCheck*> Stages;

	bool IsPlayerBattle = false;
	bool IsInBossStage = false;
	bool IsAllStageClear = false;

	hos::com::GameObject* BossGate = nullptr;

	bool IsInputCheat = false;

public:
	NoteManager* m_NoteManager = nullptr;
	Portal* BossPortal = nullptr;

public:
	StageManager();
	virtual ~StageManager();

public:
	void Reset() override;
	void Awake() override;
	void Update() override;

private:
	void AllMonsterDead();

};

