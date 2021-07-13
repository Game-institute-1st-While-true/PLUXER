#ifndef _ANIMATIONBUNDLE_H
#define _ANIMATIONBUNDLE_H

// [2021/03/15 신호식]
// 특정 모델의 애니메이션 클립(동작) 모음.

// LOG
// [2021/03/15 신호식] Graphics Mk2에서 이동.
// [2021/04/05] 노종원 : Serialize/ Deserialize, FILE_EXTENSION, GetModelName() 추가

#pragma once

#include "AnimationClip.h"

namespace hos
{

	class AnimationBundle
	{
	public:
		static constexpr string_literal FILE_EXTENSION = L".bundle";
	public:
		struct BoneTransform
		{
			DirectX::SimpleMath::Vector3 vPos;
			DirectX::SimpleMath::Vector3 vRot;
			DirectX::SimpleMath::Vector3 vScale;
			DirectX::SimpleMath::Matrix mTM;
		};

		std::vector<BoneTransform> BoneTransformList;

	private:
		// 모델 이름
		hos::string ModelName;

		// 본 정보
		std::vector<hos::string> BoneNameList;		// 뼈 이름들 벡터
		std::vector<I32> BoneHierarchy;			// 뼈 계층구조 인덱스
		std::vector<DirectX::SimpleMath::Matrix> BoneOffsetList;		// 각 뼈의 애니메이션 행렬


		// 애니메이션 정보
		std::vector<hos::string> AnimationName;
		std::unordered_map<hos::string, AnimationClip*> AnimationClipList;

		// 생성자 소멸자
	public:
		AnimationBundle() = delete;
		AnimationBundle(hos::string modelName);
		~AnimationBundle();

		// 외부 공개 함수
	public:
		// 겟터
		I32 GetBoneListSize() const { return (I32)BoneHierarchy.size(); };
		std::vector<DirectX::SimpleMath::Matrix>& GetBoneOffSetList() { return BoneOffsetList; };
		std::vector<BoneTransform>& GetBoneTransformList() { return BoneTransformList; };
		std::vector<hos::string>& GetBoneList() { return BoneNameList; };
		AnimationClip* GetClip(hos::string_view clipName) const;
		std::vector<hos::string>& GetAnimationClipName() { return AnimationName; };

		I32 GetBoneIndexByName(hos::string boneName);

		void GetBoneOffSetTM(hos::Matrix dest[]);
		hos::string GetBoneName(hos::U32 index) { return BoneNameList[index]; };

		string GetName() const { return ModelName; }
		// 셋터
		void AddBoneHierarchy(I32 index) { BoneHierarchy.push_back(index); };
		void SetBoneName(hos::string boneName) { BoneNameList.push_back(boneName); };
		void ResizeBoneOffSetList(I32 count) { BoneOffsetList.resize(count); BoneTransformList.resize(count); };
		void AddClip(hos::string clipName, AnimationClip* clip);

		const std::vector<U8> Serialize();
		bool Deserialize(mbstring_view data);
	};

}

#endif //!_ANIMATIONBUNDLE_H