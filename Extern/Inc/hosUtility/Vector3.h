#ifndef _VECTOR3_H
#define _VECTOR3_H

// [2020/12/31 노종원]
// vector3
// 여러가지 vector3 타입의 컨버터 역할을 할 vector3클래스
// 일부러 ut네임스페이스를 정의하지않았다.

// LOG
// [2121/03/19] 노종원 : 레퍼런스로 가지고있을때 문제가 발생해서 simpleMath만 상속받고 PhysX는 캐스팅 오퍼레이터를 오버로딩해서 해결한다.

#include "BaseTypes.h"

namespace hos
{
	class Matrix;
	class Vector3 : public DirectX::SimpleMath::Vector3
	{
	public:
		Vector3(F32 x = 0.f, F32 y = 0.f, F32 z = 0.f);
		Vector3(DirectX::SimpleMath::Vector3 vec);
		~Vector3();

		Vector3& operator=(const DirectX::SimpleMath::Vector3& vec);
		Vector3& operator=(const Vector3& vec);

	#ifdef PHYSX_MATH
		Vector3(physx::PxVec3 vec);
		Vector3& operator=(const physx::PxVec3& vec);
		operator physx::PxVec3();
	#endif // PHYSX_MATH
	};

	Vector3 operator*(const Vector3& v, const Matrix& m);
	Vector3 operator*(const Vector3& v, float s);
}

#endif // !_VECTOR3_H