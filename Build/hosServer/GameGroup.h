#pragma once
#include "User.h"
#include "hosEngine/Transform.h"
#include "hosEngine/GroupCallBackcomponent.h"
#include "hosEngine/GroupSystem.h"

using namespace hos;
using namespace com;

class GameGroup : public GroupCallBackcomponent
{
private:
	std::vector<User*> Users;
	std::function<void(GameGroup&)> updatefunc;

	float LoadingTime;
	int Loadingcount = 0;
	float GameTime;

public:
	GameGroup() = delete;
	GameGroup(GroupSystem* engine, Group* group, GameObject* obj, float sendtime);
	virtual ~GameGroup();

public:
	void AddSceneUser();

private:
	void OnEnable()override;
	void OnDisable()override;
	void Awake() override;
	void Reset() override;

	void Update() override;
	void LateUpdate() override;

	virtual void OnAddToken(NetworkToken* token, int tokenIndex);		// 그룹에 토큰이 생성되었을 때 (유저입장)
	virtual void OnDelToken(NetworkToken* token, int tokenIndex);		// 그룹에 토큰이 제거되었을 때 (유저퇴장)
	virtual void OnStateUpdate();
	
	void OnUpateFunc() override;										// 일정시간 마다 Update 호출	
	void MappingProtocol(Networkcomponent* network);
	
	void PositionUpdate();
	void LoadingSceneUpdate();
	void GameCloseCheck();

	void ReadyCheck();
	void NotImplemented();

	void AddTokenEventSend(User* user);
	void DelTokenEventSend(User* user);

	void Req_FourceLobby();
	void Req_SetUp();
	void Req_LoadScene(int scene_number);
	void Req_LoadScene(NetworkToken* token, int scene_number);

	void Ans_ReqID(PACKAGE& package);
	void Ans_Ready(PACKAGE& package);
	void Ans_Move(PACKAGE& package);
	void Ans_SetUp(PACKAGE& package);
};