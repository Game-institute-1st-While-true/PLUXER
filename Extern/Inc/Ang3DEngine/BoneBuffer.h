// <Concept>
// Source Name		: BoneBuffer.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.02.24

// <Explanation>
// 특정 노드의 BoneTM을 가지고있을 버퍼

// Working Log
// 2021.02.24.Hosik	: 생성

#pragma once

#include "DXTK\SimpleMath.h"
#include "TempDefine.h"

namespace hos::cg
{
	class alignas(16) BoneBuffer	// 상수버퍼용 데이터는 16바이트로 정렬되어야 한다.
	{
	private:
		DirectX::SimpleMath::Matrix BoneTM[MAX_BONE];

	public:
		BoneBuffer();
		~BoneBuffer();

	public:
		void SetBoneTM(DirectX::SimpleMath::Matrix* matrixArray, unsigned int size);

	private:

	};
}

