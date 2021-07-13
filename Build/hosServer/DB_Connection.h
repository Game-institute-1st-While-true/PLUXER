#pragma once
#include "hosEngine/NetworkToken.h"

class hos::com::GroupCallBackcomponent;
class DB_Connection : public hos::com::NetworkToken
{
	static constexpr int DEFAULT_TIME = 360;
public:
	static DB_Connection* DBC;

private:
	RpcMap FuncMap;

public:
	DB_Connection();
	virtual ~DB_Connection();

public:
	void MappingProtocol();

	template<class C>
	void Mapping(C* this_class, int index, CLASS_MEM_PACKAGE_FUNC<C> func)
	{
		FuncMap.Mapping(func, index, this_class);
	}	

public:
	// FROM CLIENT TO DBA
	void Req_Certification(int sessionid, const char* id);
	void Req_ActiveRoom(int sessionid, const char* id, int room_no, const char* room_name);
	void Req_UserInRoom(int sessionid, const char* id, const char* room_name);

	// FROM SERVER TO DBA
	void Req_DeActiveRoom(int room_no);
	void Req_StateUpdateWaitRoom(int room_no);
	void Req_StateUpdateGameRoom(int room_no);
	void Req_GameInfoUpdate(int sessionid, int score, const wchar_t* playerid, double playtime);

private:
	// FROM DBA TO SERVER
	void DB_Ans_Certification(PACKAGE& package);
	void DB_Ans_ActiveRoom(PACKAGE& package);
	void DB_Ans_UserInRoom(PACKAGE& package);
	void DB_Ans_GroupCleanUp(PACKAGE& package);
	void DB_Ans_GameInfo(PACKAGE& package);

	void DB_OnConnect(PACKAGE& package);
	void DB_OnDisconnect(PACKAGE& package);

protected:
	virtual void OnConnection();
	virtual void OnDisconnect();
	virtual void OnConnectFail();
	virtual void OnJob(PACKAGE& package);
	void Disconnect();
};

