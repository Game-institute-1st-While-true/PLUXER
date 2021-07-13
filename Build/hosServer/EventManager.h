#pragma once
#include "hosPacketData_generated.h"
#include "hosMovePacket_generated.h"
#include "hosPositionPacket_generated.h"
#include "hosEngine/Transform.h"
#include "hosEngine/Networkcomponent.h"

using namespace hos::com;
class EventManager : public hos::com::Component
{
public:
	EventManager();
	virtual ~EventManager();

	GameObject* Events[10];

private:
};

