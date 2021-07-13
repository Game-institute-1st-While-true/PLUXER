#ifndef _BONE_H
#define _BONE_H

// [2021/03/15 신호식]
// 모델 프리팹 생성을 위한 임시 Bone 데이터
// 간단한 계층구조만 가지고 있습니다.

// LOG
// [2021/03/15 신호식] 모델 프리팹 생성을 위해 계층구조 추가.

#pragma once

#include "hosUtility\hosUtility.h"
#include "hosUtility\hosMath.h"
#include "HContainer.h"

namespace hos
{
	class Mesh;

	class Node
	{
		friend class FbxLoader;

	public:
		enum class NODE_TYPE
		{
			NONE,
			BONE,
			MESH,
			ROOT
		};

	private:
		hos::string NodeName;
		NODE_TYPE NodeType;
		HContainer<Node> Hierarchy;

		// 메쉬 정보(메쉬일 경우에만)
		Mesh* MeshData;

		// Transform
		Vector3 vPos;
		Vector3 vRot;
		Vector3 vScale;

		// Ani
		bool bSkin;
		bool bAnimation;

	public:
		Node();
		~Node();

	public:
		void SetParent(Node* parentsData);
		void SetChild(Node* childData);

		hos::string GetName()const;
		std::vector<Node*>& GetChilds();
		hos::string GetParentsName() { return Hierarchy.GetParent()->GetName(); };

		NODE_TYPE GetNodeType() { return NodeType; };
		Mesh* GetMesh() { return MeshData; };

		Vector3 GetPos() { return vPos; };
		Vector3 GetRot() { return vRot; };
		Vector3 GetScale() { return vScale; };

	};

}

#endif //!_BONE_H