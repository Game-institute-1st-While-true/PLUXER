#ifndef _VECTOR2_H
#define _VECTOR2_H

// [2020/12/21 노종원]
// vector2
// 여러가지 vector2 타입의 컨버터 역할을 할 vector2클래스
// 일부러 ut네임스페이스를 정의하지않았다.

// LOG
// [2121/03/19] 노종원 : 레퍼런스로 가지고있을때 문제가 발생해서 simpleMath만 상속받고 PhysX는 캐스팅 오퍼레이터를 오버로딩해서 해결한다.

#include "BaseTypes.h"

namespace hos
{
	class Vector2 : public DirectX::SimpleMath::Vector2
	{
	public:
		Vector2(F32 x = 0.f, F32 y = 0.f);
		Vector2(DirectX::SimpleMath::Vector2 vec);
		~Vector2();

		Vector2& operator=(const DirectX::SimpleMath::Vector2& vec);
		Vector2& operator=(const Vector2& vec);

	#ifdef PHYSX_MATH
		Vector2(physx::PxVec2 vec);
		Vector2& operator=(const physx::PxVec2& vec);
		operator physx::PxVec2();
	#endif // PHYSX_MATH

	};
}

#endif // !_VECTOR2_H