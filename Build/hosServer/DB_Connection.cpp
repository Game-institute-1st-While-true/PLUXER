#include "DB_Connection.h"
#include "hosEngine/Object.h"

#include "GroupSystem.h"
#include "hosUtility/hosUtility.h"
#include "hosEngine/NetworkManager.h"
#include "PacketDefine.h"
#include "Player.h"

using namespace hos;
using namespace ut;

DB_Connection* DB_Connection::DBC = nullptr;

DB_Connection::DB_Connection()
	:NetworkToken(-1)
{
}

DB_Connection::~DB_Connection()
{
}

void DB_Connection::MappingProtocol()
{
	Mapping(this, static_cast<int>(PROTOCOL::DB_CERTIFICATION), &DB_Connection::DB_Ans_Certification);
	Mapping(this, static_cast<int>(PROTOCOL::DB_ROOM_ENABLE), &DB_Connection::DB_Ans_ActiveRoom);
	Mapping(this, static_cast<int>(PROTOCOL::DB_ROOM_IN), &DB_Connection::DB_Ans_UserInRoom);
	Mapping(this, static_cast<int>(SYSTEM_PROTOCOL::CONNECT), &DB_Connection::DB_OnConnect);
	Mapping(this, static_cast<int>(SYSTEM_PROTOCOL::DISCONNECT_CONNECT), &DB_Connection::DB_OnDisconnect);
	Mapping(this, static_cast<int>(PROTOCOL::DB_GAME_CLOSE_INFO), &DB_Connection::DB_Ans_GameInfo);
}

void DB_Connection::Req_Certification(int sessionid, const char* id)
{
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = 0;
	auto id_ = builder.CreateString(id);
	auto pw_ = builder.CreateString("");

	auto protocol = CreateIdPw(builder, no_, id_, pw_, code_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_CERTIFICATION);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Req_ActiveRoom(int sessionid, const char* id, int room_no, const char* room_name)
{
	flatbuffers::FlatBufferBuilder builder;

	int sid_ = sessionid;
	int roomno_ = room_no;
	auto id_ = builder.CreateString(id);
	auto roomname_ = builder.CreateString(room_name);

	auto protocol = CreateRoomSetting(builder, sid_, roomno_, id_, roomname_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_ROOM_ENABLE);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Req_UserInRoom(int sessionid, const char* id, const char* room_name)
{
	flatbuffers::FlatBufferBuilder builder;

	int sid_ = sessionid;
	int roomno_ = -1;
	auto id_ = builder.CreateString(id);
	auto roomname_ = builder.CreateString(room_name);

	auto protocol = CreateRoomSetting(builder, sid_, roomno_, id_, roomname_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_ROOM_IN);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Req_DeActiveRoom(int room_no)
{
	flatbuffers::FlatBufferBuilder builder;

	int sid_ = -1;
	int roomno_ = room_no;
	auto id_ = builder.CreateString("");
	auto roomname_ = builder.CreateString("");

	auto protocol = CreateRoomSetting(builder, sid_, roomno_, id_, roomname_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_ROOM_DEACTIVE);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Req_StateUpdateWaitRoom(int room_no)
{
	flatbuffers::FlatBufferBuilder builder;

	int sid_ = -1;
	int roomno_ = room_no;
	auto id_ = builder.CreateString("");
	auto roomname_ = builder.CreateString("");

	auto protocol = CreateRoomSetting(builder, sid_, roomno_, id_, roomname_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_ROOM_STATE_WAIT);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Req_StateUpdateGameRoom(int room_no)
{
	flatbuffers::FlatBufferBuilder builder;

	int sid_ = -1;
	int roomno_ = room_no;
	auto id_ = builder.CreateString("");
	auto roomname_ = builder.CreateString("");

	auto protocol = CreateRoomSetting(builder, sid_, roomno_, id_, roomname_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_ROOM_STATE_GAME);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Req_GameInfoUpdate(int sessionid, int score, const wchar_t* playerid, double playtime)
{
	flatbuffers::FlatBufferBuilder builder;

	auto userid = builder.CreateString(hos::ut::UTF16ToAnsi(playerid));
	auto protocol = CreateGameInfo(builder, sessionid, userid, score, 0);

	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_GAME_CLOSE_INFO);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::DB_Ans_Certification(PACKAGE& package)
{
	auto cert = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);
	int sessionid = cert->session_id();	
	auto token = Network->GetToken(sessionid);
	if (token)
	{
		token->OnJob(package);
	}
}

void DB_Connection::DB_Ans_ActiveRoom(PACKAGE& package)
{
	auto active = GetRoomSetting(package.packet.memory + PACKET_HEADER_SIZE);

	int roomno_ = active->roomid();
	int sessionid = active->session_id();
	auto token = Network->GetToken(sessionid);
	if (token)
	{
		token->OnJob(package);
	}
}

void DB_Connection::DB_Ans_UserInRoom(PACKAGE& package)
{
	auto active = GetRoomSetting(package.packet.memory + PACKET_HEADER_SIZE);

	int roomno_ = active->roomid();
	int sessionid = active->session_id();
	auto token = Network->GetToken(sessionid);
	if (token)
	{
		token->OnJob(package);
	}
}

void DB_Connection::DB_Ans_GroupCleanUp(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"net", L"Ado Agent 와 연결되었습니다.");
}

void DB_Connection::DB_Ans_GameInfo(PACKAGE& package)
{
	auto gameinfo = GetGameInfo(package.packet.memory + PACKET_HEADER_SIZE);
	int sessionid = gameinfo->session_id();
	auto token = Network->GetToken(sessionid);
	if (token)
	{
		token->OnJob(package);
	}
}

void DB_Connection::DB_OnConnect(PACKAGE& package)
{
}

void DB_Connection::DB_OnDisconnect(PACKAGE& package)
{
}

void DB_Connection::OnConnection()
{
	Logger::GetIns()->Write(L"net", TEXT(__FILE__), std::to_wstring(__LINE__), L"id : ", SessionId, "  connect");
	//System->GetGroup(2)->SetState(hos::com::GROUP_STATE::ENABLE);
	Connection = true;

}

void DB_Connection::OnDisconnect()
{
	Logger::GetIns()->Write(L"net", TEXT(__FILE__), std::to_wstring(__LINE__), L"DB서버와 연결이 종료되었습니다. session id : ", SessionId);
	Connection = false;
}

void DB_Connection::OnConnectFail()
{
	Logger::GetIns()->Write(L"net", TEXT(__FILE__), std::to_wstring(__LINE__), L"DB서버에 접속을 실패했습니다. : ", SessionId);
	//System->GetGroup(1)->SetState(hos::com::GROUP_STATE::ENABLE);
	Connection = false;
}

void DB_Connection::OnJob(PACKAGE& package)
{
	FuncMap[package.packet.head.protocol](package);
}

void DB_Connection::Disconnect()
{
	hos::com::NetworkManager::Disconnect(SessionId);
}
