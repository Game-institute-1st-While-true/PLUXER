#pragma once

#include "hosEngine/COM_Client.h"
#include "PlayerAttack.h"
#include "PlayerState.h"
#include "PlayerAction.h"
#include "ShieldFrogState.h"
#include "BossFrogState.h"
#include "NoteManager.h"
#include "GameManager.h"
#include "ArcadeNetworkEvent.h"

class PlayerAttack;

class UIManager : public hos::com::Script
{
private:
	bool IsSmashSetting;
	int SmashNoteMoveCount = -1;
	double SmashOneBeat;
	float SmashUIDistance;

	float ComboUIActiveTime = 1.0f;
	float ComboUIActiveTimer = 0.f;

	bool IsAddDamage = false;
	float BossAddDamage = 0.f;

public:
	NoteManager* m_NoteManager;
	PlayerAttack* m_PlayerAttack;
	std::vector <hos::com::GameObject*> SmashStartVector;
	std::vector <hos::com::GameObject*> SmashEndVector;

	hos::com::UIImage* BossFrogHPUI;
	BossFrogState* m_BossFrogState;
	hos::com::UIText* BossUIDamage;
	hos::com::UIText* BossUIDamageBack;

	PlayerState* m_PlayerState;
	PlayerAction* m_PlayerAction;
	ArcadeNetworkEvent* m_NetworkEvent;

	hos::com::UIImage* DamageFrame;
	hos::com::UIImage* PlayerHealthBar;
	hos::com::GameObject* PlayerAccurencyObj;
	hos::com::UIImage* PlayerAccurency;
	hos::com::UIText* PlayerStamina;
	hos::com::UIText* PlayerStaminaBack;
	hos::com::UIImage* PlayerFeverGage;
	hos::com::UIImage* PlayerComboImage;
	hos::com::UIText* PlayerComboCount;
	hos::com::UIText* PlayerNowScore;
	hos::com::UIText* PlayerBestScore;

	GameManager* m_GameManager;
	hos::com::GameObject* GameResultUI;
	hos::com::UIButton* ResultOKBtn;
	hos::com::GameObject* ResultClear;
	hos::com::GameObject* ResultFail;
	hos::com::UIImage* ResultRank;
	hos::com::UIText* ResultBestScore;
	hos::com::UIText* ResultScore;
	hos::com::UIText* ResultPlayTime;
	hos::com::GameObject* ResultNewRecord;
	hos::com::UIText* ResultMaxCombo;
	hos::com::UIText* ResultCP;
	hos::com::UIText* ResultPerfect;
	hos::com::UIText* ResultCool;
	hos::com::UIText* ResultNice;
	hos::com::UIText* ResultMiss;

	hos::com::GameObject* SettingPopup;
	hos::com::UIButton* Check[2];
	hos::com::UIButton* CheckON[2];
	hos::com::UIInputField* VolumeInput;
	hos::com::UIText* VolumeInputText;
	hos::com::UIText* VolumeText;
	hos::com::UIButton* SettingQuit;
	hos::com::UIButton* SettingSubmit;
	hos::com::UIButton* SettingClose;

	hos::com::GameObject* ReturnMenuPopup;
	hos::com::UIButton* ReturnMenuOK;
	hos::com::UIButton* ReturnMenuCancel;

	bool resultFlag;
	hos::U32 BestScore;

public:
	UIManager();
	virtual ~UIManager();

public:
	void Reset() override;
	void Awake() override;
	void Update() override;

private:
	void SmashUIUpdate();
	void SetSmashUI();
	void BossFrogHP();
	void PlayerStateUI();
	void GameResult();
	void SetAttackDamage();
};

