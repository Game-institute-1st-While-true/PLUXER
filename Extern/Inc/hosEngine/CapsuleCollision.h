#ifndef _CAPSULE_COLLISION_H
#define _CAPSULE_COLLISION_H

// [2020/1/11 노종원]
// 캡슐 콜리전
// 오브젝트의 충돌처리를 위한 컴포넌트

// LOG
// [2020/01/20] 노종원 : 상위 클래스를 만들어서 상속받도록 변경
// [2021/03/27] 노종원 : 복사 생성자 추가, 가상 복사 생성 함수 추가

#include "Collision.h"

namespace hos::com
{
	class CapsuleCollision : public Collision
	{
	public:
		enum eDirection
		{
			DIRECTION_X,
			DIRECTION_Y,
			DIRECTION_Z,

			NUM_DIRECTION
		};

	public:
		static constexpr float DEFAULT_RADIUS = 0.5f;
		static constexpr float DEFAULT_HEIGHT = 1.f;
		static constexpr eDirection DEFAULT_DIRECTION = DIRECTION_Y;

	private:
		float Radius;
		float HalfHeight;
		float Height;
		eDirection Direction;

	public:
		CapsuleCollision(float radius = DEFAULT_RADIUS, float height = DEFAULT_HEIGHT, eDirection dir = DIRECTION_Y, PhysicMaterial* material = nullptr);
		virtual ~CapsuleCollision();

	protected:
		CapsuleCollision(const CapsuleCollision& dest);

	public:
		float GetRadius()const;
		float GetHeight()const;
		eDirection GetDirection()const;

		void SetCenter(Vector3 center)override;
		void SetRadius(float radius);
		void SetHeight(float height);
		void SetDirection(eDirection dir);

	public:
		CapsuleCollision* Clone() const override;
		void Reset()override;
		void Awake()override;
		void OnEnable()override;
		void FixedUpdate()override;
		void PreRender()override;

		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view) override;
	};
}


#endif // !_CAPSULE_COLLISION_H