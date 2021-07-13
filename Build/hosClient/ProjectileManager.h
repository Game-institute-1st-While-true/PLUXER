#pragma once
#include "hosEngine/Com_Client.h"
#include "Projectile.h"

class ProjectileManager : public hos::com::Script
{
private:
	// ������ �ִ� ����ü��
	std::vector<Projectile*> ProjectileList;

public:
	ProjectileManager();
	virtual ~ProjectileManager();

public:
	void Reset() override;
	void Awake() override;
	void Update() override;

public:
	void SetProjectile(hos::com::Transform* TF, int attackType);
	void ActiveProjectile(int index, int attackType, hos::Vector3 pos, bool btn);
	Projectile* GetProjectile(int index);
};

