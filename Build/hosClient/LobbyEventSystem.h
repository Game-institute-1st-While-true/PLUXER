#ifndef _LOBBY_EVENT_SYSTEM_H
#define _LOBBY_EVENT_SYSTEM_H

#include "hosEngine/COM_Client.h"

class LobbyEventSystem : public hos::com::Script
{
public:
	enum class eState {
		Title,					//타이틀 화면
		Main,					//시작하는 로비화면
		StoryMode,				//싱글플레이
		Login,					//로그인 팝업
		CreateAccount,			//로그인팝업중 계정생성 팝업
		ArcadeMode,				//멀티플레이
		Wait,					//멀티 씬 이동 확인 후 대기룸
		Credit,					//크레딧
	};

	enum class eLoginState
	{
		NONE,
		LOGIN,
		SIGNUP,
		WITHDRAWL
	};

private:

	hos::com::UIButton* SettingOpen;
	hos::com::GameObject* BackGroundBlink;
	hos::com::UIImage* BackLogo;
	hos::com::UIImage* BackBlink[4];
	hos::com::AudioSource* MouseClickSFX;
	hos::com::AudioSource* MouseHoverSFX;
	hos::com::AudioSource* ErrorSFX;
	hos::com::AudioSource* BGM;
	hos::com::GameObject* CreditPopup;

#pragma region Title
	hos::com::GameObject* TitlePopup;
#pragma endregion

#pragma region Main
	hos::com::GameObject* MenuPopUp;
	hos::com::UIButton* LoginOpen;
	hos::com::UIButton* StoryOpen;
	hos::com::UIButton* ArcadeOpen;
	hos::com::UIButton* MenuSettingOpen;
	hos::com::UIButton* CreditOpen;
	hos::com::GameObject* MenuSelecter;
#pragma endregion

#pragma region Login
	hos::com::GameObject* LoginPopup;
	hos::com::UIButton* LoginClose;
	hos::com::UIInputField* LoginID;
	hos::com::UIText* LoginIDText;
	hos::com::UIInputField* LoginPW;
	hos::com::UIText* LoginPWText;
	hos::com::UIButton* LoginSubmit;
	hos::com::UIButton* LoginCreateAccount;

	hos::com::GameObject* LoginFailPopup;
	hos::com::UIButton* LoginFailOK;

#pragma region CreateAccount
	hos::com::GameObject* CreateAccountPopup;
	hos::com::UIButton* CreateAccountClose;
	hos::com::UIInputField* CreateAccountID;
	hos::com::UIText* CreateAccountIDText;
	hos::com::UIInputField* CreateAccountPW;
	hos::com::UIText* CreateAccountPWText;
	hos::com::UIButton* CreateAccountSubmit;

	hos::com::GameObject* CreateAccountFailPopup;
	hos::com::UIButton* CreateAccountFailOk;

	hos::com::GameObject* CreateAccountSuccessPopup;
	hos::com::UIButton* CreateAccountSuccessOk;
#pragma endregion
#pragma endregion

#pragma region Story
	hos::com::GameObject* StoryPopup;
	hos::com::UIButton* StoryClose;
	hos::com::UIButton* StoryToArcade;
	hos::com::UIText* StoryStageExplain;
	hos::com::GameObject* StoryRank;
	hos::com::UIText* StoryScore;
	hos::com::UIText* StoryCombo;
	hos::com::UIButton* StoryGameStart;
	hos::com::UIButton* StoryGameStart2;

	hos::com::UIButton* StoryModeHardRock;
	hos::com::GameObject* StoryModeHardRockTooltip;
	hos::com::UIButton* StoryModeDouble;
	hos::com::GameObject* StoryModeDoubleTooltip;
	hos::com::UIButton* StoryModeHidden;
	hos::com::GameObject* StoryModeHiddenTooltip;
#pragma endregion

#pragma region Arcade
	hos::com::GameObject* ArcadePopup;
	hos::com::UIButton* ArcadeClose;
	hos::com::UIButton* ArcadeToStory;
	hos::com::UIText* ArcadeID;
	hos::com::UIInputField* ArcadeInputID;
	hos::com::UIText* ArcadeOtherID;
	hos::com::UIButton* ArcadeCreateRoom;
	hos::com::UIButton* ArcadeEnter;
	hos::com::UIButton* ArcadeReady;
	hos::com::UIButton* ArcadeStart;
	hos::com::UIText* ArcadeCountDown;
#pragma endregion

#pragma region Setting
	hos::com::GameObject* SettingPopup;
	hos::com::UIButton* Check[2];
	hos::com::UIButton* CheckON[2];
	hos::com::UIInputField* VolumeInput;
	hos::com::UIText* VolumeInputText;
	hos::com::UIText* VolumeText;
	hos::com::UIButton* SettingQuit;
	hos::com::UIButton* SettingSubmit;
	hos::com::UIButton* SettingClose;
#pragma endregion

#pragma region Quit
	hos::com::GameObject* QuitPopup;
	hos::com::UIButton* QuitOk;
	hos::com::UIButton* QuitNo;
#pragma endregion

	hos::com::GameObject* GameEvent;
	hos::com::Networkcomponent* GameNet;

	eState State;
	eLoginState LoginState;
	hos::string CookiePath;

	bool IsLogin;
	bool IsRoom;
	bool IsHost;

	bool IsReady;

public:
	LobbyEventSystem();
	~LobbyEventSystem();

	void Start()override;
	void FixedUpdate()override;
	void Update()override;
private:
	void ChangeState(eState state);
	template<typename T>
	T* GetUI(hos::com::GameObject* obj)
	{
		if (obj)
		{
			return obj->GetComponent<T>();
		}
		return nullptr;
	}
	void SettingPopupOpen();
	void SettingPopupClose();
	bool BtnClick(hos::com::UIButton* btn);

#pragma region Network
	// TO LOGIN SERVER
	void Req_Login();
	void Req_SingUp();
	void Req_Withdrawl();

	// TO GAME SERVER
	void Req_Certification();
	void Req_CreateRoom();
	void Req_EnterRoom();
	void Req_RoomIn(hos::string room_name);
	void Req_ReadyFlag(bool isready);

private:
	// FROM LOGIN SERVER
	void OnConnect_Login(PACKAGE& package);
	void OnConnectFail_Login(PACKAGE& package);
	void OnDisconnect_Login(PACKAGE& package);

	void OnConnect_Game(PACKAGE& package);
	void OnConnectFail_Game(PACKAGE& package);
	void OnDisconnect_Game(PACKAGE& package);

	void Ans_Login(PACKAGE& package);
	void Ans_Signup(PACKAGE& package);
	void Ans_Withdrawl(PACKAGE& package);
	void Ans_Certification(PACKAGE& package);

	void Ans_RoomCreate(PACKAGE& package);
	void Ans_RoomIn(PACKAGE& package); 

	void Ans_RoomInEvent(PACKAGE& package);
	void Ans_RoomOutEvent(PACKAGE& package);
	void Ans_ReadyFlag(PACKAGE& package);

	void Ans_ChangeScene(PACKAGE& package);
#pragma endregion
};

#endif // !_LOBBY_EVENT_SYSTEM_H
