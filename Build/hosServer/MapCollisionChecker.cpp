#include "MapCollisionChecker.h"

MapCollisionChecker::MapCollisionChecker()
{
	RectPool.reserve(10);
}

MapCollisionChecker::~MapCollisionChecker()
{
	int size = RectPool.size();
	for (int i = 0; i < size; i++)
	{
		hos::SafeDelete(RectPool[i]);
	}
}

void MapCollisionChecker::AddBoxCollision(hos::com::BoxCollision* box, hos::com::Transform* transform)
{
	// box collision 성분을 rect 성분으로 바꾸자. 1
	using namespace collider2;

	DirectX::SimpleMath::Matrix matrix = hos::Matrix::CreateScale(box->GetSize()) * transform->GetTransformMatrix() * hos::Matrix::CreateTranslation(box->GetCenter());
	Rect* obj = new Rect();
	obj->Top = DirectX::SimpleMath::Vector2::Transform(obj->Top, matrix);
	obj->Bottom = DirectX::SimpleMath::Vector2::Transform(obj->Bottom, matrix);
	RectPool.push_back(obj);
}

void MapCollisionChecker::AddBoxCollision(hos::com::BoxCollision* box)
{
	// box collision 성분을 rect 성분으로 바꾸자. 2
	using namespace DirectX::SimpleMath;
	using namespace collider2;

	Matrix matrix = Matrix::CreateScale(box->GetSize()) * box->m_GameObject->transform->GetTransformMatrix() * Matrix::CreateTranslation(box->GetCenter());

	Rect* obj = new Rect();

	hos::Vector3 trans;

	trans = Vector3::Transform(Vector3(obj->Top.x, 0, obj->Top.y), matrix);
	obj->Top.x = trans.x;
	obj->Top.y = trans.z;

	trans = Vector3::Transform(Vector3(obj->Bottom.x, 0, obj->Bottom.y), matrix);
	obj->Bottom.x = trans.x;
	obj->Bottom.y = trans.z;

	RectPool.push_back(obj);
}

void MapCollisionChecker::RectPoolClear()
{
	int size = RectPool.size();
	for (int i = 0; i < size; i++)
	{
		hos::SafeDelete(RectPool[i]);
	}
	RectPool.clear();
}

bool MapCollisionChecker::CheckCollisionMap(const collider2::Rect& rt)
{
	using namespace collider2;

	if(collider2::AABB(*RectPool[0], rt) ||
		collider2::AABB(*RectPool[1], rt) ||
		collider2::AABB(*RectPool[2], rt) ||
		collider2::AABB(*RectPool[3], rt))
	{
		return true;
	}
	return false;
}

bool MapCollisionChecker::CheckCollisionMap(DirectX::SimpleMath::Vector2 pt)
{
	using namespace collider2;

	if (collider2::RECT_POINT(*RectPool[0], pt) ||
		collider2::RECT_POINT(*RectPool[1], pt) ||
		collider2::RECT_POINT(*RectPool[2], pt) ||
		collider2::RECT_POINT(*RectPool[3], pt))
	{
		return true;
	}
	return false;
}
