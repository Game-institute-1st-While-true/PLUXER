#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "CameraMove.h"

class PlayerState : public hos::com::Script
{
public:
	enum class PlayerActionList
	{
		Dead, Damaged, Charge, Dash, Parrying, Smash, Smash1, Smash2, Smash3, DashAttack, Attack1, Attack2, Attack3, Move, Idle, ChargeStart, ChargeIdle, ChargeEnd, FeverCast
	};

private:

	// 플레이어 식별자
	hos::string Id = L"";
	int CharacterId = 0;

	// 플레이어의 행동
	PlayerActionList NowAction = PlayerActionList::Idle;
	PlayerActionList PrevAction = PlayerActionList::Idle;
	hos::F32 PrevEndPoint = 0.f;

	// 플레이어의 방향
	hos::Vector3 NowDir = hos::Vector3::Zero;
	hos::Vector3 PreDir = hos::Vector3::Zero;
	std::chrono::system_clock::time_point DirTimePoint;

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

	// 피버 게이지
	bool IsFever = false;
	bool IsGageFull = false;
	bool IsGageFullSound = false;
	float MaxFeverGage = 250.f;
	float NowFeverGage = 0;
	float GageRemoveTime = 0.1f;
	float GageRemoveTimer = 0.f;
	float FeverGageDecreaseAmount = 4.55f;

	// 피버 증가 계수
	float SmashFeverCP = 40.f;
	float SmashFeverP = 34.67f;
	float SmashFeverC = 24.f;
	float SmashFeverN = 13.33f;

	const float NormalFeverCP = 15.f;
	const float NormalFeverP = 13.f;
	const float NormalFeverC = 9.f;
	const float NormalFeverN = 5.f;

	// 애니메이션 상태
	bool IsPlayerAnimationEnd = false;

	float DashTimeScale = 2.0f;
	float PreSmashTimeScale = 0.f;
	float SmashTimeScale = 0.1f;
	float FeverTimeScale = 0.1f;

	bool UseTimeScale = false;

	int DamagedDir = 0;
	int DamageType = 0;

	// 카메라 쉐이크
	bool IsShakeCamera = false;
	float ShakeValue = 1.f;
	float ShakeTime = 0.f;
	float ShakeTimer = 0.f;

	// 느려진 상태인지 확인
	bool IsSlow = false;
	int SlowStack = 0;

	// 슈퍼아머 상태
	bool IsSuperArmor = false;
	float SuperArmorTime = 0.3f;
	float SuperArmorTimer = 0.f;

	// 콤보
	bool IsComboChange = false;
	int MaxCombo = 0;
	int NowCombo = 0;

	// 점수
	int MaxScore = 0;
	int NowScore = 0;
	int ComboBonus = 0;
	hos::U32 Judge[static_cast<int>(NoteManager::NoteErrorRangeCheck::W) + 1];

	// 점수 증가 계수
	//const int ScoreCP = 500;
	//const int ScoreP = 450;
	//const int ScoreC = 200;
	//const int ScoreN = 100;
	//const int ScoreM = 0;

	// 고정 된 플레이어 확인
	bool IsStop = false;

	// 보너스 점수 증가 계수
	const float BonusCP = 0; // 4224.f;
	const float BonusP = 0; // 2304.f;
	const float BonusC = 0; // 1152.f;
	const float BonusN = 0; // 230.4f;
	const float BonusM = 0;

	// 보너스 점수 증가 계수
	//const float BonusRateCP = 1.0f;
	//const float BonusRateP = 0.8f;
	//const float BonusRateC = 0.6f;
	//const float BonusRateN = 0.2f;
	//const float BonusRateM = 0;

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

public:
	hos::com::Animation* PlayerAnimation = nullptr;
	hos::com::CapsuleCollision* PlayerDamgedArea = nullptr;

	hos::com::AudioSource* ActionSound = nullptr;
	hos::com::AudioSource* StaminaEmptySound = nullptr;
	hos::com::AudioSource* FeverGageFullSound = nullptr;

	hos::com::GameObject* Weapon1 = nullptr;
	hos::com::GameObject* Weapon2 = nullptr;

	hos::com::MeshRenderer* Weapon1MeshRenderer = nullptr;
	
	// 네트워크 컴포넌트 --> Send 용
	hos::com::GameObject* NetEventer = nullptr;

public:
	PlayerState();
	virtual ~PlayerState();

	// GetSet
public:
	PlayerActionList GetNowPlayerAction() { return NowAction; };
	void SetNowPlayerAction(PlayerActionList playerAction);
	PlayerActionList GetPrevPlayerAction() { return PrevAction; };

	void SetDirection(hos::Vector3 dir, bool on_event);
	hos::Vector3 GetDirection() { return NowDir; }

	bool GetIsPlayerAnimationEnd() const { return IsPlayerAnimationEnd; };
	NoteManager::NoteErrorRangeCheck GetAccuracy() const { return Accuracy; }
	void SetAccuracy(NoteManager::NoteErrorRangeCheck val);
	bool GetIsChangeAccurency() const { return IsChangeAccuracy; };
	bool GetIsChangeAction() const { return IsChangeAction; };
	void SetIsChangeAction(bool on) { IsChangeAction = on; }

	hos::string GetId() { return Id; }
	void SetId(hos::string_view name) { Id = name.data(); }

	int GetCharacterId() { return CharacterId; }
	void SetCharacterId(const int uuid) { CharacterId = uuid; }

	float GetDashTimeScale() const { return DashTimeScale; }

	float GetSmashTimeScale() const { return SmashTimeScale; };
	void SetSmashTimeScale(float timeScale);

	float GetFeverTimeScale() const { return FeverTimeScale; };

	void GetDamaged(int damage, int damageType, hos::com::Transform* tr);
	void GetDamaged(int damage, int damageType, float angle);

	int GetDamagedType() const { return DamageType; };

	void SetNetworkSender(hos::com::GameObject* net) { NetEventer = net; };

	void SetHealthPoint(int hp) { NowHealthPoint = hp; };
	void SetStamina(int st) { NowStamina = st; }

	int GetMaxHealthPoint() const { return MaxHealthPoint; };
	int GetNowHealthPoint() const { return NowHealthPoint; };

	int GetMaxStamina() const { return MaxStamina; };
	int GetNowStamina() const { return NowStamina; };
	void SetNowStamina(int nowStamina) { NowStamina = nowStamina; };

	bool GetIsSuperArmor() const { return IsSuperArmor; };

	void SetIsFever(bool val) { IsFever = val; };
	bool GetIsFever() const { return IsFever; };
	bool GetIsGageFull() const { return IsGageFull; };
	float GetMaxFeverGage() const { return MaxFeverGage; };
	float GetNowFeverGage() const { return NowFeverGage; };
	void AddNowFeverGage();
	void SetNowFeverGage(float fever);
	void AddNowFeverGage(NoteManager::NoteErrorRangeCheck val);
	void RequestFever();

	bool GetIsComboChange() const { return IsComboChange; };
	void SetIsComboChange(bool val) { IsComboChange = val; };
	int GetNowCombo() const { return NowCombo; };
	int GetMaxCombo() const { return MaxCombo; };
	int GetJudge(NoteManager::NoteErrorRangeCheck j) const;
	void SetNowCombo(int combo);
	void ComboIncrease();
	void ResetCombo();

	void AddScore(float attackDamage);
	void SetNowScore(int score);
	int GetNowScore() const { return NowScore; };

	bool GetStop() const { return IsStop; }
	void SetStop(bool btn) { IsStop = btn; }

	bool GetIsSlow() const { return IsSlow; }
	void SetIsSlow(bool val, bool fource = false);

	int GetSlowStack() const { return SlowStack; }
	void SetSlowStack(int val) { SlowStack = val; }

	bool GetIsShakeCamera() const { return IsShakeCamera; }
	void SetIsShakeCamera(bool val) { IsShakeCamera = val; }
	float GetShakeValue() const { return ShakeValue; }
	void SetShakeValue(float val) { ShakeValue = val; }
	float GetShakeTime() const { return ShakeTime; }
	void SetShakeTime(float val) { ShakeTime = val; }
	void ResetShakeTimer() { ShakeTimer = 0.f; };

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void OnCollisionEnter(hos::com::Collision* other)override;

private:
	void OnStateUpdate();
	void OnTimeScaleUpdate();

	void FeverState();
	void WeaponState();
	void CheckStaminaEmpty();
};

