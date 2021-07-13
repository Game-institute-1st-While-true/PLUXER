#ifndef _BONEANIMATION_H
#define _BONEANIMATION_H

// [2021/03/15 신호식]
// 애니메이션 클립(동작) 중 특정 뼈의 애니메이션 정보

// LOG
// [2021/03/15 신호식] Graphics Mk2에서 이동.
// [2021/04/05] 노종원 : 사용하지않는 vZero 제거

#pragma once

#include "Keyframe.h"
#include <vector>

namespace hos
{
	class AnimationClip;
	class BoneAnimation
	{
		friend class AnimationClip;
	public:
		static constexpr const wchar_t* UI_ANI_FBX = L".uifbx";
		static constexpr const wchar_t* FILE_EXTENSION = L".uiani";

	private:
		std::vector<Keyframe> KeyframeList;

	public:
		BoneAnimation();
		~BoneAnimation();

	public:
		void AddKeyframe(Keyframe& keyframe) { KeyframeList.push_back(keyframe); };

		//F32 _GetStartTime() const { return KeyframeList.front().GetTimePos(); };
		//F32 _GetEndTime() const { return KeyframeList.back().GetTimePos(); };

		F32 _GetStartTime() const;
		F32 _GetEndTime() const;

		void _Interpolate(U32 index, F32 time, Matrix* matrix, Vector3* vPos = nullptr, Vector3* vScale = nullptr, Quaternion* vQuat = nullptr) const;

		const std::vector<U8> Serialize();
		bool Deserialize(mbstring_view data);
	};

}

#endif //!_BONEANIMATION_H