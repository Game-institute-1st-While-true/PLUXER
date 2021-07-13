#include <filesystem>
#include "UIManager.h"

using namespace hos;
using namespace com;

UIManager::UIManager() : hos::com::Script(L"UIManager")
{
	ExecutionOrder = 10;
}

UIManager::~UIManager()
{
	while (SmashStartVector.size() != 0)
	{
		SmashStartVector.pop_back();
		SmashEndVector.pop_back();
	}
}

void UIManager::Reset()
{

}

void UIManager::Awake()
{
	m_PlayerAttack = m_GameObject->m_Scene->FindGameObjectWithName(L"AttackArea")->GetComponent<PlayerAttack>();
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();

	for (int i = 0; i < m_PlayerAttack->GetSmashNoteMaxCount(); i++)
	{
		hos::Color _TempColor(1, 1, 1, 1);

		hos::com::UIImage* _TempComponent;

		hos::string _TempString = L"SmashStart";
		_TempString.append(std::to_wstring(i + 1));

		SmashStartVector.push_back(m_GameObject->m_Scene->FindGameObjectWithName(_TempString));
		_TempComponent = SmashStartVector[i]->GetComponent<hos::com::UIImage>();
		_TempComponent->SetColor(_TempColor);
		//_TempComponent->SetTexture(L"ui6_start_up");
		_TempComponent->SetActive(false);

		_TempString = L"SmashEnd";
		_TempString.append(std::to_wstring(i + 1));

		SmashEndVector.push_back(m_GameObject->m_Scene->FindGameObjectWithName(_TempString));
		_TempComponent = SmashEndVector[i]->GetComponent<hos::com::UIImage>();
		_TempComponent->SetColor(_TempColor);
		//_TempComponent->SetTexture(L"ui6_end_up");
		_TempComponent->SetActive(false);
	}

	IsSmashSetting = false;

	SmashOneBeat = m_NoteManager->GetOneBeatTime() * 2;

	SmashUIDistance = SmashEndVector[0]->transform->GetPosition().y - SmashStartVector[0]->transform->GetPosition().y;
	SmashUIDistance += 80;

	//////////////////////////////////////////////////////////////////////////
	// Player UI

	m_PlayerState = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerState>();
	m_PlayerAction = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerAction>();

	PlayerHealthBar = m_GameObject->m_Scene->FindGameObjectWithName(L"PlayerUIHPBarFront0")->GetComponent<hos::com::UIImage>();
	PlayerAccurencyObj = m_GameObject->m_Scene->FindGameObjectWithName(L"PlayerUIComboJudge");
	PlayerAccurency = PlayerAccurencyObj->GetComponent<hos::com::UIImage>();
	PlayerAccurency->SetNowTick(10000.f);

	DamageFrame = m_GameObject->m_Scene->FindGameObjectWithName(L"DamageFrame")->GetComponent<hos::com::UIImage>();
	DamageFrame->SetActive(false);
	PlayerStamina = m_GameObject->m_Scene->FindGameObjectWithName(L"PlayerUIStaminaTotalRemainText")->GetComponent<hos::com::UIText>();
	PlayerStaminaBack = m_GameObject->m_Scene->FindGameObjectWithName(L"PlayerUIStaminaTotalRemainTextBack")->GetComponent<hos::com::UIText>();

	PlayerFeverGage = m_GameObject->m_Scene->FindGameObjectWithName(L"PlayerUIFeverGauge0")->GetComponent<hos::com::UIImage>();

	PlayerComboImage = m_GameObject->m_Scene->FindGameObjectWithName(L"PlayerUIComboImage")->GetComponent<hos::com::UIImage>();
	PlayerComboImage->SetNowTick(10000.f);
	PlayerComboCount = m_GameObject->m_Scene->FindGameObjectWithName(L"PlayerUIComboCount")->GetComponent<hos::com::UIText>();
	PlayerComboCount->SetNowTick(10000.f);
	PlayerNowScore = m_GameObject->m_Scene->FindGameObjectWithName(L"PlayerUIScoreCurrentScore")->GetComponent<hos::com::UIText>();

	//////////////////////////////////////////////////////////////////////////
	// Boss Frog HP Bar

	BossFrogHPUI = m_GameObject->m_Scene->FindGameObjectWithName(L"BossUIHPBarFrontHigh")->GetComponent<hos::com::UIImage>();

	BossUIDamage = m_GameObject->m_Scene->FindGameObjectWithName(L"BossUIDamage")->GetComponent<hos::com::UIText>();
	BossUIDamageBack = m_GameObject->m_Scene->FindGameObjectWithName(L"BossUIDamageBack")->GetComponent<hos::com::UIText>();

	if (m_GameObject->m_Scene->FindGameObjectWithName(L"BossFrog"))
	{
		m_BossFrogState = m_GameObject->m_Scene->FindGameObjectWithName(L"BossFrog")->GetComponent<BossFrogState>();
	}
	else
	{
		BossFrogHPUI->m_GameObject->GetParent()->SetActive(false);
	}

	//////////////////////////////////////////////////////////////////////////
	// Game End
	auto gm = m_GameObject->m_Scene->FindGameObjectWithName(L"GameManager");

	if (gm)
	{
		m_GameManager = m_GameObject->m_Scene->FindGameObjectWithName(L"GameManager")->GetComponent<GameManager>();
		m_NetworkEvent = nullptr;
	}
	else
	{
		gm = m_GameObject->m_Scene->FindGameObjectWithName(L"Network");
		m_NetworkEvent = gm->GetComponent<ArcadeNetworkEvent>();
		m_GameManager = nullptr;
	}

	Scene* scene = m_GameObject->m_Scene;

	if (scene)
	{
		GameResultUI = scene->FindGameObjectWithName(L"Result");
		ResultOKBtn = scene->FindGameObjectWithName(L"OKButton")->GetComponent<UIButton>();
		ResultClear = scene->FindGameObjectWithName(L"Clear");
		ResultFail = scene->FindGameObjectWithName(L"Fail");
		ResultRank = scene->FindGameObjectWithName(L"Rank")->GetComponent<UIImage>();
		ResultBestScore = scene->FindGameObjectWithName(L"BestRecord")->GetComponent<UIText>();
		ResultScore = scene->FindGameObjectWithName(L"Score")->GetComponent<UIText>();
		ResultPlayTime = scene->FindGameObjectWithName(L"PlayTime")->GetComponent<UIText>();
		ResultNewRecord = scene->FindGameObjectWithName(L"NewRecord");
		ResultMaxCombo = scene->FindGameObjectWithName(L"MaxCombo")->GetComponent<UIText>();
		ResultCP = scene->FindGameObjectWithName(L"CriticalPerfect")->GetComponent<UIText>();
		ResultPerfect = scene->FindGameObjectWithName(L"Perfect")->GetComponent<UIText>();
		ResultCool = scene->FindGameObjectWithName(L"Cool")->GetComponent<UIText>();
		ResultNice = scene->FindGameObjectWithName(L"Nice")->GetComponent<UIText>();
		ResultMiss = scene->FindGameObjectWithName(L"Miss")->GetComponent<UIText>();
		resultFlag = false;

		ReturnMenuPopup = scene->FindGameObjectWithName(L"ReturnMenuPopup");
		if (ReturnMenuPopup)
		{
			ReturnMenuPopup->SetActive(false);
			ReturnMenuOK = scene->FindGameObjectWithName(L"ReturnMenuOK")->GetComponent<UIButton>();
			ReturnMenuCancel = scene->FindGameObjectWithName(L"ReturnMenuCancel")->GetComponent<UIButton>();
		}

		///Setting
		SettingPopup = scene->FindGameObjectWithName(L"SettingPopup");
		if (SettingPopup)
		{
			SettingPopup->SetActive(false);
		}
		Check[0] = scene->FindGameObjectWithName(L"MBCheck")->GetComponent<UIButton>();
		CheckON[0] = scene->FindGameObjectWithName(L"MBcheckOn")->GetComponent<UIButton>();
		Check[1] = scene->FindGameObjectWithName(L"FogCheck")->GetComponent<UIButton>();
		CheckON[1] = scene->FindGameObjectWithName(L"FogCheckOn")->GetComponent<UIButton>();
		VolumeInput = scene->FindGameObjectWithName(L"SoundInputField")->GetComponent<UIInputField>();
		VolumeInputText = scene->FindGameObjectWithName(L"SoundInputText")->GetComponent<UIText>();
		VolumeText = scene->FindGameObjectWithName(L"SoundNow")->GetComponent<UIText>();
		SettingQuit = scene->FindGameObjectWithName(L"ExitButton")->GetComponent<UIButton>();
		SettingSubmit = scene->FindGameObjectWithName(L"ApplyButton")->GetComponent<UIButton>();
		SettingClose = scene->FindGameObjectWithName(L"closeButton")->GetComponent<UIButton>();

	}

	if (m_NetworkEvent)
	{
	}
	else
	{
		string cookiePath = string(std::filesystem::current_path()) + L"//cookie.ini";
		string stage = std::to_wstring(scene->GetBuildIndex());
		BestScore = GetPrivateProfileInt(stage.c_str(), L"BestScore", 0, cookiePath.c_str());
	}
	PlayerBestScore = scene->FindGameObjectWithName(L"PlayerUIScoreBestScore")->GetComponent<UIText>();

	//////////////////////////////////////////////////////////////////////////
	// Image Data Load

	g_DataManager->LoadTexture(L"ui6_critical perfect.png");
	g_DataManager->LoadTexture(L"ui6_perfect.png");
	g_DataManager->LoadTexture(L"ui6_good.png");
	g_DataManager->LoadTexture(L"ui6_nice.png");
	g_DataManager->LoadTexture(L"ui6_miss.png");
	g_DataManager->LoadTexture(L"ui6_ingame_player_hpbar_middle1.png");
	g_DataManager->LoadTexture(L"ui6_ingame_player_hpbar_middle2.png");
	g_DataManager->LoadTexture(L"ui6_ingame_player_hpbar_middle3.png");
	g_DataManager->LoadTexture(L"UI6_End_Up.png");
	g_DataManager->LoadTexture(L"UI6_End_Down.png");
	g_DataManager->LoadTexture(L"UI6_End_Left.png");
	g_DataManager->LoadTexture(L"UI6_End_Right.png");
	g_DataManager->LoadTexture(L"UI6_Start_Up.png");
	g_DataManager->LoadTexture(L"UI6_Start_Down.png");
	g_DataManager->LoadTexture(L"UI6_Start_Left.png");
	g_DataManager->LoadTexture(L"UI6_Start_Right.png");
	g_DataManager->LoadTexture(L"UI6_Fail_Up.png");
	g_DataManager->LoadTexture(L"UI6_Fail_Down.png");
	g_DataManager->LoadTexture(L"UI6_Fail_Left.png");
	g_DataManager->LoadTexture(L"UI6_Fail_Right.png");
	g_DataManager->LoadTexture(L"UI6_Success_Up.png");
	g_DataManager->LoadTexture(L"UI6_Success_Down.png");
	g_DataManager->LoadTexture(L"UI6_Success_Left.png");
	g_DataManager->LoadTexture(L"UI6_Success_Right.png");
	g_DataManager->LoadTexture(L"ui6_ingame_pivergaugeoff.png");
	g_DataManager->LoadTexture(L"UI5_RankA_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankB_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankC_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankD_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankE_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankF_Image.png");
	g_DataManager->LoadTexture(L"UI5_RankS_Image.png");
	g_DataManager->LoadTexture(L"UI6_ingame_BossMonster_HPBar_Middle1.png");
	g_DataManager->LoadTexture(L"UI6_ingame_BossMonster_HPBar_Middle2.png");
	g_DataManager->LoadTexture(L"UI6_ingame_BossMonster_HPBar_Middle3.png");
}

void UIManager::Update()
{
	if (!m_PlayerState->GetIsFever())
	{
		SmashUIUpdate();
	}
	BossFrogHP();
	PlayerStateUI();
	if (!resultFlag)
	{
		GameResult();
	}
	else
	{
		if (ResultOKBtn->IsClick())
		{
			SceneManagement->ChangeScene(0);
		}
	}

	if (SettingPopup)
	{
		if (Input->GetKeyDown(KeyCode::Escape))
		{
			if(nullptr == m_NetworkEvent)
			{
				if (SettingPopup->GetActive())
				{
					Input->SetMouseMode(hos::WindowsInput::eMouseMode::LOCKED);
					Input->SetMouseVisible(false);
					if (SettingPopup)
					{
						SettingPopup->SetActive(false);
					}
				}
				else
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

					Input->SetMouseMode(hos::WindowsInput::eMouseMode::NONE);
					Input->SetMouseVisible(true);
					if (SettingPopup)
					{
						SettingPopup->SetActive(true);
					}
				}
			}
		}
	}

	if (Check[0])
	{
		if (Check[0]->IsClick())
		{
			if (CheckON[0])
			{
				CheckON[0]->m_GameObject->SetActive(true);
			}
			Check[0]->SetInteractable(false);
		}
	}

	if (CheckON[0])
	{
		if (CheckON[0]->IsClick())
		{
			if (Check[0])
			{
				Check[0]->SetInteractable(true);
			}
			CheckON[0]->m_GameObject->SetActive(false);
		}
	}

	if (Check[1])
	{
		if (Check[1]->IsClick())
		{
			if (CheckON[1])
			{
				CheckON[1]->m_GameObject->SetActive(true);
			}
			Check[1]->SetInteractable(false);
		}
	}

	if (CheckON[1])
	{
		if (CheckON[1]->IsClick())
		{
			if (Check[1])
			{
				Check[1]->SetInteractable(true);
			}
			CheckON[1]->m_GameObject->SetActive(false);
		}
	}

	if (SettingQuit)
	{
		if (SettingQuit->IsClick())
		{
			if (ReturnMenuPopup)
			{
				ReturnMenuPopup->SetActive(true);
			}
		}
	}

	if (SettingClose)
	{
		if (SettingClose->IsClick())
		{
			Input->SetMouseMode(hos::WindowsInput::eMouseMode::LOCKED);
			Input->SetMouseVisible(false);
			if (SettingPopup)
			{
				SettingPopup->SetActive(false);
			}
		}
	}

	if (SettingSubmit)
	{
		if (SettingSubmit->IsClick())
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

			Input->SetMouseMode(hos::WindowsInput::eMouseMode::LOCKED);
			Input->SetMouseVisible(false);
			if (SettingPopup)
			{
				SettingPopup->SetActive(false);
			}
		}
	}

	if (ReturnMenuCancel)
	{
		if (ReturnMenuCancel->IsClick())
		{
			if (ReturnMenuPopup)
			{
				ReturnMenuPopup->SetActive(false);
			}
		}
	}

	if (ReturnMenuOK)
	{
		if (ReturnMenuOK->IsClick())
		{
			SceneManagement->ChangeScene(0);
		}
	}
}

void UIManager::SmashUIUpdate()
{
	// 스매쉬를 맞추기 위한 노트 UI 를 띄운다. (UI Manager 가 참조하여 띄우기)
	if (m_PlayerAttack->GetIsSmashStart())
	{
		// 스매쉬 정보에 맞게 셋팅하자 (한 번만)
		if (!IsSmashSetting)
		{
			SetSmashUI();
			IsSmashSetting = true;
		}

		// 노트들을 움직여야 함
		// 노트 매니저에서 움직일 값을 가져와야 함

		// 노트는 스매쉬를 누른 직후 하나의 노트를 건너 뛴 다음 띄워야겠다.
		if (m_PlayerAttack->GetNowSmashNoteCount() % 2 == 1 && m_PlayerAttack->GetNowSmashNoteCount() != 0)
		{
			SmashNoteMoveCount = m_PlayerAttack->GetCurrentSmashNoteCount();

			if (SmashNoteMoveCount > 0)
			{
				if (m_PlayerAttack->GetSmashNoteCheck()[SmashNoteMoveCount - 1] != NoteManager::NoteErrorRangeCheck::W)
				{
					hos::Color _TempColor(1, 1, 1, 1);

					if (m_PlayerAttack->GetSmashNoteCheck()[SmashNoteMoveCount - 1] == NoteManager::NoteErrorRangeCheck::M)
					{
						switch (m_PlayerAttack->GetCurrentSmashNote()[SmashNoteMoveCount - 1])
						{
						case 0:
						{
							SmashEndVector[SmashNoteMoveCount - 1]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Up.png");
						}
						break;
						case 1:
						{
							SmashEndVector[SmashNoteMoveCount - 1]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Left.png");
						}
						break;
						case 2:
						{
							SmashEndVector[SmashNoteMoveCount - 1]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Down.png");
						}
						break;
						case 3:
						{
							SmashEndVector[SmashNoteMoveCount - 1]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Right.png");
						}
						break;
						default:
							break;
						}

					}
					else
					{
						switch (m_PlayerAttack->GetCurrentSmashNote()[SmashNoteMoveCount - 1])
						{
						case 0:
						{
							SmashEndVector[SmashNoteMoveCount - 1]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Up.png");
						}
						break;
						case 1:
						{
							SmashEndVector[SmashNoteMoveCount - 1]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Left.png");
						}
						break;
						case 2:
						{
							SmashEndVector[SmashNoteMoveCount - 1]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Down.png");
						}
						break;
						case 3:
						{
							SmashEndVector[SmashNoteMoveCount - 1]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Right.png");
						}
						break;
						default:
							break;
						}
					}

					SmashStartVector[SmashNoteMoveCount - 1]->GetComponent<hos::com::UIImage>()->SetActive(false);
				}


			}

		}
		else
		{
			if (SmashNoteMoveCount >= 0)
			{
				if (m_PlayerAttack->GetUserSmashInput() != -1)
				{
					hos::Color _TempColor(1, 1, 1, 1);

					if (m_PlayerAttack->GetSmashNoteCheck()[SmashNoteMoveCount] == NoteManager::NoteErrorRangeCheck::M)
					{
						switch (m_PlayerAttack->GetCurrentSmashNote()[SmashNoteMoveCount])
						{
						case 0:
						{
							SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Up.png");
						}
						break;
						case 1:
						{
							SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Left.png");
						}
						break;
						case 2:
						{
							SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Down.png");
						}
						break;
						case 3:
						{
							SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Right.png");
						}
						break;
						default:
							break;
						}
					}
					else
					{
						switch (m_PlayerAttack->GetCurrentSmashNote()[SmashNoteMoveCount])
						{
						case 0:
						{
							SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Up.png");
						}
						break;
						case 1:
						{
							SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Left.png");
						}
						break;
						case 2:
						{
							SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Down.png");
						}
						break;
						case 3:
						{
							SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Right.png");
						}
						break;
						default:
							break;
						}
					}

					SmashStartVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetActive(false);
				}
			}
		}


		if (SmashNoteMoveCount >= 0)
		{
			int _MoteNote = (m_PlayerAttack->GetSmashInputNoteCount() + 1) * 2;

			if (_MoteNote - 1 > m_PlayerAttack->GetNowSmashNoteCount())
			{
				float posX = SmashStartVector[SmashNoteMoveCount]->GetComponent<hos::com::Transform>()->GetLocalPosition().x;
				float posY = SmashStartVector[SmashNoteMoveCount]->GetComponent<hos::com::Transform>()->GetLocalPosition().y;

				// Smash Note UI / 한 박 동안의 시간
				posY = posY + ((SmashUIDistance / (float)SmashOneBeat) * (float)Time->DeltaTime());

				SmashStartVector[SmashNoteMoveCount]->GetComponent<hos::com::Transform>()->SetLocalPosition(hos::Vector3(posX, posY, 0));
			}
			else
			{
				if (m_PlayerAttack->GetSmashNoteCheck()[SmashNoteMoveCount] == NoteManager::NoteErrorRangeCheck::M)
				{
					switch (m_PlayerAttack->GetCurrentSmashNote()[SmashNoteMoveCount])
					{
					case 0:
					{
						SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Up.png");
					}
					break;
					case 1:
					{
						SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Left.png");
					}
					break;
					case 2:
					{
						SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Down.png");
					}
					break;
					case 3:
					{
						SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Fail_Right.png");
					}
					break;
					default:
						break;
					}

				}
				else
				{
					switch (m_PlayerAttack->GetCurrentSmashNote()[SmashNoteMoveCount])
					{
					case 0:
					{
						SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Up.png");
					}
					break;
					case 1:
					{
						SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Left.png");
					}
					break;
					case 2:
					{
						SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Down.png");
					}
					break;
					case 3:
					{
						SmashEndVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Success_Right.png");
					}
					break;
					default:
						break;
					}
				}

				SmashStartVector[SmashNoteMoveCount]->GetComponent<hos::com::UIImage>()->SetActive(false);

			}
		}


	}
	else
	{
		for (int i = 0; i < m_PlayerAttack->GetSmashNoteMaxCount(); i++)
		{
			SmashStartVector[i]->GetComponent<hos::com::UIImage>()->SetActive(false);
			SmashEndVector[i]->GetComponent<hos::com::UIImage>()->SetActive(false);
		}

		IsSmashSetting = false;
		SmashNoteMoveCount = -1;

	}
}

void UIManager::SetSmashUI()
{
	// 스매시가 시작하기 직전에 초기화

	hos::Vector3 _TempVector = hos::Vector3::Zero;
	//hos::Color _TempColor(1, 1, 1, 1);

	// 스매쉬 정보에 맞게 셋팅하자
	for (int i = 0; i < m_PlayerAttack->GetSmashInputNoteCount(); i++)
	{
		switch (m_PlayerAttack->GetCurrentSmashNote()[i])
		{
		case 0:
		{
			SmashStartVector[i]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Start_Up.png");
			SmashEndVector[i]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_End_Up.png");
		}
		break;
		case 1:
		{
			SmashStartVector[i]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Start_Left.png");
			SmashEndVector[i]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_End_Left.png");
		}
		break;
		case 2:
		{
			SmashStartVector[i]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Start_Down.png");
			SmashEndVector[i]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_End_Down.png");
		}
		break;
		case 3:
		{
			SmashStartVector[i]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_Start_Right.png");
			SmashEndVector[i]->GetComponent<hos::com::UIImage>()->SetTexture(L"UI6_End_Right.png");
		}
		break;
		default:
			break;
		}

		_TempVector.x = SmashStartVector[i]->transform->GetLocalPosition().x;
		SmashStartVector[i]->transform->SetLocalPosition(_TempVector);

	}

	for (int i = 0; i < m_PlayerAttack->GetSmashInputNoteCount(); i++)
	{
		SmashStartVector[i]->GetComponent<hos::com::UIImage>()->SetActive(true);
		SmashEndVector[i]->GetComponent<hos::com::UIImage>()->SetActive(true);
	}

}

void UIManager::BossFrogHP()
{
	if (BossFrogHPUI && m_BossFrogState)
	{
		hos::Vector4 _HPOffTil = hos::Vector4::Zero;

		// 체력이 낮아지면 이미지 바꿈
		if (m_BossFrogState->GetHealthPoint() < m_BossFrogState->GetMaxHealthPoint() * 0.3f)
		{
			BossFrogHPUI->SetTexture(L"UI6_ingame_BossMonster_HPBar_Middle3.png");
		}
		else if (m_BossFrogState->GetHealthPoint() < m_BossFrogState->GetMaxHealthPoint() * 0.6f)
		{
			BossFrogHPUI->SetTexture(L"UI6_ingame_BossMonster_HPBar_Middle2.png");
		}
		else
		{
			BossFrogHPUI->SetTexture(L"UI6_ingame_BossMonster_HPBar_Middle1.png");
		}

		_HPOffTil.x = 0;
		_HPOffTil.y = (m_BossFrogState->GetHealthPoint() / m_BossFrogState->GetMaxHealthPoint());
		_HPOffTil.z = 0;
		_HPOffTil.w = 1.0f;


		BossFrogHPUI->SetOffTil(_HPOffTil);

		//////////////////////////////////////////////////////////////////////////

		if (m_BossFrogState->GetIsDamaged())
		{
			BossUIDamage->SetActive(true);
			BossUIDamageBack->SetActive(true);

			BossUIDamage->SetNowTick(0.f);
			BossUIDamageBack->SetNowTick(0.f);

			if (!IsAddDamage)
			{
				IsAddDamage = true;
				BossAddDamage += m_BossFrogState->GetNowHitValue();

				int _temp = (int)(floor(BossAddDamage + 0.5f));

				hos::string _strDamage = std::to_wstring(_temp);

				BossUIDamage->SetText(_strDamage);
				BossUIDamageBack->SetText(_strDamage);

			}
		}
		else
		{
			IsAddDamage = false;
		}

		if (BossUIDamage->GetIsAnimationEnd())
		{
			BossAddDamage = 0.f;
			BossUIDamage->SetActive(false);
			BossUIDamageBack->SetActive(false);
		}


	}
}

void UIManager::PlayerStateUI()
{
	// 플레이어의 체력
	hos::Vector4 _OffTil = hos::Vector4::Zero;

	if (m_PlayerState->GetNowHealthPoint() < m_PlayerState->GetMaxHealthPoint() * 0.3f)
	{
		PlayerHealthBar->SetTexture(L"ui6_ingame_player_hpbar_middle3.png");
	}
	else if (m_PlayerState->GetNowHealthPoint() < m_PlayerState->GetMaxHealthPoint() * 0.6f)
	{
		PlayerHealthBar->SetTexture(L"ui6_ingame_player_hpbar_middle2.png");
	}
	else
	{
		PlayerHealthBar->SetTexture(L"ui6_ingame_player_hpbar_middle1.png");
	}

	_OffTil.x = 0;
	_OffTil.y = m_PlayerState->GetNowHealthPoint() / (float)m_PlayerState->GetMaxHealthPoint();
	_OffTil.z = 0;
	_OffTil.w = 1.0f;

	PlayerHealthBar->SetOffTil(_OffTil);

	// 플레이어의 피버 게이지
	if (m_PlayerState->GetIsFever() || m_PlayerState->GetIsGageFull())
	{
		PlayerFeverGage->SetTexture(L"ui6_ingame_pivergaugeon.png");
	}
	else
	{
		PlayerFeverGage->SetTexture(L"ui6_ingame_pivergaugeoff.png");
	}

	_OffTil.x = 0;
	_OffTil.y = m_PlayerState->GetNowFeverGage() / m_PlayerState->GetMaxFeverGage();
	_OffTil.z = 0;
	_OffTil.w = 1.0f;

	PlayerFeverGage->SetOffTil(_OffTil);

	// 플레이어의 스테미나
	PlayerStamina->SetText(std::to_wstring(m_PlayerState->GetNowStamina()));
	PlayerStaminaBack->SetText(std::to_wstring(m_PlayerState->GetNowStamina()));

	// 플레이어의 콤보
	if (m_PlayerState->GetIsComboChange())
	{
		ComboUIActiveTimer = 0.f;
		PlayerComboImage->SetNowTick(0.f);
		PlayerComboCount->SetNowTick(0.f);
		m_PlayerState->SetIsComboChange(false);
	}

	if (ComboUIActiveTimer < ComboUIActiveTime)
	{
		wchar_t strcombo[21] = { 0 };

		StringCchPrintf(strcombo, 20, L"%04d", m_PlayerState->GetNowCombo());

		PlayerComboCount->SetText(strcombo);

		PlayerComboCount->SetActive(true);
		PlayerComboImage->SetActive(true);

		ComboUIActiveTimer += (float)Time->DeltaTime();
	}
	else
	{
		PlayerComboCount->SetActive(false);
		PlayerComboImage->SetActive(false);
	}

	// 플레이어의 점수

	wchar_t strscore[21] = { 0 };
	if (m_NetworkEvent)
	{
		StringCchPrintf(strscore, 20, L"%09d", m_PlayerState->GetNowScore());
	}
	else
	{
		StringCchPrintf(strscore, 20, L"%09d", m_PlayerState->GetNowScore());
	}

	PlayerNowScore->SetText(strscore);

	// 플레이어의 판정
	if (m_PlayerState->GetIsChangeAccurency())
	{
		switch (m_PlayerState->GetAccuracy())
		{
		case NoteManager::NoteErrorRangeCheck::CP:
		{
			PlayerAccurency->SetTexture(L"ui6_critical perfect.png");
		}
		break;
		case NoteManager::NoteErrorRangeCheck::P:
		{
			PlayerAccurency->SetTexture(L"ui6_perfect.png");
		}
		break;
		case NoteManager::NoteErrorRangeCheck::C:
		{
			PlayerAccurency->SetTexture(L"ui6_nice.png");
		}
		break;
		case NoteManager::NoteErrorRangeCheck::N:
		{
			PlayerAccurency->SetTexture(L"ui6_good.png");
		}
		break;
		case NoteManager::NoteErrorRangeCheck::M:
		{
			PlayerAccurency->SetTexture(L"ui6_miss.png");
		}
		break;
		default:
			break;
		}
		PlayerAccurency->SetNowTick(0.f);

		if (!PlayerAccurencyObj->GetActive())
		{
			PlayerAccurencyObj->SetActive(true);

		}
	}

	if (PlayerAccurency->GetIsAnimationEnd())
	{
		PlayerAccurencyObj->SetActive(false);
	}

	// 플레이어 피격 상태일 때
	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Damaged)
	{
		if (!DamageFrame->GetActive())
		{
			DamageFrame->SetActive(true);
			DamageFrame->SetNowTick(0);
		}
	}
	else
	{
		if (DamageFrame->GetIsAnimationEnd())
		{
			DamageFrame->SetActive(false);
		}
	}

	if (PlayerBestScore)
	{
		U32 bestScore = 0;
		U32 score = 0;
		if (m_NetworkEvent)
		{
			score = m_NetworkEvent->MyScore;
			bestScore = m_NetworkEvent->BestScore;
		}
		else
		{
			score = (U32)m_PlayerState->GetNowScore();
			bestScore = BestScore;
		}

		U32 tt = score > bestScore ? score : bestScore;
		PlayerBestScore->SetText(std::to_wstring(tt));
	}
}

void UIManager::GameResult()
{
	string cookiePath = string(std::filesystem::current_path()) + L"//cookie.ini";
	if (m_GameManager)
	{
		if (m_GameManager->GetIsGameEnd())
		{
			// 결과창 띄우기
			for (int i = 0; i < m_GameObject->transform->GetChilds().size(); i++)
			{
				m_GameObject->transform->GetChilds()[i]->m_GameObject->SetActive(false);
			}

			GameResultUI->SetActive(true);
		}
	}
	else
	{
		if (m_NetworkEvent)
		{
			if (m_NetworkEvent->GetIsGameEnd())
			{
				// 결과창 띄우기
				for (int i = 0; i < m_GameObject->transform->GetChilds().size(); i++)
				{
					m_GameObject->transform->GetChilds()[i]->m_GameObject->SetActive(false);
				}
				GameResultUI->SetActive(true);
			}
		}
	}

	if (GameResultUI->GetActive())
	{
		U32 score = 0;
		U32 bestScore = 0;
		int rank = -1;

		string stage = std::to_wstring(m_GameObject->m_Scene->GetBuildIndex());
		if (m_NetworkEvent)
		{
			//멀티플레이
			score = m_NetworkEvent->MyScore;
			bestScore = m_NetworkEvent->BestScore;
			if (1350000 < score)
			{
				rank = 6;
			}
			else if (1300000 < score)
			{
				rank = 5;
			}
			else if (1250000 < score)
			{
				rank = 4;
			}
			else if (1200000 < score)
			{
				rank = 3;
			}
			else if (1100000 < score)
			{
				rank = 2;
			}
			else
			{
				rank = 1;
			}
		}
		else
		{
			//싱글플레이
			score = (U32)m_PlayerState->GetNowScore();
			bestScore = BestScore;
			if (1800000 < score)
			{
				rank = 6;
			}
			else if (1700000 < score)
			{
				rank = 5;
			}
			else if (1500000 < score)
			{
				rank = 4;
			}
			else if (1350000 < score)
			{
				rank = 3;
			}
			else if (1200000 < score)
			{
				rank = 2;
			}
			else
			{
				rank = 1;
			}
		}

		//플레이어 죽었을때
		if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Dead)
		{
			ResultFail->SetActive(true);
			ResultClear->SetActive(false);
			rank = 0;
		}
		else
		{
			ResultFail->SetActive(false);
			ResultClear->SetActive(true);
		}

		//Rank에 따른 표시
		switch (rank)
		{
		case 0:
			ResultRank->SetTexture(L"UI5_RankF_Image.png");
			break;
		case 1:
			ResultRank->SetTexture(L"UI5_RankE_Image.png");
			break;
		case 2:
			ResultRank->SetTexture(L"UI5_RankD_Image.png");
			break;
		case 3:
			ResultRank->SetTexture(L"UI5_RankC_Image.png");
			break;
		case 4:
			ResultRank->SetTexture(L"UI5_RankB_Image.png");
			break;
		case 5:
			ResultRank->SetTexture(L"UI5_RankA_Image.png");
			break;
		case 6:
			ResultRank->SetTexture(L"UI5_RankS_Image.png");
			break;
		}

		//NewRecord Active
		if (score >= bestScore)
		{
			bestScore = score;
			ResultNewRecord->SetActive(true);
			WritePrivateProfileString(stage.c_str(), L"BestScore", std::to_wstring(bestScore).c_str(), cookiePath.c_str());
		}
		else
		{
			ResultNewRecord->SetActive(false);
		}

		int saveRank = GetPrivateProfileIntW(stage.c_str(), L"Rank", -1, cookiePath.c_str());
		if (saveRank < rank)
		{
			WritePrivateProfileString(stage.c_str(), L"Rank", std::to_wstring(rank).c_str(), cookiePath.c_str());
		}
		int saveCombo = GetPrivateProfileIntW(stage.c_str(), L"MaxCombo", -1, cookiePath.c_str());
		int Combo = m_PlayerState->GetMaxCombo();
		if (saveCombo < Combo)
		{
			WritePrivateProfileString(stage.c_str(), L"MaxCombo", std::to_wstring(Combo).c_str(), cookiePath.c_str());
		}

		ResultBestScore->SetText(std::to_wstring(bestScore));
		ResultScore->SetText(std::to_wstring(score));
		long long clockSecond = m_GameObject->m_Scene->SceneTime->Now_Seccond();
		long long min = clockSecond / 60;
		long long second = clockSecond % 60;

		string playtime = std::to_wstring(min) + L" : " + std::to_wstring(second);
		m_GameObject->m_Scene->SceneTime->Stop();
		ResultPlayTime->SetText(playtime);
		ResultMaxCombo->SetText(std::to_wstring(Combo));
		ResultCP->SetText(std::to_wstring(m_PlayerState->GetJudge(NoteManager::NoteErrorRangeCheck::CP)));
		ResultPerfect->SetText(std::to_wstring(m_PlayerState->GetJudge(NoteManager::NoteErrorRangeCheck::P)));
		ResultCool->SetText(std::to_wstring(m_PlayerState->GetJudge(NoteManager::NoteErrorRangeCheck::C)));
		ResultNice->SetText(std::to_wstring(m_PlayerState->GetJudge(NoteManager::NoteErrorRangeCheck::N)));
		ResultMiss->SetText(std::to_wstring(m_PlayerState->GetJudge(NoteManager::NoteErrorRangeCheck::M)));
		resultFlag = true;
	}
}

void UIManager::SetAttackDamage()
{
	// 평타, 대쉬공격 등등 데미지가 다 다름
}