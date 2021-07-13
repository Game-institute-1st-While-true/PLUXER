#pragma once
#include "GroupSystem.h"
#include "NoteManager.h"
#include <mutex>

const double STADING_DAMAGED_ANI_TIME = 0.5;

class PlayerState : public hos::com::Script
{
public:
	std::mutex StateMutex;

public:
	enum class PlayerActionList
	{
		Dead, Damaged, Charge, Dash, Parrying, Smash, Smash1, Smash2, Smash3, DashAttack, Attack1, Attack2, Attack3, Move, Idle, ChargeStart, ChargeIdle, ChargeEnd, FeverCast
	};


private:
#pragma region Net
	// ��Ʈ��ũ ������Ʈ --> Send ��
	hos::com::GameObject* NetEventer;

	// �÷��̾� �ĺ���
	hos::string Id;
	int CharacterId;
#pragma endregion

#pragma region PlayerStat
	// �÷��̾��� �ൿ
	PlayerActionList NowAction = PlayerActionList::Idle;
	PlayerActionList PrevAction = PlayerActionList::Idle;

	// �÷��̾��� ����
	hos::Vector3 NowDir = hos::Vector3::Zero;
	hos::Vector3 PreDir = hos::Vector3::Zero;
	std::chrono::system_clock::time_point DirTimePoint;

	// ���� �� �÷��̾����� Ȯ��
	bool IsStop = false;

	// �ൿ�� �ٲ� �� ���� Ȯ��
	bool IsChangeAction = false;

	// ������ �ٲ� �� ���� Ȯ��
	bool IsChangeAccuracy = false;


	// �÷��̾��� ����
	NoteManager::NoteErrorRangeCheck Accuracy = NoteManager::NoteErrorRangeCheck::W;

	// �÷��̾��� ü��
	int MaxHealthPoint = 121;
	int NowHealthPoint = MaxHealthPoint;

	// �÷��̾��� ���׹̳�
	bool IsStaminaEmpty = false;
	int MaxStamina = 12;
	int NowStamina = MaxStamina;
#pragma endregion

#pragma region Fever
	// �ǹ�������
	bool IsFever = false;
	bool IsGageFull = false;
	bool IsGageFullSound = false;
	float MaxFeverGage = 250.f;
	float NowFeverGage = 0;
	double GageRemoveTime = 0.1f;
	double GageRemoveTimer = 0.f;
	float FeverGageDecreaseAmount = 4.55f;

	double FeverAniDelayTime = 1.45;
	double FeverAniDelayTimer = 0;

	// �ǹ� ���� ���
	float SmashFeverCP = 40.f;
	float SmashFeverP = 34.67f;
	float SmashFeverC = 24.f;
	float SmashFeverN = 13.33f;

	const float NormalFeverCP = 15.f;
	const float NormalFeverP = 13.f;
	const float NormalFeverC = 9.f;
	const float NormalFeverN = 5.f;
#pragma endregion

	// ���ݷ�
	int Attack;

	// ��׷� ����
	double AggroLevel;

	// �ִϸ��̼� ����
	bool IsPlayerAnimationEnd;

	float DashTimeScale;
	float SmashTimeScale;
	float FeverTimeScale;

	bool UseTimeScale;

	// �ǰ� ����
	int DamagedDir = 0;
	int DamageType = 0;

	double DamagedTimer;
	double DamagedTime;

	// �� ������ �¾Ҵ°�?
	bool IsDamaged = false;

	// ���۾Ƹ� ����
	bool IsSuperArmor;
	float SuperArmorTime;
	float SuperArmorTimer;

	// ������ �������� Ȯ��
	bool IsSlow = false;
	int SlowStack = 0;

	// ��׷� ��ġ�� �ʿ��� �����͵�.
	double DamageDealt; 
	double FirstTargetRate;

#pragma region Combo
	// �޺�
	bool IsComboChange = false;
	int MaxCombo = 0;
	int NowCombo = 0;
#pragma endregion

#pragma region Score
	// ����
	int MaxScore = 0;
	int NowScore = 0;
	int ComboBonus = 0;
	hos::U32 Judge[static_cast<int>(NoteManager::NoteErrorRangeCheck::W)];

	// ���ʽ� ���� ���� ���
	const float BonusCP = 0; // 4224.f;
	const float BonusP = 0; // 2304.f;
	const float BonusC = 0; // 1152.f;
	const float BonusN = 0; // 230.4f;
	const float BonusM = 0;

	// �޺� ���ʽ� ������ ���
	const int ComboBonusCP = 5;
	const int ComboBonusP = 3;
	const int ComboBonusC = 2;
	const int ComboBonusN = -2;
	const int ComboBonusM = -100;

	// ���Ž� ���ʽ� ���� ���
	const int SmashBonusCP = 64;
	const int SmashBonusP = 32;
	const int SmashBonusC = 16;
	const int SmashBonusN = 8;
	const int SmashBonusM = 1;

	// ��Ÿ ���ʽ� ���� ���
	const int NormalBonusCP = 64;
	const int NormalBonusP = 32;
	const int NormalBonusC = 16;
	const int NormalBonusN = 8;
	const int NormalBonusM = 1;
#pragma endregion

public:
	PlayerState();
	virtual ~PlayerState();

	// GetSet
public:

	// Action
	PlayerActionList GetNowPlayerAction();
	void SetNowPlayerAction(PlayerActionList playerAction);
	PlayerActionList GetPrevPlayerAction() { return PrevAction; };

	// Direction
	void SetDirection(hos::Vector3 dir);
	hos::Vector3 GetDirection() { return NowDir; }

	// Accuracy
	NoteManager::NoteErrorRangeCheck GetAccuracy() const { return Accuracy; }
	void SetAccuracy(NoteManager::NoteErrorRangeCheck val) { Accuracy = val; }
	bool GetIsChangeAction() { return IsChangeAction; };
	void SetIsChangeAction(bool on) {IsChangeAction = on; }

	// UUid
	hos::string GetId() { return Id; }
	void SetId(hos::string_view name) { Id = name.data(); }

	// ID
	int GetCharacterId() { return CharacterId; }
	void SetCharacterId(const int uuid) { CharacterId = uuid; }

	// Time Scale
	void SetSmashTimeScale(float timeScale);

	// OnDamaged
	void GetDamaged(int damage, int damageType, hos::com::Transform* tr);
	void GetDamaged(int damage, int damageType, float angle);

	// Stop == dir == 0
	bool GetStop() { return IsStop; }
	void SetStop(bool btn);

	// Aggro
	void SetFirstTargetRate(float rate) { FirstTargetRate = rate; }
	void AddDamageDealt(float deal) { DamageDealt += deal; }
	void AggroReset() { DamageDealt = 0; }
	double CalcAggrol();

	// Slow
	void SetSlow(bool btn);
	bool GetSlow() { return IsSlow; }

	// Network 
	void SetNetworkSender(hos::com::GameObject* net) { NetEventer = net; };

	// DamageType
	int GetDamagedType() const { return DamageType; };

	// Combo
	int GetNowCombo() const { return NowCombo; }
	int GetMaxCombo() const { return MaxCombo; }
	void SetCombo(const int combo) { NowCombo = combo; }
	void ComboIncrease();

	// Fever
	void SetIsFever(bool val) { IsFever = val; };
	bool GetIsFever() const { return IsFever; };
	bool GetIsGageFull() const { return IsGageFull; };
	float GetMaxFeverGage() const { return MaxFeverGage; };
	float GetNowFeverGage() const { return NowFeverGage; };
	void AddNowFeverGage();

	// Score
	void AddScore(float attackDamage);
	int GetScore() const { return NowScore; }

public:
	void Req_OnDamegedEvent(int damage, int damageType, float angle);
	void Req_MoveStateEvent();
	void Req_FeverResult(bool btn);

private:
	void DamegedUpdate();

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void OnCollisionEnter(hos::com::Collision* other)override;

private:
	void FeverState();
	void OnStateUpdate();
	void OnDirectionUpdate();
};

