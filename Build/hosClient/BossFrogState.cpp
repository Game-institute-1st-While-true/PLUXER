#include "BossFrogState.h"

BossFrogState::BossFrogState() : hos::com::Script(L"BossFrogState")
{

}

BossFrogState::~BossFrogState()
{

}

void BossFrogState::SetNowAction(BossFrogActionList val)
{
	PrevAction = NowAction;
	NowAction = val;
}

void BossFrogState::SetDirection(hos::Vector3 dir, bool on_event)
{
	PreDir = NowDir;
	NowDir = dir;

	if (PreDir != NowDir && !on_event)
	{
		OnStateUpdate();
	}
}

float BossFrogState::GetDamaged(float damage, int damagedType)
{
	if (AIType == 0)
	{
		if (HealthPoint <= 0)
		{
			HealthPoint = 0.f;
			return 0.f;
		}

		//if (!IsDamaged)
		//{
			//////////////////////////////////////////////////////////////////////////
			// Sound

			BossFrogDamagedSound->Stop();
			BossFrogDamagedSound->SetLoop(false);

			int _Rand = (std::rand() / ((RAND_MAX + 1u) / 3));

			switch (_Rand)
			{
			case 0:
			{
				BossFrogDamagedSound->SetAudioClip(L"mob_hit_melee1");
			}
			break;
			case 1:
			{
				BossFrogDamagedSound->SetAudioClip(L"mob_hit_melee2");
			}
			break;
			case 2:
			{
				BossFrogDamagedSound->SetAudioClip(L"mob_hit_melee3");

			}
			break;
			default:
				break;
			}
			BossFrogDamagedSound->Play();

			float _temp = 0.f;

			_temp = HealthPoint - damage;

			IsDamaged = true;
			HealthPoint -= damage;

			if (_temp <= 0.f)
			{
				NowHitValue = damage + _temp;
				return NowHitValue;
			}
			else
			{
				NowHitValue = damage;
				return NowHitValue;
			}
		//}
	}
	else
	{
		if (HealthPoint <= 0)
		{
			HealthPoint = 0.f;
			return 0.f;
		}
		else
		{
			BossFrogDamagedSound->Stop();
			BossFrogDamagedSound->SetLoop(false);

			int _Rand = (std::rand() / ((RAND_MAX + 1u) / 3));

			switch (_Rand)
			{
			case 0:
			{
				BossFrogDamagedSound->SetAudioClip(L"mob_hit_melee1");
			}
			break;
			case 1:
			{
				BossFrogDamagedSound->SetAudioClip(L"mob_hit_melee2");
			}
			break;
			case 2:
			{
				BossFrogDamagedSound->SetAudioClip(L"mob_hit_melee3");

			}
			break;
			default:
				break;
			}
			BossFrogDamagedSound->Play();

			float _temp = 0.f;

			_temp = HealthPoint - damage;

			IsDamaged = true;
			HealthPoint -= damage;

			if (_temp <= 0.f)
			{
				NowHitValue = damage + _temp;
				return NowHitValue;
			}
			else
			{
				NowHitValue = damage;
				return NowHitValue;
			}
		}
	}
	return 0.f;
}

void BossFrogState::Reset()
{

}

void BossFrogState::Awake()
{
	BossFrogAnimation = m_GameObject->transform->GetChild(0)->m_GameObject->GetComponent<hos::com::Animation>();

	m_PlayerState = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerState>();

	HealthPoint = MaxHealthPoint;

	IsAnimationEnd = false;
	IsDamaged = false;
	DamagedTime = 0.3f;
	AIType = 0;

	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_hit_melee1.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_hit_melee2.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_hit_melee3.sclip");

	BossFrogDamagedSound = m_GameObject->GetComponents<hos::com::AudioSource>()[2];
	BossFrogDamagedSound->SetAudioClip(L"mob_hit_melee1");
	BossFrogDamagedSound->SetPlayOnAwake(false);
	BossFrogDamagedSound->SetLoop(false);
	BossFrogDamagedSound->SetIs3D(true);

	BossTrafficLight = m_GameObject->m_Scene->FindGameObjectWithName(L"BossMonster_Frame")->GetComponent<hos::com::SkinnedMeshRenderer>();
	BossBody = m_GameObject->m_Scene->FindGameObjectWithName(L"BossMonster_Body")->GetComponent<hos::com::SkinnedMeshRenderer>();

	/// 텍스쳐
	g_DataManager->LoadTexture(L"Boss_green_Emissive.tga");
	g_DataManager->LoadTexture(L"Boss_yellow_Emissive.tga");
	g_DataManager->LoadTexture(L"Boss_red_Emissive.tga");
	g_DataManager->LoadTexture(L"T_Hit_Emissive.tga");
	g_DataManager->LoadTexture(L"T_Die_Emissive.tga");
	g_DataManager->LoadTexture(L"BossMonster_Body_Emissive.tga");

}

void BossFrogState::Start()
{
	if (m_GameObject->rigidbody)
		m_GameObject->GetComponent<hos::com::Rigidbody>()->SetDrag(100.f);
}

void BossFrogState::Update()
{
	PlayerSmashTime();
	PlayerFeverTime();

	IsAnimationEnd = BossFrogAnimation->GetIsAnimationEnd();

	// 피격 받으면 몇초간 데미지 안박히게
	double delta = Time->DeltaTime();

	if (IsDamaged)
	{
		if (DamagedTime < DamagedTimer)
		{
			IsDamaged = false;
			DamagedTimer = 0.f;
		}
		else
		{
			DamagedTimer += (float)delta;
		}
	}
	if (HealthPoint <= 0)
	{
		SetNowAction(BossFrogActionList::Dead);
	}

	BossFrogTrafficLightColorChange();

}

void BossFrogState::PlayerSmashTime()
{
	if (AIType == 1) return;

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
	{
		AnimationTimeScale = BossFrogAnimation->GetTimeScale();

		// 스매시이고 TimeScale 이 같은지 확인한다.
		if (AnimationTimeScale != m_PlayerState->GetSmashTimeScale())
		{
			AnimationTimeScale = m_PlayerState->GetSmashTimeScale();
			BossFrogAnimation->SetTimeScale(AnimationTimeScale);
		}

	}
	else
	{
		// 스매시가 아니면 TimeScale 을 되돌린다.
		AnimationTimeScale = 0.917f;

		if (BossFrogAnimation->GetTimeScale() != AnimationTimeScale)
		{
			BossFrogAnimation->SetTimeScale(AnimationTimeScale);
		}
	}
}

void BossFrogState::PlayerFeverTime()
{
	if (AIType == 1) return;

	if (m_PlayerState->GetIsFever())
	{
		AnimationTimeScale = BossFrogAnimation->GetTimeScale();

		// 스매시이고 TimeScale 이 같은지 확인한다.
		if (AnimationTimeScale != m_PlayerState->GetFeverTimeScale())
		{
			AnimationTimeScale = m_PlayerState->GetFeverTimeScale();
			BossFrogAnimation->SetTimeScale(AnimationTimeScale);
		}
	}
}

void BossFrogState::BossFrogTrafficLightColorChange()
{
	if (IsDamaged)
	{
		BossTrafficLight->GetMaterialRef()->SetEmissTex(L"T_Hit_Emissive.tga");
		BossBody->GetMaterialRef()->SetEmissTex(L"T_Hit_Emissive.tga");
	}
	else
	{
		switch (TrafficLightColor)
		{
		case BossFrogState::BossFrogTrafficLight::Red:
		{
			BossTrafficLight->GetMaterialRef()->SetEmissTex(L"Boss_red_Emissive.tga");
		}
		break;
		case BossFrogState::BossFrogTrafficLight::Green:
		{
			BossTrafficLight->GetMaterialRef()->SetEmissTex(L"Boss_green_Emissive.tga");
		}
		break;
		case BossFrogState::BossFrogTrafficLight::Yellow:
		{
			BossTrafficLight->GetMaterialRef()->SetEmissTex(L"Boss_yellow_Emissive.tga");
		}
		break;
		default:
			break;
		}

		BossBody->GetMaterialRef()->SetEmissTex(L"BossMonster_Body_Emissive.tga");

	}

	if (NowAction == BossFrogActionList::Dead)
	{
		BossTrafficLight->GetMaterialRef()->SetEmissTex(L"T_Die_Emissive.tga");
		BossBody->GetMaterialRef()->SetEmissTex(L"BossMonster_Body_Emissive.tga");
	}

}

void BossFrogState::OnStateUpdate()
{
}

void BossFrogState::SetStop(bool btn)
{
	if (IsStop != btn)
	{
		IsStop = btn;
	}
}