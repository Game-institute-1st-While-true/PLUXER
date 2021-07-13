#pragma once
#pragma comment(lib, "GzzNet")
#include <vector>	
#include <string>
#include <thread>

#include "Note.h"
#include "Singleton.h"
#include "GzzIOCP_EX.h"
#include "FormatUtils.h"
#include "Chunk.h"

#include "NetworkObject.h"

class User;
constexpr int START_SYS_PROTOCOL = static_cast<int>(SYSTEM_PROTOCOL::END);

class GzzNet
{
	using SystemRpc = std::function<void(GzzNet&, PACKAGE&)>;
protected:
	GzzNet();
	virtual ~GzzNet();

protected:
	GzzIOCP_EX*						Net;
	std::thread						WokerThread;

	std::vector<NetworkObject*>		UserPool;
	std::vector<SystemRpc>			SystemProcTable;

	std::wstring					Address;
	unsigned short					Port;

	int								MaxSessionCount;
	int								NetworkSendRate; // second

	bool							IsWorking;

public:
	virtual void					Initialize(int max_thread, int max_session, int net_send_rate, IPaper* paper);
	virtual void					Finalize();

	// setter
	void							SetAddress(std::wstring address) { Address = address; }
	void							SetPort(unsigned short port) { Port = port; }
	void							SetMaxSession(int max_session) { MaxSessionCount = max_session; }
	void							SetSendRate(int send_rate) { NetworkSendRate = send_rate; }

	// getter
	std::wstring					GetAddress() { return Address; }
	unsigned short					GetPort() { return Port; }
	int								GetMaxSession() { return MaxSessionCount; }
	int								GetSessionCount() { return UserPool.size(); }

	// network base
	virtual void					Connect(std::wstring ip, unsigned short port);
	virtual void					Disconnect(int session_id);
	virtual void					Listen(unsigned short port);

	virtual void					SendPacket(int dst, Chunk& stream);
	virtual void					SendPacket(int dst, PACKET& packet);
	void							Run();

private:
	void							SettingSystemProc();
	virtual void					CreateUserPool(const int max_session) abstract;

	virtual void					WorkProc();
	virtual bool					ReadPackage();
	virtual void					CreateWorkerThread();
	virtual void					DestroyThread();

	virtual void					Logic() abstract;								// server logic

	// network system call back
	virtual void					OnAccept(PACKAGE& package) abstract;			// accept sucess
	virtual void					OnConnect(PACKAGE& package) abstract;			// connect sucess
	virtual void					FailConnect(PACKAGE& package) abstract;			// connect fail
	virtual void					OnDisconnect(PACKAGE& package) abstract;		// disconnect session
};