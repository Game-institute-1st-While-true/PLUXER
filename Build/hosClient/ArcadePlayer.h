#ifndef _ARCADE_PLAYER_H
#define _ARCADE_PLAYER_H

#include "hosEngine/COM_Client.h"
#include "PlayerState.h"

class ArcadeNetworkEvent;

class ArcadePlayer:public hos::com::Script
{
private:
	ArcadeNetworkEvent* Net;
	std::chrono::steady_clock::time_point prevTime;
	
	hos::Vector3 PreDir;
	hos::Vector3 NowDir;

public:
	ArcadePlayer();
	~ArcadePlayer();

	void Start()override;
	void FixedUpdate()override;
};
#endif // !_ARCADE_PLAYER_H