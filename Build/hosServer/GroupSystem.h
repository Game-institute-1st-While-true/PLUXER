#pragma once

#include "Defines.h"
#include "Group.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <cassert>
#include <list>


#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib,"hosEngine_x64D")
#else
#pragma comment(lib,"hosEngine_x86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib,"hosEngine_x64R")
#else
#pragma comment(lib,"hosEngine_x86R")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)

class Player;
namespace hos::com
{
	class GroupSystemControlCallBack;				// 어플리케이션이 사용할 그룹시스템 콜백 함수
	class GroupNetworkCallBack;						// 그룹시스템이 사용할 네트워크 콜백 함수
	class GroupSystem
	{
		friend class NetworkManager;
		friend class GroupNetworkCallBack;

	private:
		std::thread GroupSystemThread;

		gzz::Monitoring Monitor;
		double MonitorCheckTime;

		GroupSystemControlCallBack* CallBack;
		GroupNetworkCallBack* NetCallBack;

		std::list<int> PlayGroups;
		std::list<int> ReleaseGroups;

		std::vector<Group*> Groups;
		std::deque<int> GroupIdQueue;

		std::atomic<bool> OnBtn;

	public:
		// 사용할 전역적인 객체
		inline static hos::ut::Logger* Debug = hos::ut::Logger::GetIns();

	public:
		GroupSystem() = delete;
		GroupSystem(GroupSystemControlCallBack* call_back);
		~GroupSystem();

	public:
		void Load();									// 데이터 매니저에서 데이터 로드.

		void Init(int max_group);						// 데이터 매니저에서 로드한 데이터를 Group, Scene에 적용함.

		void Start(unsigned short listen_port);			// 스레드 시작
		void Stop();									// 스레드 종료

		void Loop();									// Room Loop

		void EnableGroupMovePlayPool();					// Ready Room -> Play Room
		void GroupCleanUp();							// Release Room -> Disable Room

		void Monitoring();

		Group* GetGroup(const int index)
		{
			if (0 > index || index >= Groups.size())
			{
				return nullptr;
			}
			else
			{
				return Groups[index];
			}
		}

		bool AddGroup(Group* group);
		void DelGroup(int index);

	public:
		friend class Player;
		void Ans_ActiveRoom(PACKAGE& packet);
		void Ans_UserInRoom(PACKAGE& packet);

		void DB_Ans_ActiveRoom(PACKAGE& package);
		void DB_Ans_UserInRoom(PACKAGE& package);
	};

	class GroupNetworkCallBack : public NetworkCallBack
	{
	private:
		GroupSystem* System;
	public:
		GroupNetworkCallBack() = delete;
		GroupNetworkCallBack(GroupSystem* system);
		virtual ~GroupNetworkCallBack();

		virtual void OnAccept(PACKAGE& package) {};				// accept sucess
		virtual void OnConnect(PACKAGE& package) {};			// connect sucess
		virtual void OnConnectFail(PACKAGE& package) {};		// connect fail
		virtual void OnDisconnect_Accept(PACKAGE& package);		// disconnect session
		virtual void OnDisconnect_Connect(PACKAGE& package);	// disconnect session
		virtual void OnJob(PACKAGE& package) {};				// other packet
	};

	class GroupSystemControlCallBack
	{
	public:
		GroupSystemControlCallBack() = default;
		virtual ~GroupSystemControlCallBack() = default;

	public:
		virtual void OnStart() = 0;
		virtual void OnExit() = 0;
		virtual bool OnExitCheck() = 0;
	};
}

extern hos::com::GroupSystem* System;

