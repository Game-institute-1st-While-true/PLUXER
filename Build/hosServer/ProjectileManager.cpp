#include "BossRoom.h"
#include "BossFrog.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "PacketDefine.h"

ProjectileManager::ProjectileManager(BossFrog* boss, int projectile_cnt)
	:m_Boss(boss), Seed(0)
{
	BubblePool.reserve(projectile_cnt);

	for (int i = 0; i < projectile_cnt; ++i)
	{
		Projectile* NewBubble = new Projectile;

		NewBubble->Id = Seed++;
		NewBubble->AttackDamage = 10;
		NewBubble->AttackType = 0;
		NewBubble->IsActive = false;
		NewBubble->Speed = 8.0;
		NewBubble->Radius = 0.125;
		NewBubble->transform.SetPosition(hos::Vector3(0, 8, 0));
		NewBubble->transform.SetRotation(hos::Rotator(0, 0, 0));
		BubblePool.push_back(NewBubble);
	}

	m_ToxicManager = new ToxicManager(m_Boss, projectile_cnt * 2);
}

ProjectileManager::~ProjectileManager()
{
	for (int i = 0; i < 3; i++)
	{
		if (BubblePool[i])
		{
			delete BubblePool[i];
			BubblePool[i] = nullptr;
		}
	}
	BubblePool.clear();
	hos::SafeDelete(m_ToxicManager);
}

void ProjectileManager::CollisionCheck(Player& player)
{
	for (auto bubble : BubblePool)
	{
		if (bubble->IsActive)
		{
			if (bubble->CollisionCheck(player))
			{
				m_ToxicManager->CreateToxic(&bubble->transform, 0);

				bubble->Relese();
				Req_ProjectileActive(bubble);

				float angle = player.m_GameObject->transform->GetForward().Dot(bubble->transform.GetPosition());
				player.State->GetDamaged(bubble->AttackDamage, bubble->AttackType, angle);
			}
		}
	}
	m_ToxicManager->CollisionCheck(player);
}

void ProjectileManager::CreateProjectile(hos::com::Transform* tr, int attackType)
{
	using namespace collider2;
	using namespace DirectX::SimpleMath;

	for (auto bubble : BubblePool)
	{
		if (!bubble->IsActive)
		{
			bubble->transform.SetPosition(tr->GetPosition() + Vector3(0, 8, 0));
			bubble->transform.SetRotation(hos::Rotator(-90, 0, 0));
			bubble->transform.SetScale(hos::Vector3(1, 1, 1));
			bubble->IsActive = true;
			bubble->AttackType = 0;
			Req_ProjectileActive(bubble);
			return;
		}
	}
}

void ProjectileManager::ProjectileUpdate(float delta)
{
	using namespace std;
	using namespace chrono;

	for (auto bubble : BubblePool)
	{
		if (bubble->IsActive)
		{
			bubble->Move(delta);
			if (bubble->transform.GetPosition().y <= 0)
			{
				m_ToxicManager->CreateToxic(&bubble->transform, 0);
				bubble->Relese();

				Req_ProjectileActive(bubble);
			}
		}
	}
	m_ToxicManager->ToxicUpdate(delta);
}

int ProjectileManager::GetCurrentActiveObject()
{
	int cnt = 0;
	for (auto bubble : BubblePool)
	{
		if (bubble->IsActive)
		{
			++cnt;
		}
	}
	return cnt;
}

hos::com::Transform* ProjectileManager::ProjectileGetTransform(int index)
{
	return &BubblePool[index]->transform;
}

void ProjectileManager::Req_ProjectileActive(Projectile* bubble)
{
	if (!m_Boss) return;

	flexbuffers::Builder builder;

	builder.Map([&] {
		builder.Int("id", bubble->Id);
		builder.Float("px", bubble->transform.GetPosition().x);
		builder.Float("py", bubble->transform.GetPosition().y);
		builder.Float("pz", bubble->transform.GetPosition().z);
		builder.Int("t", bubble->AttackType);
		builder.Bool("on", bubble->IsActive);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned short>(PROTOCOL::PROJECTILE_ACTIVE_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	m_Boss->Room->Send(pack);
}

void ProjectileManager::Projectile::Move(float delta)
{
	using namespace collider2;
	using namespace DirectX::SimpleMath;

	if (!IsActive) return;

	if (AttackType == 0)
	{
		transform.SetPosition(transform.GetPosition() + Vector3(transform.GetForward() * Speed * delta));
	}
}

bool ProjectileManager::Projectile::CollisionCheck(Player& player)
{
	using namespace collider2;
	using namespace DirectX::SimpleMath;

	Circle c1;
	Circle c2;

	c1.Center.x = transform.GetPosition().x;
	c1.Center.y = transform.GetPosition().z;
	c1.Radius = Radius;

	c2.Center.x = player.m_GameObject->transform->GetPosition().x + player.Capsule->GetCenter().x;
	c2.Center.y = player.m_GameObject->transform->GetPosition().z + player.Capsule->GetCenter().z;
	c2.Radius = player.Capsule->GetRadius();

	if (CC(c1, c2))
	{
		// top view 충돌이 끝났다. bubble과 player의 높이 비교를 통해서 실제로 충돌했는가를 알 수 있다.
		if (player.Capsule->GetHeight() >= transform.GetPosition().y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return false;
}

void ProjectileManager::Projectile::Relese()
{
	transform.SetPosition(hos::Vector3(0, 8, 0));
	transform.SetScale(hos::Vector3(1, 1, 1));
	IsActive = false;
	AttackType = 0;
}
