#ifndef _TRANSFORM_H
#define _TRANSFORM_H

// [2020/10/14 노종원]
// 게임 오브젝트의 위치를 나타내는 컴포넌트
// 위치, 이동, 회전, 하이어라키 상의 부모 자식관계등을 가지고있으며 게임오브젝트에 필수적으로 포함된다.

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/03/26] 노종원 : 복사 생성자 추가, 가상 복사 생성 함수 추가
#include "Component.h"

namespace hos::com
{
	class Transform : public Component
	{
	private:
		//최상위 부모 (본인이 최상위일경우 this)
		Transform* Root;
		//부모
		Transform* Parent;
		//자식들
		std::vector<Transform*> Childs;

		//방향백터
		Vector3 Forward;
		Vector3 Up;
		Vector3 Left;

		//월드 위치 (부모가 없는경우 Local)
		Vector3 Position;
		Rotator Rotation;
		Vector3 Scale;

		//로컬위치
		Vector3 LocalPosition;
		Rotator LocalRotation;
		Vector3 LocalScale;

		//Matrix
		Matrix TransformMatrix;
		Matrix LocalTransformMatrix;
	public:
		Transform();
		~Transform();
		
	protected:
		//Only Used GameObject Copy
		Transform(const Transform& dest);

	private:
		void UpdateRoot(Transform* root);
		void UpdateTransformMatrix();

	public:
		void SetActive(bool b) = delete;

		Transform* GetRoot() const;
		Transform* GetParent() const;
		void SetParent(Transform* transform);
		void AttachChild(Transform* child);
		void DetachChild(int index, Transform* newParent = nullptr);
		void DetachChild(Transform* child, Transform* newParent = nullptr);
		Transform* GetChild(int index) const;
		Transform* Find(string_view name) const;
		std::size_t GetChildCount() const;
		std::vector<Transform*>& GetChilds();

		void SetForward(Vector3 forward);
		void SetUp(Vector3 up);
		void SetLeft(Vector3 right);
		void SetPosition(Vector3 position);
		void SetRotation(Rotator rotation);
		void SetScale(Vector3 scale);
		void SetLocalPosition(Vector3 position);
		void SetLocalRotation(Rotator rotation);
		void SetLocalScale(Vector3 scale);
		void SetTransform(Vector3 position, Rotator rotation, Vector3 scale);
		void SetLocalTransform(Vector3 position, Rotator rotation, Vector3 scale);

		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetLeft() const;
		Vector3 GetPosition() const;
		Rotator GetRotation() const;
		Vector3 GetScale() const;
		Vector3 GetLocalPosition() const;
		Rotator GetLocalRotation() const;
		Vector3 GetLocalScale() const;

		Matrix GetLocalTransformMatrix() const;
		Matrix GetTransformMatrix() const;

		void Translate(Vector3 translation);
		void Rotate(Rotator rotate);

	public:
		void Bind(GameObject* gameObject) override;
		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view data) override;

	public:
		Transform* Clone() const override;
		void Reset() override;
		//ICallable
		void Awake() override {};
		void Start() override {};
		void FixedUpdate() override {};
		void OnTriggerEnter(Collision* ohter) override {};
		void OnTriggerStay(Collision* ohter) override {};
		void OnTriggerExit(Collision* ohter) override {};
		void OnCollisionEnter(Collision* ohter) override {};
		void OnCollisionStay(Collision* ohter) override {};
		void OnCollisionExit(Collision* ohter) override {};
		void Update() override {};
		void LateUpdate() override;
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
	};
}

#endif // !_TRANSFORM