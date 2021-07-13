#ifndef _VECTOR4_H
#define _VECTOR4_H

// [2021/1/12 노종원]
// vector4
// 여러가지 vector4 타입의 컨버터 역할을 할 vector4클래스
// 일부러 ut네임스페이스를 정의하지않았다.

//LOG
// [2121/03/19] 노종원 : 레퍼런스로 가지고있을때 문제가 발생해서 simpleMath만 상속받고 PhysX는 캐스팅 오퍼레이터를 오버로딩해서 해결한다.

#include "BaseTypes.h"

namespace hos
{
	class Vector4 : public DirectX::SimpleMath::Vector4
	{
	public:
		Vector4(F32 x = 0.f, F32 y = 0.f, F32 z = 0.f, F32 w = 0.f);
		Vector4(DirectX::SimpleMath::Vector4 vec);
		~Vector4();

		Vector4& operator=(const DirectX::SimpleMath::Vector4& vec);
		Vector4& operator=(const Vector4& vec);

	#ifdef PHYSX_MATH
		Vector4(physx::PxVec4 vec);
		Vector4& operator=(const physx::PxVec4& vec);
		operator physx::PxVec4();
	#endif // PHYSX_MATH

	};
}

#endif // !_VECTOR4_H