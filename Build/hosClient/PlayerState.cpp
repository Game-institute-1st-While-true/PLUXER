#include "PlayerState.h"
#include "PacketDefine.h"

PlayerState::PlayerState() : hos::com::Script(L"PlayerState"), NetEventer(nullptr)
{
	ExecutionOrder = 0;
}

PlayerState::~PlayerState()
{

}

void PlayerState::SetNowPlayerAction(PlayerActionList playerAction)
{
	PrevAction = NowAction;
	PrevEndPoint = this->PlayerAnimation->GetBlendPoint();

	NowAction = playerAction;
	IsChangeAction = true;

	if (PrevAction != NowAction)
	{
		OnStateUpdate();
	}
}

void PlayerState::SetDirection(hos::Vector3 dir, bool on_event)
{
	if (NowAction == PlayerActionList::Move || NowAction == PlayerActionList::Idle || NowAction == PlayerActionList::Dash)
	{
		PreDir = NowDir;
		NowDir = dir;

		if (PreDir != NowDir && !on_event)
		{
			OnStateUpdate();
		}
	};
}

void PlayerState::SetSmashTimeScale(float timeScale)
{
	SmashTimeScale = timeScale;
	if (NowAction == PlayerActionList::Smash ||
		NowAction == PlayerActionList::Smash1 ||
		NowAction == PlayerActionList::Smash2 ||
		NowAction == PlayerActionList::Smash3)
	{
		PlayerAnimation->SetTimeScale(SmashTimeScale);

		if (SmashTimeScale != PreSmashTimeScale)
		{
			OnTimeScaleUpdate();
		}
		PreSmashTimeScale = SmashTimeScale;
	}

}

void PlayerState::GetDamaged(int damage, int damageType, hos::com::Transform* tr)
{
	if (NowAction == PlayerActionList::Dash)
	{
		return;
	}

	if (NowAction != PlayerActionList::Damaged)
	{
		if (NowHealthPoint > 0)
		{
			ResetCombo();

			ComboBonus = 0;

			// 차지 상태면 데미지만 닳게 함
			if (NowAction == PlayerActionList::ChargeStart ||
				NowAction == PlayerActionList::ChargeIdle ||
				NowAction == PlayerActionList::ChargeEnd ||
				NowAction == PlayerActionList::Smash ||
				NowAction == PlayerActionList::Smash1 ||
				NowAction == PlayerActionList::Smash2 ||
				NowAction == PlayerActionList::Smash3 ||
				NowAction == PlayerActionList::FeverCast)
			{
				if (!IsSuperArmor)
				{
					IsSuperArmor = true;

					NowHealthPoint -= damage;

					// 슈퍼아머일 때 소리
					ActionSound->Stop();
					ActionSound->SetAudioClip(L"pc_hit_superarmour");
					ActionSound->Play();

				}

				return;
			}

			NowHealthPoint -= damage;

			SetNowPlayerAction(PlayerActionList::Damaged);

			DamageType = damageType;

			if (DamageType == 0)
			{
				float angle = m_GameObject->transform->GetForward().Dot(tr->GetForward());

				angle = hos::RadToDegree(angle);

				if (angle < 0)
				{
					angle = 360 + angle;
				}

				if (angle >= 45.f && angle < 135.f)
				{
					// 좌
					DamagedDir = 3;
				}
				else if (angle >= 225.f && angle < 315.f)
				{
					// 우
					DamagedDir = 2;
				}
				else if (angle >= 135.f && angle < 225.f)
				{
					// 앞
					DamagedDir = 0;
				}
				else if (angle >= 315.f || angle < 45.f)
				{
					// 뒤
					DamagedDir = 1;
				}

				if (ActionSound)
				{
					ActionSound->Stop();

					int _Rand = (std::rand() / ((RAND_MAX + 1u) / 3));

					switch (_Rand)
					{
					case 0:
					{
						ActionSound->SetAudioClip(L"pc_hit_melee1");
					}
					break;
					case 1:
					{
						ActionSound->SetAudioClip(L"pc_hit_melee2");
					}
					break;
					case 2:
					{
						ActionSound->SetAudioClip(L"pc_hit_melee3");

					}
					break;
					default:
						break;
					}
					ActionSound->Play();
				}

			}

			if (DamageType == 1)
			{
				hos::Vector3 _temp = tr->GetPosition() - m_GameObject->transform->GetPosition();

				_temp.Normalize();

				m_GameObject->transform->SetForward(_temp);

				if (ActionSound)
				{// 넘어질 때 소리				
					ActionSound->Stop();
					ActionSound->SetAudioClip(L"pc_hit_falldown");
					ActionSound->Play();
				}
			}

			//////////////////////////////////////////////////////////////////////////
		}
	}
}

void PlayerState::SetAccuracy(NoteManager::NoteErrorRangeCheck val)
{
	if (IsFever)
	{
		Accuracy = NoteManager::NoteErrorRangeCheck::CP;
	}
	else
	{
		Accuracy = val;
	}

	if (Accuracy == NoteManager::NoteErrorRangeCheck::M)
	{
		ResetCombo();

		ComboBonus -= 100;

		if (ComboBonus < 0)
		{
			ComboBonus = 0;
		}

		Judge[static_cast<int>(Accuracy)] += 1;
	}

	IsChangeAccuracy = true;
}

void PlayerState::GetDamaged(int damage, int damageType, float angle)
{
	if (NowAction == PlayerActionList::Dash)
	{
		return;
	}

	if (NowAction != PlayerActionList::Damaged)
	{
		if (NowHealthPoint > 0)
		{
			ResetCombo();

			ComboBonus = 0;

			// 차지 상태면 데미지만 닳게 함
			if (NowAction == PlayerActionList::ChargeStart ||
				NowAction == PlayerActionList::ChargeIdle ||
				NowAction == PlayerActionList::ChargeEnd ||
				NowAction == PlayerActionList::Smash ||
				NowAction == PlayerActionList::Smash1 ||
				NowAction == PlayerActionList::Smash2 ||
				NowAction == PlayerActionList::Smash3 ||
				NowAction == PlayerActionList::FeverCast)
			{

				if (!IsSuperArmor)
				{
					IsSuperArmor = true;

					NowHealthPoint -= damage;

					// 슈퍼아머일 때 소리
					if (ActionSound)
					{
						ActionSound->Stop();
						ActionSound->SetAudioClip(L"pc_hit_superarmour");
						ActionSound->Play();
					}
				}
				return;
			}

			NowHealthPoint -= damage;

			SetNowPlayerAction(PlayerActionList::Damaged);

			DamageType = damageType;

			if (DamageType == 0)
			{
				if (angle < 0)
				{
					angle = 360 + angle;
				}

				if (angle >= 45.f && angle < 135.f)
				{
					// 좌
					DamagedDir = 3;
				}
				else if (angle >= 225.f && angle < 315.f)
				{
					// 우
					DamagedDir = 2;
				}
				else if (angle >= 135.f && angle < 225.f)
				{
					// 앞
					DamagedDir = 0;
				}
				else if (angle >= 315.f || angle < 45.f)
				{
					// 뒤
					DamagedDir = 1;
				}

				if (ActionSound)
				{
					ActionSound->Stop();

					int _Rand = (std::rand() / ((RAND_MAX + 1u) / 3));

					switch (_Rand)
					{
					case 0:
						ActionSound->SetAudioClip(L"pc_hit_melee1");
						break;

					case 1:
						ActionSound->SetAudioClip(L"pc_hit_melee2");
						break;

					case 2:
						ActionSound->SetAudioClip(L"pc_hit_melee3");
						break;
					default:
						break;
					}

					ActionSound->Play();
				}
			}
			else
			{
				if (ActionSound)
				{
					// 넘어질 때 소리				
					ActionSound->Stop();
					ActionSound->SetAudioClip(L"pc_hit_falldown");
					ActionSound->Play();
				}
			}

		}
	}
}

void PlayerState::AddNowFeverGage()
{
	if (!IsFever)
	{
		if (NowAction == PlayerActionList::Attack1 ||
			NowAction == PlayerActionList::Attack2 ||
			NowAction == PlayerActionList::Attack3 ||
			NowAction == PlayerActionList::ChargeEnd)
		{
			switch (Accuracy)
			{
			case NoteManager::NoteErrorRangeCheck::CP:
			{
				NowFeverGage += NormalFeverCP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::P:
			{
				NowFeverGage += NormalFeverP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::C:
			{
				NowFeverGage += NormalFeverC;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::N:
			{
				NowFeverGage += NormalFeverN;
			}
			break;
			default:
				break;
			}
		}
		else
		{

			int _NowSmash = 0;

			switch (NowAction)
			{

			case PlayerState::PlayerActionList::Smash:
				_NowSmash = 2;
				break;
			case PlayerState::PlayerActionList::Smash1:
				_NowSmash = 3;
				break;
			case PlayerState::PlayerActionList::Smash2:
				_NowSmash = 4;
				break;
			case PlayerState::PlayerActionList::Smash3:
				_NowSmash = 5;
				break;
			}

			switch (Accuracy)
			{
			case NoteManager::NoteErrorRangeCheck::CP:
			{
				SmashFeverCP = NormalFeverCP * _NowSmash * 1.3f;

				NowFeverGage += SmashFeverCP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::P:
			{
				SmashFeverP = NormalFeverP * _NowSmash * 1.3f;

				NowFeverGage += SmashFeverP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::C:
			{
				SmashFeverC = NormalFeverC * _NowSmash * 1.3f;

				NowFeverGage += SmashFeverC;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::N:
			{
				SmashFeverN = NormalFeverN * _NowSmash * 1.3f;

				NowFeverGage += SmashFeverN;
			}
			break;
			default:
				break;
			}

		}

		if (NowFeverGage > MaxFeverGage)
		{
			NowFeverGage = MaxFeverGage;
		}
	}
}

void PlayerState::SetNowFeverGage(float fever)
{
	NowFeverGage = fever;
}

void PlayerState::AddNowFeverGage(NoteManager::NoteErrorRangeCheck val)
{
	if (!IsFever)
	{
		if (NowAction == PlayerActionList::Attack1 ||
			NowAction == PlayerActionList::Attack2 ||
			NowAction == PlayerActionList::Attack3 ||
			NowAction == PlayerActionList::ChargeEnd)
		{
			switch (val)
			{
			case NoteManager::NoteErrorRangeCheck::CP:
			{
				NowFeverGage += NormalFeverCP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::P:
			{
				NowFeverGage += NormalFeverP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::C:
			{
				NowFeverGage += NormalFeverC;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::N:
			{
				NowFeverGage += NormalFeverN;
			}
			break;
			default:
				break;
			}
		}
		else
		{
			switch (val)
			{
			case NoteManager::NoteErrorRangeCheck::CP:
			{
				NowFeverGage += SmashFeverCP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::P:
			{
				NowFeverGage += SmashFeverP;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::C:
			{
				NowFeverGage += SmashFeverC;
			}
			break;
			case NoteManager::NoteErrorRangeCheck::N:
			{
				NowFeverGage += SmashFeverN;
			}
			break;
			default:
				break;
			}
		}
	}
}

int PlayerState::GetJudge(NoteManager::NoteErrorRangeCheck j) const
{
	return Judge[static_cast<int>(j)];
}

void PlayerState::SetNowCombo(int combo)
{
	NowCombo = combo;
}

void PlayerState::ComboIncrease()
{
	NowCombo++;

	IsComboChange = true;

	if (NowCombo > MaxCombo)
	{
		MaxCombo = NowCombo;
	}
}

void PlayerState::ResetCombo()
{
	NowCombo = 0;

	IsComboChange = true;
}

void PlayerState::AddScore(float attackDamage)
{
	int _NowAttackDamage = 0;
	float _CheckScore = 0;
	int _BonusScore = 0;
	bool IsSmash = false;

	if (NowAction == PlayerActionList::Smash ||
		NowAction == PlayerActionList::Smash1 ||
		NowAction == PlayerActionList::Smash2 ||
		NowAction == PlayerActionList::Smash3)
	{
		IsSmash = true;
	}

	_NowAttackDamage = (int)(floor(attackDamage + 0.5f));

	if (NetEventer == nullptr)
	{
		switch (Accuracy)
		{
		case NoteManager::NoteErrorRangeCheck::CP:
		{
			// 현재 판정에 따른 점수
			//_AddScore = ScoreCP;

			// 현재 판정에 따른 보너스 점수
			ComboBonus += ComboBonusCP;

			if (ComboBonus > 100)
			{
				ComboBonus = 100;
			}

			if (ComboBonus < 0)
			{
				ComboBonus = 0;
			}

			if (IsSmash)
			{
				_CheckScore = float(_NowAttackDamage * SmashBonusCP);// + (BonusCP * (ComboBonus * 0.01f));
			}
			else
			{
				_CheckScore = float(_NowAttackDamage * NormalBonusCP);// + (BonusCP * (ComboBonus * 0.01f));
			}

			_NowAttackDamage = (int)(floor(_CheckScore + 0.5f));

			/*
			_CheckScore = BonusCP * (ComboBonus * 0.01f);

			// 현재 콤보 보너스
			_BonusScore = (int)(floor(_CheckScore + 0.5f));

			_NowAttackDamage += _BonusScore;

			if (IsSmash)
			{
				float _SmashScore = _NowAttackDamage * SmashBonusCP;
				_NowAttackDamage = (int)(floor(_SmashScore + 0.5f));
			}
			*/

		}
		break;
		case NoteManager::NoteErrorRangeCheck::P:
		{
			//_AddScore = ScoreP;
			ComboBonus += ComboBonusP;

			if (ComboBonus > 100)
			{
				ComboBonus = 100;
			}

			if (ComboBonus < 0)
			{
				ComboBonus = 0;
			}

			if (IsSmash)
			{
				_CheckScore = float(_NowAttackDamage * SmashBonusP);// + (BonusP * (ComboBonus * 0.01f));
			}
			else
			{
				_CheckScore = float(_NowAttackDamage * NormalBonusP);// +(BonusP * (ComboBonus * 0.01f));
			}

			_NowAttackDamage = (int)(floor(_CheckScore + 0.5f));

			/*
			_CheckScore = BonusP * (ComboBonus * 0.01f);

			_BonusScore = (int)(floor(_CheckScore + 0.5f));

			_NowAttackDamage += _BonusScore;

			if (IsSmash)
			{
				float _SmashScore = _NowAttackDamage * SmashBonusP;
				_NowAttackDamage = (int)(floor(_SmashScore + 0.5f));
			}
			*/
		}
		break;
		case NoteManager::NoteErrorRangeCheck::C:
		{
			//_AddScore = ScoreC;
			ComboBonus += ComboBonusC;

			if (ComboBonus > 100)
			{
				ComboBonus = 100;
			}

			if (ComboBonus < 0)
			{
				ComboBonus = 0;
			}

			if (IsSmash)
			{
				_CheckScore = float(_NowAttackDamage * SmashBonusC);// +(BonusC * (ComboBonus * 0.01f));
			}
			else
			{
				_CheckScore = float(_NowAttackDamage * NormalBonusC);// +(BonusC * (ComboBonus * 0.01f));
			}

			_NowAttackDamage = (int)(floor(_CheckScore + 0.5f));

			/*_CheckScore = BonusC * (ComboBonus * 0.01f);

			_BonusScore = (int)(floor(_CheckScore + 0.5f));

			_NowAttackDamage += _BonusScore;

			if (IsSmash)
			{
				float _SmashScore = _NowAttackDamage * SmashBonusC;
				_NowAttackDamage = (int)(floor(_SmashScore + 0.5f));
			}*/

		}
		break;
		case NoteManager::NoteErrorRangeCheck::N:
		{
			//_AddScore = ScoreN;
			ComboBonus += ComboBonusN;

			if (ComboBonus > 100)
			{
				ComboBonus = 100;
			}

			if (ComboBonus < 0)
			{
				ComboBonus = 0;
			}

			if (IsSmash)
			{
				_CheckScore = float(_NowAttackDamage * SmashBonusN);// +(BonusN * (ComboBonus * 0.01f));
			}
			else
			{
				_CheckScore = float(_NowAttackDamage * NormalBonusN);// +(BonusN * (ComboBonus * 0.01f));
			}

			_NowAttackDamage = (int)(floor(_CheckScore + 0.5f));

			/*_CheckScore = BonusN * (ComboBonus * 0.01f);

			_BonusScore = (int)(floor(_CheckScore + 0.5f));

			_NowAttackDamage += _BonusScore;

			if (IsSmash)
			{
				float _SmashScore = _NowAttackDamage * SmashBonusN;
				_NowAttackDamage = (int)(floor(_SmashScore + 0.5f));
			}*/

		}
		break;
		case NoteManager::NoteErrorRangeCheck::M:
		{
			//_AddScore = ScoreM;
			ComboBonus += ComboBonusM;

			if (ComboBonus > 100)
			{
				ComboBonus = 100;
			}

			if (ComboBonus < 0)
			{
				ComboBonus = 0;
			}

			if (IsSmash)
			{
				_CheckScore = float(_NowAttackDamage * SmashBonusM);// +(BonusM * (ComboBonus * 0.01f));
			}
			else
			{
				_CheckScore = float(_NowAttackDamage * NormalBonusM);// +(BonusM * (ComboBonus * 0.01f));
			}

			_NowAttackDamage = (int)(floor(_CheckScore + 0.5f));

			/*_CheckScore = BonusM * (ComboBonus * 0.01f);

			_BonusScore = (int)(floor(_CheckScore + 0.5f));

			_NowAttackDamage += _BonusScore;

			if (IsSmash)
			{
				float _SmashScore = _NowAttackDamage * SmashBonusM;
				_NowAttackDamage = (int)(floor(_SmashScore + 0.5f));
			}*/

		}
		break;
		default:
			break;
		}
		NowScore += _NowAttackDamage;
	}
	Judge[static_cast<int>(Accuracy)] += 1;
}

void PlayerState::SetNowScore(int score)
{
	NowScore = score;
}

void PlayerState::SetIsSlow(bool val, bool fource)
{
	if (!fource)
	{
		if (val)
		{
			SlowStack++;
		}
		else
		{
			SlowStack--;

			if (SlowStack < 0)
			{
				SlowStack = 0;
			}

		}
		if (SlowStack > 0)
		{
			IsSlow = true;
		}
		else
		{
			IsSlow = false;
		}
	}
	else
	{
		if (IsSlow != val)
		{
			IsSlow = val;
		}
	}
}

void PlayerState::Reset()
{

}

void PlayerState::Awake()
{
	// 방향정보 초기화
	DirTimePoint = std::chrono::system_clock::now();

	// 애니메이션 초기화
	PlayerAnimation = m_GameObject->transform->GetChild(0)->m_GameObject->GetComponent<hos::com::Animation>();

	if (PlayerAnimation == nullptr)
	{
		PlayerAnimation = m_GameObject->transform->GetChild(1)->m_GameObject->GetComponent<hos::com::Animation>();
	}
	PlayerDamgedArea = m_GameObject->GetComponent<hos::com::CapsuleCollision>();

	// 무기
	Weapon1 = m_GameObject->m_Scene->FindGameObjectWithName(L"Weapon_01");
	Weapon2 = m_GameObject->m_Scene->FindGameObjectWithName(L"Weapon_02");

	Weapon1MeshRenderer = Weapon1->GetComponent<hos::com::MeshRenderer>();

	g_DataManager->LoadTexture(L"Weapon1F_Emissive.tga");
	g_DataManager->LoadTexture(L"Weapon1N_Emissive.tga");
	g_DataManager->LoadTexture(L"Weapon1OFF_Emissive.tga");


	//////////////////////////////////////////////////////////////////////////
	// Audio Source Data Load
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_hit_melee1.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_hit_melee2.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_hit_melee3.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\ui_hud_fevergaugefull.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_hit_falldown.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_hit_superarmour.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_status_poweroff.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\pc_die.sclip");

	std::vector<hos::com::AudioSource*> audios = m_GameObject->GetComponents<hos::com::AudioSource>();

	if (audios.size() > 0)
	{
		if (audios[0])
		{
			ActionSound = audios[0];
		}
	}
	if (audios.size() > 2)
	{
		if (audios[2])
		{
			StaminaEmptySound = audios[2];
			StaminaEmptySound->SetAudioClip(L"pc_status_poweroff");
			StaminaEmptySound->SetIs3D(false);
			StaminaEmptySound->SetPlayOnAwake(false);
			StaminaEmptySound->SetLoop(false);
		}
		if (audios[3])
		{
			FeverGageFullSound = audios[3];
			FeverGageFullSound->SetAudioClip(L"ui_hud_fevergaugefull");
			FeverGageFullSound->SetIs3D(false);
			FeverGageFullSound->SetPlayOnAwake(false);
			FeverGageFullSound->SetLoop(false);
		}
	}
}

void PlayerState::Start()
{

	NowAction = PlayerActionList::Idle;
	PrevAction = PlayerActionList::Idle;
	IsStop = false;

	SetNowPlayerAction(PlayerActionList::Idle);



}

void PlayerState::Update()
{
	IsChangeAction = false;
	IsChangeAccuracy = false;
	if (PlayerAnimation)
	{
		IsPlayerAnimationEnd = PlayerAnimation->GetIsAnimationEnd();

		if (NowHealthPoint <= 0)
		{
			SetNowPlayerAction(PlayerActionList::Dead);
			m_GameObject->GetComponent<hos::com::Rigidbody>()->SetUseGravity(false);
			PlayerDamgedArea->SetActive(false);
		}

		if (IsPlayerAnimationEnd &&
			NowAction == PlayerActionList::ChargeStart)
		{
			SetNowPlayerAction(PlayerActionList::ChargeIdle);
		}

		if (!IsPlayerAnimationEnd && NowAction == PlayerActionList::FeverCast)
		{
			if (PlayerAnimation->GetNowTick() >= 0.85f)
			{
				Weapon1->SetActive(false);
				Weapon2->SetActive(true);
			}

			if (PlayerAnimation->GetNowTick() > 1.45f)
			{
				IsFever = true;
				NowStamina = MaxStamina;
			}
		}

		if (IsPlayerAnimationEnd &&
			NowAction != PlayerActionList::Idle &&
			NowAction != PlayerActionList::Move &&
			NowAction != PlayerActionList::Dead &&
			NowAction != PlayerActionList::ChargeIdle)
		{
			SetNowPlayerAction(PlayerActionList::Idle);
		}

		if (IsSuperArmor)
		{
			if (SuperArmorTimer > SuperArmorTime)
			{
				IsSuperArmor = false;
			}
			else
			{
				SuperArmorTimer += (float)Time->DeltaTime();
			}
		}

		if (m_GameObject->rigidbody)
			m_GameObject->rigidbody->ClearForce();
	}

	FeverState();
	CheckStaminaEmpty();

	if (IsShakeCamera)
	{
		if (ShakeTimer > ShakeTime)
		{
			IsShakeCamera = false;
			ShakeTimer = 0.f;
		}
		else
		{
			ShakeTimer += (float)Time->DeltaTime();
		}
	}

	WeaponState();

	if (bDebug)
	{
		if (Input->GetKeyDown(KeyCode::Add))
		{
			NowHealthPoint = MaxHealthPoint;
		}
	}
}

void PlayerState::LateUpdate()
{
	switch (NowAction)
	{
	case PlayerState::PlayerActionList::Dead:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"die") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"die", 0, true);

				if (ActionSound)
				{
					ActionSound->Stop();
					ActionSound->SetLoop(false);
					ActionSound->SetAudioClip(L"pc_die");
					ActionSound->Play();
				}
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Damaged:
	{
		if (PlayerAnimation)
		{
			// 현재 방향
			switch (DamageType)
			{
			case 0:
			{
				switch (DamagedDir)
				{
				case 0:
				{
					if (PlayerAnimation->GetAniClipName().compare(L"damaged_front") != 0)
					{
						PlayerAnimation->SetAnimation(L"character", L"damaged_front");
					}
				}
				break;
				case 1:
				{
					if (PlayerAnimation->GetAniClipName().compare(L"damaged_back") != 0)
					{
						PlayerAnimation->SetAnimation(L"character", L"damaged_back");
					}
				}
				break;
				case 2:
				{
					if (PlayerAnimation->GetAniClipName().compare(L"damaged_right") != 0)
					{
						PlayerAnimation->SetAnimation(L"character", L"damaged_right");
					}
				}
				break;
				case 3:
				{
					if (PlayerAnimation->GetAniClipName().compare(L"damaged_left") != 0)
					{
						PlayerAnimation->SetAnimation(L"character", L"damaged_left");
					}
				}
				break;
				}
			}
			break;
			case 1:
			{
				if (PlayerAnimation->GetAniClipName().compare(L"fallstand") != 0)
				{
					PlayerAnimation->SetAnimation(L"character", L"fallstand");
				}
			}
			break;
			default:
				break;
			}
		}
		break;
	}
	case PlayerState::PlayerActionList::ChargeStart:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"charge") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"charge");
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::ChargeIdle:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"chargeidle") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"chargeidle");
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::ChargeEnd:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"chargeattack") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"chargeattack");
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Dash:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"dash_0") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"dash_0");
				PlayerAnimation->SetTimeScale(DashTimeScale);
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Parrying:
		break;
	case PlayerState::PlayerActionList::Smash:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"smashattack_4_0") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"smashattack_4_0");
				if (!IsFever)
				{
					PlayerAnimation->SetTimeScale(SmashTimeScale);
				}
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Smash1:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"smashattack_1_0") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"smashattack_1_0");
				if (!IsFever)
				{
					PlayerAnimation->SetTimeScale(SmashTimeScale);
				}
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Smash2:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"smashattack_2_0") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"smashattack_2_0");
				if (!IsFever)
				{
					PlayerAnimation->SetTimeScale(SmashTimeScale);
				}
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Smash3:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"smashattack_3_0") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"smashattack_3_0");
				if (!IsFever)
				{
					PlayerAnimation->SetTimeScale(SmashTimeScale);
				}
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::DashAttack:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"dashattack_0") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"dashattack_0");
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Attack1:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"normalattack_1_0") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"normalattack_1_0");
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Attack2:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"normalattack_2_0") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"normalattack_2_0");
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Attack3:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"normalattack_3_0") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"normalattack_3_0");
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::Move:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"run") != 0)
			{
				if (this->PrevAction == PlayerState::PlayerActionList::Idle)
				{
					PlayerAnimation->SetBlendAnimation(L"character", L"run", L"idle", PrevEndPoint, 0.15f);
				}
				else
				{
					PlayerAnimation->SetAnimation(L"character", L"run");
				}
			}

			if (IsSlow)
			{
				if (PlayerAnimation->GetTimeScale() > 0.5f)
				{
					PlayerAnimation->SetTimeScale(0.5f);
				}
			}
			else
			{
				if (PlayerAnimation->GetTimeScale() <= 0.5f)
				{
					PlayerAnimation->SetTimeScale(1.f);
				}
			}

		}
	}
	break;
	case PlayerState::PlayerActionList::Idle:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"idle") != 0)
			{
				if (this->PrevAction == PlayerState::PlayerActionList::Move)
				{
					PlayerAnimation->SetBlendAnimation(L"character", L"idle", L"run", PrevEndPoint, 0.15f);
				}

				else
				{
					PlayerAnimation->SetAnimation(L"character", L"idle");
				}
			}
		}
	}
	break;
	case PlayerState::PlayerActionList::FeverCast:
	{
		if (PlayerAnimation)
		{
			if (PlayerAnimation->GetAniClipName().compare(L"fevercast") != 0)
			{
				PlayerAnimation->SetAnimation(L"character", L"fevercast");
			}
		}
	}
	break;
	default:
		break;
	}
}

void PlayerState::Render()
{
	//int x = 200;
	//int y = 200;
	//int font = 14;

	//switch (Accuracy)
	//{
	//case NoteManager::NoteErrorRangeCheck::CP:
	//	g_Renderer->_DebugText(x, y, font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Critical Perfect");
	//	break;
	//case NoteManager::NoteErrorRangeCheck::P:
	//	g_Renderer->_DebugText(x, y, font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Perfect");
	//	break;
	//case NoteManager::NoteErrorRangeCheck::C:
	//	g_Renderer->_DebugText(x, y, font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Cool");
	//	break;
	//case NoteManager::NoteErrorRangeCheck::N:
	//	g_Renderer->_DebugText(x, y, font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Nice");
	//	break;
	//case NoteManager::NoteErrorRangeCheck::M:
	//	g_Renderer->_DebugText(x, y, font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Miss");
	//	break;
	//case NoteManager::NoteErrorRangeCheck::W:
	//	g_Renderer->_DebugText(x, y, font, hos::Vector4(1, 1, 1, 1), L"Accuracy : Wait");
	//	break;
	//default:
	//	break;
	//}

	//g_Renderer->_DebugText(x, y + 20, font, hos::Vector4(1, 1, 1, 1), L"Damaged Dir : %d", DamagedDir);
	//g_Renderer->_DebugText(x, y + 40, font, hos::Vector4(1, 1, 1, 1), L"Player Position : %f, %f, %f", m_GameObject->transform->GetPosition().x, m_GameObject->transform->GetPosition().y, m_GameObject->transform->GetPosition().z);
}

void PlayerState::OnCollisionEnter(hos::com::Collision* other)
{
	int a = 0;
}

void PlayerState::OnStateUpdate()
{
	//hos::string_literal now[] = {
	//	L"Dead",
	//	L"Damaged", L"Charge", L"Dash", L"Parrying", L"Smash", L"Smash1", L"Smash2", L"Smash3", L"DashAttack", L"Attack1", L"Attack2", L"Attack3", L"Move", L"Idle"
	//};

	//NetLogger::GetIns()->Write(L"test", L"State Update !!!!  ", now[static_cast<int>(NowAction)]);

	// 여기서 Send Message
	if (!NetEventer) return;

	auto End = std::chrono::system_clock::now() - DirTimePoint;

	if (End.count() < 0.016) return;

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Int("Id", CharacterId);
		builder.Int("State", static_cast<int>(NowAction));
		builder.Float("RotX", m_GameObject->transform->GetRotation().x);
		builder.Float("RotY", m_GameObject->transform->GetRotation().y);
		builder.Float("RotZ", m_GameObject->transform->GetRotation().z);
		builder.Float("DirX", NowDir.x);
		builder.Float("DirY", NowDir.y);
		builder.Float("DirZ", NowDir.z);
		builder.Float("Hp", (float)NowHealthPoint);
		builder.Int("St", NowStamina);
		builder.Float("Fiber", 0.0);
		builder.Bool("On", IsChangeAction);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned short>(PROTOCOL::STATE_EVENT);
	pack.Make(pack, pack.head, buffer, (int)builder.GetSize());
	NetEventer->network->SendMsg(pack);

	DirTimePoint = std::chrono::system_clock::now();
}

void PlayerState::OnTimeScaleUpdate()
{
	// 여기서 Send Message
	if (!NetEventer) return;

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Int("Id", CharacterId);
		builder.Int("State", static_cast<int>(NowAction));
		builder.Float("Scale", SmashTimeScale);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned short>(PROTOCOL::TIME_SCALE_EVENT);
	pack.Make(pack, pack.head, buffer, (int)builder.GetSize());

	NetEventer->network->SendMsg(pack);
}

void PlayerState::RequestFever()
{
	// 여기서 Send Message
	if (!NetEventer) return;

	flexbuffers::Builder builder;
	builder.Map([&] {
		builder.Int("Id", CharacterId);
		});

	builder.Finish();
	const uint8_t* buffer = builder.GetBuffer().data();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned short>(PROTOCOL::FEVER_EVENT);
	pack.Make(pack, pack.head, buffer, (int)builder.GetSize());

	NetEventer->network->SendMsg(pack);
}

void PlayerState::FeverState()
{
	if (IsFever)
	{
		if (GageRemoveTimer > GageRemoveTime)
		{
			if (NowFeverGage >= 0)
			{
				NowFeverGage -= FeverGageDecreaseAmount;
			}

			GageRemoveTimer = 0.f;
		}
		else
		{
			GageRemoveTimer += (float)Time->DeltaTime();
		}

		if (NowFeverGage <= 0)
		{
			if (NowAction == PlayerActionList::Smash ||
				NowAction == PlayerActionList::Smash1 ||
				NowAction == PlayerActionList::Smash2 ||
				NowAction == PlayerActionList::Smash3)
			{

			}
			else
			{
				NowFeverGage = 0;
				IsFever = false;
				Weapon1->SetActive(true);
				Weapon2->SetActive(false);
			}
		}
	}

	if (NowFeverGage >= MaxFeverGage)
	{
		IsGageFull = true;

		if (!IsGageFullSound)
		{
			IsGageFullSound = true;

			if (FeverGageFullSound)
				FeverGageFullSound->Play();
		}

	}
	else
	{
		IsGageFull = false;
		IsGageFullSound = false;
	}

}

void PlayerState::WeaponState()
{
	if (NowStamina > 0)
	{
		Weapon1MeshRenderer->GetMaterialRef()->SetEmissTex(L"Weapon1N_Emissive.tga");
	}
	else
	{
		Weapon1MeshRenderer->GetMaterialRef()->SetEmissTex(L"Weapon1OFF_Emissive.tga");
	}

	if (IsGageFull)
	{
		Weapon1MeshRenderer->GetMaterialRef()->SetEmissTex(L"Weapon1F_Emissive.tga");
	}


}

void PlayerState::CheckStaminaEmpty()
{
	if (NowStamina == 0)
	{
		if (!IsStaminaEmpty)
		{
			IsStaminaEmpty = true;

			StaminaEmptySound->Play();

		}
	}
	else
	{
		IsStaminaEmpty = false;
	}
}
