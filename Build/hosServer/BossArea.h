#pragma once
#include "GroupSystem.h"
#include "MapCollisionChecker.h"
#include "BossFrog.h"
#include "Player.h"

struct BossArea
{
	BossFrog* m_Boss;

	collider2::Line Ray;				// ������ �Ÿ� ������ -- Circle to Line

	collider2::Rect BackJumpBox;		// Back Jump �Ÿ� ������ -- AABB
	bool BackJumpAble;

	collider2::Rect RushBox;			// Rush �Ÿ� ������ -- AABB
	bool RushAble;

	collider2::Circle TongueArea;		// ���ٴ� �ֵθ���� -- CC
	bool OnTongueCollision;

	collider2::Rect JumpArea;			// ���� ���ݿ� -- AABB
	bool OnJumpCollision;
	bool JumpAble;

public:
	BossArea(BossFrog* boss);
	~BossArea();

	bool CheckTongue(const collider2::Rect& rt);
	bool CheckJump(const collider2::Rect& rt);
	bool CheckRay(const collider2::Rect& rt);
	bool CheckDetection(const collider2::Rect& rt);

	int CheckTongue(const Player* player);
	int CheckJump(const Player* player);
	int CheckRay(const Player* player);
	int CheckDetection(const Player* player);
	int CheckRush(const Player* player);
};

