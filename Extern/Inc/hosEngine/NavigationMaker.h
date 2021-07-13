#pragma once
#include <map>
#include "NavDefine.h"
#include "BuildContext.h"
#include "Mesh.h"

// [2021/03/18 김효곤]
// 메쉬 데이터를 이용하여 Recast로 네부메시를 생성하는 클래스

namespace hos
{
	namespace com
	{
		class Scene;
	}
	namespace nv
	{
		class Navigation;

		struct NavMeshSetHeader
		{
			int magic;
			int version;
			int numTiles;
			dtNavMeshParams params;
		};

		struct NavMeshTileHeader
		{
			dtTileRef tileRef;
			int dataSize;
		};

		class NavigationMaker
		{
			enum PartitionType
			{
				WATERSHED,
				MONOTONE,
				LAYERS,
			};

		public:
			NavigationMaker() = delete;
			NavigationMaker(Navigation* nav);
			virtual ~NavigationMaker();

			std::vector<Mesh*>	InputMeshs;
		private:
			// Input Data를 합쳐놓은 데이터 (맵 데이터)
			float* Vtxs;
			unsigned int VtxCount;
			int VtxNow;

			int* FaceBuffers;
			int FaceCount;
			int FaceNow;


			// 변환된 NavMesh 데이터
		private:
			Navigation* Nav;
			unsigned char* NavFaceBuffers;

			float BoundBoxMin[3];
			float BoundBoxMax[3];

			// NavMesh Bake 할 옵션
		private:
			float CellSize;																		// 복셀 필드를 구성하는 Cell의 넓이 
			float CellHeight;																	// 복셀 필드를 구성하는 Cell의 높이 

			float AgentHeight;																	// 가상의 객체의 올라갈 수 있는 경사도
			float AgentRadius;																	// 가상의 객체의 높이
			float AgentMaxClimb;																// 가상의 객체의 올라갈 수 있는 높이.
			float AgentMaxSlope;																// 가상 객체의 둘레

			float RegionMinSize;																// 네비게이션 메쉬의 경계를 나타내는 다각형 가장자리의 최대 길이를 조절할 수 있다
			float RegionMergeSize;																// 네비게이션 메쉬의 가장자리를 최대한 지형 메쉬의 윤곽에 따라 만들어 준다.
			float EdgeMaxLen;																	// 연결되어 있지 않은 부분에 대해서 독립적인 네비게이션 메쉬를 생성해준다.
			float EdgeMaxError;																	// 작은 영역의 수를 주위의 큰 지역에 병합시켜준다.
			float VertsPerPoly;																	// 다각형을 이루는 정점들의 수 
			float DetailSampleDist;																// 지형메쉬의 표면에 네비게이션 메쉬를 일치시킬 때 사용하는 샘플링 거리를 설정한다.
			float DetailSampleMaxError;															// Sample Distance에서 발생할 수 있는 부족한 부분을 좀 더 디테일하게 만들어주는 역할을 한다.

			bool FilterLowHangingObstacles;														// 계단과 같은 낮게 누워있는 물체를 갈 수 있는지 여부
			bool FilterLedgeSpans;																// 공중에 매달려 있는 영역을 체크할 것 인지 여부.
			bool FilterWalkableLowHeightSpans;

			int Type;

		private:
			rcConfig Config;
			rcHeightfield* Solid;																// 막힌 공간을 나타내는 동적 높이필드
			rcCompactHeightfield* CompactHeightField;											// 방해받지않는 공간을 나타내는 컴팩트한 정적 높이필드
			rcContourSet* ContourSet;															// 관련된 윤곽 그룹을 나타냅니다.
																								// 집합 내의 모든 윤곽선은 집합의 최소 경계와 셀 크기를 공유합니다.

			rcPolyMesh* PolyMesh;																// 탐색 메시를 작성하는 데 사용하기에 적합한 다각형 메시
			rcPolyMeshDetail* PolyMeshDetail;													// 연관된 다각형 메쉬 개체의 다각형과 관련된 자세한 높이 데이터를 나타내는 삼각형 메쉬를 포함합니다.
																								// 세부 메시는 연결된 다각형 메시의 각 다각형에 대한 추가 높이 세부 정보를 제공하는 삼각형 하위 메시로 구성됩니다.

			BuildContext* Context;
			dtNavMesh* NavMesh;

		public:
			bool AddMesh(Mesh* mesh);

			void ClearMesh();
			bool MakeInputData();

			void Bake();
			bool Bake(com::Scene* scene);

			bool Save(mbstring name, const dtNavMesh* mesh);									// 자신이 저장 할 파일 이름과 mesh 데이터
			bool Load(mbstring name);															// 가져올데이터

			dtNavMesh* GetMesh() { return NavMesh; }

		public:
			// get, set
			void SetCellSize(float size) { CellSize = size; }
			float GetCellSize() { return CellSize; }
			void SetCellHeight(float height) { CellHeight = height; }
			float GetCellHeight() { return CellHeight; }

			void SetAgentHeight(float height) { AgentHeight = height; }
			float GetAgentHeight() { return AgentHeight; }
			void SetAgentRadius(float radius) { AgentRadius = radius; }
			float GetAgentRadius() { return AgentRadius; }
			void SetAgentMaxClimb(float climb) { AgentMaxClimb = climb; }
			float GetAgentMaxClimb() { return AgentMaxClimb; }
			void SetAgentMaxSlope(float slope) { AgentMaxSlope = slope; }
			float GetAgentMaxSlope() { return AgentMaxSlope; }

			void SetRegionMinSize(float size) { RegionMinSize = size; }
			float GetRegionMinSize() { return RegionMinSize; }
			void SetRegionMergeSize(float size) { RegionMergeSize = size; }
			float GetRegionMergeSize() { return RegionMergeSize; }

			void SetEdgeMaxLen(float len) { EdgeMaxLen = len; }
			float GetEdgeMaxLen() { return EdgeMaxLen; }
			void SetEdgeMaxError(float err) { EdgeMaxError = err; }
			float GetEdgeMaxError() { return EdgeMaxError; }

			void SetVertsPerPoly(float perpoly) { VertsPerPoly = perpoly; }
			float GetVertsPerPoly() { return VertsPerPoly; }
			void SetDetailSampleDist(float dist) { DetailSampleDist = dist; }
			float GetDetailSampleDist() { return DetailSampleDist; }
			void SetDetailSampleMaxError(float err) { DetailSampleMaxError = err; }
			float GetDetailSampleMaxError() { return DetailSampleMaxError; }

		private:
			void AddVertex(DirectX::SimpleMath::Vector3 vertex, int& cap);
			void AddTriangle(int triA, int triB, int triC, int offset, int& cap);

		private:
			void SettingConfig();																	// Step 1. Initialize build config. 빌드 컨피그를 이니셜라이즈 합니다.
			bool MakeTriangle();																	// Step 2. Rasterize input polygon soup. 폴리곤 수프를 레스터라이즈 합니다.  ** 다각형 수프는 구조화 작업을 적용하기 전에 조직화되지 않은 다각형을 말함
			void FilteringWalkableSurfaces();														// Step 3. 걸을 수 있는 영역을 필터링 합니다.
			bool ParttionWalkableSurfacesSimple();													// Step 4. 걸을 수있는 표면을 단순한 영역으로 분할합니다.
			bool TraceAndSimplifyRgionContours();													// Step 5. 영역 윤곽을 추적하고 단순화합니다.
			bool CreatePolyMesh();																	// Step 6. 윤곽선에서 다각형 메쉬를 만듭니다.
			bool CreatePolyMeshDetail();															// Step 7. 각 다각형의 대략적인 높이에 액세스 할 수있는 세부 메시를 만듭니다.
		};
	}
}

