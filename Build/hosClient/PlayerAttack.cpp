#include "PlayerAttack.h"

#include "PacketDefine.h"

PlayerAttack::PlayerAttack() : hos::com::Script(L"PlayerAttack")
{
	ExecutionOrder = 3;
}

PlayerAttack::~PlayerAttack()
{

}

void PlayerAttack::Reset()
{

}

void PlayerAttack::Awake()
{
	AttackArea = m_GameObject->GetComponent<hos::com::BoxCollision>();
	//AttackArea->SetActive(false);
	m_PlayerState = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerState>();
	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();
	PlayerTransform = m_PlayerState->m_GameObject->transform;
	AttackAreaTransform = m_GameObject->transform;
	HitSound = m_GameObject->GetComponent<hos::com::AudioSource>();
	ActionSound = m_PlayerState->m_GameObject->GetComponents<hos::com::AudioSource>()[0];
	CriMissSound = m_PlayerState->m_GameObject->GetComponents<hos::com::AudioSource>()[1];

	for (int i = 0; i < SmashNoteMaxCount; i++)
	{
		//SmashNoteCheck[i] = NoteManager::NoteErrorRangeCheck::W;
		CurrentSmashNote[i] = -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// Audio Source Data Load

	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_impactwithenergy_kick.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_impactwithenergy_snare.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smashinputsuccess1.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smashinputsuccess2.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smashinputsuccess3.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smashinputsuccess4.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smashinputsuccess5.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smashinputfail.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash1impact.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash2impact.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash3impact.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash4impact.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash1whoosh__.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash2whoosh__.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash3whoosh__.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash4whoosh__.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash1whoosh_.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash2whoosh_.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash3whoosh_.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_attack_smash4whoosh_.sclip");
}

void PlayerAttack::Start()
{
	AttackArea->SetActive(false);
}

void PlayerAttack::Update()
{
	NormalAttack();

	if (m_PlayerState->GetIsFever())
	{
		FeverSmashAttack();
	}
	else
	{
		if (m_PlayerState->NetEventer == nullptr)
		{
			SmashAttack();
		}
		else
		{
			FeverSmashAttack();
		}
	}
}

void PlayerAttack::Render()
{
	/*
	int x = 1000;
	int y = 200;
	int font = 14;
	int length = 20;

	for (int i = 0; i < SmashNoteMaxCount; i++)
	{
		switch (SmashNoteCheck[i])
		{
		case NoteManager::NoteErrorRangeCheck::CP:
			g_Renderer->_DebugText(x, y + (length * i), font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Critical Perfect");
			break;
		case NoteManager::NoteErrorRangeCheck::P:
			g_Renderer->_DebugText(x, y + (length * i), font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Perfect");
			break;
		case NoteManager::NoteErrorRangeCheck::C:
			g_Renderer->_DebugText(x, y + (length * i), font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Cool");
			break;
		case NoteManager::NoteErrorRangeCheck::N:
			g_Renderer->_DebugText(x, y + (length * i), font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Nice");
			break;
		case NoteManager::NoteErrorRangeCheck::M:
			g_Renderer->_DebugText(x, y + (length * i), font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Miss");
			break;
		case NoteManager::NoteErrorRangeCheck::W:
			g_Renderer->_DebugText(x, y + (length * i), font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Wait");
			break;
		default:
			break;
		}
	}

	g_Renderer->_DebugText(x, y + (length * 6), font, hos::Vector4(1, 1, 1, 1), L"Now Smash Count : %d", NowSmashNoteCount);
	g_Renderer->_DebugText(x, y + (length * 7), font, hos::Vector4(1, 1, 1, 1), L"Current Smash Count : %d", CurrentSmashNoteCount);
	*/
}

void PlayerAttack::OnTriggerEnter(hos::com::Collision* other)
{
	// ÇöÀç °ø°Ý, ÇöÀç ÆÇÁ¤¿¡ µû¶ó °ø°Ý·ÂÀÌ ´Ù¸§
	// °ø°Ý·Â * ¹è¼ö

	if (other->m_GameObject->GetTag().compare(L"Enemy") == 0)
	{
		// Enmey ·Î ¹Ù²Ù°í µ¥¹ÌÁö¸¦ ÁÖÀÚ

		int type = 0;
		bool _IsDead = false;

		if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
			m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
			m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
			m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
		{
			type = 1;

			if (m_PlayerState->NetEventer == nullptr)
			{
				//for (int i = 0; i < SmashNoteMaxCount; i++)
				//{
				//	SmashNoteCheck[i] = m_PlayerState->GetAccuracy();
				//}
				m_PlayerState->SetAccuracy(CheckSmashNoteAccurancyAverage());
			}

			//m_PlayerState->AddNowFeverGage(m_PlayerState->GetAccuracy());
		}
		else
		{
			if (m_PlayerState->GetIsFever())
			{
				type = 1;
			}
			else
			{
				type = 0;
			}
		}

		// ±ÙÁ¢
		Component* _temp = other->m_GameObject->GetComponent<MeleeFrogState>();

		if (_temp != nullptr)
		{
			if (other->m_GameObject->GetComponent<MeleeFrogState>()->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Dead)
			{
				_IsDead = true;
			}

			if (!_IsDead)
			{
				m_PlayerState->ComboIncrease();

				if (m_PlayerState->NetEventer == nullptr)
				{
					m_PlayerState->AddNowFeverGage();
					SetAttackDamage();
					float _DamageScore = other->m_GameObject->GetComponent<MeleeFrogState>()->GetDamaged(AttackDamage, type);
					m_PlayerState->AddScore(_DamageScore);
				}
				else
				{
					other->m_GameObject->GetComponent<MeleeFrogState>()->GetDamaged(AttackDamage, type);
				}

				m_PlayerState->SetIsShakeCamera(true);
				m_PlayerState->SetShakeValue(0.1f);
				m_PlayerState->SetShakeTime(0.3f);

			}
		}

		// ¿ø°Å¸®
		_temp = other->m_GameObject->GetComponent<RangeFrogState>();

		if (_temp != nullptr)
		{
			if (other->m_GameObject->GetComponent<RangeFrogState>()->GetNowAction() == RangeFrogState::RangeFrogActionList::Dead)
			{
				_IsDead = true;
			}

			if (!_IsDead)
			{
				m_PlayerState->ComboIncrease();

				if (m_PlayerState->NetEventer == nullptr)
				{
					m_PlayerState->AddNowFeverGage();
					SetAttackDamage();
					float _DamageScore = other->m_GameObject->GetComponent<RangeFrogState>()->GetDamaged(AttackDamage, type);
					m_PlayerState->AddScore(_DamageScore);
				}
				else
				{
					other->m_GameObject->GetComponent<RangeFrogState>()->GetDamaged(AttackDamage, type);
				}


				m_PlayerState->SetIsShakeCamera(true);
				m_PlayerState->SetShakeValue(0.1f);
				m_PlayerState->SetShakeTime(0.3f);

			}
		}

		// º¸½º
		_temp = other->m_GameObject->GetComponent<BossFrogState>();

		if (_temp != nullptr)
		{

			auto state = m_GameObject->GetParent()->GetComponent<PlayerState>();

			if (other->m_GameObject->GetComponent<BossFrogState>()->GetAiType() == 0)
			{
				if (other->m_GameObject->GetComponent<BossFrogState>()->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
				{
					_IsDead = true;
				}

				if (!_IsDead)
				{
					m_PlayerState->ComboIncrease();

					if (m_PlayerState->NetEventer == nullptr)
					{
						m_PlayerState->AddNowFeverGage();
						SetAttackDamage();
						float _DamageScore = other->m_GameObject->GetComponent<BossFrogState>()->GetDamaged(AttackDamage, type);
						m_PlayerState->AddScore(_DamageScore);
					}
					else
					{
						other->m_GameObject->GetComponent<BossFrogState>()->GetDamaged(AttackDamage, type);
					}

					m_PlayerState->SetIsShakeCamera(true);
					m_PlayerState->SetShakeValue(0.1f);
					m_PlayerState->SetShakeTime(0.3f);
				}
			}

			if (other->m_GameObject->GetComponent<BossFrogState>()->GetAiType() == 1)
			{
				m_PlayerState->ComboIncrease();
				m_PlayerState->AddScore(0);
				if (state->NetEventer != nullptr)
				{
					flexbuffers::Builder builder;
					builder.Map([&] {
						builder.Int("Acc", static_cast<int>(m_PlayerState->GetAccuracy()));
						builder.Int("Combo", m_PlayerState->GetNowCombo());
						builder.Int("sm_cnt", SmashSuccessInput);
						builder.Int("Type", type);
						});

					builder.Finish();
					const uint8_t* buffer = builder.GetBuffer().data();

					PACKET pack;
					pack.head.size = PACKET_HEADER_SIZE;
					pack.head.protocol = static_cast<unsigned short>(PROTOCOL::ON_ATTACK_EVENT);
					pack.Make(pack, pack.head, buffer, (int)builder.GetSize());
					state->NetEventer->network->SendMsg(pack);
				}
			}

		}

		/*
		Component* _temp = other->m_GameObject->GetComponent<ShieldFrogState>();

		if (_temp != nullptr)
		{
			other->m_GameObject->GetComponent<ShieldFrogState>()->GetDamaged(AttackDamage);
		}
		*/

		//////////////////////////////////////////////////////////////////////////
		// Sound

		HitSound->Stop();

		switch (m_PlayerState->GetNowPlayerAction())
		{
		case PlayerState::PlayerActionList::Attack1:
		{
			HitSound->SetAudioClip(L"pc_attack_impactwithenergy_kick");
		}
		break;
		case PlayerState::PlayerActionList::Attack2:
		case PlayerState::PlayerActionList::Attack3:
		{
			HitSound->SetAudioClip(L"pc_attack_impactwithenergy_snare");
		}
		break;
		case PlayerState::PlayerActionList::Smash:
		{
			HitSound->SetAudioClip(L"pc_attack_smash1impact");
		}
		break;
		case PlayerState::PlayerActionList::Smash1:
		{
			HitSound->SetAudioClip(L"pc_attack_smash2impact");
		}
		break;
		case PlayerState::PlayerActionList::Smash2:
		{
			HitSound->SetAudioClip(L"pc_attack_smash3impact");
		}
		break;
		case PlayerState::PlayerActionList::Smash3:
		{
			HitSound->SetAudioClip(L"pc_attack_smash4impact");
		}
		break;
		default:
			break;
		}

		HitSound->Play();

		if (m_PlayerState->GetAccuracy() == NoteManager::NoteErrorRangeCheck::CP)
		{
			CriMissSound->Stop();
			CriMissSound->SetAudioClip(L"ui_hud_criticalperfect");
			CriMissSound->Play();
		}
	}
}

void PlayerAttack::NormalAttack()
{
	// ÇÃ·¹ÀÌ¾îÀÇ »óÅÂ°¡ °ø°Ý »óÅÂÀÏ ¶§
	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3 ||
		//m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::DashAttack ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::ChargeEnd)
	{
		// °ø°Ý¿¡ µû¶ó ¿µ¿ª°ú °Å¸®¸¦ ´Ù¸£°Ô ÇÒ ¼ö ÀÖÀ½

		if (NowAttackAreaActiveTime < AttackAreaActiveTime)
		{
			if (m_PlayerState->GetIsFever())
			{
				AttackArea->SetSizeX(1.5f);
				AttackArea->SetSizeY(1.5f);
				AttackArea->SetSizeZ(1.5f);
				AttackRange = 1.5f;
			}
			else
			{
				AttackArea->SetSizeX(1.f);
				AttackArea->SetSizeY(1.f);
				AttackArea->SetSizeZ(1.f);
				AttackRange = 1.f;
			}

			hos::Vector3 _AttackPos = PlayerTransform->GetPosition() + (PlayerTransform->GetForward() * AttackRange);

			_AttackPos.y += 1;

			AttackAreaTransform->SetPosition(_AttackPos);

			AttackDamage = 10.f;

			AttackArea->SetActive(true);

			NowAttackAreaActiveTime += (float)Time->DeltaTime();
		}
		else
		{
			AttackArea->SetActive(false);
		}
	}

	// °ø°ÝÀÌ ¹Ù²î¸é Active Time À» ÃÊ±âÈ­
	if (m_PlayerState->GetIsChangeAction())
	{
		NowAttackAreaActiveTime = 0.0f;
		//¿©±â¼­ ¾ÈÇØÁàµµµÊ
		//AttackArea->SetActive(false);
	}
}

void PlayerAttack::SmashAttack()
{
	// ÇÃ·¹ÀÌ¾îÀÇ »óÅÂ°¡ ½º¸Å½¬ »óÅÂÀÏ ¶§

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
	{
		// ½º¸Å½¬ ¼ÂÆÃ
		if (!IsSmashStart)
		{
			SetSmashAttack();
		}

		if (m_NoteManager->GetIsChangeNote())
		{
			NowSmashNoteCount++;
		}

		if (CurrentSmashNoteCount < SmashInputNoteCount)
		{

			// ÇöÀç BGM Àº ¹Ý ¹Ú ±âÁØÀ¸·Î ³ëÆ®°¡ ¸¸µé¾îÁ® ÀÖÀ½
			// ½º¸Å½Ã´Â ÇÑ ¹Ú ±âÁØÀ¸·Î ÀÔ·Â ¼º°øÀ» ÆÇ´Ü ÇÒ °ÍÀÓ

			if (NowSmashNoteCount >= 1 && NowSmashNoteCount % 2 == 1)
			{
				if (SmashInputNoteCount * 2 >= NowSmashNoteCount)
				{
					CurrentSmashNoteCount = NowSmashNoteCount / 2;

					if (IsUserSmashInput)
					{
						UserSmashInput = -1;
						IsUserSmashInput = false;
					}
					else
					{

						//m_PlayerState->SetAccuracy(NoteManager::NoteErrorRangeCheck::M);
						SmashNoteCheck[CurrentSmashNoteCount] = NoteManager::NoteErrorRangeCheck::M;

					}

					IsNextSmash = true;
				}
			}
			else
			{
				IsNextSmash = false;
			}
			// ½º¸Å½ÃÀÇ ÀÔ·ÂÀ» È®ÀÎÇÏ°í µ¥¹ÌÁö ¹è¼ö¸¦ Áõ°¡½ÃÄÑ¾ß ÇÔ
			CheckSmashAttackInput();
		}

		if (IsNextSmash)
		{
			if (NextSmashTime < 0.2f)
			{
				m_PlayerState->SetSmashTimeScale(0.2f);
				NextSmashTime += (float)Time->DeltaTime();
			}
			else
			{
				m_PlayerState->SetSmashTimeScale(0.1f);
			}
		}
		else
		{
			m_PlayerState->SetSmashTimeScale(0.1f);
			NextSmashTime = 0.0f;
		}

		if (SmashInputNoteCount * 2 < NowSmashNoteCount)
		{
			m_PlayerState->SetSmashTimeScale(1.0f);

			if (NowAttackAreaActiveTime < AttackAreaActiveTime)
			{

				AttackArea->SetSizeX(1.f);
				AttackArea->SetSizeY(1.f);
				AttackArea->SetSizeZ(1.f);
				AttackRange = 1.f;

				hos::Vector3 _AttackPos = PlayerTransform->GetPosition() + (PlayerTransform->GetForward() * AttackRange);

				_AttackPos.y += 1;

				AttackAreaTransform->SetPosition(_AttackPos);

				if (!AttackArea->GetActive())
				{
					AttackArea->SetActive(true);

					//////////////////////////////////////////////////////////////////////////
					// Attack Sound

					ActionSound->Stop();

					switch (m_PlayerState->GetNowPlayerAction())
					{
					case PlayerState::PlayerActionList::Smash:
					{
						ActionSound->SetAudioClip(L"pc_attack_smash1whoosh__");
					}
					break;
					case PlayerState::PlayerActionList::Smash1:
					{
						ActionSound->SetAudioClip(L"pc_attack_smash2whoosh__");
					}
					break;
					case PlayerState::PlayerActionList::Smash2:
					{
						ActionSound->SetAudioClip(L"pc_attack_smash3whoosh__");
					}
					break;
					case PlayerState::PlayerActionList::Smash3:
					{
						ActionSound->SetAudioClip(L"pc_attack_smash4whoosh__");
					}
					break;
					default:
						break;
					}

					ActionSound->Play();

					m_NoteManager->m_AudioSource->SetVolume(1.0f);

				}


				NowAttackAreaActiveTime += (float)Time->DeltaTime();
			}
			else
			{
				AttackArea->SetActive(false);
			}
		}
	}
	else
	{
		IsSmashStart = false;
		m_PlayerState->SetSmashTimeScale(0.1f);
	}

}

void PlayerAttack::FeverSmashAttack()
{
	m_PlayerState->SetSmashTimeScale(1.0f);

	if (!IsSetFeverSmashTime)
	{
		IsSetFeverSmashTime = true;
		FeverSmashTimer = 0.f;
		IsPlayerFeverSmashSound = false;


		switch (m_PlayerState->GetNowPlayerAction())
		{
		case PlayerState::PlayerActionList::Smash:
		{
			FeverSmashStart = 0.48f;
			FeverSmashEnd = 0.53f;
			SmashSuccessInput = 2;
			SmashInputNoteCount = 2;
		}
		break;
		case PlayerState::PlayerActionList::Smash1:
		{
			FeverSmashStart = 0.26f;
			FeverSmashEnd = 0.275f;
			SmashSuccessInput = 3;
			SmashInputNoteCount = 3;
		}
		break;
		case PlayerState::PlayerActionList::Smash2:
		{
			FeverSmashStart = 0.5f;
			FeverSmashEnd = 0.55f;
			SmashSuccessInput = 4;
			SmashInputNoteCount = 4;
		}
		break;
		case PlayerState::PlayerActionList::Smash3:
		{
			FeverSmashStart = 0.58f;
			FeverSmashEnd = 0.63f;
			SmashSuccessInput = 5;
			SmashInputNoteCount = 5;
		}
		break;
		default:
			break;
		}

	}

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
	{
		if (FeverSmashStart < FeverSmashTimer)
		{
			if (m_PlayerState->GetIsFever())
			{
				AttackArea->SetSizeX(1.5f);
				AttackArea->SetSizeY(1.5f);
				AttackArea->SetSizeZ(1.5f);
				AttackRange = 1.5f;
			}
			else
			{
				AttackArea->SetSizeX(1.f);
				AttackArea->SetSizeY(1.f);
				AttackArea->SetSizeZ(1.f);
				AttackRange = 1.f;
			}

			hos::Vector3 _AttackPos = PlayerTransform->GetPosition() + (PlayerTransform->GetForward() * AttackRange);

			_AttackPos.y += 1;

			AttackAreaTransform->SetPosition(_AttackPos);

			AttackArea->SetActive(true);

			if (!IsPlayerFeverSmashSound)
			{
				IsPlayerFeverSmashSound = true;

				ActionSound->Stop();

				//////////////////////////////////////////////////////////////////////////

				switch (m_PlayerState->GetNowPlayerAction())
				{
				case PlayerState::PlayerActionList::Smash:
				{
					ActionSound->SetAudioClip(L"pc_attack_smash1whoosh_");
				}
				break;
				case PlayerState::PlayerActionList::Smash1:
				{
					ActionSound->SetAudioClip(L"pc_attack_smash2whoosh_");
				}
				break;
				case PlayerState::PlayerActionList::Smash2:
				{
					ActionSound->SetAudioClip(L"pc_attack_smash3whoosh_");
				}
				break;
				case PlayerState::PlayerActionList::Smash3:
				{
					ActionSound->SetAudioClip(L"pc_attack_smash4whoosh_");
				}
				break;
				default:
					break;
				}

				ActionSound->Play();
			}


		}
		if (FeverSmashEnd < FeverSmashTimer)
		{
			AttackArea->SetActive(false);
		}

		FeverSmashTimer += (float)Time->DeltaTime();
	}
	else
	{
		IsSetFeverSmashTime = false;
	}
}

void PlayerAttack::CheckSmashAttackInput()
{
	if (!IsUserSmashInput)
	{
		if (Input->GetKeyDown(KeyCode::W))
		{
			UserSmashInput = 0;
			IsUserSmashInput = true;
		}
		if (Input->GetKeyDown(KeyCode::A))
		{
			UserSmashInput = 1;
			IsUserSmashInput = true;
		}
		if (Input->GetKeyDown(KeyCode::S))
		{
			UserSmashInput = 2;
			IsUserSmashInput = true;
		}
		if (Input->GetKeyDown(KeyCode::D))
		{
			UserSmashInput = 3;
			IsUserSmashInput = true;
		}

		// È¦¼ö¿¡ ÀÔ·Â ÇÒ ¶§¸¸ È®ÀÎ (¹Ý ¹ÚÀÚ¸¦ °Ç³Ê¶Ù±â À§ÇØ)
		if (IsUserSmashInput)
		{
			if (NowSmashNoteCount % 2 == 0 && NowSmashNoteCount > 1)
			{
				if (UserSmashInput == CurrentSmashNote[CurrentSmashNoteCount])
				{
					m_PlayerState->SetAccuracy(m_NoteManager->CheckUserInput());
					SmashNoteCheck[CurrentSmashNoteCount] = m_PlayerState->GetAccuracy();

					SmashSuccessInput++;

					switch (CurrentSmashNoteCount)
					{
					case 0:
					{
						HitSound->Stop();
						HitSound->SetAudioClip(L"pc_attack_smashinputsuccess1");
						HitSound->Play();
					}
					break;
					case 1:
					{
						HitSound->Stop();
						HitSound->SetAudioClip(L"pc_attack_smashinputsuccess2");
						HitSound->Play();
					}
					break;
					case 2:
					{
						HitSound->Stop();
						HitSound->SetAudioClip(L"pc_attack_smashinputsuccess3");
						HitSound->Play();
					}
					break;
					case 3:
					{
						HitSound->Stop();
						HitSound->SetAudioClip(L"pc_attack_smashinputsuccess4");
						HitSound->Play();
					}
					break;
					case 4:
					{
						HitSound->Stop();
						HitSound->SetAudioClip(L"pc_attack_smashinputsuccess5");
						HitSound->Play();
					}
					break;
					default:
						break;
					}
				}
				else
				{
					m_PlayerState->SetAccuracy(NoteManager::NoteErrorRangeCheck::M);
					SmashNoteCheck[CurrentSmashNoteCount] = NoteManager::NoteErrorRangeCheck::M;

					HitSound->Stop();
					HitSound->SetAudioClip(L"pc_attack_smashinputfail");
					HitSound->Play();

				}

			}
			else
			{
				m_PlayerState->SetAccuracy(NoteManager::NoteErrorRangeCheck::M);

				//HitSound->Stop();
				//HitSound->SetAudioClip(L"pc_attack_smashinputfail");
				//HitSound->Play();

			}
		}
	}


}

void PlayerAttack::SetSmashAttack()
{
	NowSmashNoteCount = 0;
	CurrentSmashNoteCount = 0;
	UserSmashInput = -1;
	SmashSuccessInput = 0;

	for (int i = 0; i < SmashNoteMaxCount; i++)
	{
		SmashNoteCheck[i] = NoteManager::NoteErrorRangeCheck::W;
		CurrentSmashNote[i] = -1;
	}

	// ¸î ¹øÂ° ½º¸Å½ÃÀÎ°¡?
	switch (m_PlayerState->GetNowPlayerAction())
	{
	case PlayerState::PlayerActionList::Smash:
	{
		SmashInputNoteCount = 2;
	}
	break;
	case PlayerState::PlayerActionList::Smash1:
	{
		SmashInputNoteCount = 3;
	}
	break;
	case PlayerState::PlayerActionList::Smash2:
	{
		SmashInputNoteCount = 4;
	}
	break;
	case PlayerState::PlayerActionList::Smash3:
	{
		SmashInputNoteCount = 5;
	}
	break;

	default:
		break;
	}

	// ¸Â­„ ¹æÇâ Á¤ÇÏ±â // & ÆÇÁ¤ ÃÊ±âÈ­ (ÆÇÁ¤Àº ¹Ù·Î Àû¿ë, µ¥¹ÌÁö ¹è¼öµµ ¹Ù·Î ½×ÀÚ) 
	std::srand((unsigned int)std::time(nullptr));
	for (int i = 0; i < SmashInputNoteCount; i++)
	{
		CurrentSmashNote[i] = std::rand() / ((RAND_MAX + 1u) / 4);
		SmashNoteCheck[i] = NoteManager::NoteErrorRangeCheck::W;
	}

	IsSmashStart = true;

	m_NoteManager->m_AudioSource->SetVolume(0.35f);

}

NoteManager::NoteErrorRangeCheck PlayerAttack::CheckSmashNoteAccurancyAverage()
{
	float _AccurancyAverage = 0.f;

	for (int i = 0; i < SmashInputNoteCount; i++)
	{
		switch (SmashNoteCheck[i])
		{
		case NoteManager::NoteErrorRangeCheck::CP:
		{
			_AccurancyAverage += 4;
		}
		break;
		case NoteManager::NoteErrorRangeCheck::P:
		{
			_AccurancyAverage += 3;
		}
		break;
		case NoteManager::NoteErrorRangeCheck::C:
		{
			_AccurancyAverage += 2;
		}
		break;
		case NoteManager::NoteErrorRangeCheck::N:
		{
			_AccurancyAverage += 1;
		}
		break;
		default:
			break;
		}
	}

	_AccurancyAverage = _AccurancyAverage / (float)SmashInputNoteCount;

	int _SmashNoteAccurancy = (int)ceil(_AccurancyAverage);

	switch (_SmashNoteAccurancy)
	{
	case 4:
	{
		return NoteManager::NoteErrorRangeCheck::CP;
	}
	break;
	case 3:
	{
		return NoteManager::NoteErrorRangeCheck::P;
	}
	break;
	case 2:
	{
		return NoteManager::NoteErrorRangeCheck::C;
	}
	break;
	case 1:
	{
		return NoteManager::NoteErrorRangeCheck::N;
	}
	break;
	case 0:
	{
		return NoteManager::NoteErrorRangeCheck::M;
	}
	break;

	default:
		return NoteManager::NoteErrorRangeCheck::M;
		break;
	}

	return NoteManager::NoteErrorRangeCheck::M;

}

void PlayerAttack::SetAttackDamage()
{
	// °ø°Ý°ú ÄÞº¸, ÆÇÁ¤¿¡ µû¸¥ µ¥¹ÌÁöµé

	// ÄÞº¸ °è¼ö
/*	if (m_PlayerState->GetNowCombo() >= 200)
	{
		ComboDamagedIncrease = 0.5f;
	}
	else if (m_PlayerState->GetNowCombo() >= 190)
	{
		ComboDamagedIncrease = 0.5f;
	}
	else if (m_PlayerState->GetNowCombo() >= 180)
	{
		ComboDamagedIncrease = 0.475f;
	}
	else if (m_PlayerState->GetNowCombo() >= 170)
	{
		ComboDamagedIncrease = 0.475f;
	}
	else */
	if (m_PlayerState->GetNowCombo() >= 160)
	{
		ComboDamagedIncrease = 0.5f;
	}
	else if (m_PlayerState->GetNowCombo() >= 150)
	{
		ComboDamagedIncrease = 0.49f;
	}
	else if (m_PlayerState->GetNowCombo() >= 140)
	{
		ComboDamagedIncrease = 0.48f;
	}
	else if (m_PlayerState->GetNowCombo() >= 130)
	{
		ComboDamagedIncrease = 0.475f;
	}
	else if (m_PlayerState->GetNowCombo() >= 120)
	{
		ComboDamagedIncrease = 0.47f;
	}
	else if (m_PlayerState->GetNowCombo() >= 110)
	{
		ComboDamagedIncrease = 0.44f;
	}
	else if (m_PlayerState->GetNowCombo() >= 100)
	{
		ComboDamagedIncrease = 0.42f;
	}
	else if (m_PlayerState->GetNowCombo() >= 90)
	{
		ComboDamagedIncrease = 0.4f;
	}
	else if (m_PlayerState->GetNowCombo() >= 80)
	{
		ComboDamagedIncrease = 0.38f;
	}
	else if (m_PlayerState->GetNowCombo() >= 70)
	{
		ComboDamagedIncrease = 0.34f;
	}
	else if (m_PlayerState->GetNowCombo() >= 60)
	{
		ComboDamagedIncrease = 0.32f;
	}
	else if (m_PlayerState->GetNowCombo() >= 50)
	{
		ComboDamagedIncrease = 0.3f;
	}
	else if (m_PlayerState->GetNowCombo() >= 40)
	{
		ComboDamagedIncrease = 0.25f;
	}
	else if (m_PlayerState->GetNowCombo() >= 30)
	{
		ComboDamagedIncrease = 0.2f;
	}
	else if (m_PlayerState->GetNowCombo() >= 20)
	{
		ComboDamagedIncrease = 0.15f;
	}
	else if (m_PlayerState->GetNowCombo() >= 10)
	{
		ComboDamagedIncrease = 0.1f;
	}
	else
	{
		ComboDamagedIncrease = 0.0f;
	}

	// ÆÇÁ¤ °è¼ö
	switch (m_PlayerState->GetAccuracy())
	{
	case NoteManager::NoteErrorRangeCheck::CP:
	{
		AccuracyRate = 2.f;
	}
	break;
	case NoteManager::NoteErrorRangeCheck::P:
	{
		AccuracyRate = 1.5f;
	}
	break;
	case NoteManager::NoteErrorRangeCheck::C:
	{
		AccuracyRate = 1.25f;
	}
	break;
	case NoteManager::NoteErrorRangeCheck::N:
	{
		AccuracyRate = 1;
	}
	break;
	case NoteManager::NoteErrorRangeCheck::M:
	{
		AccuracyRate = 0.8f;
	}
	break;
	default:
		break;
	}

	if (m_PlayerState->GetIsFever())
	{
		AccuracyRate = 2.f;
	}

	// º£ÀÌ½º µ¥¹ÌÁö
	switch (m_PlayerState->GetNowPlayerAction())
	{
	case PlayerState::PlayerActionList::Attack1:
	{
		BaseDamage = (NormalAttackDamage + (float)(1 / m_NoteManager->GetOneBeatTime())) * NormalAttackDamageRate * AccuracyRate;
	}
	break;
	case PlayerState::PlayerActionList::Attack2:
	{
		BaseDamage = (NormalAttackDamage + (float)(2 / m_NoteManager->GetOneBeatTime())) * NormalAttackDamageRate * AccuracyRate;
	}
	break;
	case PlayerState::PlayerActionList::Attack3:
	{
		BaseDamage = (NormalAttackDamage + (float)(3 / m_NoteManager->GetOneBeatTime())) * NormalAttackDamageRate * AccuracyRate;

	}
	break;
	case PlayerState::PlayerActionList::ChargeEnd:
	{
		BaseDamage = (NormalAttackDamage + (float)(1 / m_NoteManager->GetOneBeatTime())) * NormalAttackDamageRate * AccuracyRate;
	}
	break;
	case PlayerState::PlayerActionList::Smash:
	{
		// ½º¸Å½Ã µ¥¹ÌÁö
		if (m_PlayerState->GetIsFever())
		{
			BaseDamage = FeverSmash1Damage * (FeverSmashAttackRate + (FeverSmashDamageRate * 1));
		}
		else
		{

			BaseDamage = (SmashAttackDamage * Smash1DamageRate) / (2.f / 2.f);

			// ½º¸Å½Ã ÆÇÁ¤ °è¼ö
			SmashAccuracyRate = SmashBaseRate + (SmashSuccessInput * SmashSuccessRateIncreaseAmount);

			// ½º¸Þ½Ã µ¥¹ÌÁö * ½º¸Þ½Ã ÆÇÁ¤ °è¼ö
			BaseDamage *= SmashAccuracyRate;

		}

		// (±¸) ½º¸Å½Ã µ¥¹ÌÁö
		//BaseDamage = (float)(2 / m_NoteManager->GetOneBeatTime()) * (float)((NormalAttackDamage + (1 / m_NoteManager->GetOneBeatTime()) * NormalAttackDamageRate * AccuracyRate)) * Smash1DamageRate;
	}
	break;
	case PlayerState::PlayerActionList::Smash1:
	{
		if (m_PlayerState->GetIsFever())
		{
			BaseDamage = FeverSmash2Damage * (FeverSmashAttackRate + (FeverSmashDamageRate * 2));
		}
		else
		{
			BaseDamage = (SmashAttackDamage * Smash2DamageRate) / (2.f / 3.f);

			SmashAccuracyRate = SmashBaseRate + (SmashSuccessInput * SmashSuccessRateIncreaseAmount);

			BaseDamage *= SmashAccuracyRate;
		}

		//BaseDamage = (float)(3 / m_NoteManager->GetOneBeatTime()) * (float)((NormalAttackDamage + (1 / m_NoteManager->GetOneBeatTime()) * NormalAttackDamageRate * AccuracyRate)) * Smash2DamageRate;
	}
	break;
	case PlayerState::PlayerActionList::Smash2:
	{
		if (m_PlayerState->GetIsFever())
		{
			BaseDamage = FeverSmash3Damage * (FeverSmashAttackRate + (FeverSmashDamageRate * 3));
		}
		else
		{
			BaseDamage = (SmashAttackDamage * Smash3DamageRate) / (2.f / 4.f);

			SmashAccuracyRate = SmashBaseRate + (SmashSuccessInput * SmashSuccessRateIncreaseAmount);

			BaseDamage *= SmashAccuracyRate;

		}

		//BaseDamage = (float)(4 / m_NoteManager->GetOneBeatTime()) * ((NormalAttackDamage + (float)(1 / m_NoteManager->GetOneBeatTime()) * NormalAttackDamageRate * AccuracyRate)) * Smash3DamageRate;
	}
	break;
	case PlayerState::PlayerActionList::Smash3:
	{
		if (m_PlayerState->GetIsFever())
		{
			BaseDamage = FeverSmash4Damage * (FeverSmashAttackRate + (FeverSmashDamageRate * 4));
		}
		else
		{
			BaseDamage = (SmashAttackDamage * Smash4DamageRate) / (2.f / 5);

			SmashAccuracyRate = SmashBaseRate + (SmashSuccessInput * SmashSuccessRateIncreaseAmount);

			BaseDamage *= SmashAccuracyRate;
		}

		//BaseDamage = (float)(5 / m_NoteManager->GetOneBeatTime()) * ((NormalAttackDamage + (float)(1 / m_NoteManager->GetOneBeatTime()) * NormalAttackDamageRate * AccuracyRate)) * Smash4DamageRate;
	}
	break;
	default:
		break;
	}

	// ÄÞº¸ µ¥¹ÌÁö Áõ°¡À² Àû¿ë
	BaseDamage += (BaseDamage * ComboDamagedIncrease);

	// ÇÇ¹ö Àû¿ë ¿©ºÎ
	if (m_PlayerState->GetIsFever())
	{
		if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack1 ||
			m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack2 ||
			m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Attack3 ||
			m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::ChargeEnd)
		{
			AttackDamage = BaseDamage * FeverNormalDamageRate;
		}
		// ÇÇ¹öÀÏ ¶§ ½º¸Å½Ã
		else
		{
			AttackDamage = BaseDamage;
		}
	}
	else
	{
		AttackDamage = BaseDamage;
	}

}
