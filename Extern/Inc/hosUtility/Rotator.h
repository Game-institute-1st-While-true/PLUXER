#ifndef _ROTATOR_H
#define _ROTATOR_H

// [2020/12/31 노종원]
// Rotator
// EulerAngle의 회전값을 저장하는 클래스
// -180 ~ 180의 회전만 저장한다.

// LOG

#include "BaseTypes.h"

namespace hos
{
	class Vector3;
	class Quaternion;
	class Rotator
	{
	public:
		static const Rotator Identity;
	public:
		union
		{
			struct {
				float x;
				float y;
				float z;
			};
			struct {
				float roll;
				float pitch;
				float yaw;
			};

		};
	public:
		Rotator(F32 pitch = 0.f, F32 yaw = 0.f, F32 roll = 0.f);
		Rotator(Quaternion q);
		~Rotator();

	private:
		void Normalize();
	public:
		Rotator& operator=(const Vector3& v);
		Rotator& operator=(const Quaternion& q);
		Rotator& operator-();

		operator Vector3();
		operator Quaternion();

	#ifdef PHYSX_MATH
		Rotator(physx::PxQuat q);
		Rotator& operator=(const physx::PxQuat& q);
		operator physx::PxQuat();
	#endif // PHYSX_MATH

	};

	// Binary operators
	Rotator operator+(const Rotator& lhs, const Rotator& rhs);
	Rotator operator-(const Rotator& lhs, const Rotator& rhs);
	Rotator operator*(const Rotator& r, float s);
}

#endif // !_ROTATOR_H