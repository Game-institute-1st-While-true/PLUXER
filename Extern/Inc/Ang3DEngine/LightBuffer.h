// <Concept>
// Source Name		: LightBuffer.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.03.10

// <Explanation>
// 현재 씬의 빛 정보를 담고있을 버퍼
/// Render Loop가 끝나면 LightBuffer는 초기화됩니다.
/// 빛의 세팅은 매 루프마다 이루어져야합니다.

// Working Log
// 2021.03.10.Hosik	: 생성

#pragma once

#include "DXTK\SimpleMath.h"


namespace hos::cg
{
	class alignas(16) LightBuffer	// 상수버퍼용 데이터는 16바이트로 정렬되어야 한다.
	{

	private:
		// 직사광용
		DirectX::SimpleMath::Matrix DirLightViewProj[5];
		DirectX::SimpleMath::Vector4 DirLightDir[5];
		DirectX::SimpleMath::Vector4 DirLightDiffColor[5];
		DirectX::SimpleMath::Vector4 DirLightShaowColor[5];

		DirectX::SimpleMath::Vector4 PointLightPos[20];
		DirectX::SimpleMath::Vector4 PointLightColor[20];
		DirectX::SimpleMath::Vector4 PointLightShadowColor[20];
		DirectX::SimpleMath::Vector4 PointLightAttenuation[20];

		DirectX::SimpleMath::Vector4 AmbLightColor;

		unsigned int DirLightNum;
		unsigned int PointLightNum;
		float ShadowBias;


	public:
		LightBuffer();
		~LightBuffer();

	public:
		// Direction Light
		void AddDirectionLight(DirectX::SimpleMath::Vector4& dir, DirectX::SimpleMath::Vector4& color);

		// Point Light
		void AddPointLight(DirectX::SimpleMath::Vector4& pos, DirectX::SimpleMath::Vector4& color, DirectX::SimpleMath::Vector4& attenuation);

		// Ambient Light
		void AddAmbientLight(DirectX::SimpleMath::Vector4& color);
		

		void UpdateLightBuffForShadow(DirectX::SimpleMath::Vector3 cameraPos, float fovDegree, unsigned int screenWidth, unsigned int screenHeight);
		void ClearDirLight();
		
	private:
		DirectX::SimpleMath::Vector4 GetShadowColor(DirectX::SimpleMath::Vector4& color);
	};
}

