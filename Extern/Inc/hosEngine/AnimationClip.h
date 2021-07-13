#ifndef _ANIMATIONCLIP_H
#define _ANIMATIONCLIP_H

// [2021/03/15 신호식]
// 특정 모델의 애니메이션 한 동작.

// LOG
// [2021/03/15 신호식] Graphics Mk2에서 이동.
// [2021/04/05] 노종원 : Serialize/ Deserialize 추가 임시 수학함수 제거

#pragma once

#include "BoneAnimation.h"


namespace hos
{
	class AnimationClip
	{
		friend class FbxLoader;

	private:
		hos::string ClipName;
		std::vector<BoneAnimation> BoneAnimationList;
		bool bSaved[150];

		hos::Vector3 BBox[9];	// 0 : Center

	public:
		AnimationClip(string_view name = L"Empty");
		~AnimationClip();

	public:
		// 세터
		void SetBoneAnimationSize(U32 size) { BoneAnimationList.resize(size); };
		void SetBoneAnimation(BoneAnimation& boneAni, I32 index);

		bool CheckBoneAnimation(I32 index);

		// 겟터
		hos::F32 GetClipStartTime() const;
		hos::F32 GetClipEndTime() const;

		bool GetStateBoneIndex(U32 index) { return bSaved[index]; };
		string GetName()const { return ClipName; }

		hos::Vector3* GetBBoxAddress() { return BBox; };

		// 보간
		U32 Interpolate(F32 time, Matrix boneOffsetList[], Vector3 vPos[] = nullptr, Vector3 vScale[] = nullptr, Quaternion vQuat[] = nullptr) const;

		const std::vector<U8> Serialize();
		bool Deserialize(mbstring_view data);
	};

}

#endif //!_ANIMATIONCLIP_H