// <Concept>
// Source Name		: RenderQue.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.02.15

// <Explanation>
// 렌더할 객체의 리소스 정보와 순서를 등록해두는 Que.

// <Working Log>
// 2021.02.15.Hosik	: 생성
// 2021.03.16.Hosik : Resource를 라이브러리 바깥으로 이동시키면서 RenderQue의 구조 변경 및 사용 구조체 수정.
// 2021.03.17.Hosik : vector list의 push_back을 사용하지 않고 resize된 index에 접근하도록 구조 변경.


// <Memo>
// 2021.03.17.Hosik	: 언젠가 Vector가 아니라 일반 배열로 해도 될 것 같다.

#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "DXTK\SimpleMath.h"
#include "RendererDesc.h"

// 기본 렌더큐 크기
#define DEFAULT_RENDERQUE_COUNT 1000

namespace hos::cg
{

	class RenderQue
	{

	public:

		struct RenderUIObejct
		{
			enum Alignment { LeftBottom = 1, Bottom = 2, RightBottom = 3, Left = 4, Center = 5, Right = 6, LeftTop = 7, Top = 8, RightTop = 9 };

			bool bFont = false;
			std::wstring FontName = L"";
			std::wstring Text = L"";
			unsigned int FontSize = 10;
			unsigned int Width = 0;
			unsigned int Height = 0;
			float FontRot = 0.0f;
			int Align = LeftTop;
			DirectX::SimpleMath::Vector2 Origin = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
			
			DirectX::SimpleMath::Vector2 Aspect = DirectX::SimpleMath::Vector2(1.0f, 1.0f);

			DirectX::SimpleMath::Vector3 Pos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			DirectX::SimpleMath::Vector3 Rot = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			DirectX::SimpleMath::Vector3 Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

			bool bAni = false;
			DirectX::SimpleMath::Matrix AniTM;
			DirectX::SimpleMath::Vector3 vAniPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			DirectX::SimpleMath::Vector3 vAniScale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
			DirectX::SimpleMath::Vector3 vAniRot = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

			DirectX::SimpleMath::Vector4 Color = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
			DirectX::SimpleMath::Vector4 OffTill = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
			
			int LayerOrder = 0;

			bool bTexture = false;
			std::wstring TextureName = L"";

			bool bFollow = false;
			int CoordU = 0;
			int CoordV = 0;

			bool operator < (const RenderUIObejct& rhs)
			{
				return (LayerOrder < rhs.LayerOrder);
			}
		};

		enum class RenderPrimitiveType
		{
			BOX,
			SPHERE,
			CAPSULE,
		};

		struct RenderMaterial
		{
			// Material
			DirectX::SimpleMath::Vector4 DiffColor = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
			DirectX::SimpleMath::Vector4 AmbColor = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
			DirectX::SimpleMath::Vector4 SpecColor = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
			float SpecPower = 0.0f;

			// texture
			bool bAlbedo = false;
			bool bNormal = false;
			bool bARM = false;
			bool bEmiss = false;
			std::wstring TexNameAlbedo = L"";
			std::wstring TexNameNormal = L"";
			std::wstring TexNameARM = L"";
			std::wstring TexNameEmiss = L"";
		};

		struct RenderObject
		{
			// User TM
			bool InView = true;
			bool ViewCullSkip = false;
			DirectX::SimpleMath::Vector3 vPos;
			DirectX::SimpleMath::Vector3 vBBox[9];

			DirectX::SimpleMath::Matrix mTM;
			float ViewDistance = -1.0f;

			// Animation
			bool bAnimation = false;
			bool bSkinning = false;
			unsigned int BoneCount = 0;
			DirectX::SimpleMath::Matrix* mSkinnedAni;
			DirectX::SimpleMath::Matrix mRootBone;
			DirectX::SimpleMath::Matrix mNode;

			// Mesh, Vertex Data
			std::wstring MeshName = L"";

			// Render Option
			RendererDesc::RenderModeList RenderMode = RendererDesc::RenderModeList::RENDER_DEFAULT;
			/// DrawMode 리스트(드로우 타입?) 알파블렌드, 프론트페이스 컬링 등

			RenderMaterial MaterialInfo;

			RenderPrimitiveType PrimitiveType = RenderPrimitiveType::BOX;
			bool CollisionState = false;

			bool operator < (const RenderObject& rhs)
			{
				return (ViewDistance > rhs.ViewDistance);
			}
		};

	private:
		
		std::vector<RenderObject> RenderObjectList;
		unsigned int NowObjectCount;
		unsigned int MaxObjectCount;

		std::vector<RenderObject> AlphaObjectList;
		unsigned int NowAlphaCount;
		unsigned int MaxAlphaCount;

		std::vector<RenderObject> EffectList;
		unsigned int NowEffectCount;
		unsigned int MaxEffectCount;

		std::vector<RenderObject> DebugObjectList;
		std::vector<RenderUIObejct> UIObjectList;

		// 프러스텀용
		DirectX::SimpleMath::Plane mPlane[6];

	public:
		RenderQue();
		~RenderQue();

		// 외부 공개 함수
	public:
		// 렌더큐에 렌더할 객체를 등록합니다.
		void AddRenderObejct(RenderObject& objectInfo);
		void AddAlphaObject(RenderObject& objectInfo);
		void AddEffectObject(RenderObject& objectInfo);
		void AddDebugObject(RenderObject& objectInfo);
		void AddUIObject(RenderUIObejct& objectInfo);

		unsigned int GetRenderObjectListSize() { return NowObjectCount; };
		unsigned int GetAlphaObjectListSize() { return NowAlphaCount; };
		unsigned int GetEffectListSize() { return NowEffectCount; };
		unsigned int GetDebugObjectListSize() { return (unsigned int)DebugObjectList.size(); };
		unsigned int GetUIObjectListSize() { return (unsigned int)UIObjectList.size(); };

		std::vector<RenderObject>& GetRenderObjectList() { return RenderObjectList; };
		std::vector<RenderObject>& GetAlphaObjectList() { return AlphaObjectList; };
		std::vector<RenderObject>& GetEffectList() { return EffectList; };
		std::vector<RenderObject>& GetDebugObjectList() { return DebugObjectList; };
		std::vector<RenderUIObejct>& GetUIObjectList() { return UIObjectList; };

		void ClearObjectRenderQue();		// 렌더큐를 비워줍니다.
		void ClearUIRenderQue();

		void CheckMaxSize();
		void CheckMaxSize_Alpha();
		void CheckMaxSize_Effect();

		// 렌더큐 정렬 함수
		void SortUIQue_LayerOrder();

		// 알파큐 정렬 함수
		void CalculateViewDistanceInAlphaQue(DirectX::SimpleMath::Vector3 eyepos);

		// 프로스텀 함수
		void ViewFrustum(float screenDepth, DirectX::SimpleMath::Matrix projMatrix, DirectX::SimpleMath::Matrix viewMatrix, bool bUI = false);
		bool CheckFrustum_Point(DirectX::SimpleMath::Vector3 vPos);

	private:

	

	};

}
