// <Concept>
// Source Name		: ScreenBuffer.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.05.20

// <Explanation>
// PostProcessing을 위해 Screen 및 각종 Map의 크기를 저장할 버퍼.
// 원본 ScreenSize의 Texture에서 Size가 다른 Texture의 Pixel uv를 추출하기 위해 사용.

// Working Log
// 2021.05.20.Hosik	: 생성

#pragma once

#include "DXTK\SimpleMath.h"

namespace hos::cg
{
	class alignas(16) ScreenBuffer	// 상수버퍼용 데이터는 16바이트로 정렬되어야 한다.
	{
	private:
		DirectX::SimpleMath::Matrix NowIvsViewProj;
		DirectX::SimpleMath::Matrix OldViewProj;

		// RenderScreen Size
		float ScreenWidth;
		float ScreenHeight;

		// ShadowMap Size
		float ShadowMapWidth;
		float ShadowMapHeight;

		// EmissionMap Size
		float EmissMapWidth;
		float EmissMapHeight;

		// Emission Value
		float EmissIntensity;

		// Blur Value
		float BlurPixelInterval;

		// Radial Blur Value
		// xy=center, z=sampleDist, w=sampleStrength
		DirectX::SimpleMath::Vector4 RadialValue;

		// Fog
		DirectX::SimpleMath::Vector3 FogColor;
		float FogDensity;

		// Mortion Blur
		float dTime;
		unsigned int MortionBlurSampleCount;

		// Bloom Value
		float BloomPower;

		bool bFog;


	public:
		ScreenBuffer();
		~ScreenBuffer();

	public:

		unsigned int GetMBSampleCount() { return this->MortionBlurSampleCount; };

		void SetNowCameraInverseMatrix(DirectX::SimpleMath::Matrix nowViewTM, DirectX::SimpleMath::Matrix nowProjTM);
		void SetOldCameraMatrix(DirectX::SimpleMath::Matrix oldViewTM, DirectX::SimpleMath::Matrix oldProjTM);

		void SetScreenSize(unsigned int width, unsigned int height);
		void SetShadowMapSize(unsigned int width, unsigned int height);
		void SetEmissMapSize(float width, float height);

		void SetEmissIntensity(float intensity);
		
		void SetBlurPixelInterval(float interval);

		void SetDeltaTime(float dTime);
		void SetMotionBlurSampleCount(unsigned int sampleCount);

		void SetFog(float r, float g, float b, float density, bool bFog);

		void SetRadialBlurValue(float uv_x, float uv_y, float distance, float strength);

		void SetBloomPower(float bloomPower) { this->BloomPower = bloomPower; };

	private:

	};
}

