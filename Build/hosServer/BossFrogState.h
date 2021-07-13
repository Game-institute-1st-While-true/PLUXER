#pragma once
#include "GroupSystem.h"

static constexpr int MAX_HEALTH_POINT = 100;

class BossRoom;
class BossFrogState : public hos::com::Script
{
public:
	enum class BossFrogActionList
	{
		Idle, Dead, Move, JumpAttack, BackJump, RangeAttack, TongueAttack, RushAttack, BackMove, RotateToTarget, SearchTarget
	};

private:
	// Room
	BossRoom* Room;

	// 몬스터의 행동
	BossFrogActionList NowAction;

	BossFrogActionList NextAction;
	BossFrogActionList PrevAction;
	bool IsChangeAction;

	// 몬스터 식별자
	hos::string Id;
	int CharacterId;

	// 몬스터의 방향
	hos::Vector3 NowDir;
	hos::Vector3 PreDir;
	std::chrono::system_clock::time_point DirTimePoint;
	bool IsStop;

	// 몬스터의 체력
	float MaxHealthPoint;
	float NowHealthPoint;

	// 몬스터의 공격력
	int AttackDamage = 1;

	// 몬스터의 데미지 타이머
	float DamagedTimer = 0;
	float DamagedTime = 0.01f;

	bool IsDamaged = false;

	// 몬스터의 스피드 (동적으로 변하는 속도는 State에 넣어뒀다.)
	float JumpMoveSpeed;

public:
	BossFrogState();
	virtual ~BossFrogState();

public:
	BossFrogActionList GetNowAction() const { return NowAction; };
	BossFrogActionList GetNextAction() const { return NextAction; };
	void SetNowAction(BossFrogActionList action);
	void SetNextAction(BossFrogActionList action);
	void PlayNextAction();
	BossFrogActionList GetPrevAction() const { return PrevAction; };

	void SetJumpMoveSpeed(const float speed);
	float GetJumpMoveSpeed() { return JumpMoveSpeed; }

	void GetDamaged(float damage, int damagedType);
	float GetHealthPoint() const { return NowHealthPoint; };

	void SetDirection(hos::Vector3 dir, bool on_event);
	hos::Vector3 GetDirection() { return NowDir; };
	void SetRoom(BossRoom* room) { Room = room; };

	int GetAttackDamege();

	void SetStop(bool btn);
	bool GetStop() { return IsStop; };

private:
	void Req_OnDamegedEvent(float damage, int damageType, float angle);
	void Req_OnStopEvent();

private:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	void OnStateUpdate();
};