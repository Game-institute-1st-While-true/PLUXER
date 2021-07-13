#pragma once
#include "GroupCallBackcomponent.h"
#include "MapCollisionChecker.h"
#include "Player.h"
#include "BossFrog.h"
#include "NoteManager.h"

class BossRoom : public hos::com::GroupCallBackcomponent
{
public:
	std::vector<Player*> Players;
	MapCollisionChecker* MapCollider;
	BossFrog* Boss;

private:
	int Manager;
	std::function<void(BossRoom&)> UpdateFunc;
	NoteManager* m_NoteManager;
	std::chrono::high_resolution_clock::time_point PlayStartTime;
	
	int BestScore;
	int FirstConnectUserCount;

	bool LoadFlag;

	double PositionTime;
	double PositionCheckTime;
	unsigned int LoopCount;

	bool IsGameEnd;

	bool GameInfoSetupRequest;
	bool GotGameInfo;
	int GameCloseInfoGetCount;

	double EndTime;
	double EndTimer;

	bool IsGameRelease;

	bool GameClearResult;	 // true : Win / false : Lose

public:
	BossRoom(hos::com::Group* group);
	virtual ~BossRoom();

	void Req_PositionEvent();

private:
	void Req_LoadState();
	void Req_WaitState();				// ��� ������ �غ� ������.
	void Req_GameState();				// ������ ���� ���� �غ� ������.
	void Req_GameEnd();					// ������ ������. UI�� ������ �����.
	void Req_ReleaseState();			// ������ ������ Release ���°� �Ǿ���.	

	void Req_RoomInEvent(hos::string name, int tokenIndex);
	void Req_RoomOutEvent(hos::string name, int tokenIndex);

public:
	void Awake() override;
	void Update() override;
	void LateUpdate() override;
	void OnClose() override;

public:
	void AddPlayer(Player* player, PlayerState* state, PlayerMove* move);
	void Send(PACKET& pack);

	virtual void OnLoad();
	virtual void OnAddToken(hos::com::NetworkToken* token, int tokenIndex, std::wstring name, int uuid);
	virtual void OnDelToken(hos::com::NetworkToken* token, int tokenIndex);
	virtual void OnStateUpdate();

	bool CheckGameEnd() { return IsGameEnd; }

	void Req_GameInfo(Player& player);
	void Req_GameInfo(BossFrog& boss);
	void AddGameCloseCount();
	void SetBestScore(int score) { BestScore = score; }

private:
	void OutPlayerCheck();
	void GameEndCheck();
	void GameLifeCycle();
	void PositionUpdate();

private:	
	void UpdateNotImplemented();
	void UpdateReadyProcess();
	void UpdateLoadProcess();
	void UpdateGameProcess();
};