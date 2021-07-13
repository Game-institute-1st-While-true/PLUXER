#include "ToxicManager.h"
#include "BossRoom.h"
#include "BossFrog.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "PacketDefine.h"

ToxicManager::ToxicManager(BossFrog* boss, int venner_cnt)
	:m_Boss(boss), Seed(0)
{
	VeneerPool.reserve(venner_cnt);

	for (int i = 0; i < venner_cnt; ++i)
	{
		ToxicRect* NewToxic = new ToxicRect();

		NewToxic->Id = i;
		NewToxic->Size = 2.5f;
		NewToxic->LifeTimer = 0;
		NewToxic->LifeTime = 10.0;
		NewToxic->transform.SetPosition(hos::Vector3(0, 0, 0));
		NewToxic->transform.SetRotation(hos::Rotator(0, 0, 0));
		NewToxic->IsActive = false;
		VeneerPool.push_back(NewToxic);
		VeneerIdPool.push_back(i);
	}
}

ToxicManager::~ToxicManager()
{
	for (size_t i = 0; i < VeneerPool.size(); i++)
	{
		if (VeneerPool[i])
		{
			delete VeneerPool[i];
			VeneerPool[i] = nullptr;
		}
	}
	VeneerPool.clear();
}

void ToxicManager::CollisionCheck(Player& player)
{
	for (auto rt : VeneerPool)
	{
		if (rt->IsActive)
		{
			bool ret = rt->CollisionCheck(player);
			if (ret)
			{
				player.State->SetSlow(true);
				return;
			}
		}
	}
	player.State->SetSlow(false);
}

void ToxicManager::ToxicUpdate(double delta)
{
	for (auto rt : VeneerPool)
	{
		if (rt->IsActive)
		{
			if (!rt->LifeCheck(delta))
			{
				rt->Release();
				Req_ToxicVeneereActive(rt);
			}
		}
	}
}

void ToxicManager::CreateToxic(hos::com::Transform* tr, int attackType)
{
	using namespace collider2;
	using namespace DirectX::SimpleMath;

	for (auto rt : VeneerPool)
	{
		if (!rt->IsActive)
		{
			rt->transform.SetPosition(Vector3(tr->GetPosition().x, 0.01, tr->GetPosition().z));
			rt->transform.SetScale(hos::Vector3(1, 1, 1));
			rt->IsActive = true;

			rt->Rt.SetSizeCenter(Vector2(rt->Size, rt->Size), Vector2(tr->GetPosition().x, tr->GetPosition().z));

			Req_ToxicVeneereActive(rt);
			return;
		}
	}
}

hos::com::Transform* ToxicManager::ToxicGetTransform(int index)
{
	return &VeneerPool[index]->transform;
}

void ToxicManager::Req_ToxicVeneereActive(ToxicRect* veneer)
{
	if (!m_Boss) return;

	flexbuffers::Builder builder;

	builder.Map([&] {
		builder.Int("id", veneer->Id);
		builder.Float("px", veneer->transform.GetPosition().x);
		builder.Float("py", veneer->transform.GetPosition().y);
		builder.Float("pz", veneer->transform.GetPosition().z);
		builder.Bool("on", veneer->IsActive);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned short>(PROTOCOL::TOXIC_ACTIVE_EVENT);
	pack.Make(pack, pack.head, buffer, static_cast<int>(builder.GetSize()));
	m_Boss->Room->Send(pack);
}

bool ToxicManager::ToxicRect::LifeCheck(double delta)
{
	if (LifeTimer >= LifeTime)
	{
		return false;
	}
	else
	{
		LifeTimer += delta;
		return true;
	}
}

bool ToxicManager::ToxicRect::CollisionCheck(Player& player)
{
	using namespace collider2;
	using namespace DirectX::SimpleMath;

	Vector2 pt(player.m_GameObject->transform->GetPosition().x, player.m_GameObject->transform->GetPosition().z);

	if (RECT_POINT(Rt, pt))
	{
		return true;
	}
	return false;
}

void ToxicManager::ToxicRect::Release()
{
	LifeTimer = 0;
	IsActive = false;
}
