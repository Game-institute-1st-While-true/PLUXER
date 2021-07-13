#pragma once
#include "GroupSystem.h"
#include "Collider2D.h"

#include <vector>
// 그냥 컴포넌트로 생성하지 않는다.


class MapCollisionChecker
{
private:
	std::vector<collider2::Rect*> RectPool;

public:
	MapCollisionChecker();
	virtual ~MapCollisionChecker();

public:
	void AddBoxCollision(hos::com::BoxCollision* box, hos::com::Transform* transform);
	void AddBoxCollision(hos::com::BoxCollision* box);

	void RectPoolClear();
	bool CheckCollisionMap(const collider2::Rect& rt);
	bool CheckCollisionMap(DirectX::SimpleMath::Vector2 pt);
};
