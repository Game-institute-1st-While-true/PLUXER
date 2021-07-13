#pragma once
#include <shared_mutex>

#include <atomic>
#include "NavDefine.h"
#include "hosUtility/hosMath.h"

namespace hos::com 
{ 
	class NavAgent;
}

namespace hos::nv
{
	class NavQuery
	{
		friend class Navigation;
		friend class com::NavAgent;
	public:
		std::mutex PositionLock;							// pos update lock

	private:
		std::atomic<bool> Del;

		int MeshId;											// 자신이 속해있는 NavMesh의 번호
		int NavId;											// Navgation 에 들어가 있는 자신의 Id	

		dtNavMeshQuery* Query;								// NavQuery
		dtQueryFilter Filter;								// Option Filter
		dtStatus Status;									// Return Value

		dtPolyRef StartRef;									// Start Polygon Id
		dtPolyRef EndRef;									// End Polygon Id
		dtPolyRef Polys[MAX_POLYS];							// All Polygon Id
		int PolyCount;										// All Polygon Count

		dtPolyRef ExiPolys[MAX_POLYS];						// ? 실행된 Polygon Id들 사용하지 않음.
		int ExiPolyCount;									// Exisit Polygon Count

		float PolyPickExt[3];

		float StartPos[3];									// 시작 위치
		float EndPos[3];									// 끝 위치

		float StraightPath[MAX_POLYS * 3];					// 지나온 직선 경로
		unsigned char StraightPathFlags[MAX_POLYS];
		dtPolyRef StraightPathPolys[MAX_POLYS];
		int StraightPathCount;

		int TryCount;										// Agent가 Query를 시도할 횟수
		int MaxPathCount;									// Agent가 시도할 Max Count;
		int CurrentPathCount;								// Agent가 현재 시도한 Count;
		Vector3 Dir;

	public:
		NavQuery();
		~NavQuery();

	public:
		// get set
		void SetMaxPathCount(int count) { MaxPathCount = count; }
		int GetMaxPathCount() { return MaxPathCount; }

		int GetPathCount() { return CurrentPathCount; }

		void SetUpdateCount(int count) { TryCount = count; }
		int GetUpdateCount() { return TryCount; }

	public:
		void SetDefaultFilter();
		void SetFilter(dtQueryFilter filter) { Filter = filter; }

		bool InitQuery(dtNavMesh* mesh);

		void SetStart(Vector3 vec);
		void SetEnd(Vector3 vec);
		
		Vector3 GetStart();
		Vector3 GetEnd();
		
		void ClearQuery();

		void NearStraightPathPolys();	// 직선 데이터를 얻는다.
	private:
		void ProcessPartial();
		void Process();
	};
}
