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
	// 네트워크 컴포넌트 --> Send 용
	hos::com::GameObject* NetEventer;

	// 플레이어 식별자
	hos::string Id;
	int CharacterId;
#pragma endregion

#pragma region PlayerStat
	// 플레이어의 행동
	PlayerActionList NowAction = PlayerActionList::Idle;
	PlayerActionList PrevAction = PlayerActionList::Idle;

	// 플레이어의 방향
	hos::Vector3 NowDir = hos::Vector3::Zero;
	hos::Vector3 PreDir = hos::Vector3::Zero;
	std::chrono::system_clock::time_point DirTimePoint;

	// 고정 된 플레이어인지 확인
	bool IsStop = false;

	// 행동이 바뀔 때 마다 확인
	bool IsChangeAction = false;

	// 판정이 바뀔 때 마다 확인
	bool IsChangeAccuracy = false;


	// 플레이어의 판정
	NoteManager::NoteErrorRangeCheck Accuracy = NoteManager::NoteErrorRangeCheck::W;

	// 플레이어의 체력
	int MaxHealthPoint = 121;
	int NowHealthPoint = MaxHealthPoint;

	// 플레이어의 스테미나
	bool IsStaminaEmpty = false;
	int MaxStamina = 12;
	int NowStamina = MaxStamina;
#pragma endregion

#pragma region Fever
	// 피버게이지
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

	// 피버 증가 계수
	float SmashFeverCP = 40.f;
	float SmashFeverP = 34.67f;
	float SmashFeverC = 24.f;
	float SmashFeverN = 13.33f;

	const float NormalFeverCP = 15.f;
	const float NormalFeverP = 13.f;
	const float NormalFeverC = 9.f;
	const float NormalFeverN = 5.f;
#pragma endregion

	// 공격력
	int Attack;

	// 어그로 레벨
	double AggroLevel;

	// 애니메이션 상태
	bool IsPlayerAnimationEnd;

	float DashTimeScale;
	float SmashTimeScale;
	float FeverTimeScale;

	bool UseTimeScale;

	// 피격 상태
	int DamagedDir = 0;
	int DamageType = 0;

	double DamagedTimer;
	double DamagedTime;

	// 이 유저가 맞았는가?
	bool IsDamaged = false;

	// 슈퍼아머 상태
	bool IsSuperArmor;
	float SuperArmorTime;
	float SuperArmorTimer;

	// 느려진 상태인지 확인
	bool IsSlow = false;
	int SlowStack = 0;

	// 어그로 수치에 필요한 데이터들.
	double DamageDealt; 
	double FirstTargetRate;

#pragma region Combo
	// 콤보
	bool IsComboChange = false;
	int MaxCombo = 0;
	int NowCombo = 0;
#pragma endregion

#pragma region Score
	// 점수
	int MaxScore = 0;
	int NowScore = 0;
	int ComboBonus = 0;
	hos::U32 Judge[static_cast<int>(NoteManager::NoteErrorRangeCheck::W)];

	// 보너스 점수 증가 계수
	const float BonusCP = 0; // 4224.f;
	const float BonusP = 0; // 2304.f;
	const float BonusC = 0; // 1152.f;
	const float BonusN = 0; // 230.4f;
	const float BonusM = 0;

	// 콤보 보너스 증감소 계수
	const int ComboBonusCP = 5;
	const int ComboBonusP = 3;
	const int ComboBonusC = 2;
	const int ComboBonusN = -2;
	const int ComboBonusM = -100;

	// 스매쉬 보너스 점수 계수
	const int SmashBonusCP = 64;
	const int SmashBonusP = 32;
	const int SmashBonusC = 16;
	const int SmashBonusN = 8;
	const int SmashBonusM = 1;

	// 평타 보너스 점수 계수
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

