#ifndef _DATA_MANAGER_H
#define _DATA_MANAGER_H

// [2021/03/12 노종원]
// 게임의 자원을 관리하는 메니저 클래스

// LOG
// [2021/04/03] 노종원 : Mesh Save/Load, Get함수 추가
// [2021/04/05] 노종원 : Materila Save/Load, Get함수 추가
// [2021/04/16] 김효곤 : GetScene bulidIndex 추가

#include <map>
#include "hosUtility\hosUtility.h"

// 그래픽 렌더용
#include <unordered_map>
#include "FbxLoader.h"
#include "ModelPrefab.h"
#include "Mesh.h"
#include "Bone.h"
#include "Material.h"
#include "AnimationBundle.h"
#include "FontData.h"

extern bool bDebug;
extern bool bClient;

namespace hos
{
	namespace com
	{
		class Scene;
		class GameObject;
	}
	class AudioClip;
	class PhysicMaterial;
}

namespace hos::com
{
	class DataManager
	{
		static constexpr U32 MAX_FILE_EXETENSION_LENGTH = 11;
	public:
		hos::FbxLoader* FbxLoaderInstance;

		std::map<string, com::Scene*> Scenes;
		std::map<string, com::GameObject*> Prefabs;		
		std::map<string, AudioClip*> AudioClips;
		std::map<string, PhysicMaterial*> PhysicMaterials;
		std::map<string, FontData*> Fonts;
		std::map<string, HCURSOR> MouseCursors;

		std::unordered_map<hos::string, hos::ModelPrefab*> ModelList;
		std::unordered_map<hos::string, hos::Mesh*> MeshList;
		std::unordered_map<hos::string, hos::Material*> MaterialList;
		std::unordered_map<hos::string, hos::AnimationBundle*> AnimationList;
		std::unordered_map<hos::string, hos::BoneAnimation*> UIAnimationList;

		std::unordered_map<hos::string, bool> TextureList;

		std::vector<hos::string> SkyBoxList;

		// 기본 마테리얼(Renderer Component 생성 시 기본으로 붙는 Material)
		Material* DefaultMaterial;

		string FBX_Material_Path;
	public:
		DataManager();
		~DataManager();

		void SaveAll(string_view path);

		static bool SaveScene(string_view path, Scene* scene);
		Scene* LoadScene(string_view pathName);
		Scene* GetScene(string_view name) const;
		int GetSceneBulidIndex(string_view name) const;

		Scene* GetScene(U32 index)const;
		bool RemoveScene(U32 index);
		bool RemoveScene(string_view name);
		//Testbed 전용
		bool AddScene(Scene* scene);

		static bool SavePrefab(string_view path, GameObject* prefab);
		GameObject* LoadPrefab(string_view pathName);
		GameObject* GetPrefab(string_view name) const;

		static bool SaveAudioClip(string_view path, AudioClip* clip);
		AudioClip* LoadAudioClip(string_view pathName);
		AudioClip* GetAudioClip(string_view name) const;

		static bool SavePhysicMaterial(string_view path, PhysicMaterial* material);
		PhysicMaterial* LoadPhysicMaterial(string_view pathName);
		PhysicMaterial* GetPhysicMaterial(string_view name);

		static bool SaveFont(string_view path, FontData* font);
		FontData* LoadFont(string_view pathName);
		FontData* GetFont(string_view name);

		static bool SaveMesh(string_view path, Mesh* mesh);
		Mesh* LoadMesh(string_view pathName);
		Mesh* GetMesh(string_view name);

		static bool SaveAnimationBundle(string_view path, AnimationBundle* bundle);
		AnimationBundle* LoadAnimationBundle(string_view pathName);
		AnimationBundle* GetAnimationBundle(string_view name);

		static bool SaveMaterial(string_view path, Material* material);
		Material* LoadMaterial(string_view pathName);

		static bool SaveUIAnimation(string_view pathName, BoneAnimation* anim);
		BoneAnimation* LoadUIAnimation(string_view pathName);

		void LoadData(string_view pathName);
		void LoadAssets();
		//void LoadFbxFile_StaticModel(const CHAR* fileRoot);
		void LoadFbxFile(const CHAR* fileRoot);
		BoneAnimation* LoadUIFile(hos::string_view fileRoot);

		// Render Variable(ex. DirectX11 or Vulkan)
		//void CreateALLRenderVariable_InDataManager();	// 데이터 매니저 내부의 모든 리소스를 만들어줌.
		void ReleaseAllRenderVariable_InDataManager();

		// Get Data
		Material* GetDefaultMaterial() const { return DefaultMaterial; };
		Material* GetMaterial(hos::string materialName);// { return MaterialList.at(materialName); };

		BoneAnimation* GetUIAnimation(hos::string_view aniName);

		// 텍스쳐 생성(Load), 제거
		void LoadTexture(hos::string texName);
		void LoadTextureOnly(hos::string filePath);
		void ReleaseTexture(hos::string textureName);
		
		bool HasTexture(string_view name);
		bool IsAlpha(string_view name);

		// SkyBox
		void LoadSkyBox(hos::string filePath);
		bool HasSkyBox(string_view name);

		HCURSOR LoadMouseCursor(string pathName);
		HCURSOR GetMouseCursor(string name);
	private:
		void FBXMakeHierarchyFromBone(Node* parent, GameObject* parentObj);

		void _FBXMakeHierarchyFromBone(ModelPrefab* fbx, Node* parent, GameObject* parentObj);

		//void CreateMeshVariable(hos::string meshName);
		//void ReleaseMeshVariable(hos::string meshName);

		//void CreateTextureVariable(hos::string materialName);
	};
}

#endif // !_DATA_MANAGER_H