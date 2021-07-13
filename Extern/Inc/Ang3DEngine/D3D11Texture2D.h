// <Concept>
// Source Name		: D3D11ExtraRenderTarget.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.04.20

// <Explanation>
// D3D11에서 활용할 Texture class.
// 주로 Deferred 및 PostProcessing 결과를 저장할 예정이다.

// Working Log
// 2021.05.03.Hosik	: 생성

#pragma once

#include <d3d11_4.h>
#include <wrl.h>

namespace hos::cg
{

	class D3D11Texture2D
	{
		// 멤버 인스턴스
	private:
		ID3D11Device& DeviceRef;
		ID3D11DeviceContext& ContextRef;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> TextureBuffer;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> ShaderResourceView;

		unsigned int Width;
		unsigned int Height;
		unsigned int ValueAA;

		// 생성자 소멸자
	public:
		D3D11Texture2D() = delete;
		D3D11Texture2D(ID3D11Device* device, ID3D11DeviceContext* context, unsigned int width = 512, unsigned int height = 512, unsigned int valueAA = 1);
		~D3D11Texture2D();

		// 공개 함수
	public:
		void CopyTexture(ID3D11Texture2D* texture);
		ID3D11ShaderResourceView* GetSRV();

		// 내부 함수
	private:
		HRESULT CreateShaderResourceView(ID3D11Texture2D* texture);

	};

}