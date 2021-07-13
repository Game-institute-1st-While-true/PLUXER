#ifndef _COLLISION_H
#define _COLLISION_H

// [2020/1/19 노종원]
// 물리 충돌을 위한 콜리전
// 오브젝트의 충돌처리를 위한 컴포넌트

// LOG
// [2021/03/27] 노종원 : 복사 생성자 추가

#include "Component.h"
#include "GameObject.h"
#include "PhysX.h"
#include "PhysicMaterial.h"

namespace hos
{
	namespace com
	{
		class Collision : public Component
		{
		protected:
			//콜리전의 크기가 0과 같거나 작을경우 생성에 실패한다 따라서 유효한 정밀도를 가지않는 소수점 7번째 자릿수로 0이 되지않도록 한다.
			static constexpr float CORRECTION_VALUE = 0.0000001f;

		protected:
			PhysicMaterial* Material;
			bool IsTrigger;
			physx::PxShape* Shape;
			Vector3 Center;
			U32 Layer;

		public:
			Collision(string_view name = L"Collision", PhysicMaterial* material = nullptr);
			virtual ~Collision();

		protected:
			Collision(const Collision& dest);

		public:
			PhysicMaterial* GetMaterial();
			bool GetIsTrigger();
			physx::PxShape* GetShape();
			U32 GetLayer()const;
			void SetLayer(U32 index);
			Vector3 GetCenter()const;
			virtual void SetCenter(Vector3 center);

			void SetMaterial(PhysicMaterial* material);
			void SetIsTrigger(bool b);

		public:
			void Bind(hos::com::GameObject* gameObject);
			//ICallbale
			void Reset()override;
			void Awake()override;
			void Start()override;
			void OnEnable()override;
			void OnDisable()override;
		};
	}
}

#endif // !_COLLISION_H