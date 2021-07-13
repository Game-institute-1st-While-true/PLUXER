// <Concept>
// Source Name		: D3D11.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.01.10

// <Explanation>
// D3D11의 API 클래스

// Working Log
// 2021.01.10.Hosik	: 생성
// 2021.02.04.Hosil : DeviceManager용 디바이스 자원의 겟터 추가

#pragma once

#include <d3d11_4.h>
#include <wrl.h>

#include "D3D11Data.h"
#include "RendererDesc.h"


namespace hos::cg
{
	class D3D11
	{
		// Device와 Context는 DeviceManager를 거쳐서만 가져갈 수 있다.
		friend class D3D11DeviceManager;

		// 멤버 인스턴스
	private:
		// 그래픽 설정 정보
		//DEVICE_INFO DeviceInfo;
		//VGA_INFO	VgaInfo;
		//HWND Hwnd;
		RendererDesc& RenderDesc;

		// DirectX 디바이스 객체
		Microsoft::WRL::ComPtr<ID3D11Device> Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;

		// 스왑체인, 렌더타겟 뷰
		Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTargetBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;

		// 뎁스 스텐실 버퍼 및 뷰
		Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;

		// 상태객체 목록
		unsigned int NowDSState;
		unsigned int NowRSState;
		unsigned int NowBSState;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState[DS_MAX];
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterState[RS_MAX_];
		Microsoft::WRL::ComPtr<ID3D11BlendState> BlendState[BS_MAX_];

		// 생성자 소멸자
	public:
		D3D11() = delete;
		D3D11(RendererDesc& desc);
		~D3D11();

		// 외부용 함수
	public:
		void BeginScene(float red, float green, float blue, float alpha);
		void EndScene();

		void SetMainRenderTarget();

		ID3D11Device* GetDevice() { return Device.Get(); };
		ID3D11DeviceContext* GetContext() { return DeviceContext.Get(); };

		// 뷰포트 설정
		void SetViewPort(float left, float right, float top, float bottom);
		void SetViewPort_Size(float left, float right, float top, float bottom);

		// 렌더상태 설정
		void SetRasterizeState(unsigned int stateIndex);
		void SetDepthStencilState(unsigned int stateIndex);
		void SetBlendState(unsigned int stateIndex);

		bool GetFullScreenState() const;

		//void _DrawText(std::wstring_view text, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector4 color, float fontSize, float rot);

		bool ResizeSwapChain(unsigned int screenWidth, unsigned int screenHeight, unsigned int valueAA);

		ID3D11BlendState* GetAlphaBlendState() { return BlendState[BS_ALPHA_BLEND].Get(); };

		// 내부용 함수
	private:
		// 디바이스 세팅
		//void SetupDeviceInfo(HWND hwnd, DWORD screenWidth, DWORD screenHeight, bool vsync, bool fullScreen,
		//	float screenDepth, float screenNear, DWORD valueAA, DWORD valueAF);
		// 생성자에 전달받은 기기정보 세팅
		HRESULT GetVGAInfo();		// 비디오 카드 정보 획득
		HRESULT CreateRTView();		// 렌더타겟 뷰 생성
		HRESULT CreateDSView();		// 깊이, 스텐실 뷰 생성
		HRESULT SetViewport();		// 뷰포트 생성

		// 렌더상태객체 생성
		HRESULT CreateStateObject();		// 렌더링 상태객체 생성
		HRESULT CreateDSState();			// 뎁스스텐실 상태 생성
		HRESULT CreateRasterizeState();		// 레스터라이즈 상태 생성
		HRESULT CreateBlendState();			// 블렌드 상태 생성

		// 디버그용
		bool SdkLayersAvailable();
		void CheckReferenceCountForDebug();

		// 디바이스 매니저용
		ID3D11Device* GetD3D11Device() const { return Device.Get(); };
		ID3D11DeviceContext* GetD3D11Context() const { return DeviceContext.Get(); };

		// ScreenMode Check
		void CheckScreenState();
	};
}