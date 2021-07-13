#pragma once
#include "GroupSystem.h"
#include "MapCollisionChecker.h"
#include "BossFrog.h"
#include "Player.h"

struct BossArea
{
	BossFrog* m_Boss;

	collider2::Line Ray;				// 유저와 거리 감지용 -- Circle to Line

	collider2::Rect BackJumpBox;		// Back Jump 거리 감지용 -- AABB
	bool BackJumpAble;

	collider2::Rect RushBox;			// Rush 거리 감지용 -- AABB
	bool RushAble;

	collider2::Circle TongueArea;		// 혓바닥 휘두르기용 -- CC
	bool OnTongueCollision;

	collider2::Rect JumpArea;			// 점프 공격용 -- AABB
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

