#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

// [2020/10/14 노종원]
// 게임에 사용하는 기본 오브젝트
// 오브젝트를 상속 받으면서 컴포넌트들을 포함하고 있다.

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/03/15] 노종원 : FILE_EXTENSION 추가
// [2021/03/15] 신호식 : 애니메이션 인스턴싱을 위해 boneTag(가칭)을 추가. 앞으로 부모의 이름으로 Animation을 찾을 땐 해당 Tag를 사용합니다.
//						boneTag를 활용해서 Object를 찾기위해 FindObject의 boneTag 버전 추가.
// [2021/04/03] 노종원 : Renderer 중복 방지 추가
// [2021/04/13] 노종원 : Tag추가, 
//					FineObject 제거
//					FineObjectRecursive->FineObject 이름변경
//					FindObjectRecursivebyTag->FindObjectByBoneTag 이름변경
//					Get/Set Tag추가
//					FindObjectByTag 추가

#include <typeinfo>
#include <typeindex>
#include <initializer_list>
#include "Object.h"
#include "PhysX.h"
#include "Scene.h"
#include "transform.h"

namespace hos
{
	class ScriptFactory;
}

namespace hos::com
{
	class Component;
	class Scene;

	class Transform;
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

	class GameObject : public Object
	{
	private:
		friend class Component;
		friend class Scene;

		friend class Transform;
		friend class Collision;
		friend class BoxCollision;
		friend class CapsuleCollision;
		friend class SphereCollision;
		friend class MeshCollision;
		friend class NavAgent;
		friend class Animation;

		friend class ScriptFactory;

	public:
		static constexpr string_literal FBX_FILE = L".fbx";
		static constexpr string_literal FILE_EXTENSION = L".prefab";

	public:
		Scene* m_Scene;

		Transform* transform;
		Rigidbody* rigidbody;
		MeshRenderer* meshrenderer;
		SkinnedMeshRenderer* skinnedmeshrenderer;
		MeshFilter* meshfilter;
		AudioListener* listener;
		Networkcomponent* network;
		UIImage* uiimage;
		UIText* uitext;
		UIButton* uibutton;
		UIInputField* uiinputfield;

		hos::string boneTag;
		bool IsActiveRecursive;
		string Tag;
		mutable U32 Layer;

		//Internal
		bool ActiveEventDuplicatedCheck = true;

	private:
		//보유한 컴포넌트들
		std::vector<Component*> Components;

	public:
		physx::PxRigidActor* PhysicsActor;

	public:
		GameObject();
		GameObject(string_view name);
		GameObject(string_view name, std::initializer_list<Component*> list);
		GameObject(const GameObject& dest);
		virtual ~GameObject();

	public:
		Component* AddComponent(Component* component);
		void RemoveComponent(Component* component);
		U32 FindComponentIndex(Component* component);
		void SetName(string_view _name);
		std::vector<Component*>& GetComponents();
		string GetTag()const;
		void SetTag(string_view tag);
		U32 GetLayer()const;
		void SetLayer(U32 index);

		//Transform
		GameObject* GetRoot() const;
		GameObject* GetParent() const;
		GameObject* FindObject(string_view name) const;
		GameObject* FineObjectByTag(string_view tag) const;

	protected:
		void Bind(Scene* scene);
		void UnBind(int index);

	private:
		void SetActiveRecursive(bool b);
		GameObject* FindObjectByBoneTag(string_view name) const;
	public:
		bool GetActive()const;	//overload
		void SetActive(bool b);

		//ICallable
		void Awake() override {};
		void Start() override {};
		void FixedUpdate() override {};
		void OnTriggerEnter(Collision* other) override;
		void OnTriggerStay(Collision* other) override;
		void OnTriggerExit(Collision* other) override;
		void OnCollisionEnter(Collision* other) override;
		void OnCollisionStay(Collision* other) override;
		void OnCollisionExit(Collision* other) override;
		void Update() override {};
		void LateUpdate() override {};
		void OnStateMachine() override {};
		void OnAnimation() override {};
		void PreRender() override {};
		void Render() override {};
		void PostRender() override {};
		void RenderImage() override {};
		void OnGUI(float aspect_x, float aspect_y) override {};
		void OnPause() override {};
		void OnClose() override {};
		void OnQuit() override {};
		void OnDestory() override {};
		void OnEnable() override;
		void OnDisable() override;

		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view) override;

	public:
		template<typename T>
		T* AddComponent()
		{
			T* _tmp = new T();
			Component* component = dynamic_cast<Component*>(_tmp);
			if (component)
			{
				try
				{
					component->Bind(this);
				}
				catch (std::invalid_argument e)
				{
					return nullptr;
				}
				Components.push_back(component);

				if (m_Scene)
				{
					m_Scene->ScriptExecution.insert({ component->ExecutionOrder, component });
				}

				return dynamic_cast<T*>(component);
			}
			return nullptr;
		}

		template<typename T>
		T* GetComponent()
		{
			for (Component* _com : Components)
			{
				T* tmp = dynamic_cast<T*>(_com);
				if (tmp)
				{
					return tmp;
				}
			}

			return nullptr;
		}

		template<typename T>
		std::vector<T*> GetComponents()
		{
			std::vector<T*> components;
			T* tmp = nullptr;
			for (Component* _com : Components)
			{
				tmp = dynamic_cast<T*>(_com);
				if (tmp)
				{
					components.emplace_back(tmp);
				}
			}
			return components;
		}

		template<typename T>
		T* GetComponentInChildren()
		{
			T* find = nullptr;
			if (transform)
			{
				for (Transform* child : transform->GetChilds())
				{
					if (nullptr == child->m_GameObject)
					{
						continue;
					}

					for (Component* _com : child->m_GameObject->Components)
					{
						find = dynamic_cast<T*>(_com);
						if (find)
						{
							return find;
						}
					}

					find = GetComponentInChildren<T>();
					if (find)
					{
						return find;
					}
				}
			}
			return nullptr;
		}

		template<typename T>
		void RemoveComponent()
		{
			int index = 0;
			for (auto it = Components.begin(); it != Components.end(); ++it, index++)
			{
				Component* component = dynamic_cast<T*>(*it);
				if (component)
				{
					if (m_Scene)
					{
						for (auto it = m_Scene->ScriptExecution.begin(); it != m_Scene->ScriptExecution.end();)
						{
							if (it->second == component)
							{
								m_Scene->ScriptExecution.erase(it);
								break;
							}
							else
							{
								++it;
							}
						}
					}

					component->UnBind(index);
					Components.erase(it);
					return;
				}
			}
		}
	};
}

#endif // !_GAME_OBJECT_H