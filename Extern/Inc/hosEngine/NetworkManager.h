#pragma once
#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib,"GzzNet_x64D")
#else
#pragma comment(lib,"GzzNet_x86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib,"GzzNet_x64R")
#else
#pragma comment(lib,"GzzNet_x86R")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)

#include "NetDefine.h"
#include "NetworkToken.h"

#include "../hosUtility/hosUtility.h"


namespace hos::com
{
	template<class C>
	using CLASS_MEM_PACKAGE_FUNC = void(C::*)(PACKAGE&);

	class NetworkCallBack;								// 네트워크시스템적으로 처리할 콜백 함수 클래스
	class NetworkManager
	{
		using NetworkFunc = std::function<void(NetworkManager&, PACKAGE&)>;

	private:
		static IOCP* Net;
		std::thread	WokerThread;
		std::vector<NetworkFunc> NetworkFuncTable;
		std::vector<NetworkToken*> Tokens;
		std::deque<int> ExtraIdQueue;

		NetworkCallBack* CallBack;

		int	MaxSessionCount;
		int ExtraSessionCount;							// 확장 세션

		int	CurrentSessionCount;
		unsigned short Port;
		bool IsWorking;
		int RunCount;

	public:
		NetworkManager();
		~NetworkManager();

	public:
		void Initialize(int max_thread, int max_session, int extra_session, IPaper* paper, int run_cnt);
		void Initialize(int max_thread, int max_session, int extra_session, NetworkCallBack* callback, IPaper* paper, int run_cnt);

		void Finalize();

		static void CreateNet(int thread_count, int max_session);
		static void	Connect(std::wstring ip, unsigned short port);
		static void	Connect(int sessionid, std::wstring ip, unsigned short port);
		static int	UnExConnect(std::wstring ip, unsigned short port);
		static bool	UnExConnect(int sessionid, std::wstring ip, unsigned short port);


		static void	Disconnect(int session_id);
		static void SendPacket(int dst, PACKET& packet);
		static void	FourceEnque(PACKAGE& package);

		// getter
		int	GetMaxSession() { return MaxSessionCount; }
		unsigned short GetPort() { return Port; }
		NetworkToken* GetToken(const int index)
		{
			int size = (int)Tokens.size();
			if (0 > index || index >= size)
			{
				return nullptr;
			}
			return Tokens[index];
		}

	public:
		// listen start & accept start
		void Listen(unsigned short port);
		void Start();
		void CreateWorkerThread();
		virtual void Run();
		bool ReadPackage();

	public:
		int AddExtraToken(NetworkToken* token);
		void DelExtraToken(NetworkToken* token);

	private:
		void WorkProc();
		void DestroyThread();
		void CreateSession();
		void CreateFuncTable();

	private:
		// network system call back
		void NotImplemented(PACKAGE& package);
		virtual void OnAccept(PACKAGE& package);					// accept sucess
		virtual void OnConnect(PACKAGE& package);					// connect sucess
		virtual void OnConnectFail(PACKAGE& package);				// connect fail
		virtual void OnDisconnect_Accept(PACKAGE& package);			// disconnect session
		virtual void OnDisconnect_Connect(PACKAGE& package);		// disconnect session
		virtual void OnJob(PACKAGE& package);						// other packet
	};

	class NetworkCallBack
	{
	public:
		NetworkCallBack() = default;
		virtual ~NetworkCallBack() = default;

	public:
		virtual void OnAccept(PACKAGE& package) {};					// accept sucess
		virtual void OnConnect(PACKAGE& package) {};				// connect sucess
		virtual void OnConnectFail(PACKAGE& package) {};			// connect fail
		virtual void OnDisconnect_Accept(PACKAGE& package) {};		// disconnect session
		virtual void OnDisconnect_Connect(PACKAGE& package) {};		// disconnect session
		virtual void OnJob(PACKAGE& package) {};					// other packet
	};
};