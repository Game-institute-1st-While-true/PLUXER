#ifndef _SPHERE_COLLISION_H
#define _SPHERE_COLLISION_H

// [2020/1/11 노종원]
// 구 콜리전
// 오브젝트의 충돌처리를 위한 컴포넌트

// LOG
// [2020/01/19] 노종원 : 상위 클래스를 만들어서 상속받도록 변경
// [2021/03/27] 노종원 : 복사 생성자 추가, 가상 복사 생성 함수 추가

#include "Collision.h"

namespace hos::com
{
	class SphereCollision : public Collision
	{
	public:
		static constexpr float DEFAULT_RADIUS = 0.5f;

	private:
		float Radius;

	public:
		SphereCollision(float radius = DEFAULT_RADIUS, PhysicMaterial* material = nullptr);
		virtual ~SphereCollision();

	protected:
		SphereCollision(const SphereCollision& dest);

	public:
		float GetRadius();
		void SetRadius(float radius);

	public:
		SphereCollision* Clone() const override;
		void Reset()override;
		void Awake()override;
		void OnEnable()override;
		void Start()override;
		void FixedUpdate()override;
		void PreRender()override;

		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view) override;
	};
}


#endif // !_SPHERE_COLLISION_H