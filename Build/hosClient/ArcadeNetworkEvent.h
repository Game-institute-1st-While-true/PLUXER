#ifndef _ARCADE_NETWORK_EVENT_H
#define _ARCADE_NETWORK_EVENT_H

#include "hosEngine/COM_Client.h"
#include "PlayerState.h"
#include "ProjectileManager.h"

class ArcadeNetworkEvent : public hos::com::Script
{
private:
	bool IsGameEnd = false;
	bool IsGameRelease = false;

	float EndTime = 3.f;
	float EndTimer = 0.f;

public:
	hos::com::GameObject* Player;
	hos::com::GameObject* Remote;
	hos::com::GameObject* Boss;
	ProjectileManager* m_ProjectileManager;

	hos::com::GameObject* m_NoteManager;

	int BestCombo = 0;

	int MyScore = 0;
	int BestScore = 0;

	int BestComboPlayer = -1;
	int BestScorePlayer = -1;

public:
	ArcadeNetworkEvent();
	~ArcadeNetworkEvent();

	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render();

public:
	void Req_SceneSetting();
	bool GetIsGameEnd();

private:

	void Ans_GameStart(PACKAGE& package);

	void Ans_GameEnd(PACKAGE& package);
	void Ans_ReleaseGame(PACKAGE& package);

	void Ans_PositionEvent(PACKAGE& package);
	void Ans_StateUpdate(PACKAGE& package);
	void Ans_GetDamagedEvent(PACKAGE& package);
	void Ans_TimeScaleUpdate(PACKAGE& package);
	void Ans_MoveStateEvent(PACKAGE& package);
	void Ans_GameInfoUpdate(PACKAGE& package);
	void Ans_ProjectileActiveEvent(PACKAGE& package);
	void Ans_ToxicActiveEvent(PACKAGE& package);
	void Ans_FeverEvent(PACKAGE& package);
};

#endif // !_ARCADE_NETWORK_EVENT_H