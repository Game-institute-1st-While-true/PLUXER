#ifndef _QUATERNION_H
#define _QUATERNION_H

// [2020/12/31 노종원]
// 쿼터니언
// 일부러 ut네임스페이스를 정의하지않았다.

// LOG
// [2021/1/12 노종원] PxQuat를 protected상속해서 PxQuat의 맴버에 접근하지못하도록 해서 맴버변수들의 모호함을 없애려고 하였으나
// 컴파일되지 않아서 자식클래스에서 래퍼런스타입의 변수로 맴버변수의 모호함을 제거하였다.
// SimpleMath의 Quaternion을 public상속해서 해당 맴버함수들을 바로 가져다 쓸수있게 하고, 레퍼런스타입이 해당 맴버를 가리키도록해서
// 맴버변수들의 값이 변할때 SimpleMath내부의 맴버변수도 함께 변하도록 했다. 
// PxQuat를 protected로 상속받았기때문에 PxQuat로의 형변환이 자동으로 되지않는 문제가 생겼는데 클래스 내부에서는 변환할 수 있음으로
// To함수를 만들어서 PxQuat로 값들을 복사하고 형변환 해서 리턴하도록 했다.
// 다른 수학 클래스들도 이와같이 만들 예정
// [2121/03/19] 노종원 : 레퍼런스로 가지고있을때 문제가 발생해서 simpleMath만 상속받고 PhysX는 캐스팅 오퍼레이터를 오버로딩해서 해결한다.

#include "BaseTypes.h"

namespace hos
{
	class Vector3;
	class Rotator;
	class Quaternion :public DirectX::SimpleMath::Quaternion
	{
	public:
		Quaternion(float x = 0.f, float y = 0.f, float z = 0.f, float w = 1.f);
		Quaternion(DirectX::SimpleMath::Quaternion quat);
		Quaternion(Rotator r);
		~Quaternion();

		Quaternion& operator=(const DirectX::SimpleMath::Quaternion& quat);
		Quaternion& operator=(const Quaternion& quat);

		Vector3 ToEuler();

	#ifdef PHYSX_MATH
		Quaternion(physx::PxQuat quat);
		Quaternion& operator=(const physx::PxQuat& quat);
		operator physx::PxQuat();
	#endif // PHYSX_MATH
	};
}

#endif // !_QUATERNION_H