#pragma once
#include "hosBasePacket_generated.h"
#include "hosEventPacket_generated.h"
#include "hosPacketData_generated.h"
#include "hosMovePacket_generated.h"
#include "hosPositionPacket_generated.h"
#include "hosPositionPacketList_generated.h"
#include "UserEventPacket_generated.h"
#include "GroupListPacket_generated.h"
#include "ID_PW_generated.h"
#include "OneCode_generated.h"
#include "RoomSetting_generated.h"
#include "GameInfo_generated.h"

using namespace hos::Packet;
using namespace db::Packet;

enum class ROOM_TYPE : unsigned char
{
	LOBBY,
	GAME,
	LOADING
};

enum class PROTOCOL : unsigned int
{
	LOGIN = 8,
	DB_LOGIN,
	LOGOUT,
	DB_LOGOUT,
	SIGNUP,
	DB_SIGNUP,
	WITHDRAWL,
	DB_WITHDRAWL,

	CERTIFICATION,
	DB_CERTIFICATION,

	ROOM_IN,
	DB_ROOM_IN,

	ROOM_OUT,
	DB_ROOM_OUT,

	ROOM_ENABLE,
	DB_ROOM_ENABLE,

	ROOM_LIST,

	DB_ROOM_DEACTIVE,
	DB_ROOM_STATE_WAIT,
	DB_ROOM_STATE_GAME,
	
	GAME_CLOSE_INFO,
	DB_GAME_CLOSE_INFO,

	STRESS_TEST_ECHO,
	STRESS_TEST_ECHO_RETURN,

	STRESS_TEST_BROADCAST,
	STRESS_TEST_BROADCAST_RETURN,

	HEARTBEAT,

	ROOM_IN_EVENT,
	ROOM_OUT_EVENT,
	ROOM_WAIT_EVENT,
	ROOM_RELEASE_EVENT,
	ROOM_GAME_EVENT,
	ROOM_LOAD_EVENT,
	ROOM_GAMEEND_EVENT,

	POSITION_EVENT,
	STATE_EVENT,
	TIME_SCALE_EVENT,
	MOVE,	
	GET_DEMEGED_EVENT,
	ON_ATTACK_EVENT,
	MOVE_STATE_EVENT,
	PROJECTILE_ACTIVE_EVENT,
	GAME_INFO_EVENT,
	TOXIC_ACTIVE_EVENT,
	FEVER_EVENT,

	READY,
	GROUPSEND,
	LOAD,
	EXPORT_ME_GROUPSEND,
	END
};