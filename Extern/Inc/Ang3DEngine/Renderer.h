// <Concept>
// Source Name		: Renderer.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.01.10

// <Explanation>
// 각종 API에 대응하기 위한 Renderer 클래스.
// 해당 클래스를 인터페이스로 그래픽API의 Renderer를 생성한다.
// ResourceManager는 그래픽API에 비종속적이므로 여기서 소유한다.

// <Working Log>
// 2021.01.10.Hosik	: 생성
// 2021.03.14.Hosik : FbxLoader 및 ResourceManager 분리(게임 클라이언트 프로젝트로 이동)
// 2021.03.25.Hosik : DebugLogger 파라미터 std::string, wstring으로 변경, 매개변수 케이스 별로 함수 정리, 헤더 및 전방선언 정리
// 2021.05.20.Hosik	: PostProcessing용 Screen 상수버퍼 추가.
// 2021.05.26.Hosik : Deferred용 함수 및 인스턴스 일단 삭제(Forward 모두 구현 후 재 입력 예정)


#pragma once

// 기능 및 수학
#include "framework.h"
#include "DXTK\SimpleMath.h"

// 렌더 필수 요소
#include "RendererDesc.h"
#include "RenderQue.h"
#include "D3D11Data.h"	// 이름만 D3D11임
#include "Camera.h"

// 버퍼
#include "MatrixBuffer.h"
#include "MaterialBuffer.h"
#include "BoneBuffer.h"
#include "LightBuffer.h"
#include "ScreenBuffer.h"

// 임시용(최대 뼈 갯수)
#include "TempDefine.h"

// Define Value
#define SHADOWMAPSIZE 14336

namespace hos::cg
{
	class Camera;

	class Renderer
	{
		// 추가 속성(ex.firend class 명시)

		// 멤버 인스턴스
	protected:
		RendererDesc RenderDesc;	// 외부에서 전달된 렌더러의 설정을 복사해서 가지고 있는다.
									// 추후 D3D11_API(그래픽API) 클래스에서는 이 Desc를 래퍼런스 해서 가지고 있는다(연동되어 있음)

		float dTime;

		// 렌더 Data
		MatrixBuffer MatrixBuff;
		MaterialBuffer MaterialBuff;
		BoneBuffer BoneBuff;
		LightBuffer LightBuff;
		ScreenBuffer ScreenBuff;

		/// Camera 행렬
		// Now Scene 행렬
		bool bViewSet;
		bool bProjSet;
		DirectX::SimpleMath::Matrix CameraViewTM;
		DirectX::SimpleMath::Matrix CameraProjTM;

		// Old Scene 행렬
		DirectX::SimpleMath::Matrix OldViewTM;
		DirectX::SimpleMath::Matrix OldProjTM;

		// UI 행렬
		DirectX::SimpleMath::Matrix UIViewTM;
		DirectX::SimpleMath::Matrix UIProjTM;

		// 렌더 큐
		RenderQue* RenderQueInstance;

		// 카메라
		Camera* CameraInstance;

		// 임시 FogColor
		DirectX::SimpleMath::Vector4 FogColor;

		// ViewCulling On/Off
		bool bViewCull;

		// SHADOWMAP CULL MODE
		RendererDesc::ShadowMapCullList ShadowMapCullMode;

		// 생성자, 소멸자
	public:
		Renderer() = delete;
		Renderer(RendererDesc desc);
		virtual ~Renderer();

		//////////////////////////////////////////////////////////////////////////////////////////
		//						외부 사용 함수 : 추후 인터페이스화 해야한다.					//
		//////////////////////////////////////////////////////////////////////////////////////////

		// 비가상 함수
	public:
		// Create Renderer
		static Renderer* CreateRenderer(RendererDesc& desc);	/// 해당 static 함수로 Renderer를 return 받아 저장한 뒤 사용하세요
		RendererDesc& GetRenderDesc();

		// Render Function
		void ForwardRender_ToCameraBuffer(float dTime, float nearDepth, DirectX::SimpleMath::Vector3 cameraPos, DirectX::SimpleMath::Vector3 cameraRot, float cameraFovDegree,
			float screenWidth, float screenHeight, float screenNear, float screenDepth, RendererDesc::CameraModeList cameraMode = RendererDesc::CameraModeList::CAMERA3D_DEFAULT,
			unsigned int mortionBlurCount = 0, unsigned int cameraIndex = 0, bool skyBox = false, std::wstring_view skyBoxName = L"",
			DirectX::SimpleMath::Vector4 clearColor = DirectX::SimpleMath::Vector4(0, 0, 1, 0), bool bDebug = false);
		void RenderCamera_ToSwapChainBuffer(bool bRadial, unsigned int cameraIndex = 0, unsigned int outputIndex = 0);
		void RenderUI_ToSwapChainBuffer();

		// Camera Setting: Direct Set To PipeLine
		void SetViewMatrixToPipeline(const DirectX::SimpleMath::Matrix mView);
		void SetProjMatrixToPipeline(const DirectX::SimpleMath::Matrix mProj);

		// Light 세팅: 입력할 때마다 1개씩, 최개 갯수까지 추가됩니다 (Direction:MAX_3, Point:MAX_20)
		void SetDirLight(DirectX::SimpleMath::Vector4& dir, DirectX::SimpleMath::Vector4& diffColor);
		void SetPointLight(DirectX::SimpleMath::Vector4& pos, DirectX::SimpleMath::Vector4& color, DirectX::SimpleMath::Vector4& attenuation);
		void SetAmbientLight(DirectX::SimpleMath::Vector4& ambColor);

		// 렌더큐 함수
		void AddRenderObejct(RenderQue::RenderObject& objectInfo);
		void AddAlphaObject(RenderQue::RenderObject& objectInfo);
		void AddEffectObject(RenderQue::RenderObject& objectInfo);
		void AddDebugObject(RenderQue::RenderObject& objectInfo);
		void AddRenderUIObejct(RenderQue::RenderUIObejct& objectInfo);

		// Graphics Option Setting
		bool GetMotionBlurState() { return this->RenderDesc.MotionBlurState; };
		bool GetFogState() { return this->RenderDesc.FogState; };

		void SetMotionBlurState(bool bOn) { this->RenderDesc.MotionBlurState = bOn; };
		void SetFogState(bool bOn) { this->RenderDesc.FogState = bOn; };
		void SetFogOption(float r, float g, float b, float density);

		// RadialBlurValue Setting
		void SetRadialValue(float uv_x = 0.5f, float uv_y = 0.5f, float distance = 0.25f, float sterngth = 3.0f);

		// ViewFrustum On/Off
		void SetViewFrustum(bool isBool) { this->bViewCull = isBool; };

		// Bloom Setting
		void SetBloomPower(float bloomPower);

		// ShadowMap Cull Mode Setting
		void SetShadowMapCullMode(RendererDesc::ShadowMapCullList cullMode) { this->ShadowMapCullMode = cullMode; };

		// 외부용 추상함수
	public:
		// Device RenderTarget Contoll
		virtual void BeginDraw(unsigned int screenWidth = 1920, unsigned int screenHeight = 1080, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) = 0;
		virtual void EndDraw() = 0;

		// Text
		virtual bool CreateSpriteFont(std::wstring_view fontName, uint8_t const* data, size_t dataSize) = 0;
		virtual void _DebugText(int posX, int posY, int size, DirectX::SimpleMath::Vector4 col, const WCHAR* text, ...) = 0;
		virtual DirectX::SimpleMath::Vector2 GetTextBoundSize(std::wstring_view text, float fontSize, DirectX::SimpleMath::Vector2 scale, std::wstring_view FontName = L"Arial") = 0;

		// SkyBox
		virtual bool LoadSkyBox(std::wstring_view skyBoxName, std::wstring_view filePath) = 0;
		
		// ViewPort
		virtual void SetViewPort(float left, float right, float top, float bottom) = 0;

		// Resource Managemnet
		virtual void CreateMeshVariable(std::wstring meshName, void* vtxData, unsigned int vtxCount, void* indexData, unsigned int indexCount) = 0;
		virtual bool CreateTextureVariable(std::wstring textureName, std::wstring fileRoot, bool* bAlpha = nullptr) = 0;
		virtual void ReleaseMeshVariable(std::wstring meshName) = 0;
		virtual void ReleaseTextureVariable(std::wstring meshName) = 0;
		virtual void AllClearMeshVariableList() = 0;
		virtual void AllClearTextureVariableList() = 0;

		virtual bool GetFullScreenState() const = 0;


		//////////////////////////////////////////////////////////////////////////////////////////
		//					내부 사용 함수 : 추후 인터페이스에서 빼야 한다.						//
		//////////////////////////////////////////////////////////////////////////////////////////

	private:
		// Render Starting, End Point
		virtual void PreRender(RendererDesc::CameraModeList cameraMode, DirectX::SimpleMath::Vector3 cameraPos, DirectX::SimpleMath::Vector3 cameraRot, float fov, float nearValue, float depthValue) = 0;
		virtual void LastRender(bool bRadial = false, unsigned int cameraIndex = 0, unsigned int outputIndex = 0) = 0;

		// Ready To Draw by ObjectType
		virtual void ClearAllRenderTarget(unsigned int cameraIndex = 0, float r = 0, float g = 0, float b = 1, float a = 1, bool bDeferred = false) = 0;
		virtual void Draw_GameObject(float nearDepth, DirectX::SimpleMath::Vector3 cameraPos, bool bDeferred = false, unsigned int cameraIndex = 0) = 0;
		virtual void Draw_GameObject_ShadowMap(DirectX::SimpleMath::Vector3 cameraPos, float cameraFovDegree) = 0;
		virtual void Draw_GameObject_EmissMap(DirectX::SimpleMath::Vector3 vPos, DirectX::SimpleMath::Vector3 vRot,
			float fov, float screenWidth, float screenHeight, float screenNear, float screenDepth,
			float intensity = 1.0f, float pixelInterval = 1.0f) = 0;
		virtual void Draw_GameObject_VelocityMap() = 0;

		virtual void Draw_DebugObject(unsigned int cameraIndex = 0) = 0;
		virtual void DrawDebugGrid(unsigned int cameraIndex = 0) = 0;

		// Render Function
		virtual void DrawUIObject_3D(RenderQue::RenderUIObejct& targetQue, float screenWidth, float screenHeight) = 0;
		virtual void DrawSkyBox(std::wstring_view skyboxName, unsigned int cameraIndex = 0) = 0;
		virtual void DrawFontText(RenderQue::RenderUIObejct& targetQue) = 0;
		

		// Render State Setting
		virtual void SetRasterizeState(int index) = 0;
		virtual void SetBlendState(int index) = 0;
		virtual void SetDepthStencilState(int index) = 0;
	};
}