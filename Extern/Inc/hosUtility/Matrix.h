#ifndef _MATRIX_H
#define _MATRIX_H

// [2020/12/31 노종원]
// 행렬
// 일부러 ut네임스페이스를 정의하지않았다.

// LOG
// [2021/1/12 노종원] PxMat44와 SimpleMath::Matrix는 이름 충돌이 없기때문에 참조는 만들지 않는다.
// [2121/03/19] 노종원 : 레퍼런스로 가지고있을때 문제가 발생해서 simpleMath만 상속받고 PhysX는 캐스팅 오퍼레이터를 오버로딩해서 해결한다.

#include "BaseTypes.h"

namespace hos
{
	class Matrix :public DirectX::SimpleMath::Matrix
	{
	public:
		Matrix(float m00 = 1.f, float m01 = 0.f, float m02 = 0.f, float m03 = 0.f,
			float m10 = 0.f, float m11 = 1.f, float m12 = 0.f, float m13 = 0.f,
			float m20 = 0.f, float m21 = 0.f, float m22 = 1.f, float m23 = 0.f,
			float m30 = 0.f, float m31 = 0.f, float m32 = 0.f, float m33 = 1.f);
		Matrix(DirectX::SimpleMath::Matrix matrix);
		~Matrix();

		Matrix& operator=(const DirectX::SimpleMath::Matrix& mat);
		Matrix& operator=(const Matrix& mat);

	#ifdef PHYSX_MATH
		Matrix(physx::PxMat44 matrix);
		Matrix& operator=(const physx::PxMat44& mat);
		operator physx::PxMat44();
	#endif // PHYSX_MATH
	};
}

#endif // !_MATRIX_H