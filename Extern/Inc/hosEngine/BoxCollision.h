#ifndef _BOX_COLLISION_H
#define _BOX_COLLISION_H

// [2020/12/10 노종원]
// 박스 콜리전
// 오브젝트의 충돌처리를 위한 컴포넌트

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2020/01/20] 노종원 : 상위 클래스를 만들어서 상속받도록 변경
// [2021/03/27] 노종원 : 복사 생성자 추가, 가상 복사 생성 함수 추가, Awake 버그수정

#include "Collision.h"

namespace hos::com
{
	class BoxCollision : public Collision
	{
	public:
		static constexpr float DEFAULT_SIZE = 1.0f;

	private:
		Vector3 Size;

	public:
		BoxCollision(float x = DEFAULT_SIZE, float y = DEFAULT_SIZE, float z = DEFAULT_SIZE, PhysicMaterial* material = nullptr);
		virtual ~BoxCollision();

	protected:
		BoxCollision(const BoxCollision& dest);

	public:
		Vector3 GetSize()const;
		float GetSizeX()const;
		float GetSizeY()const;
		float GetSizeZ()const;

		void SetSizeX(float x);
		void SetSizeY(float y);
		void SetSizeZ(float z);
		void SetSize(Vector3 size);

	public:
		BoxCollision* Clone() const override;
		void Reset()override;
		void Awake()override;
		void OnEnable()override;
		void FixedUpdate()override;

		void PreRender()override;

		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view) override;
	};
}

#endif // !_BOX_COLLISION_H