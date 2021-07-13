#pragma once
#include "../packets/PacketDefine.h"
#include "hosEngine/Transform.h"
#include "hosEngine/Networkcomponent.h"

#include "Move.h"

constexpr int POSSYNCTIME = 1.0f;
using namespace hos::com;
class PositionTest : public hos::com::Component
{
public:
	PositionTest() = delete;
	PositionTest(bool ishost, int netid);

private:
	int	 NetId;
	bool IsHost;
	Transform* transfrom;
	Networkcomponent* network;
	PlayerBehaivor* move;

	float position_sync_time = 0.0f;
	
public:
	void Awake()override;
	void Reset()override;
	void Update()override;
	void LateUpdate()override;

private:
	void MakeMovePacket(hos::Packet::MOVE_DIR dir_toggle, bool dash);
	void MakePositionPacket();
	void PositionUpdate(PACKAGE& package);
	void MoveInputUpdate(PACKAGE& package);
};