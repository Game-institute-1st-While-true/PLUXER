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

	// ������ �ൿ
	BossFrogActionList NowAction;

	BossFrogActionList NextAction;
	BossFrogActionList PrevAction;
	bool IsChangeAction;

	// ���� �ĺ���
	hos::string Id;
	int CharacterId;

	// ������ ����
	hos::Vector3 NowDir;
	hos::Vector3 PreDir;
	std::chrono::system_clock::time_point DirTimePoint;
	bool IsStop;

	// ������ ü��
	float MaxHealthPoint;
	float NowHealthPoint;

	// ������ ���ݷ�
	int AttackDamage = 1;

	// ������ ������ Ÿ�̸�
	float DamagedTimer = 0;
	float DamagedTime = 0.01f;

	bool IsDamaged = false;

	// ������ ���ǵ� (�������� ���ϴ� �ӵ��� State�� �־�״�.)
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