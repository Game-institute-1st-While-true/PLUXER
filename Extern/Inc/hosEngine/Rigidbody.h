#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

// [2020/12/10 노종원]
// 리기드바디
// 오브젝트의 물리연산을 위한 컴포넌트
// 임시 선언만 해놓는다

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/03/27] 노종원 : 복사 생성자 추가, 가상 복사 생성 함수 추가

#include "Component.h"
#include "PhysX.h"

namespace hos::com
{
	class Rigidbody : public Component
	{
	public:
		static constexpr float DEFAULT_MASS = 1.f;
		static constexpr float DEFAULT_DRAG = 0.f;
		static constexpr float DEFAULT_ANGULAR_DRAG = 0.005f;
		static constexpr bool DEFAULT_USE_GRAVITY = true;
		static constexpr bool DEFAULT_IS_KINEMATIC = false;
		static constexpr bool DEFAULT_FREEZE = false;

		enum eForceMode
		{
			FORCE_MODE_FORCE = physx::PxForceMode::eFORCE,
			FORCE_MODE_IMPULSE = physx::PxForceMode::eIMPULSE,
			FORCE_MODE_VELOCITYCHANGE = physx::PxForceMode::eVELOCITY_CHANGE,
			FORCE_MODE_ACCELERATION = physx::PxForceMode::eACCELERATION,

			NUM_FORCE_MODE
		};
	private:
		//physx의 리기드바디 GameObject의 PhisicsActor의 PxRigidDynamic포인터
		physx::PxRigidDynamic* Actor;

		//질량 [ 0 ~ F32_MAX ]
		float Mass;
		//저항 (선형감쇠) [ 0 ~ F32_MAX ]
		float Drag;
		//회전 저항 (회전 감쇠 계수) [ 0 ~ F32_MAX ]
		float AngularDrag;
		//중력 작용 여부 setActorFalg() - eDISABLE_GRAVITY
		bool UseGravity;
		//물리시뮬레이션 여부 setRigidBodyFlag() - eKINEMATIC, setKinematicTarget()
		bool IsKinematic;

		//각각 위치, 회전 축 잠금 여부
		bool FreezePositionX, FreezePositionY, FreezePositionZ;
		bool FreezeRotationX, FreezeRotationY, FreezeRotationZ;

	public:
		Rigidbody();
		virtual ~Rigidbody();

	protected:
		Rigidbody(const Rigidbody& dest);

	public:
		float GetMass()const;
		float GetDrag()const;
		float GetAngularDrag()const;
		bool GetUseGravity()const;
		bool GetIsKinematic()const;
		bool GetFreezePositionX()const;
		bool GetFreezePositionY()const;
		bool GetFreezePositionZ()const;
		bool GetFreezeRotationX()const;
		bool GetFreezeRotationY()const;
		bool GetFreezeRotationZ()const;

		void SetMass(float mass);
		void SetDrag(float drag);
		void SetAngularDrag(float angularDrag);
		void SetUseGravity(bool b);
		void SetIsKinematic(bool b);
		void SetFreezePositionX(bool b);
		void SetFreezePositionY(bool b);
		void SetFreezePositionZ(bool b);
		void SetFreezeRotationX(bool b);
		void SetFreezeRotationY(bool b);
		void SetFreezeRotationZ(bool b);

		void AddForce(Vector3 force, eForceMode mode = FORCE_MODE_FORCE);
		void AddTorque(Vector3 torque, eForceMode mode = FORCE_MODE_FORCE);
		void ClearForce();
		void ClearTorque();

	public:
		void Bind(GameObject* gameObject) override;
		void UnBind(int index) override;
		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view) override;

		void SetActive(bool) = delete;

		Rigidbody* Clone()const override;
		void Reset()override;
		void Awake()override;
		void OnEnable()override;
		void OnDisable()override;
	};
}

#endif // !_RIGIDBODY_H