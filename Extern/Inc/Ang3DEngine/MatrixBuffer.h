// <Concept>
// Source Name		: MatrixBuffer.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.02.03

// <Explanation>
// DirectX의 상수버퍼에 사용할 행렬버퍼.
// 다른 그래픽API(ex.Vulkan, OpenGL 등)에서는 어떻게 사용되는지 아직 모른다.

// Working Log
// 2021.02.03.Hosik	: 생성

#pragma once

#include "DXTK\SimpleMath.h"

namespace hos::cg
{
	class alignas(16) MatrixBuffer	// 상수버퍼용 데이터는 16바이트로 정렬되어야 한다.
	{
	private:
		DirectX::SimpleMath::Matrix WorldTM;
		DirectX::SimpleMath::Matrix ViewTM;
		DirectX::SimpleMath::Matrix ProjTM;
		DirectX::SimpleMath::Matrix WorldViewTM;
		DirectX::SimpleMath::Matrix WorldViewProjTM;
		DirectX::SimpleMath::Matrix InverseViewTM;
		DirectX::SimpleMath::Matrix InverseProjTM;
		DirectX::SimpleMath::Vector3 CameraWorldPos;

	public:
		MatrixBuffer();
		~MatrixBuffer();

	public:
		void SetWorldTM(const DirectX::SimpleMath::Matrix& mTM);
		void SetViewTM(const DirectX::SimpleMath::Matrix& mView);
		void SetProjTM(const DirectX::SimpleMath::Matrix& mProj);

	private:

	};
}

