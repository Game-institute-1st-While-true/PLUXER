#pragma once
#include "../packets/PacketDefine.h"
#include "hosEngine/Transform.h"
#include "hosEngine/Networkcomponent.h"

#include "Move.h"
#include "User.h"

using namespace hos::com;
using namespace hos::Packet;
class NetworkEvent : public hos::com::Component
{
	Group* group;
	std::vector<User*> users;
	int objcount;
	float synctime;

public:
	NetworkEvent() = delete;
	NetworkEvent(Group* now_group);
	virtual ~NetworkEvent();

public:
	void Awake() override;
	void Update() override;
	void LateUpdate() override;
	void Reset() {};

private:
	void MappingProtocol(Networkcomponent* network);
	void MakeEventPacket(int id, int event_types, int result);
	void MakeActiveUserPosition();

	void EventUpdate(PACKAGE& package);
	void MoveUpdate(PACKAGE& package);
};

