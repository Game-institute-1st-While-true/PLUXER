#ifndef _MESH_H
#define _MESH_H

// [2021/03/15 신호식]
// 메쉬 데이터.
// 정점 구조체는 Graphics Engine에서 참조한다.

// LOG
// [2021/03/15 신호식] Graphics Engine Mk2에서 엔진 클라이언트 내부로 이동.
// [2021/04/03] 노종원 : Serialize/Deserialize 추가 FILE_EXTENSION 추가
// [2021/04/05] 노종원 : 소멸자 수정

#include "hosUtility\hosUtility.h"
#include "hosUtility\hosMath.h"
#include "Ang3DEngine\VertexDataType.h"

namespace hos
{
	class Mesh
	{
		friend class FbxLoader;
	public:
		static constexpr string_literal FILE_EXTENSION = L".mesh";

	private:
		hos::string MeshName;

		hos::U32 VtxCount;
		hos::cg::VertexDataType::FULL_VTX* FullVtxList;

		hos::U32 IndexCount;
		hos::U32* IndexList;

		Matrix mNodeTM;
		Matrix mGeoTM;

		hos::string DefaultMaterialName;

		hos::Vector3 BoundingBox[9];	// 0번: Center
		hos::Vector3 BoundingBoxSize;

	public:
		Mesh();
		~Mesh();

	public:
		hos::string GetMeshName() const { return MeshName; };
		void SetMeshName(string_view name) { MeshName = name; }

		U32 GetVtxCount() const { return VtxCount; };
		void SetVtxCount(U32 count) { VtxCount = count; };
		hos::cg::VertexDataType::FULL_VTX* GetVtxDataAdress() const { return FullVtxList; };
		void SetVtxDataAdress(cg::VertexDataType::FULL_VTX* vtx) { FullVtxList = vtx; };

		U32 GetIndexCount() const { return IndexCount; };
		void SetIndexCount(U32 index) { IndexCount = index; };
		hos::U32* GetIndexDataAdress() const { return IndexList; };
		void SetIndexDataAdress(U32* index) { IndexList = index; };

		U32 GetDefaultBoneIndex() { return FullVtxList[0].index[0]; };
		hos::string GetMaterialName() { return DefaultMaterialName; };

		hos::Matrix GetGeoTM() { return mGeoTM; };
		hos::Matrix GetNodeTM() { return mNodeTM; };

		hos::Vector3* GetBBoxAddress() { return BoundingBox; };
		hos::Vector3 GetBBoxSize() { return BoundingBoxSize; };

		const std::vector<U8> Serialize();
		bool Deserialize(mbstring_view data);
	};
}

#endif // !_MESH_H