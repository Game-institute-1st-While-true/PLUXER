#ifndef _MESHFILTER_H
#define _MESHFILTER_H

// [2021/03/18 신호식]
// 메쉬필터 컴포넌트

// LOG
// [2021/03/18] 신호식 : 생성.
// [2021/03/27] 노종원 : 복사 생성자 추가, 가상 복사 생성 함수 추가
// [2021/04/03] 노종원 : Serialize/Deserialize 추가

#include "Component.h"
#include "Mesh.h"

namespace hos::com
{

	class MeshFilter : public Component
	{
	private:
		Mesh* MeshData;

	public:
		MeshFilter();
		~MeshFilter();

	protected:
		MeshFilter(const MeshFilter& dest);

	public:
		void SetMeshData(Mesh* data);
		void RemoveMeshData();

		hos::Mesh* GetMeshData() const;
		hos::string GetMeshName() const { return MeshData->GetMeshName(); };
		hos::Matrix GetGeoTM() const { return MeshData->GetGeoTM(); };
		hos::Matrix GetNodeTM() const { return MeshData->GetNodeTM(); };

		U32 GetDefaultBoneIndex() const { return MeshData->GetDefaultBoneIndex(); };
		
		hos::Vector3* GetBBoxAddress();
		hos::Vector3 GetBBoxSize() { return MeshData->GetBBoxSize(); };

		void SetActive(bool) = delete;
		MeshFilter* Clone() const override;
		void Bind(GameObject* gameObject) override;
		void UnBind(int index) override;
		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view data) override;

		void Reset() override {};
		void OnEnable() override {};
		void OnDisable() override {};
	};

}

#endif //!_MESHFILTER_H