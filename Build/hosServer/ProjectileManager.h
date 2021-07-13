#pragma once

#include "GroupCallBackcomponent.h"
#include "ToxicManager.h"
#include "Collider2D.h"
#include <vector>

class BossFrog;
class ProjectileManager
{
	struct Projectile
	{
		int Id;

		bool IsActive;
		double Radius;
		hos::com::Transform transform;

		int AttackType;
		int AttackDamage;
		float Speed;

		void Move(float delta);
		bool CollisionCheck(Player& player);
		void Relese();
	};

public:

private:
	BossFrog* m_Boss;
	ToxicManager* m_ToxicManager;

	std::vector<ProjectileManager::Projectile*> BubblePool;
	int Seed;

public:
	ProjectileManager() = delete;
	ProjectileManager(BossFrog* boss, int projectile_cnt);
	~ProjectileManager();

public:
	void CollisionCheck(Player& player);
	void CreateProjectile(hos::com::Transform* tr, int attackType);
	void ProjectileUpdate(float delta);

	int GetCurrentActiveObject();

	hos::com::Transform* ProjectileGetTransform(int index);

private:
	void Req_ProjectileActive(Projectile* bubble);
};