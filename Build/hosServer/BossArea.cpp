#include "BossArea.h"

BossArea::BossArea(BossFrog* boss) :
	m_Boss(boss), OnJumpCollision(false), OnTongueCollision(false), RushAble(false), JumpAble(false)
{
}

BossArea::~BossArea()
{
}

bool BossArea::CheckTongue(const collider2::Rect& rt)
{
	return false;
}

bool BossArea::CheckJump(const collider2::Rect& rt)
{
	return false;
}

bool BossArea::CheckRay(const collider2::Rect& rt)
{
	return false;
}

bool BossArea::CheckDetection(const collider2::Rect& rt)
{
	return false;
}

int BossArea::CheckTongue(const Player* player)
{
	using namespace DirectX::SimpleMath;
	using namespace collider2;

	Vector3 player_pos;
	{
		std::scoped_lock<std::mutex> lock(player->Move->PositionMutex);
		player_pos = player->m_GameObject->transform->GetPosition();
	}

	Circle player_cir;

	player_cir.Center.x = player_pos.x;
	player_cir.Center.y = player_pos.z;

	player_cir.Radius = 0.5;

	Circle boss_cir;

	boss_cir.Center.x = TongueArea.Center.x + m_Boss->m_GameObject->transform->GetPosition().x;
	boss_cir.Center.y = TongueArea.Center.y + m_Boss->m_GameObject->transform->GetPosition().z;

	boss_cir.Radius = TongueArea.Radius;

	if (CC(boss_cir, player_cir))
	{
		Vector2 pt1(m_Boss->m_GameObject->transform->GetForward().x, m_Boss->m_GameObject->transform->GetForward().z);
		Vector2 pt2(player_pos.x - m_Boss->m_GameObject->transform->GetPosition().x, player_pos.z - m_Boss->m_GameObject->transform->GetPosition().z);

		pt1.Normalize();
		pt2.Normalize();

		float cos_theta = pt1.Dot(pt2);

		if (cos_theta >= 0)
		{
			return 0;
		}
		else
		{
			return -2;
		}
	}
	else
	{
		//NetLogger::GetIns()->Write(L"test", L"Attack Fail");
		return -1;
	}
}

int BossArea::CheckJump(const Player* player)
{
	using namespace DirectX::SimpleMath;
	using namespace collider2;

	//Circle p_circle;
	Circle b_circle;

	//p_circle.Radius = player->Capsule->GetRadius();
	//p_circle.Center.x = player->m_GameObject->transform->GetPosition().x;
	//p_circle.Center.y = player->m_GameObject->transform->GetPosition().z;

	Vector2 v;
	v.x = player->m_GameObject->transform->GetPosition().x;
	v.y = player->m_GameObject->transform->GetPosition().z;

	b_circle.Radius = 4.5;
	b_circle.Center.x = m_Boss->m_GameObject->transform->GetPosition().x;
	b_circle.Center.y = m_Boss->m_GameObject->transform->GetPosition().z;

	//if (collider2::CC(p_circle, b_circle))
	//{
	//	return 1;
	//}

	if (collider2::CIRCLE_POINT(b_circle, v))
	{
		return 1;
	}

	return -1;
}

int BossArea::CheckRay(const Player* player)
{
	return 0;
}

int BossArea::CheckDetection(const Player* player)
{
	return 0;
}

int BossArea::CheckRush(const Player* player)
{
	using namespace DirectX::SimpleMath;
	using namespace collider2;
	
	Circle p_circle;
	Circle b_circle;

	p_circle.Radius = player->Capsule->GetRadius();
	p_circle.Center.x = player->m_GameObject->transform->GetPosition().x;
	p_circle.Center.y = player->m_GameObject->transform->GetPosition().z;

	b_circle.Radius = 3.5;
	b_circle.Center.x = m_Boss->m_GameObject->transform->GetPosition().x;
	b_circle.Center.y = m_Boss->m_GameObject->transform->GetPosition().z;

	if (collider2::CC(p_circle, b_circle))
	{
		return 1;
	}

	return -1;
}