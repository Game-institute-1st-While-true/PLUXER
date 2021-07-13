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

	virtual void OnAddToken(NetworkToken* token, int tokenIndex);		// �׷쿡 ��ū�� �����Ǿ��� �� (��������)
	virtual void OnDelToken(NetworkToken* token, int tokenIndex);		// �׷쿡 ��ū�� ���ŵǾ��� �� (��������)
	virtual void OnStateUpdate();
	void OnUpateFunc() override;										// �����ð� ���� Update ȣ��

	void AddTokenEventSend(User* user);
	void DelTokenEventSend(User* user);
};