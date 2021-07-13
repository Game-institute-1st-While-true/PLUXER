// <Concept>
// Source Name		: D3D11Shader.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.01.10

// <Explanation>
// D3D11에서 사용하는 쉐이더 클래스

// Working Log
// 2021.01.10.Hosik	: 생성
// 2021.02.04.Hosik : Device 래퍼런스 제거, ynErrorW 부활
// 2021.05.11.Hosik : 여러가지 Sampler를 사용할 수 있도록 개선하면서 Sampler를 Index만 가지고 있도록 변경. 

#pragma once

#include <d3d11_4.h>
#include <wrl.h>

#include <vector>

namespace hos::cg
{
	class D3D11Shader
	{
		// 내부 멤버 인스턴스
	private:
		// Context Ref
		ID3D11Device& DeviceRef;
		ID3D11DeviceContext& ContextRef;
		std::vector<ID3D11SamplerState*>& samplerListRef;

		// 쉐이더 경로
		WCHAR FileName[256];
		WCHAR ShaderName[256];

		// 쉐이더 객체
		Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
		Microsoft::WRL::ComPtr<ID3DBlob> VSCode;

		// 그리기용
		//Microsoft::WRL::ComPtr<ID3D11SamplerState> SampleState;
		unsigned int SamplerCount;
		int* SamplerIndexList;

	public:
		// 생성자 소멸자
		D3D11Shader() = delete;
		D3D11Shader(WCHAR* fileName, int shaderIndex, const WCHAR* shaderName, ID3D11Device& deviceRef, ID3D11DeviceContext& contextRef, std::vector<ID3D11SamplerState*>& samplerList);
		D3D11Shader(WCHAR* vsFileName, WCHAR* psFileName, int shaderIndex, ID3D11Device& deviceRef, ID3D11DeviceContext& contextRef, std::vector<ID3D11SamplerState*>& samplerList);
		~D3D11Shader();

	public:
		// 그리기
		void Draw_Index(unsigned int indexCount);
		void Draw_Vtx(unsigned int vtxCount);

	private:
		// 쉐이더 생성용
		HRESULT LoadCompliedShader(WCHAR* vsFileName, WCHAR* psFileName, int shaderIndex);

		HRESULT InitializeShader();
		HRESULT VS_Load();
		HRESULT PS_Load();
		HRESULT CompilerLoad(const CHAR* entry, const CHAR* target, ID3DBlob** ppCode);

		// 사용할 레이아웃 종류에 따라 한가지를 선택해서 생성합니다.
		HRESULT CreateVBLayout(int shaderIndex);
		HRESULT CreateSamplerIndex(int shaderIndex);

		int ynErrorW(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob, TCHAR* filename, char* EntryPoint, char* ShaderModel);
	};
}