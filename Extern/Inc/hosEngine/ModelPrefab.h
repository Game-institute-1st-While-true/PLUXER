#ifndef _MODEL_H
#define _MODEL_H

// [2021/03/15 신호식]
// 모델 프리팹 생성을 위한 임시 모델 데이터
// Bone 구조와 MeshList를 가지고 있습니다.

// LOG
// [2021/03/15 신호식] Graphics Engine Mk2에서 엔진 클라이언트 내부로 이동.
// [2021/04/05] 노종원 : 소멸자 수정

#pragma once

#include "hosUtility\hosUtility.h"
#include "HContainer.h"
#include "Bone.h"
#include "Mesh.h"

#include <vector>

namespace hos
{

	class ModelPrefab
	{
		friend class FbxLoader;

	private:
		hos::string ModelName;

		Node* RootNode;
		bool bAni;
		bool bSkin;

	public:
		std::vector<Node*> MeshList;
		std::vector<Node*> BoneList;

	public:
		ModelPrefab(hos::string modelName);
		~ModelPrefab();

	public:
		void AddMesh(Node* meshData);
		void AddBone(Node* boneData);
		Node* GetRootNode()const { return RootNode; }



		bool IsBoolSkin() { return bSkin; };
	};

}

#endif //!_MODEL_H