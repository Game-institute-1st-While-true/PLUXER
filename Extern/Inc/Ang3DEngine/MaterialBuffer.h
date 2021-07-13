// <Concept>
// Source Name		: MaterialBuffer.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.02.18

// <Explanation>
// DirectX의 재질 버퍼에 사용할 버퍼.
// 다른 그래픽API(ex.Vulkan, OpenGL 등)에서는 어떻게 사용되는지 아직 모른다.

// Working Log
// 2021.02.18.Hosik	: 생성

#pragma once

#include "DXTK\SimpleMath.h"


namespace hos::cg
{

	class alignas(16) MaterialBuffer
	{
		// 멤버 인스턴스
	private:
		DirectX::SimpleMath::Vector4 DiffuseColor;
		DirectX::SimpleMath::Vector4 AmbientColor;
		DirectX::SimpleMath::Vector4 SpecularColor;
		DirectX::SimpleMath::Vector4 OffTill;
		float SpecularPower;


		// 생성자 소멸자
	public:
		MaterialBuffer();
		~MaterialBuffer();

		// 외부 함수
	public:
		void SetDiffColor(const DirectX::SimpleMath::Vector4& color);
		void SetAmbColor (const DirectX::SimpleMath::Vector4& color);
		void SetSpecColor(const DirectX::SimpleMath::Vector4& color);
		void SetOffTill  (const DirectX::SimpleMath::Vector4& offTill);
		void SetSpecPower(const float& power);
	};

}