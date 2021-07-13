#pragma once
#include "User.h"
#include "hosEngine/Transform.h"
#include "hosEngine/GroupCallBackcomponent.h"
#include "hosEngine/GroupSystem.h"

using namespace hos;
using namespace com;

class LobbyGroup : public GroupCallBackcomponent
{
private:
	std::vector<User*> Users;

public:
	LobbyGroup() = delete;
	LobbyGroup(GroupSystem* engine, Group* group, GameObject* obj, float sendtime);
	virtual ~LobbyGroup();

private:
	void OnEnable()override;
	void OnDisable()override;
	void Reset() override;
	void Req_LoadScene(NetworkToken* token, int scene_number);

	virtual void OnAddToken(NetworkToken* token, int tokenIndex);		// 그룹에 토큰이 생성되었을 때 (유저입장)
	virtual void OnDelToken(NetworkToken* token, int tokenIndex);		// 그룹에 토큰이 제거되었을 때 (유저퇴장)
	virtual void OnStateUpdate();
	void OnUpateFunc() override;										// 일정시간 마다 Update 호출

	void AddTokenEventSend(User* user);
	void DelTokenEventSend(User* user);
};