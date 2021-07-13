// <Concept>
// Source Name		: D3D11Renderer.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.01.10

// <Explanation>
// D3D11의 렌더러 클래스.
// API에 비종속적인 Renderer 클래스를 상속받아 만들어졌다.

// Working Log
// 2021.01.10.Hosik	: 생성

#pragma once

// 상속용
#include "Renderer.h"
#include "D3D11.h"

#include <d3d11_4.h>

#define MAX_CAMERA 1

namespace hos::cg
{
	// 참조용 클래스
	class D3D11;
	class RendererDesc;
	class D3D11VariableManager;
	class D3D11ExtraRenderTarget;
	class D3D11Texture2D;

	class D3D11ShadowBuffer;

	class D3D11ParticleSystem;

	//class MaterialData;
	//class NodeData;

	class DebugGrid;

	class D3D11Renderder : public Renderer
	{
		// 멤버 인스턴스
	private:
		// 장치
		D3D11* D3D11Instance;						// Direct3D 클래스.
		D3D11VariableManager* VariableManagerInstance;

		ID3D11Device* DeviceRef;
		ID3D11DeviceContext* DeviceContextRef;

		// 카메라용 렌더타겟(5개)
		//D3D11ExtraRenderTarget* PreRenderTarget[MAX_CAMERA];
		D3D11ExtraRenderTarget* PreRenderTarget;

		// 그림자
		D3D11ShadowBuffer* RenderTarget_LightViewShadowMap;
		D3D11ExtraRenderTarget* RT_SoftShadow;
		
		// 가우시안 블러용
		D3D11ExtraRenderTarget* RT_DownSample_x4;
		D3D11ExtraRenderTarget* RT_DownSample_x4_2;

		D3D11ExtraRenderTarget* RT_DownSample_x16;
		D3D11ExtraRenderTarget* RT_DownSample_x16_2;

		D3D11ExtraRenderTarget* RT_DownSample_x64;
		D3D11ExtraRenderTarget* RT_DownSample_x64_2;

		// 깊이 속도맵
		D3D11ExtraRenderTarget* RenderTarget_NowDepth;
		D3D11ExtraRenderTarget* RT_Velocity;

		// Debug Grid
		DebugGrid* DebugGridInstance;

		// Particle
		D3D11ParticleSystem* ParticleSystem;

		// 생성자, 소멸자
	public:
		D3D11Renderder() = delete;
		D3D11Renderder(RendererDesc& desc);
		~D3D11Renderder();

		// 외부 공개용 함수
	public:
		// Device RenderTarget Contoll
		void BeginDraw(unsigned int screenWidth = 1920, unsigned int screenHeight = 1080, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
		void EndDraw();

		// Text
		bool CreateSpriteFont(std::wstring_view fontName, uint8_t const* data, size_t dataSize);
		void _DebugText(int posX, int posY, int size, DirectX::SimpleMath::Vector4 col, const WCHAR* text, ...);

		// SkyBox
		bool LoadSkyBox(std::wstring_view skyBoxName, std::wstring_view filePath);

		// ViewPort
		void SetViewPort(float left, float right, float top, float bottom);
		void SetViewPort_Size(float left, float right, float top, float bottom);

		// Resource Managemnet
		void CreateMeshVariable(std::wstring meshName, void* vtxData, unsigned int vtxCount, void* indexData, unsigned int indexCount);
		bool CreateTextureVariable(std::wstring textureName, std::wstring fileRoot, bool* bAlpha = nullptr);
		void ReleaseMeshVariable(std::wstring meshName);
		void ReleaseTextureVariable(std::wstring textureName);
		void AllClearMeshVariableList();
		void AllClearTextureVariableList();

		bool GetFullScreenState() const;


		// 내부용 함수
	private:
		// Render Starting, End Point
		void PreRender(RendererDesc::CameraModeList cameraMode, DirectX::SimpleMath::Vector3 cameraPos, DirectX::SimpleMath::Vector3 cameraRot, float fov, float nearValue, float depthValue);
		void LastRender(bool bRadial = false, unsigned int cameraIndex = 0, unsigned int outputIndex = 0);
		
		// Ready To Draw by ObjectType
		void ClearAllRenderTarget(unsigned int cameraIndex = 0, float r = 0, float g = 0, float b = 1, float a = 1, bool bDeferred = false);
		
		void Draw_GameObject(float nearDepth, DirectX::SimpleMath::Vector3 cameraPos, bool bDeferred = false, unsigned int cameraIndex = 0);
		void Draw_GameObject_ShadowMap(DirectX::SimpleMath::Vector3 cameraPos, float cameraFovDegree);
		void Draw_GameObject_EmissMap(DirectX::SimpleMath::Vector3 vPos, DirectX::SimpleMath::Vector3 vRot,
			float fov, float screenWidth, float screenHeight, float screenNear, float screenDepth,
			float intensity = 1.0f, float pixelInterval = 1.0f);
		void Draw_GameObject_VelocityMap();

		void Draw_DebugObject(unsigned int cameraIndex = 0);

		// Render Function
		void DrawEachObject(RenderQue::RenderObject& targetQue, bool bAlpha = false, bool bDeferred = false, unsigned int cameraIndex = 0);
		bool SetMaterial(RenderQue::RenderMaterial& targetMaterial);
		void DrawByShaderType(RenderQue::RenderObject& targetQue, bool bAlpha, bool bDeferred, bool bAlbedo, unsigned int cameraIndex, unsigned int indexCount);

		void DrawUIObject_3D(RenderQue::RenderUIObejct& targetQue, float screenWidth, float screenHeight);
		void DrawSkyBox(std::wstring_view skyboxName, unsigned int cameraIndex = 0);
		void DrawFontText(RenderQue::RenderUIObejct& targetQue);

		DirectX::SimpleMath::Vector2 GetTextBoundSize(std::wstring_view text, float fontSize, DirectX::SimpleMath::Vector2 scale, std::wstring_view FontName = L"Arial");

		void DrawSkinShadowMap(RenderQue::RenderObject& targetQue);
		void DrawShadowMap(RenderQue::RenderObject& targetQue);

		void DrawSkinEmissMap(RenderQue::RenderObject& targetQue);
		void DrawEmissMap(RenderQue::RenderObject& targetQue);

		void DrawDebugGrid(unsigned int cameraIndex = 0);
		void DrawDebugPrimitive(RenderQue::RenderObject& targetQue);

		// Render State Setting
		void SetRasterizeState(int index) { D3D11Instance->SetRasterizeState(index); };
		void SetBlendState(int index) { D3D11Instance->SetBlendState(index); };
		void SetDepthStencilState(int index) { D3D11Instance->SetDepthStencilState(index); };

		// D3D11_Varaible
		void CreateAllConstBuffer();
		void CreateAllShader();
		void CreateAllSampler();

		void CreateExtraRenderTarget();
		void ResizeExtraRenderTarget(unsigned int screenWidth, unsigned int screenHeight, unsigned int valueAA);

		void BeginFont(ID3D11BlendState* state);
		void EndFont();
	};
}