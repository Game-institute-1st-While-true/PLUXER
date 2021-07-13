#pragma once
#include <thread>
#include <mutex>
#include <atomic>

#include <deque>
#include <map>

#include "NavDefine.h"
#include "NavigationMaker.h"
#include "NavQuery.h"


// [2021/03/18 김효곤]
// 메쉬 데이터를 이용하여 생성된 NavMesh 를 보유하는 클래스. 
// Agent의 데이터를 이용하여 길찾기 로직을 수행하고 반환하는 클래스.

namespace hos::nv
{
	constexpr int MAX_AGENT = 300;

	class Navigation
	{
		friend class NavigationMaker;
	
	private:
		int MeshSeed;

		BuildContext* Context;
		NavigationMaker* Maker;

		std::map<int, dtNavMesh*> NavMeshPool;	// int : MapDataIdx
		std::vector<NavQuery*> QueryPool;
		std::deque<int> QueryIdPool;

		std::atomic<bool> IsRun;
		std::atomic<bool> IsConfig;
		std::mutex AgentLock;

		std::thread	NavWorker;
	
	public:
		Navigation();
		virtual ~Navigation();

	public:
		NavigationMaker* GetMaker() { return Maker; }		
		void CreateMaker();
		void Load();
		void Run();

	public:
		bool AddQuery(NavQuery* query);
		void DelQuery(NavQuery* query);
		
		int AddMesh(dtNavMesh* mesh);
		int AddMesh(dtNavMesh* mesh, int seed);

	private:
		void MakeAgentPool();
		void StartThread();
		void DestroyThread();
		void WorkFunc();

		void QueryProcss();
	};
}
