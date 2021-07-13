#pragma once

#include "GroupCallBackcomponent.h"

#include "Collider2D.h"
#include <vector>

class BossFrog;
class Player;
class ProjectileManager;
class ToxicManager
{
	struct ToxicRect
	{
		int Id;

		bool IsActive;
		bool OnVeneer;

		collider2::Rect Rt;
		float Size;
		hos::com::Transform transform;

		double LifeTimer;
		double LifeTime;

		bool LifeCheck(double delta);
		bool CollisionCheck(Player& player);
		void Release();
	};

public:
	ToxicManager() = delete;
	ToxicManager(BossFrog* boss, int venner_cnt);
	~ToxicManager();

private:
	BossFrog* m_Boss;

	std::vector<ToxicManager::ToxicRect*> VeneerPool;
	std::vector<int> VeneerIdPool;

	int Seed;

public:
	void CollisionCheck(Player& player);
	void ToxicUpdate(double delta);
	void CreateToxic(hos::com::Transform* tr, int attackType);

	hos::com::Transform* ToxicGetTransform(int index);

private:
	void Req_ToxicVeneereActive(ToxicRect* veneer);
};

