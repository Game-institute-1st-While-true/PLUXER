#ifndef _SCENE_H
#define _SCENE_H

// [2020/10/16 노종원]
// 게임의 씬

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/02/03] 박용의 : 해당 오브젝트 삭제 시 하위 오브젝트 삭제 추가
// [2021/02/22] 노종원 : PhysX Scene 추가
// [2021/03/12] 노종원 : Path 지정에 의한 Save/Load 변경
// [2021/04/14] 노종원 : DataManager에 의해 사용하지않게된 Save/Load 제거, Layer추가

#include <map>
#include "Object.h"
#include "../GzzNet/Clock.h"

namespace hos::com
{
	class Component;
	class Transform;
	class Camera;
	class Collision;
	class BoxCollision;
	class CapsuleCollision;
	class SphereCollision;
	class MeshCollision;
	class Rigidbody;
	class Script;
	class AudioListener;
	class Networkcomponent;
	class MeshFilter;
	class MeshRenderer;
	class SkinnedMeshRenderer;
	class UIImage;
	class UIText;
	class NavAgent;
	class Animation;
	class UIButton;
	class UIInputField;
	class CollisionCallback;
	class GameObject;
	using Clock = gzz::Clock;
	class RaycastInfinityCallBack;

	enum class eRaycastMode
	{
		CLOSEST,
		ANY,
		MULTIPLE
	};

	enum class eRaycastType
	{
		All,
		DYNAMIC,
		STATIC
	};

	class Scene : public ICallable
	{
		friend class SceneManager;
		friend class GameObject;
		friend class Transform;
		friend class Rigidbody;
		friend class Collision;
		friend class RaycastInfinityCallBack;

	public:
		enum { VALID = -1 };
		static constexpr string_literal FILE_EXTENSION = L".scene";
		static constexpr string_literal FILE_PATH = L"\\Assets\\Scenes\\";

		inline static const Vector3 DEFAULT_GRAVITY = Vector3(0.f, -9.81f, 0.f);
		static constexpr float DEFAULT_SIMULATE_TIME = 1.f / 60.f;
		static constexpr int DEFAULT_SIMULATE_TIME_MILLI = 16;
		static constexpr int MAX_LAYER = 32;
		static constexpr int BUILTIN_LAYER_COUNT = 8;
		static constexpr int DEFAULT_LAYER = 0;
	private:
		string Name;
		bool IsLoaded;
		int BuildIndex;

		std::vector<GameObject*> GameObjects;
		std::vector<GameObject*> RootGameObjects;
		std::multimap<int, Component*> ScriptExecution;

		AudioListener* Listener;
		Camera* MainCamera;

		//PhysX
		physx::PxScene* PxScene;
		CollisionCallback* PxSceneCallback;
		Vector3 Gravity;

		bool Layer[MAX_LAYER][MAX_LAYER];
		string LayerName[MAX_LAYER];

	public:
		Clock* SceneTime;

	public:
		explicit Scene();
		explicit Scene(string_view pathName);
		Scene(const Scene& dest);
		Scene(Scene&&) = delete;
		virtual ~Scene();

	private:
		string ObjectNameDuplicateCheck(string_view name);
	public:
		void ExecutionOrderRebuild();
		void AddCollision(Collision* collision);
		void RemoveCollision(Collision* collision);

		const std::vector<U8> Serialize();
		bool Deserialize(mbstring_view data);

		Clock* GetTimer() { return SceneTime; }
		string GetName();
		void SetName(string_view name);
		bool GetIsLoaded();
		void SetIsLoaded(bool b);
		int GetBuildIndex();
		void SetBuildIndex(int index);
		bool IsValid();
		void AddGameObject(GameObject* obj, GameObject* parent = nullptr);
		void AddGameObjectRecursive(GameObject* obj, GameObject* rootParent = nullptr);
		void RemoveGameObject(GameObject* obj);
		void RemoveRootGameObject(GameObject* obj);
		GameObject* FindGameObjectWithName(string name);
		std::vector<GameObject*>& GetObjects();
		std::vector<GameObject*>& GetRoots();
		unsigned long long GetHashCode();
		void SetLayerName(U32 Layer, string_view name);
		string GetLayerName(U32 Layer)const;
		void SetLayerCollision(U32 Layer1, U32 Layer2, bool enable);
		bool GetLayerCollision(U32 Layer1, U32 Layer2)const;
		Vector3 GetGravity()const;
		void SetGravity(Vector3 gravity);

		void PhysicsUpdate();

		void SetAudioListener(AudioListener* listener);
		AudioListener* GetAudioListener();
		void SetMainCamera(Camera* cam);
		Camera* GetMainCamera() const;

		bool RayCast(Vector3 pos, Vector3 dir, float distance, eRaycastMode mode = eRaycastMode::CLOSEST, eRaycastType type = eRaycastType::All, U32 num = 2);
		std::vector<const physx::PxRaycastHit*> RayCastAll(Vector3 pos, Vector3 dir, eRaycastType type = eRaycastType::All);

		//ICallable
		void Awake() override;
		void Start() override;
		void FixedUpdate() override;
		void OnTriggerEnter(Collision* other) override;
		void OnTriggerStay(Collision* other) override;
		void OnTriggerExit(Collision* other) override;
		void OnCollisionEnter(Collision* other) override;
		void OnCollisionStay(Collision* other) override;
		void OnCollisionExit(Collision* other) override;
		void Update() override;
		void LateUpdate() override;
		void OnStateMachine() override;
		void OnAnimation() override;
		void PreRender() override;
		void Render() override;
		void PostRender() override;
		void RenderImage() override;
		void OnGUI(float aspect_x, float aspect_y) override;
		void OnPause() override;
		void OnClose() override;
		void OnQuit() override;
		void OnDestory() override;
		void OnEnable() override {};
		void OnDisable() override;
	};

	__forceinline static bool operator==(Scene& lhs, Scene& rhs)
	{
		if (lhs.GetHashCode() == rhs.GetHashCode())
		{
			return true;
		}

		return false;
	}
	__forceinline static bool operator!=(Scene& lhs, Scene& rhs)
	{
		if (lhs.GetHashCode() == rhs.GetHashCode())
		{
			return false;
		}

		return true;
	}

	class RaycastInfinityCallBack :public physx::PxRaycastCallback
	{
	private:
		Scene* m_Scene;

	public:
		std::vector<const physx::PxRaycastHit*> Collisions;

	public:
		RaycastInfinityCallBack(Scene* scene);
		~RaycastInfinityCallBack();

		virtual physx::PxAgain processTouches(const physx::PxRaycastHit* buffer, physx::PxU32 nbHits);
		virtual void finalizeQuery();
	};

	class CollisionCallback :public physx::PxSimulationEventCallback
	{
		friend Scene;
		friend RaycastInfinityCallBack;
	private:
		std::map<physx::PxShape*, Collision*> Collisions;
	public:
		CollisionCallback();
		~CollisionCallback();

		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {}
		void onWake(physx::PxActor** actors, physx::PxU32 count) {}
		void onSleep(physx::PxActor** actors, physx::PxU32 count) {}
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {}
	};
}

#endif // !_SCENE_H