#pragma once
#include "GroupCallBackcomponent.h"
#include "Player.h"

class Lobby : public hos::com::GroupCallBackcomponent
{
private:
	std::vector<Player*> Players;

public:
	Lobby(hos::com::Group* group);
	virtual ~Lobby();

	void Awake() override;

	void AddPlayer(Player* player);
	virtual void OnLoad();
	virtual void OnAddToken(hos::com::NetworkToken* token, int tokenIndex, std::wstring name, int uuid);
	virtual void OnDelToken(hos::com::NetworkToken* token, int tokenIndex);
	virtual void OnStateUpdate();
};

