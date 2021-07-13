#include "PlayerAction.h"


PlayerAction::PlayerAction() : hos::com::Script(L"PlayerAction")
{
	ExecutionOrder = 1;

}

PlayerAction::~PlayerAction()
{

}

void PlayerAction::Reset()
{

}

void PlayerAction::Awake()
{
	PlayerTransform = m_GameObject->transform;
	CameraTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"MainCamera")->transform;
	//PlayerRig = m_GameObject->rigidbody;

	ActionSound = m_GameObject->GetComponents<hos::com::AudioSource>()[0];
	CriMissSound = m_GameObject->GetComponents<hos::com::AudioSource>()[1];

	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();
	m_PlayerState = m_GameObject->GetComponent<PlayerState>();

	m_PlayerLookEnemy = m_GameObject->m_Scene->FindGameObjectWithName(L"LookAtArea")->GetComponent<PlayerLookEnemy>();
	//////////////////////////////////////////////////////////////////////////
	// Audio Source Data Load

	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_whooshwithenergy1.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_whooshwithenergy2.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_whooshwithenergy3.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_whooshfever1.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_whooshfever2.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_whooshfever3.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smashcast.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_fevercast.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_reload.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_dash.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\ui_hud_criticalperfect.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\ui_hud_miss.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_staminazero.sclip");

	CriMissSound->SetIs3D(false);
	CriMissSound->SetLoop(false);
	CriMissSound->SetPlayOnAwake(false);
	CriMissSound->SetAudioClip(L"ui_hud_criticalperfect");
}

void PlayerAction::Start()
{
	CameraTransform->SetPosition(hos::Vector3(0, 1, -2));
}

void PlayerAction::Update()
{
	// 입력 후 반 박자 동안 입력하지 못 함
	CameraUpdate();

	// 차지 상태 중에는 움직을 수 없음
	if (m_PlayerState->GetNowPlayerAction() != PlayerState::PlayerActionList::ChargeStart ||
		m_PlayerState->GetNowPlayerAction() != PlayerState::PlayerActionList::ChargeIdle ||
		m_PlayerState->GetNowPlayerAction() != PlayerState::PlayerActionList::ChargeEnd ||
		m_PlayerState->GetNowPlayerAction() != PlayerState::PlayerActionList::Dash)
	{
		InputDirection();
	}

	if (m_NoteManager->GetIsBGMStart())
	{
		if (IsInputActive)
		{
			if (m_PlayerState->GetNowPlayerAction() != PlayerState::PlayerActionList::Dead &&
				m_PlayerState->GetNowPlayerAction() != PlayerState::PlayerActionList::Damaged)
			{
				InputFeverCast();

				// 충전
				if (m_PlayerState->GetNowStamina() != m_PlayerState->GetMaxStamina())
				{
					InputCharge();
				}

				// 스테미나가 없으면 안 됨
				// 대쉬
				if (!m_PlayerState->GetIsSlow())
				{
					InputDash();
				}

				// 스매시
				InputSmash();
				// 평타
				InputAttack();

			}
		}
		else
		{
			// 일반 공격을 할 시에만 애니메이션 건너뛰기 적용

			if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1 ||
				m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2 ||
				m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3)

			{
				if (m_PlayerState->GetIsFever())
				{
					if (InputDelay > m_NoteManager->GetInputDelayRange() * 0.5f)
					{
						IsInputActive = true;
						InputDelay = 0.0f;
					}
				}
				else
				{
					if (InputDelay > m_NoteManager->GetInputDelayRange())
					{
						IsInputActive = true;
						InputDelay = 0.0f;
					}
				}
			}

			if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Dash)
			{
				CameraTransform->m_GameObject->GetComponent<hos::com::Camera>()->SetMortionBlurState(3);
			}
			else
			{
				CameraTransform->m_GameObject->GetComponent<hos::com::Camera>()->SetMortionBlurState(0);
			}

			if (m_PlayerState->GetIsPlayerAnimationEnd())
			{
				IsInputActive = true;
				InputDelay = 0.0f;
			}
			else
			{
				InputDelay += (float)Time->DeltaTime();
			}
		}
	}

	NoteManagerSetTransform();
	CameraBulrActive();
}

void PlayerAction::Render()
{
	//g_Renderer->_DebugText(700, 400, 15, hos::Vector4(1, 1, 1, 1), L"MouseX : %f", NowCameraMouseMoveX);
}

void PlayerAction::CameraUpdate()
{
	CameraMove* cammove = CameraTransform->m_GameObject->GetComponent<CameraMove>();

	bool b = false;
	if (cammove)
	{
		b = cammove->GetIsDebugCamera();
	}
	if (!b)
	{
		//////////////////////////////////////////////////////////////////////////
		// 마우스 X 값을 받아서 카메라 좌우로 움직이기

		float _DeltaMouseMoveX = Input->GetAxis(Axis::MouseX);
		if (_DeltaMouseMoveX < 0)
		{
			CameraThetaY += (float)Time->DeltaTime() * CameraMoveSpeedX;
		}

		if (_DeltaMouseMoveX > 0)
		{
			CameraThetaY -= (float)Time->DeltaTime() * CameraMoveSpeedX;
		}

		PrevCameraMouseMoveX = NowCameraMouseMoveX;

		if (CameraTransform->m_GameObject->GetComponent<hos::com::Camera>()->GetMode() != hos::com::Camera::eMode::FOLLOW)
		{
			CameraTransform->m_GameObject->GetComponent<hos::com::Camera>()->ChangeMode(hos::com::Camera::eMode::FOLLOW);
		}

		hos::Matrix mRotY = hos::Matrix::CreateRotationY(CameraThetaY);

		DirectX::SimpleMath::Vector3 _result = XMVector3Transform(hos::Vector3::Forward, mRotY);

		_result.Normalize();
		_result = _result * CameraDistance;

		DirectX::SimpleMath::Vector3 playerPosition = PlayerTransform->GetPosition();
		DirectX::SimpleMath::Vector3 cameraPosition = playerPosition - _result;

		//////////////////////////////////////////////////////////////////////////
		// 마우스 Y 값을 받아서 카메라 위아래로 움직이기

		float _DeltaMouseMoveY = Input->GetAxis(Axis::MouseY);
		//카메라 위치 플레이어기준으로 조정
		float CameraPosMax = MaxCameraPosY + playerPosition.y;
		float CameraPosMin = MinCameraPosY + playerPosition.y;

		if (_DeltaMouseMoveY < 0)
		{
			CameraPosY += (float)Time->DeltaTime() * CameraMoveSpeedY;
		}

		if (_DeltaMouseMoveY > 0)
		{
			CameraPosY -= (float)Time->DeltaTime() * CameraMoveSpeedY;
		}

		if (CameraPosY > CameraPosMax)
		{
			CameraPosY = CameraPosMax;
		}
		else if(CameraPosY < CameraPosMin)
		{
			CameraPosY = CameraPosMin;
		}

		PrevCameraMouseMoveY = NowCameraMouseMoveY;

		cameraPosition.y = CameraPosY;

		if (m_PlayerState->GetIsShakeCamera())
		{
			float _SelectState = std::rand() / (float)RAND_MAX;
			float _Temp = 0.f;

			if (_SelectState < 0.5f)
			{
				_Temp = -_SelectState * m_PlayerState->GetShakeValue();
			}
			else
			{
				_Temp = _SelectState * m_PlayerState->GetShakeValue();
			}

			cameraPosition.x += _Temp;
			//cameraPosition.y += _Temp;
			cameraPosition.z += _Temp;

		}

		CameraTransform->SetPosition(cameraPosition);

		hos::Vector3 _TargetPosition = PlayerTransform->GetPosition();

		_TargetPosition.y = _TargetPosition.y + FollowCameraTargetPosY;

		CameraTransform->m_GameObject->GetComponent<hos::com::Camera>()->SetFollowTarget(_TargetPosition);

		//////////////////////////////////////////////////////////////////////////
		// 카메라를 플레이어 방향 쪽으로 회전시키자

		float degreeY = DirectX::XMConvertToDegrees(CameraThetaY);

		CameraTransform->SetRotation(hos::Rotator(0, degreeY, 0));
	}
}

void PlayerAction::InputDirection()
{
	auto state = m_PlayerState->GetNowPlayerAction();
	if (state == PlayerState::PlayerActionList::Idle ||
		state == PlayerState::PlayerActionList::Move ||
		/*state == PlayerState::PlayerActionList::Attack1 ||
		state == PlayerState::PlayerActionList::Attack2 || state == PlayerState::PlayerActionList::Attack3 ||*/
		state == PlayerState::PlayerActionList::Dash
		)
	{
		hos::Vector3 forward;
		hos::Vector3 left;
		hos::Vector3 MoveVector = hos::Vector3::Zero;
		float playerRotationY = 0.f;
		bool IsW = false;
		bool IsA = false;
		bool IsS = false;
		bool IsD = false;

		forward = PlayerTransform->GetPosition() - CameraTransform->GetPosition();

		forward.y = 0;

		forward.Normalize();

		left = forward.Cross(hos::Vector3::Up);

		if (Input->GetKey(KeyCode::W))
		{
			IsW = true;

			PlayerTransform->SetRotation(hos::Rotator(0, CameraTransform->GetRotation().y + playerRotationY, 0));
			MoveVector = MoveVector + forward;
		}

		if (Input->GetKey(KeyCode::A))
		{
			IsA = true;

			playerRotationY = -90;
			PlayerTransform->SetRotation(hos::Rotator(0, CameraTransform->GetRotation().y + playerRotationY, 0));

			MoveVector = MoveVector + left;
		}

		if (Input->GetKey(KeyCode::S))
		{
			IsS = true;

			forward = forward * -1;

			playerRotationY = 180;
			PlayerTransform->SetRotation(hos::Rotator(0, CameraTransform->GetRotation().y + playerRotationY, 0));

			MoveVector = MoveVector + forward;
		}
		if (Input->GetKey(KeyCode::D))
		{
			IsD = true;

			left = left * -1;

			playerRotationY = 90;
			PlayerTransform->SetRotation(hos::Rotator(0, CameraTransform->GetRotation().y + playerRotationY, 0));

			MoveVector = MoveVector + left;
		}

		if (IsW && IsA)
		{
			playerRotationY = -45;
			PlayerTransform->SetRotation(hos::Rotator(0, CameraTransform->GetRotation().y + playerRotationY, 0));
		}

		if (IsW && IsD)
		{
			playerRotationY = 45;
			PlayerTransform->SetRotation(hos::Rotator(0, CameraTransform->GetRotation().y + playerRotationY, 0));
		}

		if (IsS && IsA)
		{
			playerRotationY = -135;
			PlayerTransform->SetRotation(hos::Rotator(0, CameraTransform->GetRotation().y + playerRotationY, 0));
		}

		if (IsS && IsD)
		{
			playerRotationY = 135;
			PlayerTransform->SetRotation(hos::Rotator(0, CameraTransform->GetRotation().y + playerRotationY, 0));
		}

		if (m_PlayerState->GetNowPlayerAction() != PlayerState::PlayerActionList::Dash)
		{
			if (IsW || IsA || IsS || IsD)
			{
				m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Move);
			}

			else
			{
				MoveVector = hos::Vector3::Zero;
				m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Idle);
			}
			//PlayerRig->ClearForce();
		}

		MoveVector.Normalize();
		MoveVector.y = 0;
		m_PlayerState->SetDirection(MoveVector, false);
	}
}

void PlayerAction::InputCharge()
{
	if (Input->GetKeyDown(KeyCode::R))
	{
		m_PlayerState->SetAccuracy(m_NoteManager->CheckUserInput());

		if (m_PlayerState->GetAccuracy() != NoteManager::NoteErrorRangeCheck::M)
		{
			if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::ChargeIdle)
			{
				m_PlayerState->SetNowStamina(m_PlayerState->GetMaxStamina());

				m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::ChargeEnd);

				ActionSound->Stop();

				ActionSound->SetAudioClip(L"pc_attack_whooshwithenergy3");

				ActionSound->Play();

			}
			else
			{
				m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::ChargeStart);


				ActionSound->Stop();

				ActionSound->SetAudioClip(L"pc_reload");

				ActionSound->Play();

			}

			IsInputActive = false;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// Sound

			CriMissSound->Stop();
			CriMissSound->SetAudioClip(L"ui_hud_miss");
			CriMissSound->Play();
		}
	}
}

void PlayerAction::InputDash()
{
	if (Input->GetKeyDown(KeyCode::Space))
	{
		m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Dash);

		IsInputActive = false;

		ActionSound->Stop();

		ActionSound->SetAudioClip(L"pc_dash");

		ActionSound->Play();
		/*
		// 판정 보내기
		m_PlayerState->SetAccuracy(m_NoteManager->CheckUserInput());

		if (m_PlayerState->GetAccuracy() != NoteManager::NoteErrorRangeCheck::M)
		{
			m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Dash);

			IsInputActive = false;

		}
		else
		{
			m_PlayerState->SetNowStamina(m_PlayerState->GetNowStamina() - 1);

			//////////////////////////////////////////////////////////////////////////
			// Sound

			CriMissSound->Stop();
			CriMissSound->SetAudioClip(L"ui_hud_miss");
			CriMissSound->Play();
		}
		*/
	}
}

void PlayerAction::InputParrying()
{
	if (Input->GetKeyDown(KeyCode::MouseMiddle))
	{
		m_PlayerState->SetAccuracy(m_NoteManager->CheckUserInput());

		if (m_PlayerState->GetAccuracy() != NoteManager::NoteErrorRangeCheck::M)
		{
			m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Parrying);

			IsInputActive = false;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// Sound

			CriMissSound->Stop();
			CriMissSound->SetAudioClip(L"ui_hud_miss");
			CriMissSound->Play();
		}
	}
}

void PlayerAction::InputSmash()
{
	if (Input->GetKeyDown(KeyCode::MouseRight))
	{
		if (m_PlayerState->GetNowStamina() > 0)
		{

			m_PlayerState->SetAccuracy(m_NoteManager->CheckUserInput());

			if (m_PlayerState->GetAccuracy() != NoteManager::NoteErrorRangeCheck::M)
			{
				// 이전 상태에 따라 다른 스매시 공격이 나가도록
				if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1)
				{
					m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Smash1);
				}
				else if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2)
				{
					m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Smash2);
				}
				else if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3)
				{
					m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Smash3);
				}
				else
				{
					m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Smash);
				}
				IsInputActive = false;

				//////////////////////////////////////////////////////////////////////////
				if (m_PlayerLookEnemy->GetIsInEnemy())
				{
					m_PlayerState->SetDirection(m_PlayerLookEnemy->GetPlayerForword(), false);
					PlayerTransform->SetForward(m_PlayerLookEnemy->GetPlayerForword());
				}

				//////////////////////////////////////////////////////////////////////////
				// Attack Sound

				ActionSound->Stop();

				ActionSound->SetAudioClip(L"pc_attack_smashcast");

				ActionSound->Play();

			}
			else
			{
				m_PlayerState->SetNowStamina(m_PlayerState->GetNowStamina() - 1);

				//////////////////////////////////////////////////////////////////////////
				// Sound

				CriMissSound->Stop();
				CriMissSound->SetAudioClip(L"ui_hud_miss");
				CriMissSound->Play();


			}
		}
		else
		{
			// 틱
			ActionSound->Stop();

			ActionSound->SetAudioClip(L"pc_staminazero");

			ActionSound->Play();
		}
	}
}

void PlayerAction::InputAttack()
{
	if (Input->GetKeyDown(KeyCode::MouseLeft))
	{
		if (m_PlayerState->GetNowStamina() > 0)
		{

			m_PlayerState->SetAccuracy(m_NoteManager->CheckUserInput());

			if (m_PlayerState->GetAccuracy() != NoteManager::NoteErrorRangeCheck::M)
			{
				// 이전 상태에 따라 다른 애니메이션이 되도록
				if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1)
				{
					m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Attack2);
				}
				else if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2)
				{
					m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Attack3);
				}
				else
				{
					m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::Attack1);
				}

				IsInputActive = false;

				// 주변에 몬스터가 있으면 해당 방향으로 전환
				if (m_PlayerLookEnemy->GetIsInEnemy())
				{
					m_PlayerState->SetDirection(m_PlayerLookEnemy->GetPlayerForword(), false);
					PlayerTransform->SetForward(m_PlayerLookEnemy->GetPlayerForword());
				}

				// 콜리젼 킨다
				// 총돌이 될까


				//////////////////////////////////////////////////////////////////////////
				// Sound

				ActionSound->Stop();

				int _Rand = (std::rand() / ((RAND_MAX + 1u) / 3));

				switch (_Rand)
				{
				case 0:
				{
					if (m_PlayerState->GetIsFever())
					{
						ActionSound->SetAudioClip(L"pc_attack_whooshfever1");
					}
					else
					{
						ActionSound->SetAudioClip(L"pc_attack_whooshwithenergy1");
					}
				}
				break;
				case 1:
				{
					if (m_PlayerState->GetIsFever())
					{
						ActionSound->SetAudioClip(L"pc_attack_whooshfever2");
					}
					else
					{
						ActionSound->SetAudioClip(L"pc_attack_whooshwithenergy2");
					}
				}
				break;
				case 2:
				{
					if (m_PlayerState->GetIsFever())
					{
						ActionSound->SetAudioClip(L"pc_attack_whooshfever3");
					}
					else
					{
						ActionSound->SetAudioClip(L"pc_attack_whooshwithenergy3");
					}

				}
				break;
				default:
					break;
				}

				ActionSound->Play();

			}
			else
			{
				m_PlayerState->SetNowStamina(m_PlayerState->GetNowStamina() - 1);

				//////////////////////////////////////////////////////////////////////////
				// Sound

				CriMissSound->Stop();
				CriMissSound->SetAudioClip(L"ui_hud_miss");
				CriMissSound->Play();
			}
		}
		else
		{
			// 틱
			ActionSound->Stop();

			ActionSound->SetAudioClip(L"pc_staminazero");

			ActionSound->Play();
		}
	}
}

void PlayerAction::InputFeverCast()
{
	if (Input->GetKeyDown(KeyCode::Q))
	{
		if (m_PlayerState->NetEventer)
		{
			if (m_PlayerState->GetIsGageFull())
			{
				m_PlayerState->RequestFever();
			}
		}
		else
		{
			if (m_PlayerState->GetIsGageFull())
			{
				m_PlayerState->SetNowPlayerAction(PlayerState::PlayerActionList::FeverCast);

				IsInputActive = false;

				if (ActionSound)
				{
					ActionSound->Stop();
				}
				ActionSound->SetAudioClip(L"pc_fevercast");

				if (ActionSound)
				{
					ActionSound->Play();
				}
			}
		}
	}
}

void PlayerAction::NoteManagerSetTransform()
{
	m_NoteManager->m_GameObject->transform->SetPosition(PlayerTransform->GetPosition());
	m_NoteManager->m_GameObject->transform->SetRotation(hos::Rotator(0, PlayerTransform->GetRotation().y, 0));
}

void PlayerAction::CameraBulrActive()
{

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
	{
		CameraTransform->m_GameObject->GetComponent<hos::com::Camera>()->SetRadialBlurState(true, 0.5f, 0.5f, 0.3f, 1.5f);
	}
	else
	{
		CameraTransform->m_GameObject->GetComponent<hos::com::Camera>()->SetRadialBlurState(false);
	}

	if (m_PlayerState->GetIsFever())
	{
		CameraTransform->m_GameObject->GetComponent<hos::com::Camera>()->SetRadialBlurState(true, 0.5f, 0.5f, 0.3f, 1.5f);
	}
}