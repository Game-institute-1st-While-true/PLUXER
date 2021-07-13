#ifndef _KEYFRAME_H
#define _KEYFRAME_H

// [2021/03/15 신호식]
// 특정 시점의 애니메이션 키프레임.
// 해당 키프레임과 다음 키프레임 사이를 보간해서 애니메이션 행렬을 구합니다.

// LOG
// [2021/03/15 신호식] Graphics Mk2에서 이동.

#pragma once

#include "hosUtility\hosUtility.h"
#include "hosUtility\hosMath.h"

namespace hos
{

	class Keyframe
	{
	private:
		F32 TimePos;	// 시간

		// Transform
		Vector3 vPos;
		Vector3 vScale;
		Quaternion vRotQuat;

	public:
		Keyframe();
		~Keyframe();

	public:
		void SetTimePos(F32 timePos) { TimePos = timePos; };
		void SetTransform(Vector3 vPos, Vector3 vScale, Vector4 vRotQuat);
		void SetTransform(DirectX::SimpleMath::Vector3 vPos, DirectX::SimpleMath::Vector3 vScale, DirectX::SimpleMath::Vector4 vRotQuat);

		F32 GetTimePos() const { return TimePos; };
		Vector3 GetPos() const { return vPos; };
		Vector3 GetScale() const { return vScale; };
		Quaternion GetRotQuat() const { return vRotQuat; };

	public:
		bool operator == (const Keyframe& key)
		{
			if (vPos.x != key.vPos.x ||
				vPos.y != key.vPos.y ||
				vPos.z != key.vPos.z)
			{
				return false;
			}

			if (vScale.x != key.vScale.x ||
				vScale.y != key.vScale.y ||
				vScale.z != key.vScale.z)
			{
				return false;
			}

			if (vRotQuat.x != key.vRotQuat.x ||
				vRotQuat.y != key.vRotQuat.y ||
				vRotQuat.z != key.vRotQuat.z ||
				vRotQuat.w != key.vRotQuat.w)
			{
				return false;
			}

			return true;
		}

	};

}

#endif //!_KEYFRAME_H