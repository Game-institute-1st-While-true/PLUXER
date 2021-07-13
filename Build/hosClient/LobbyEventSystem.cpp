#include "LobbyEventSystem.h"
#include "PacketDefine.h"
#include <filesystem>
#include "SceneManager.h"

using namespace hos;
using namespace com;

std::once_flag TitleFlag;

LobbyEventSystem::LobbyEventSystem() :
	Script(L"LobbyEventSystem"), State(eState::Main),
	LoginState(eLoginState::NONE), CookiePath(L""), IsLogin(false), IsRoom(false), IsHost(true), IsReady(false)
{
	ExecutionOrder = 1;
}

LobbyEventSystem::~LobbyEventSystem()
{
}

void LobbyEventSystem::Start()
{
	string dirpath = std::filesystem::current_path();
	string iniName = L"\\cookie.ini";

	CookiePath = dirpath + iniName;

	Scene* scene = m_GameObject->m_Scene;

	///Setting
	SettingOpen = GetUI<UIButton>(scene->FindGameObjectWithName(L"SettingBtn"));
	CreditPopup = scene->FindGameObjectWithName(L"credit");

	/// Title
	TitlePopup = scene->FindGameObjectWithName(L"title");

	///Menu
	MenuPopUp = scene->FindGameObjectWithName(L"Menu");
	MenuSelecter = scene->FindGameObjectWithName(L"Selecter");
	MenuSettingOpen = GetUI<UIButton>(scene->FindGameObjectWithName(L"MenuSettingBtn"));
	CreditOpen = GetUI<UIButton>(scene->FindGameObjectWithName(L"CreditOpen"));

	///BackGround
	BackGroundBlink = scene->FindGameObjectWithName(L"BackGroundBlink");
	BackLogo = GetUI<UIImage>(scene->FindGameObjectWithName((L"logo")));
	BackBlink[0] = GetUI<UIImage>(scene->FindGameObjectWithName((L"BackBlink_1")));
	BackBlink[1] = GetUI<UIImage>(scene->FindGameObjectWithName((L"BackBlink_2")));
	BackBlink[2] = GetUI<UIImage>(scene->FindGameObjectWithName((L"BackBlink_3")));
	BackBlink[3] = GetUI<UIImage>(scene->FindGameObjectWithName((L"BackBlink_4")));

	/// Login
	LoginOpen = GetUI<UIButton>(scene->FindGameObjectWithName((L"LoginBtn")));
	LoginPopup = scene->FindGameObjectWithName(L"LoginPopup");
	LoginClose = GetUI<UIButton>(scene->FindGameObjectWithName((L"LoginCloseBtn")));
	LoginID = GetUI<UIInputField>(scene->FindGameObjectWithName((L"LoginInputID")));
	LoginIDText = GetUI<UIText>(scene->FindGameObjectWithName((L"LoginInputIDText")));
	LoginPW = GetUI<UIInputField>(scene->FindGameObjectWithName((L"LoginInputPW")));
	LoginPWText = GetUI<UIText>(scene->FindGameObjectWithName((L"LoginInputPWText")));
	LoginSubmit = GetUI<UIButton>(scene->FindGameObjectWithName((L"LoginLoginBtn")));
	LoginCreateAccount = GetUI<UIButton>(scene->FindGameObjectWithName((L"LoginCreateAccountBtn")));

	LoginFailPopup = scene->FindGameObjectWithName(L"LoginFailed");
	LoginFailOK = GetUI<UIButton>(scene->FindGameObjectWithName((L"LoginFailedOKBtn")));

	/// CreateAccount
	CreateAccountPopup = scene->FindGameObjectWithName(L"CreateAccountPopup");
	CreateAccountClose = GetUI<UIButton>(scene->FindGameObjectWithName((L"CreateAccountCloseBtn")));
	CreateAccountID = GetUI<UIInputField>(scene->FindGameObjectWithName((L"CreateAccountInputID")));
	CreateAccountIDText = GetUI<UIText>(scene->FindGameObjectWithName((L"CreateAccountInputIDText")));
	CreateAccountPW = GetUI<UIInputField>(scene->FindGameObjectWithName((L"CreateAccountInputPW")));
	CreateAccountPWText = GetUI<UIText>(scene->FindGameObjectWithName((L"CreateAccountInputPWText")));
	CreateAccountSubmit = GetUI<UIButton>(scene->FindGameObjectWithName((L"CreateAccountBtn")));

	CreateAccountFailPopup = scene->FindGameObjectWithName(L"CreateAccountFail");
	CreateAccountFailOk = GetUI<UIButton>(scene->FindGameObjectWithName((L"CreateAccountFailOK")));

	CreateAccountSuccessPopup = scene->FindGameObjectWithName(L"CreateAccountSuccess");
	CreateAccountSuccessOk = GetUI<UIButton>(scene->FindGameObjectWithName((L"CreateAccountSuccessOK")));

	/// StoryMode
	StoryOpen = GetUI<UIButton>(scene->FindGameObjectWithName((L"StoryModeBtn")));
	StoryPopup = scene->FindGameObjectWithName(L"StoryModePopup");
	StoryClose = GetUI<UIButton>(scene->FindGameObjectWithName((L"StoryModeBackBtn")));
	StoryToArcade = GetUI<UIButton>(scene->FindGameObjectWithName((L"StoryModeToArcadeBtn")));
	StoryStageExplain = GetUI<UIText>(scene->FindGameObjectWithName((L"StoryModeStageExplainText")));
	StoryRank = scene->FindGameObjectWithName(L"StoryModeRankImg");
	StoryScore = GetUI<UIText>(scene->FindGameObjectWithName((L"StoryModeScoreText")));
	StoryCombo = GetUI<UIText>(scene->FindGameObjectWithName((L"StoryModeComboText")));
	StoryGameStart = GetUI<UIButton>(scene->FindGameObjectWithName((L"StartBtn_Story")));
	StoryGameStart2 = GetUI<UIButton>(scene->FindGameObjectWithName((L"StoryModeStage2Img")));

	StoryModeHardRock = GetUI<UIButton>(scene->FindGameObjectWithName((L"StoryModeModesHardRockBtn")));
	StoryModeHardRockTooltip = scene->FindGameObjectWithName(L"StoryModeModesHardRockToolTip");
	StoryModeDouble = GetUI<UIButton>(scene->FindGameObjectWithName((L"StoryModeModesDoubleBtn")));
	StoryModeDoubleTooltip = scene->FindGameObjectWithName(L"StoryModeModesDoubleToolTip");
	StoryModeHidden = GetUI<UIButton>(scene->FindGameObjectWithName((L"StoryModeModesHidden")));
	StoryModeHiddenTooltip = scene->FindGameObjectWithName(L"StoryModeModesHiddenToolTip");

	/// ArcadeMode
	ArcadeOpen = GetUI<UIButton>(scene->FindGameObjectWithName((L"ArcadeModeBtn")));
	ArcadePopup = scene->FindGameObjectWithName(L"ArcadeModePopup");
	ArcadeClose = GetUI<UIButton>(scene->FindGameObjectWithName((L"ArcadeModeBackBtn")));
	ArcadeToStory = GetUI<UIButton>(scene->FindGameObjectWithName((L"ArcadeModeToStoryModeBtn")));
	ArcadeID = GetUI<UIText>(scene->FindGameObjectWithName((L"ArcadeModeMyCode")));
	ArcadeInputID = GetUI<UIInputField>(scene->FindGameObjectWithName((L"ArcadeModeInputOtherCode")));
	ArcadeOtherID = GetUI<UIText>(scene->FindGameObjectWithName((L"ArcadeModeInputOtherCodeText")));
	ArcadeCreateRoom = GetUI<UIButton>(scene->FindGameObjectWithName((L"ArcadeModeCreateRoomBtn")));
	ArcadeEnter = GetUI<UIButton>(scene->FindGameObjectWithName((L"ArcadeModeEnterBtn")));
	ArcadeReady = GetUI<UIButton>(scene->FindGameObjectWithName((L"ArcadeModeReadyBtn")));
	ArcadeStart = GetUI<UIButton>(scene->FindGameObjectWithName((L"ArcadeModeStartBtn")));
	ArcadeCountDown = GetUI<UIText>(scene->FindGameObjectWithName((L"ArcadeModeCountDownText")));

	///Setting
	SettingPopup = scene->FindGameObjectWithName(L"SettingPopup");
	Check[0] = GetUI<UIButton>(scene->FindGameObjectWithName(L"MBCheck"));
	CheckON[0] = GetUI<UIButton>(scene->FindGameObjectWithName(L"MBcheckOn"));
	Check[1] = GetUI<UIButton>(scene->FindGameObjectWithName(L"FogCheck"));
	CheckON[1] = GetUI<UIButton>(scene->FindGameObjectWithName(L"FogCheckOn"));
	VolumeInput = GetUI<UIInputField>(scene->FindGameObjectWithName(L"SoundInputField"));
	VolumeInputText = GetUI<UIText>(scene->FindGameObjectWithName(L"SoundInputText"));
	VolumeText = GetUI<UIText>(scene->FindGameObjectWithName(L"SoundNow"));
	SettingQuit = GetUI<UIButton>(scene->FindGameObjectWithName(L"ExitButton"));
	SettingSubmit = GetUI<UIButton>(scene->FindGameObjectWithName(L"ApplyButton"));
	SettingClose = GetUI<UIButton>(scene->FindGameObjectWithName(L"closeButton"));

	///Quit
	QuitPopup = scene->FindGameObjectWithName(L"QuitPopup");
	QuitOk = GetUI<UIButton>(scene->FindGameObjectWithName((L"QuitPopupOKBtn")));
	QuitNo = GetUI<UIButton>(scene->FindGameObjectWithName((L"QuitPopupNoBtn")));

	m_GameObject->network->RequestToken(0); // login socket == 0; // game socket = 1

	m_GameObject->network->Mapping(this, static_cast<int>(SYSTEM_PROTOCOL::CONNECT), &LobbyEventSystem::OnConnect_Login);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::LOGIN), &LobbyEventSystem::Ans_Login);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::SIGNUP), &LobbyEventSystem::Ans_Signup);
	m_GameObject->network->Mapping(this, static_cast<int>(PROTOCOL::WITHDRAWL), &LobbyEventSystem::Ans_Withdrawl);

	///SFX
	MouseClickSFX = GetUI<AudioSource>(scene->FindGameObjectWithName(L"MouseClick"));
	MouseHoverSFX = GetUI<AudioSource>(scene->FindGameObjectWithName(L"MouseHover"));
	ErrorSFX = GetUI<AudioSource>(scene->FindGameObjectWithName(L"ErrorSFX"));

	BGM = GetUI<AudioSource>(scene->FindGameObjectWithName(L"BGM"));
	BGM->Play();
	///Network
	GameEvent = m_GameObject->transform->GetChild(0)->m_GameObject;
	if (GameEvent)
	{
		GameNet = GameEvent->network;
		GameNet->RequestToken(1);
		GameEvent->network->Mapping(this, static_cast<int>(SYSTEM_PROTOCOL::CONNECT), &LobbyEventSystem::OnConnect_Game);
		GameEvent->network->Mapping(this, static_cast<int>(PROTOCOL::CERTIFICATION), &LobbyEventSystem::Ans_Certification);
		GameEvent->network->Mapping(this, static_cast<int>(PROTOCOL::ROOM_ENABLE), &LobbyEventSystem::Ans_RoomCreate);
		GameEvent->network->Mapping(this, static_cast<int>(PROTOCOL::ROOM_IN), &LobbyEventSystem::Ans_RoomIn);
		GameEvent->network->Mapping(this, static_cast<int>(PROTOCOL::ROOM_IN_EVENT), &LobbyEventSystem::Ans_RoomInEvent);
		GameEvent->network->Mapping(this, static_cast<int>(PROTOCOL::ROOM_OUT_EVENT), &LobbyEventSystem::Ans_RoomOutEvent);
		GameEvent->network->Mapping(this, static_cast<int>(PROTOCOL::READY), &LobbyEventSystem::Ans_ReadyFlag);
		GameEvent->network->Mapping(this, static_cast<int>(PROTOCOL::ROOM_LOAD_EVENT), &LobbyEventSystem::Ans_ChangeScene);
	}

	int loginCookie = GetPrivateProfileIntW(L"Login", L"CODE", -1, CookiePath.c_str());
	IsLogin = loginCookie > 0;

	g_DataManager->LoadTexture(L"UI5_RankA_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankB_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankC_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankD_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankE_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankF_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankS_Image.png");

	ChangeState(eState::Main);
	std::call_once(TitleFlag, [&] {ChangeState(LobbyEventSystem::eState::Title); });
}

void LobbyEventSystem::FixedUpdate()
{
	if (Input->GetKeyDown(KeyCode::NumPad0))
	{
		if (BGM->GetState() == DirectX::SoundState::PLAYING)
		{
			BGM->Stop();
		}
		else
		{
			BGM->Play();
		}
	}
	
	if (State == eState::Title)
	{
		if (BackLogo)
		{
			BackLogo->SetNowTick(0.f);
		}
		if (Input->AnyKeyDown())
		{
			ChangeState(eState::Main);
		}
		return;
	}

	if (BtnClick(CreditOpen))
	{
		ChangeState(eState::Credit);
	}

	if (BtnClick(LoginOpen))
	{
		if (!IsLogin)
		{
			ChangeState(eState::Login);
		}
	}

	if (BtnClick(CreditOpen))
	{
		ChangeState(eState::CreateAccount);
	}

	if (BtnClick(LoginFailOK))
	{
		ChangeState(eState::Login);
	}

	if (BtnClick(LoginClose))
	{
		ChangeState(eState::Main);
	}

	if (BtnClick(LoginCreateAccount))
	{
		ChangeState(eState::CreateAccount);
	}

	if (BtnClick(CreateAccountFailOk))
	{
		ChangeState(eState::CreateAccount);
	}

	if (BtnClick(CreateAccountSuccessOk))
	{
		ChangeState(eState::Login);
	}

	if (BtnClick(CreateAccountClose))
	{
		ChangeState(eState::Login);
	}

	if (BtnClick(StoryOpen))
	{
		ChangeState(eState::StoryMode);
	}

	if (BtnClick(StoryToArcade))
	{
		if (IsLogin)
		{
			int ret_code;
			ret_code = GetPrivateProfileInt(L"Login", L"CODE", -1, CookiePath.c_str());

			if (GameNet->IsConnection())
			{
				ChangeState(eState::ArcadeMode);
			}
			else
			{
				if (ret_code >= 0)
				{
					GameNet->UnExConnect(L"221.163.91.109", 9445);
				}
				else
				{
					NetLogger::GetIns()->Write(L"net", L"로그인을 먼저 하세요");
					/// TODO : Login Fail PopUp
				}
			}
		}
	}

	if (BtnClick(StoryClose))
	{
		ChangeState(eState::Main);
	}

	if (BtnClick(ArcadeOpen))
	{
		if (IsLogin)
		{
			int ret_code;
			ret_code = GetPrivateProfileInt(L"Login", L"CODE", -1, CookiePath.c_str());

			if (GameNet->IsConnection())
			{
				ChangeState(eState::ArcadeMode);
			}
			else
			{
				if (ret_code >= 0)
				{
					GameNet->UnExConnect(L"221.163.91.109", 9445);
				}
				else
				{
					NetLogger::GetIns()->Write(L"net", L"로그인을 먼저 하세요");
					/// TODO : Login Fail PopUp
				}
			}
		}
	}

	if (BtnClick(ArcadeToStory))
	{
		if (IsRoom)
		{
			Network->Disconnect(GameNet->GetToken()->GetSessionId());
		}
		ChangeState(eState::StoryMode);
	}

	if (BtnClick(ArcadeClose))
	{
		if (IsRoom)
		{
			Network->Disconnect(GameNet->GetToken()->GetSessionId());
		}
		ChangeState(eState::Main);
	}

	if (BtnClick(MenuSettingOpen))
	{
		SettingPopupOpen();
	}

	//TODO : stroy,arcade mode에서 세팅변경할 수 있도록
	/*if (BtnClick(SettingOpen))
	{
		SettingPopupOpne();
	}*/

	if (BtnClick(Check[0]))
	{
		if (CheckON[0])
		{
			CheckON[0]->m_GameObject->SetActive(true);
		}
		Check[0]->SetInteractable(false);
	}

	if (BtnClick(CheckON[0]))
	{
		if (Check[0])
		{
			Check[0]->SetInteractable(true);
		}
		CheckON[0]->m_GameObject->SetActive(false);
	}

	if (BtnClick(Check[1]))
	{
		if (CheckON[1])
		{
			CheckON[1]->m_GameObject->SetActive(true);
		}
		Check[1]->SetInteractable(false);
	}

	if (BtnClick(CheckON[1]))
	{
		if (Check[1])
		{
			Check[1]->SetInteractable(true);
		}
		CheckON[1]->m_GameObject->SetActive(false);
	}

	if (BtnClick(SettingQuit))
	{
		QuitPopup->SetActive(true);
	}

	if (BtnClick(SettingClose))
	{
		SettingPopupClose();
	}

	if (BtnClick(SettingSubmit))
	{
		if (g_Renderer)
		{
			if (Check[0])
			{
				bool b = Check[0]->GetInteractable();
				g_Renderer->SetMotionBlurState(!b);
			}
			if (Check[1])
			{
				bool b = Check[1]->GetInteractable();
				g_Renderer->SetFogState(!b);
			}
		}
		string str = VolumeInput->GetText();
		if (str.size() > 0)
		{
			int volume = Min(Max(std::stoi(str), 0), 100);
			g_Audio->SetMasterVolume(volume / 100.f);
		}
		SettingPopupClose();
	}

	//ESC 처리
	if (Input->GetKeyDown(KeyCode::Escape))
	{
		if (QuitPopup)
		{
			if (QuitPopup->GetActive())
			{
				QuitPopup->SetActive(false);
			}
			else
			{
				switch (State)
				{
				case LobbyEventSystem::eState::Main:
					SettingPopupClose();
					break;
				case LobbyEventSystem::eState::StoryMode:
					ChangeState(eState::Main);
					break;
				case LobbyEventSystem::eState::Login:
					ChangeState(eState::Main);
					break;
				case LobbyEventSystem::eState::CreateAccount:
					ChangeState(eState::Login);
					break;
				case LobbyEventSystem::eState::ArcadeMode:
					ChangeState(eState::Main);
					break;
				case LobbyEventSystem::eState::Wait:
					break;
				case LobbyEventSystem::eState::Credit:
					ChangeState(eState::Main);
					break;
				}
			}
		}

	}
}

void LobbyEventSystem::Update()
{
	if (MenuSelecter)
	{
		Vector3 pos = Vector3(0, -100, 0);
		if (LoginOpen)
		{
			UIButton::eState state = LoginOpen->GetState();
			if (LoginOpen->OnHover())
			{
				MouseHoverSFX->Stop();
				MouseHoverSFX->Play();
			}

			if (state == UIButton::eState::Hover || state == UIButton::eState::Pressed)
			{
				pos = LoginOpen->m_GameObject->transform->GetPosition();
			}
		}

		if (StoryOpen)
		{
			UIButton::eState state = StoryOpen->GetState();

			if (StoryOpen->OnHover())
			{
				MouseHoverSFX->Stop();
				MouseHoverSFX->Play();
			}

			if (state == UIButton::eState::Hover || state == UIButton::eState::Pressed)
			{
				pos = StoryOpen->m_GameObject->transform->GetPosition();
			}
		}
		if (ArcadeOpen)
		{
			UIButton::eState state = ArcadeOpen->GetState();

			if (ArcadeOpen->OnHover())
			{
				MouseHoverSFX->Stop();
				MouseHoverSFX->Play();
			}

			if (state == UIButton::eState::Hover || state == UIButton::eState::Pressed)
			{
				pos = ArcadeOpen->m_GameObject->transform->GetPosition();
			}
		}

		if (MenuSettingOpen)
		{
			UIButton::eState state = MenuSettingOpen->GetState();

			if (MenuSettingOpen->OnHover())
			{
				MouseHoverSFX->Stop();
				MouseHoverSFX->Play();
			}

			if (state == UIButton::eState::Hover || state == UIButton::eState::Pressed)
			{
				pos = MenuSettingOpen->m_GameObject->transform->GetPosition();
			}
		}

		MenuSelecter->transform->SetPosition(Vector3(MenuSelecter->transform->GetPosition().x, pos.y, 0));
	}

	if (BtnClick(LoginSubmit))
	{
		WritePrivateProfileString(L"Login", L"CERTIFICATION", L"FALSE", CookiePath.c_str());
		string id, pw;

		if (LoginID)
		{
			id = LoginID->GetText();
			WritePrivateProfileString(L"Login", L"ID", id.c_str(), CookiePath.c_str());
		}
		if (LoginPW)
		{
			pw = LoginPW->GetText();
			WritePrivateProfileString(L"Login", L"PW", pw.c_str(), CookiePath.c_str());
		}

		//여기서 로그인
		if (m_GameObject->network)
		{
			// 로그인 서버와 연결시도
			m_GameObject->network->UnExConnect(L"221.163.91.109", 9547);
			LoginState = eLoginState::LOGIN;
		}
	}

	if (BtnClick(CreateAccountSubmit))
	{
		string id, pw;
		if (CreateAccountID)
		{
			id = CreateAccountID->GetText();
		}
		if (CreateAccountPW)
		{
			pw = CreateAccountPW->GetText();
		}
		//여기서 회원가입
		if (m_GameObject->network)
		{
			// 로그인 서버와 연결시도
			m_GameObject->network->UnExConnect(L"221.163.91.109", 9547);
			LoginState = eLoginState::SIGNUP;
		}
	}

	if (BtnClick(StoryGameStart))
	{
		SceneManagement->ChangeScene(1);
	}

	if (StoryModeHardRock)
	{
		UIButton::eState state = StoryModeHardRock->GetState();

		if (StoryModeHardRock->OnHover())
		{
			MouseHoverSFX->Stop();
			MouseHoverSFX->Play();
		}

		if (state == UIButton::eState::Hover || state == UIButton::eState::Pressed)
		{
			StoryModeHardRockTooltip->SetActive(true);
		}
		else
		{
			StoryModeHardRockTooltip->SetActive(false);
		}
	}

	if (StoryModeDouble)
	{
		UIButton::eState state = StoryModeDouble->GetState();

		if (StoryModeDouble->OnHover())
		{
			MouseHoverSFX->Stop();
			MouseHoverSFX->Play();
		}

		if (state == UIButton::eState::Hover || state == UIButton::eState::Pressed)
		{
			StoryModeDoubleTooltip->SetActive(true);
		}
		else
		{
			StoryModeDoubleTooltip->SetActive(false);
		}
	}

	if (StoryModeHidden)
	{
		UIButton::eState state = StoryModeHidden->GetState();

		if (StoryModeHidden->OnHover())
		{
			MouseHoverSFX->Stop();
			MouseHoverSFX->Play();
		}

		if (state == UIButton::eState::Hover || state == UIButton::eState::Pressed)
		{
			StoryModeHiddenTooltip->SetActive(true);
		}
		else
		{
			StoryModeHiddenTooltip->SetActive(false);
		}
	}

	if (BtnClick(ArcadeCreateRoom))
	{
		IsHost = true;
		ArcadeCreateRoom->m_GameObject->SetActive(false);
		if (ArcadeEnter)
		{
			ArcadeEnter->m_GameObject->SetActive(false);
		}
		Req_CreateRoom();
	}

	if (BtnClick(ArcadeEnter))
	{
		IsHost = false;
		if (ArcadeCreateRoom)
		{
			ArcadeCreateRoom->m_GameObject->SetActive(false);
		}
		ArcadeEnter->m_GameObject->SetActive(false);
		ArcadeReady->m_GameObject->SetActive(true);
		ArcadeStart->m_GameObject->SetActive(false);
		Req_EnterRoom();
	}

	if (BtnClick(ArcadeReady))
	{
		IsReady = !IsReady;
		Req_ReadyFlag(IsReady);
	}

	if (BtnClick(ArcadeStart))
	{
		Req_ReadyFlag(true);
	}

	if (BtnClick(QuitOk))
	{
		SceneManagement->Quit();
	}

	if (BtnClick(QuitNo))
	{
		QuitPopup->SetActive(false);
	}
}

void LobbyEventSystem::ChangeState(eState state)
{
	State = state;
	switch (State)
	{
	case LobbyEventSystem::eState::Title:
		WritePrivateProfileString(L"Login", L"ID", L"", CookiePath.c_str());
		WritePrivateProfileString(L"Login", L"PW", L"", CookiePath.c_str());
		WritePrivateProfileString(L"Login", L"CODE", L"-1", CookiePath.c_str());
		WritePrivateProfileString(L"Login", L"CERTIFICATION", L"", CookiePath.c_str());
		WritePrivateProfileString(L"Login", L"ISHOST", L"", CookiePath.c_str());
		//Inactive
		if (MenuPopUp)
		{
			MenuPopUp->SetActive(false);
		}
		if (LoginPopup)
		{
			LoginPopup->SetActive(false);
		}
		if (CreateAccountPopup)
		{
			CreateAccountPopup->SetActive(false);
		}
		if (StoryPopup)
		{
			StoryPopup->SetActive(false);
		}
		if (ArcadePopup)
		{
			ArcadePopup->SetActive(false);
		}
		if (ArcadeOpen)
		{
			ArcadeOpen->SetInteractable(false);
		}
		if (SettingPopup)
		{
			SettingPopup->SetActive(false);
		}
		if (CreditPopup)
		{
			CreditPopup->SetActive(false);
		}
		if (QuitPopup)
		{
			QuitPopup->SetActive(false);
		}

		//Active
		if (BackGroundBlink)
		{
			BackGroundBlink->SetActive(true);
		}
		for (int i = 0; i < 4; i++)
		{
			if (BackBlink[i])
			{
				BackBlink[i]->SetNowTick(i * 10.f);
			}
		}
		if (TitlePopup)
		{
			TitlePopup->SetActive(true);
		}
		break;
	case LobbyEventSystem::eState::Main:
	{
		//LoginCheck
		int loginCookie = GetPrivateProfileIntW(L"Login", L"CODE", -1, CookiePath.c_str());
		IsLogin = loginCookie > 0;

		//Inactive
		if (TitlePopup)
		{
			TitlePopup->SetActive(false);
		}
		if (LoginPopup)
		{
			LoginPopup->SetActive(false);
		}
		if (CreateAccountPopup)
		{
			CreateAccountPopup->SetActive(false);
		}
		if (StoryPopup)
		{
			StoryPopup->SetActive(false);
		}
		if (ArcadePopup)
		{
			ArcadePopup->SetActive(false);
		}
		if (SettingPopup)
		{
			SettingPopup->SetActive(false);
		}
		if (CreditPopup)
		{
			CreditPopup->SetActive(false);
		}
		if (QuitPopup)
		{
			QuitPopup->SetActive(false);
		}
		if (SettingOpen)
		{
			SettingOpen->m_GameObject->SetActive(false);
		}
		if (BackGroundBlink)
		{
			BackGroundBlink->SetActive(true);
		}
		if (IsLogin)
		{
			if (LoginOpen)
			{
				LoginOpen->SetInteractable(false);
				ArcadeOpen->SetInteractable(true);
			}
		}
		else
		{
			if (ArcadeOpen)
			{
				LoginOpen->SetInteractable(true);
				ArcadeOpen->SetInteractable(false);
			}
		}

		//Active
		if (MenuPopUp)
		{
			MenuPopUp->SetActive(true);
		}
	}
	break;
	case LobbyEventSystem::eState::StoryMode:
	{
		//Inactive
		if (MenuPopUp)
		{
			MenuPopUp->SetActive(false);
		}
		if (ArcadePopup)
		{
			ArcadePopup->SetActive(false);
		}
		if (SettingPopup)
		{
			SettingPopup->SetActive(false);
		}
		if (CreditPopup)
		{
			CreditPopup->SetActive(false);
		}
		if (QuitPopup)
		{
			QuitPopup->SetActive(false);
		}
		if (BackGroundBlink)
		{
			BackGroundBlink->SetActive(false);
		}
		if (StoryStageExplain)
		{
			StoryStageExplain->SetText(L"");
		}

		//Active
		if (StoryPopup)
		{
			StoryPopup->SetActive(true);
		}
		if (SettingOpen)
		{
			SettingOpen->m_GameObject->SetActive(true);
		}

		int rank = GetPrivateProfileIntW(L"1", L"Rank", -1, CookiePath.c_str());
		int score = GetPrivateProfileIntW(L"1", L"BestScore", -1, CookiePath.c_str());
		int combo = GetPrivateProfileIntW(L"1", L"MaxCombo", -1, CookiePath.c_str());

		if (StoryRank && StoryRank->uiimage)
		{
			Color c;
			if (rank < 0)
			{
				c = Color(0, 0, 0, 0);
				StoryRank->uiimage->SetColor(c);
			}
			else
			{
				c = Color(1, 1, 1, 1);
				StoryRank->uiimage->SetColor(c);
			}
			switch (rank)
			{
			case 0:
				StoryRank->uiimage->SetTexture(L"UI5_RankF_Image.png");
				break;
			case 1:
				StoryRank->uiimage->SetTexture(L"UI5_RankE_Image.png");
				break;
			case 2:
				StoryRank->uiimage->SetTexture(L"UI5_RankD_Image.png");
				break;
			case 3:
				StoryRank->uiimage->SetTexture(L"UI5_RankC_Image.png");
				break;
			case 4:
				StoryRank->uiimage->SetTexture(L"UI5_RankB_Image.png");
				break;
			case 5:
				StoryRank->uiimage->SetTexture(L"UI5_RankA_Image.png");
				break;
			case 6:
				StoryRank->uiimage->SetTexture(L"UI5_RankS_Image.png");
				break;
			default:
				StoryRank->uiimage->SetIsTexture(false);
				break;
			}
		}

		if (StoryScore)
		{
			if (0 < score)
			{
				StoryScore->SetText(std::to_wstring(score));
			}
			else
			{
				StoryScore->SetText(L"");
			}
		}

		if (StoryCombo)
		{
			if (0 < combo)
			{
				StoryCombo->SetText(std::to_wstring(combo));
			}
			else
			{
				StoryCombo->SetText(L"");
			}
		}
	}
		break;
	case LobbyEventSystem::eState::Login:
		//Inactive
		if (MenuPopUp)
		{
			MenuPopUp->SetActive(false);
		}
		if (CreateAccountPopup)
		{
			CreateAccountPopup->SetActive(false);
		}
		if (LoginFailPopup)
		{
			LoginFailPopup->SetActive(false);
		}

		//Active
		if (LoginPopup)
		{
			LoginPopup->SetActive(true);
		}
		if (LoginClose)
		{
			LoginClose->SetInteractable(true);
		}
		if (LoginID)
		{
			LoginID->SetInteractable(true);
		}
		if (LoginIDText)
		{
			LoginIDText->SetText(L"");
		}
		if (LoginPW)
		{
			LoginPW->SetInteractable(true);
		}
		if (LoginPWText)
		{
			LoginPWText->SetText(L"");
		}
		if (LoginSubmit)
		{
			LoginSubmit->SetInteractable(true);
		}
		if (LoginCreateAccount)
		{
			LoginCreateAccount->SetInteractable(true);
		}
		break;
	case LobbyEventSystem::eState::CreateAccount:
		//Inactive
		if (LoginClose)
		{
			LoginClose->SetInteractable(false);
		}
		if (LoginID)
		{
			LoginID->SetInteractable(false);
		}
		if (LoginPW)
		{
			LoginPW->SetInteractable(false);
		}
		if (LoginSubmit)
		{
			LoginSubmit->SetInteractable(false);
		}
		if (LoginCreateAccount)
		{
			LoginCreateAccount->SetInteractable(false);
		}
		if (CreateAccountFailPopup)
		{
			CreateAccountFailPopup->SetActive(false);
		}
		if (CreateAccountSuccessPopup)
		{
			CreateAccountSuccessPopup->SetActive(false);
		}

		//Active
		if (CreateAccountPopup)
		{
			CreateAccountPopup->SetActive(true);
		}

		if (CreateAccountClose)
		{
			CreateAccountClose->SetInteractable(true);
		}
		if (CreateAccountID)
		{
			CreateAccountID->SetInteractable(true);
		}
		if (CreateAccountIDText)
		{
			CreateAccountIDText->SetText(L"");
		}
		if (CreateAccountPW)
		{
			CreateAccountPW->SetInteractable(true);
		}
		if (CreateAccountPWText)
		{
			CreateAccountPWText->SetText(L"");
		}
		if (CreateAccountSubmit)
		{
			CreateAccountSubmit->SetInteractable(true);
		}
		break;
	case LobbyEventSystem::eState::ArcadeMode:
		//Inactive
		if (MenuPopUp)
		{
			MenuPopUp->SetActive(false);
		}
		if (StoryPopup)
		{
			StoryPopup->SetActive(false);
		}
		if (SettingPopup)
		{
			SettingPopup->SetActive(false);
		}
		if (CreditPopup)
		{
			CreditPopup->SetActive(false);
		}
		if (QuitPopup)
		{
			QuitPopup->SetActive(false);
		}
		if (BackGroundBlink)
		{
			BackGroundBlink->SetActive(false);
		}

		//Active
		if (SettingOpen)
		{
			SettingOpen->m_GameObject->SetActive(true);
		}
		if (ArcadePopup)
		{
			ArcadePopup->SetActive(true);
		}
		if (ArcadeID)
		{
			wchar_t szSrc[MAX_PATH + 1];
			GetPrivateProfileStringW(L"Login", L"ID", L"HAHA", szSrc, MAX_PATH, CookiePath.c_str());

			ArcadeID->SetText(szSrc);
		}
		if (ArcadeOtherID)
		{
			ArcadeOtherID->SetText(L"");
		}

		if (ArcadeCreateRoom)
		{
			ArcadeCreateRoom->m_GameObject->SetActive(true);
		}
		if (ArcadeEnter)
		{
			ArcadeEnter->m_GameObject->SetActive(true);
		}
		if (ArcadeInputID)
		{
			ArcadeInputID->SetInteractable(true);
		}

		IsReady = false;
		if (ArcadeReady)
		{
			ArcadeReady->m_GameObject->SetActive(false);
		}
		if (ArcadeStart)
		{
			ArcadeStart->m_GameObject->SetActive(true);
		}

		break;
	case LobbyEventSystem::eState::Wait:
		if (LoginOpen)
		{
			LoginOpen->SetInteractable(false);
		}
		if (ArcadeOpen)
		{
			ArcadeOpen->SetInteractable(false);
		}
		break;
	case LobbyEventSystem::eState::Credit:
		//Inactive
		if (MenuPopUp)
		{
			MenuPopUp->SetActive(false);
		}
		if (LoginPopup)
		{
			LoginPopup->SetActive(false);
		}
		if (CreateAccountPopup)
		{
			CreateAccountPopup->SetActive(false);
		}
		if (StoryPopup)
		{
			StoryPopup->SetActive(false);
		}
		if (ArcadePopup)
		{
			ArcadePopup->SetActive(false);
		}
		if (SettingPopup)
		{
			SettingPopup->SetActive(false);
		}
		if (QuitPopup)
		{
			QuitPopup->SetActive(false);
		}

		//Active
		if (CreditPopup)
		{
			CreditPopup->SetActive(true);
		}
		break;
	default:
		break;
	}
}

void LobbyEventSystem::SettingPopupOpen()
{
	if (g_Audio)
	{
		int volume = (int)(g_Audio->GetMasterVolume() * 100);
		VolumeInputText->SetText(std::to_wstring(volume));
		VolumeText->SetText(std::to_wstring(volume));
	}

	if (g_Renderer)
	{
		bool b = g_Renderer->GetMotionBlurState();
		if (CheckON[0])
		{
			CheckON[0]->m_GameObject->SetActive(b);
		}
		Check[0]->SetInteractable(!b);

		b = g_Renderer->GetFogState();
		if (CheckON[1])
		{
			CheckON[1]->m_GameObject->SetActive(b);
		}
		Check[1]->SetInteractable(!b);
	}

	if (SettingPopup)
	{
		SettingPopup->SetActive(true);
		switch (State)
		{
		case LobbyEventSystem::eState::Title:
			break;
		case LobbyEventSystem::eState::Main:
			MenuPopUp->SetActive(false);
			break;
		case LobbyEventSystem::eState::StoryMode:
			break;
		case LobbyEventSystem::eState::Login:
			break;
		case LobbyEventSystem::eState::CreateAccount:
			break;
		case LobbyEventSystem::eState::ArcadeMode:
			break;
		case LobbyEventSystem::eState::Wait:
			break;
		case LobbyEventSystem::eState::Credit:
			break;
		}
	}
}

void LobbyEventSystem::SettingPopupClose()
{
	if (SettingPopup)
	{
		SettingPopup->SetActive(false);
		switch (State)
		{
		case LobbyEventSystem::eState::Title:
			break;
		case LobbyEventSystem::eState::Main:
			MenuPopUp->SetActive(true);
			break;
		case LobbyEventSystem::eState::StoryMode:
			break;
		case LobbyEventSystem::eState::Login:
			break;
		case LobbyEventSystem::eState::CreateAccount:
			break;
		case LobbyEventSystem::eState::ArcadeMode:
			break;
		case LobbyEventSystem::eState::Wait:
			break;
		case LobbyEventSystem::eState::Credit:
			break;
		}
	}
}

bool LobbyEventSystem::BtnClick(hos::com::UIButton* btn)
{
	if (btn)
	{
		if (btn->IsClick())
		{
			if (MouseClickSFX)
			{
				MouseClickSFX->Play();
			}
			return true;
		}
	}
	return false;
}

void LobbyEventSystem::Req_Login()
{
	LoginClose->SetInteractable(false);
	wchar_t szSrc[MAX_PATH];

	string loginid;
	string loginpw;

	ZeroMemory(szSrc, MAX_PATH);
	GetPrivateProfileString(L"Login", L"ID", L"HAHA", szSrc, MAX_PATH, CookiePath.c_str());

	loginid = szSrc;

	ZeroMemory(szSrc, MAX_PATH);
	GetPrivateProfileString(L"Login", L"PW", L"HEHE", szSrc, MAX_PATH, CookiePath.c_str());
	loginpw = szSrc;

	flatbuffers::FlatBufferBuilder builder;

	int no_ = 0;
	int code_ = 0;

	auto id_ = builder.CreateString(ut::UTF16ToAnsi(loginid.c_str()));
	auto pw_ = builder.CreateString(ut::UTF16ToAnsi(loginpw.c_str()));

	auto protocol = CreateIdPw(builder, no_, id_, pw_, code_);

	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::LOGIN);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	m_GameObject->network->SendMsg(pack);
}

void LobbyEventSystem::Req_SingUp()
{
	CreateAccountClose->SetInteractable(false);
	NetLogger::GetIns()->Write(L"Client", L"회원가입을 요청 합니다.");

	flatbuffers::FlatBufferBuilder builder;

	int no_ = 0;
	int code_ = 0;
	mbstring id = ut::UTF16ToAnsi(CreateAccountID->GetText());
	mbstring pw = ut::UTF16ToAnsi(CreateAccountPW->GetText());
	auto id_ = builder.CreateString(id);
	auto pw_ = builder.CreateString(pw);

	auto protocol = CreateIdPw(builder, no_, id_, pw_, code_);

	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::SIGNUP);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	m_GameObject->network->SendMsg(pack);
}

void LobbyEventSystem::Req_Withdrawl()
{
}

void LobbyEventSystem::Req_Certification()
{
	//로그인 상태 체크
	string loginid;
	string loginpw;
	wchar_t szSrc[MAX_PATH];

	ZeroMemory(szSrc, MAX_PATH);
	GetPrivateProfileString(L"Login", L"ID", L"HAHA", szSrc, MAX_PATH, CookiePath.c_str());
	loginid = szSrc;

	ZeroMemory(szSrc, MAX_PATH);
	GetPrivateProfileString(L"Login", L"PW", L"HEHE", szSrc, MAX_PATH, CookiePath.c_str());
	loginpw = szSrc;

	int retcode = GetPrivateProfileInt(L"Login", L"CODE", -100, CookiePath.c_str());

	if (retcode < 0)
	{
		NetLogger::GetIns()->Write(L"net", L"먼저 로그인을 하세요");
		Network->Disconnect(m_GameObject->network->GetToken()->GetSessionId());
	}
	else
	{
		NetLogger::GetIns()->Write(L"net", L"인증을 요청합니다.");
		flexbuffers::Builder builder;

		builder.Map([&] {
			builder.String("Id", ut::UTF16ToAnsi(loginid.c_str()));
			builder.String("Pw", ut::UTF16ToAnsi(loginpw.c_str()));
			builder.Int("UUID", retcode);
			});
		builder.Finish();

		const uint8_t* buffer = builder.GetBuffer().data();

		PACKET pack;
		pack.head.size = PACKET_HEADER_SIZE;
		pack.head.protocol = static_cast<int>(PROTOCOL::CERTIFICATION);
		pack.Make(pack, pack.head, buffer, (int)builder.GetSize());

		GameNet->SendMsg(pack);
	}
}

void LobbyEventSystem::Req_CreateRoom()
{
	//방 만들기
	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.String("UserId", ut::UTF16ToAnsi(ArcadeID->GetText()));
		});
	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_ENABLE);
	pack.Make(pack, pack.head, buffer, (int)builder.GetSize());
	GameNet->SendMsg(pack);
}

void LobbyEventSystem::Req_EnterRoom()
{
	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.String("RoomName", ut::UTF16ToAnsi(ArcadeInputID->GetText()));
		builder.String("UserId", ut::UTF16ToAnsi(ArcadeID->GetText()));
		});
	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_IN);
	pack.Make(pack, pack.head, buffer, (int)builder.GetSize());
	GameNet->SendMsg(pack);
}

void LobbyEventSystem::Req_RoomIn(hos::string room_name)
{
	mbstring rname = ut::UTF16ToAnsi(room_name);
	//NetLogger::GetIns()->Write(L"Client", L"방 입장 요청   ", ut::UTF16ToAnsi(room_name));

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.String("RoomName", rname);
		builder.String("UserId", ut::UTF16ToAnsi(ArcadeID->GetText()));
		});
	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::ROOM_IN);
	pack.Make(pack, pack.head, buffer, (int)builder.GetSize());
	GameNet->SendMsg(pack);
}

void LobbyEventSystem::Req_ReadyFlag(bool isready)
{
	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.String("Id", ut::UTF16ToAnsi(ArcadeID->GetText()));
		builder.Bool("IsReady", isready);
		});
	builder.Finish();

	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::READY);
	pack.Make(pack, pack.head, buffer, (int)builder.GetSize());
	GameNet->SendMsg(pack);
}

void LobbyEventSystem::OnConnect_Login(PACKAGE& package)
{
	switch (LoginState)
	{
	case LobbyEventSystem::eLoginState::LOGIN:
		Req_Login();
		break;
	case LobbyEventSystem::eLoginState::SIGNUP:
		Req_SingUp();
		break;
	case LobbyEventSystem::eLoginState::WITHDRAWL:
		Req_Withdrawl();
		break;
	}
}

void LobbyEventSystem::OnConnectFail_Login(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"net", L"Login Server Connect Fail... ");

	WritePrivateProfileString(L"Login", L"ID", L"", CookiePath.c_str());
	WritePrivateProfileString(L"Login", L"PW", L"", CookiePath.c_str());
	WritePrivateProfileString(L"Login", L"CODE", L"", CookiePath.c_str());
	WritePrivateProfileString(L"Login", L"CERTIFICATION", L"", CookiePath.c_str());

	// Connect Retry
}

void LobbyEventSystem::OnDisconnect_Login(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"net", L"Disconnect Login Server");

	WritePrivateProfileString(L"Login", L"ID", L"", CookiePath.c_str());
	WritePrivateProfileString(L"Login", L"PW", L"", CookiePath.c_str());
	WritePrivateProfileString(L"Login", L"CODE", L"", CookiePath.c_str());
	WritePrivateProfileString(L"Login", L"CERTIFICATION", L"", CookiePath.c_str());
}

void LobbyEventSystem::OnConnect_Game(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"net", L"Game Server Connect Sucess !! ");
	Req_Certification();
}

void LobbyEventSystem::OnConnectFail_Game(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"net", L"Game Server Connect Fail... ");

	// Connect Retry
	WritePrivateProfileString(L"Login", L"CERTIFICATION", L"", CookiePath.c_str());
}

void LobbyEventSystem::OnDisconnect_Game(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"net", L"Disconnect Game Server");

	WritePrivateProfileString(L"Login", L"CERTIFICATION", L"", CookiePath.c_str());
}

void LobbyEventSystem::Ans_Login(PACKAGE& package)
{
	// Login Sucess or Login Fail 

	// code -1 : 없는 유저
	// code -2  : 이미 로그인 함.

	//.. Dummy Code
	auto onecode = GetOneCode(package.packet.memory + PACKET_HEADER_SIZE);
	int code = onecode->code();

	WritePrivateProfileString(L"Login", L"CODE", std::to_wstring(code).c_str(), CookiePath.c_str());
	if (code > 0)
	{
		NetLogger::GetIns()->Write(L"Client", L"로그인 성공, UId : ", code);
		ChangeState(eState::Main);
	}
	else
	{
		switch (code)
		{
		case -1:
			NetLogger::GetIns()->Write(L"Client", L"없는 계정입니다.");
			break;
		case -2:
			NetLogger::GetIns()->Write(L"Client", L"이미 로그인 했습니다.");
			break;
		default:
			NetLogger::GetIns()->Write(L"Client", L"잘못 된 접근입니다.");
			break;
		}
		Network->Disconnect(m_GameObject->network->GetToken()->GetSessionId());
		LoginFailPopup->SetActive(true);
		if (ErrorSFX)
		{
			ErrorSFX->Play();
		}
	}
}

void LobbyEventSystem::Ans_Signup(PACKAGE& package)
{
	// Signup Sucess or Signup Fail

	// code -1, -2 : id / pw 정규식 실패 잘못된 아이디 입니다.

	// code -3 :  이미 있는 아이디 임

	auto onecode = GetOneCode(package.packet.memory + PACKET_HEADER_SIZE);
	int code = onecode->code();

	/// Dummy Code...

	if (code > 0)
	{
		NetLogger::GetIns()->Write(L"Client", L"회원가입 성공, UId : ", code);
		if (CreateAccountSuccessPopup)
		{
			CreateAccountSuccessPopup->SetActive(true);
		}
	}
	else
	{
		switch (code)
		{
		case -1:
			NetLogger::GetIns()->Write(L"Client", L"없는 계정입니다.");
			break;
		case -2:
			NetLogger::GetIns()->Write(L"Client", L"이미 로그인 했습니다.");
			break;
		case -3:
			NetLogger::GetIns()->Write(L"Client", L"이미 로그인 했습니다.");
			break;
		default:
			NetLogger::GetIns()->Write(L"Client", L"잘못 된 접근입니다.");
			break;
		}
		if (CreateAccountFailPopup)
		{
			CreateAccountFailPopup->SetActive(true);
		}
		if (ErrorSFX)
		{
			ErrorSFX->Play();
		}
	}
	Network->Disconnect(m_GameObject->network->GetToken()->GetSessionId());
}

void LobbyEventSystem::Ans_Withdrawl(PACKAGE& package)
{
	// Withdrawl Sucess or Withdrawl Fail

	// code -1 : 없는 유저
	// code -2 : 이미 탈퇴한 유저

	/// Dummy Code...

	auto onecode = GetOneCode(package.packet.memory + PACKET_HEADER_SIZE);
	int code = onecode->code();

	if (code > 0)
	{
		NetLogger::GetIns()->Write(L"Client", L"회원탈퇴 성공, UId : ", code);
	}
	else
	{
		switch (code)
		{
		case -1:
			NetLogger::GetIns()->Write(L"Client", L"없는 계정입니다.");
			break;
		case -2:
			NetLogger::GetIns()->Write(L"Client", L"이미 로그인 했습니다.");
			break;
		default:
			NetLogger::GetIns()->Write(L"Client", L"잘못 된 접근입니다.");
			break;
		}
	}
	Network->Disconnect(m_GameObject->network->GetToken()->GetSessionId());
}

void LobbyEventSystem::Ans_Certification(PACKAGE& package)
{
	// 게임 인증 성공~!!
	NetLogger::GetIns()->Write(L"net", L"Game Server Certification Sucess !! ");
	ChangeState(eState::ArcadeMode);
}

void LobbyEventSystem::Ans_RoomCreate(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	__int64 roomno = m["RoomNo"].AsInt64();
	std::string roomname = m["RoomName"].AsString().c_str();

	if (roomno <= 0)
	{
		NetLogger::GetIns()->Write(L"Client", L"방 생성 실패");
		ArcadeCreateRoom->m_GameObject->SetActive(true);
		ArcadeEnter->m_GameObject->SetActive(true);
	}
	else
	{
		NetLogger::GetIns()->Write(L"Client", L"방 생성 성공");
		if (ArcadeInputID)
		{
			ArcadeInputID->SetInteractable(false);
		}
		if (ArcadeOtherID)
		{
			ArcadeOtherID->SetText(L"");
		}
		
		Req_RoomIn(ut::AnsiToUTF16(roomname));
	}
}

void LobbyEventSystem::Ans_RoomIn(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	__int64 roomno = m["RoomNo"].AsInt64();
	std::string roomname = m["RoomName"].AsString().c_str();
	std::string userid = m["UserID"].AsString().c_str();

	if (roomno < 0)
	{
		NetLogger::GetIns()->Write(L"Client", L"방 입장 실패");
		ArcadeCreateRoom->m_GameObject->SetActive(true);
		ArcadeEnter->m_GameObject->SetActive(false);
	}
	else if (roomno == 0)
	{
		NetLogger::GetIns()->Write(L"Client", L"로비 입장");
		IsRoom = false;
		ArcadeCreateRoom->m_GameObject->SetActive(true);
		ArcadeEnter->m_GameObject->SetActive(true);
		ArcadeReady->m_GameObject->SetActive(false);
		ArcadeStart->m_GameObject->SetActive(true);
		ArcadeInputID->SetActive(true);
		ArcadeOtherID->SetText(L"");
	}
	else
	{
		//NetLogger::GetIns()->Write(L"Client", roomno, L"번 방 입장");
		NetLogger::GetIns()->Write(L"Client", L"방 입장 성공");
		IsRoom = true;
		if (IsHost)
		{
			ArcadeReady->m_GameObject->SetActive(false);
			ArcadeStart->m_GameObject->SetActive(true);
		}
		else
		{
			ArcadeReady->m_GameObject->SetActive(true);
			ArcadeStart->m_GameObject->SetActive(false);
		}
		if (SettingOpen)
		{
			SettingOpen->m_GameObject->SetActive(false);
		}
	}
}

void LobbyEventSystem::Ans_RoomInEvent(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	__int64 index = m["Index"].AsInt64();
	std::string userid = m["Id"].AsString().c_str();

	string id = ut::AnsiToUTF16(userid);
	if (id != ArcadeID->GetText())
	{
		ArcadeStart->SetInteractable(false);
		ArcadeInputID->SetInteractable(false);
		ArcadeOtherID->SetText(id);
	}
}

void LobbyEventSystem::Ans_RoomOutEvent(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	__int64 index = m["Index"].AsInt64();
	std::string userid = m["Id"].AsString().c_str();

	if (IsHost)
	{
		//ArcadeStart->SetInteractable(true);
	}
	else
	{
		ChangeState(eState::Main);
		Network->Disconnect(GameNet->GetToken()->GetSessionId());
	}
}

void LobbyEventSystem::Ans_ReadyFlag(PACKAGE& package)
{
	char* buffer = package.packet.memory + PACKET_HEADER_SIZE;
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(buffer), package.packet.head.size - PACKET_HEADER_SIZE).AsMap();

	std::string userid = m["Id"].AsString().c_str();
	bool isready = m["IsReady"].AsBool();

	string ID = ut::AnsiToUTF16(userid);
	if (ID != ArcadeID->GetText())
	{
		ArcadeStart->SetInteractable(isready);
	}
}

void LobbyEventSystem::Ans_ChangeScene(PACKAGE& package)
{
	if (IsHost)
	{
		WritePrivateProfileString(L"Login", L"ISHOST", L"T", CookiePath.c_str());
	}
	else
	{
		WritePrivateProfileString(L"Login", L"ISHOST", L"F", CookiePath.c_str());
	}
	SceneManagement->ChangeScene(2);
}