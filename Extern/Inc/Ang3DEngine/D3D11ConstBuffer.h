// <Concept>
// Source Name		: D3D11ConstBuffer.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.01.10

// <Explanation>
// D3D11에서 사용하는 상수버퍼 클래스.

// Working Log
// 2021.01.10.Hosik	: 생성
// 2021.02.04.Hosil : Device 래퍼런스 제거

#pragma once

#include <d3d11_4.h>
#include <wrl.h>

#include "D3D11Data.h"

namespace hos::cg
{
	class D3D11ConstBuffer
	{

	private:
		ID3D11Device& DeviceRef;
		ID3D11DeviceContext& ContextRef;

		WCHAR BufferName[256];
		LPVOID Data;
		size_t BufferSize;
		Microsoft::WRL::ComPtr<ID3D11Buffer> ConstBuffer;

	public:
		D3D11ConstBuffer() = delete;
		D3D11ConstBuffer(ID3D11Device* device, ID3D11DeviceContext* context, LPVOID data, UINT size, const WCHAR* bufferName);
		~D3D11ConstBuffer();

	public:
		HRESULT Update();
		ID3D11Buffer** GetID3D11Buffer() { return ConstBuffer.GetAddressOf(); };
	};
}