// <Concept>
// Source Name		: FbxLoader.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.02.08

// <Explanation>
// FBX 파일을 Load하기 위한 Loader클래스.

// Working Log
// 2021.02.09.Hosik	: 생성.
// 2021.02.09.Hosik : Node단위 Import시 동적할당을 줄이기 위해 클래스 내에 임시 Data영역 생성.
// 2021.02.17.Hosik	: Material Load 추가.
// 2021.02.23.Hosil : Animation Load 추가
// [2021/03/23] 노종원 : GameObject Prefab 만드는 작업 기존 작업물은 남기고 추가로 만든다.
// [2021/04/05] 노종원 : 소멸자 수정

#pragma once

#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib, "libfbxsdk_x64D")
#pragma comment(lib, "libxml2_x64D")
#pragma comment(lib, "zlib_x64D")
#else
#pragma comment(lib, "libfbxsdk_x86D")
#pragma comment(lib, "libxml2_x86D")
#pragma comment(lib, "zlib_x86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib, "libfbxsdk_x64R")
#pragma comment(lib, "libxml2_x64R")
#pragma comment(lib, "zlib_x64R")
#else
#pragma comment(lib, "libfbxsdk_x86R")
#pragma comment(lib, "libxml2_x86R")
#pragma comment(lib, "zlib_x86R")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)

#include "fbxsdk.h"
#include <string>
#include <algorithm>
#include <unordered_map>

#include "ModelPrefab.h"
#include "Mesh.h"
#include "Material.h"

#include "AnimationBundle.h"
#include "AnimationClip.h"
#include "BoneAnimation.h"
#include "Keyframe.h"

#define MAX_FULL_VTX 3000000
#define MAX_FACE 1000000

namespace hos
{
	namespace com
	{
		class GameObject;
		class Transform;
	}

	class FbxLoader
	{
		struct BoneInfo
		{
			BYTE BoneIndices;
			float BoneWeight;

			bool operator < (const BoneInfo& rhs)
			{
				return (BoneWeight > rhs.BoneWeight);
			}
		};

		struct CtrlPoint
		{
			DirectX::SimpleMath::Vector3 vPos;
			std::vector<BoneInfo> BoneInfoList;
			hos::string BoneName;

			CtrlPoint()
			{
				BoneInfoList.reserve(4);
			}

			void SortBlendingInfoByWeight()
			{
				std::sort(BoneInfoList.begin(), BoneInfoList.end());
			}
		};

	private:
		std::unordered_map<unsigned int, CtrlPoint*> TempControlPoints;

		// 클래스들 주소
		std::unordered_map<hos::string, ModelPrefab*>& ModelListRef;
		std::unordered_map<hos::string, Mesh*>& MeshListRef;
		std::unordered_map<hos::string, AnimationBundle*>& AnimationBundleListRef;
		std::unordered_map<hos::string, Material*>& MaterialListRef;
		std::unordered_map<hos::string, bool>& TextureListRef;
		std::unordered_map<hos::string, BoneAnimation*>& UIAnimationRef;

		// Fbx SDK
		FbxManager* Manager;
		FbxIOSettings* IosSetting;
		FbxImporter* Importer;

		// Loading Info
		hos::string FileRoot;
		hos::string FileName;
		hos::string ModelName;
		hos::string ClipName;
		bool bAnimation;
		unsigned int nowBoneCount;

		// Temp Data
		ModelPrefab* TempModel;
		AnimationBundle* BundleRef;
		AnimationClip* TempClip;

		////////////////////////////////////////////////////////////////////////////////
		// 매 노드를 Import할 때마다 동적할당을 하지 않기 위해 미리 공간을 만들어
		// ObjectPool과 비슷한 효과를 얻기를 기대함.
		// 추후 정점 Weld를 하기 위해서는 임시 데이터를 만들어야 용이하므로,
		// 미리 임시데이터를 만들어 운용해보는 방향으로 제작하고 싶었다.
		////////////////////////////////////////////////////////////////////////////////

		// 간소화된 버텍스 리스트(중복되는 버텍스 제거)
		unsigned int IndexVtxCount;
		//NodeData::INDEXED_VTX tempIndexedVtxList[MAX_INDEXED_VTX];

		// 버텍스의 인덱스 리스트
		/// (간소화된 버텍스 리스트와 해당 인덱스를 사용해서 풀버텍스를 만듭니다.)
		unsigned int FaceCount;
		hos::cg::VertexDataType::FACE_INDEX* tempFaceList;

		// 실질적 데이터로 채워진 풀버텍스 리스트
		unsigned int FullVtxCount;
		hos::cg::VertexDataType::FULL_VTX* tempFullVtxList;

		////////////////////////////////////////////////////////////////////////////////


		// 생성자 소멸자
	public:
		FbxLoader(
			std::unordered_map<hos::string, ModelPrefab*>& modelList,
			std::unordered_map<hos::string, Mesh*>& meshList,
			std::unordered_map<hos::string, AnimationBundle*>& aniBundleList,
			std::unordered_map <hos::string, Material*>& materialList,
			std::unordered_map<hos::string, bool>& TextureList,
			std::unordered_map <hos::string, BoneAnimation*>& uiAnimationRef
		);
		~FbxLoader();

		// 외부 공개 함수
	public:
		hos::ModelPrefab* LoadFbxFile(hos::mbstring_view fileRoot);
		hos::BoneAnimation* LoadFbxFile_UIAnimation(hos::mbstring_view fileroot);

		// 내부 사용 함수
	private:
		// Data 획득 함수
		bool LoadFbxData(FbxScene* fbxScene, FbxNode* fbxRootNode, bool onlyAnimation);
		bool LoadFbxData_UIAnimation(FbxScene* fbxScene, FbxNode* fbxRootNode, BoneAnimation* animationData);

		void CreateBoneDataAndAniClip(bool onlyAnimation, FbxNode* fbxRootNode);
		void CorrectBoneDataCount(bool onlyAnimation);

		// Transform 정보 획득 및 메쉬-본 생성
		void CreateMeshData(FbxMesh* fbxMesh, Node* nodeData);

		// 정점 데이터 획득 관련 함수
		void MakeVertexList(FbxMesh* fbxMesh);
		void CreateControlPoints(FbxNode* fbxNode, FbxMesh* fbxMesh, Mesh* mesh);

		void MakeFullVertexList(const FbxMesh* fbxMesh, int controlPointIndex, int vertexCounter);
		void ReadNormal(const FbxMesh* fbxMesh, int controlPointIndex, int vertexCounter);
		void ReadBiNormal(const FbxMesh* fbxMesh, int controlPointIndex, int vertexCounter);
		void ReadTangent(const FbxMesh* fbxMesh, int controlPointIndex, int vertexCounter);
		void ReadUV(const FbxMesh* fbxMesh, int fullVtxCount, int index);

		void CopyVtxDataToMesh(Mesh* meshData);

		// Material
		void GetMaterialData(FbxNode* fbxNode, Mesh* nodeData, bool onlyAnimation);
		void GetMaterialAttribute(FbxSurfaceMaterial* fbxMaterial, Material* materialData);
		void GetMaterialTexture(FbxSurfaceMaterial* fbxMaterial, Material* materialData);

		public:
		void CreateMaterialRenderResource(Material* materialData);

		private:

		// BoneAnimation
		FbxAMatrix GetGeometryTransformation(FbxNode* fbxNode);

		// 기반작업용 함수
		void ClearTempData();				// 임시 저장소를 비워줍니다(새 노드를 불러줄 준비)
		bool ReadFileName(hos::mbstring_view fileRoot);
		float DegreeToRadian(double degree);	// 각도 to 라디안
		std::wstring GetFileName(std::string string);

		void ConvertFbxMatrixToD3DMatrix(DirectX::SimpleMath::Matrix& dest, FbxAMatrix data);
		void ConvertFbxMatrixToD3DMatrix_Direct(DirectX::SimpleMath::Matrix& dest, FbxAMatrix data);
		void ConvertFbxVector4ToSimpleMathVector3(DirectX::SimpleMath::Vector3& dest, FbxVector4 data, float mutipleValue = 1.0f);
		void ConvertFbxQuatToSimpleMathVector4(DirectX::SimpleMath::Vector4& dest, FbxQuaternion data);


		// 개선용
		void LoadNodeData(FbxScene* fbxScene, FbxNode* fbxNode, Node* parentsNode, bool onlyAnimation);
		void LoadMeshData(FbxScene* fbxScene, FbxNode* fbxNode, FbxMesh* fbxMesh, Node* parentsNode, bool onlyAnimation);
		void LoadBoneData(FbxScene* fbxScene, FbxNode* fbxNode, Node* parentsNode, bool onlyAnimation);

		void GetAnimation(FbxScene* fbxScene, FbxNode* fbxNode, Node* nodeData, bool onlyAnimation);

		void GetBoneAnimation(FbxScene* fbxScene, FbxNode* fbxNode, Node* nodeData);
		void GetSkinningAnimation(FbxScene* fbxScene, FbxNode* fbxNode, FbxMesh* fbxMesh, Node* nodeData, I32 deformerCount, bool onlyAnimation);
		void GetUIAnimation(FbxScene* fbxScene, FbxNode* fbxNode, BoneAnimation* animationData);

		void GetAllBone(FbxNode* fbxNode);

		void GetSkinningBBox(FbxScene* fbxScene);
	};

}

