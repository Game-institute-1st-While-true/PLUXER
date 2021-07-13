#include "MeleeFrogState.h"

MeleeFrogState::MeleeFrogState() : hos::com::Script(L"MeleeFrogState")
{
	ExecutionOrder = 5;
}

MeleeFrogState::~MeleeFrogState()
{

}

void MeleeFrogState::SetNowAction(MeleeFrogActionList val)
{
	PrevAction = NowAction;
	NowAction = val;
}

float MeleeFrogState::GetDamaged(float damage, int damagedType)
{
	if (NowAction == MeleeFrogActionList::Dead)
	{
		return 0.f;
	}

	//if (damagedType == 0)
	//{
	//	// 맞은 상태에서 
	//	if (NowAction != MeleeFrogActionList::Damaged)
	//	{
	//		HealthPoint -= damage;

	//		NowAction = MeleeFrogActionList::Damaged;
	//	}
	//}
	//else
	//{

	if (HealthPoint <= 0)
	{
		HealthPoint = 0.f;
		return 0.f;
	}

	float _temp = 0.f;

	_temp = HealthPoint - damage;

	HealthPoint -= damage;

	SetNowAction(MeleeFrogActionList::Damaged);

	MeleeFrogAnimation->ResetAniTick();

	IsDamaged = true;

	//}

	// 슈퍼아머 상태 만들기

	//////////////////////////////////////////////////////////////////////////
	// Sound

	ActionSound->Stop();
	ActionSound->SetLoop(false);

	int _Rand = (std::rand() / ((RAND_MAX + 1u) / 3));

	switch (_Rand)
	{
	case 0:
	{
		ActionSound->SetAudioClip(L"mob_hit_melee1");
	}
	break;
	case 1:
	{
		ActionSound->SetAudioClip(L"mob_hit_melee2");
	}
	break;
	case 2:
	{
		ActionSound->SetAudioClip(L"mob_hit_melee3");

	}
	break;
	default:
		break;
	}
	ActionSound->Play();

	//////////////////////////////////////////////////////////////////////////

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

void MeleeFrogState::Reset()
{

}

void MeleeFrogState::Awake()
{
	MeleeFrogAnimation = m_GameObject->transform->GetChild(0)->m_GameObject->GetComponent<hos::com::Animation>();

	DamagedArea = m_GameObject->GetComponent<hos::com::SphereCollision>();
	m_PlayerState = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerState>();
	ActionSound = m_GameObject->GetComponent<hos::com::AudioSource>();

	//////////////////////////////////////////////////////////////////////////
	// Audio Source Data Load
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_hit_melee1.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_hit_melee2.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_hit_melee3.sclip");

	//////////////////////////////////////////////////////////////////////////
	// UI
	HPBar = m_GameObject->transform->GetChild(2)->m_GameObject;
	HPUI = HPBar->transform->GetChild(2)->m_GameObject->GetComponent<hos::com::UIImage>();
	DamageUI = HPBar->transform->GetChild(3)->m_GameObject->GetComponent<hos::com::UIText>();
	DamageUIBack = HPBar->transform->GetChild(4)->m_GameObject->GetComponent<hos::com::UIText>();


}

void MeleeFrogState::Start()
{
	//m_GameObject->GetComponent<hos::com::Rigidbody>()->SetDrag(100.f);
}

void MeleeFrogState::Update()
{
	PlayerSmashTime();
	PlayerFeverTime();

	IsAnimationEnd = MeleeFrogAnimation->GetIsAnimationEnd();

	if (HealthPoint <= 0)
	{
		SetNowAction(MeleeFrogActionList::Dead);
	}

	PlayerDetect();

	ShowUI();

	IsDamaged = false;

}

void MeleeFrogState::LateUpdate()
{
	switch (NowAction)
	{
	case MeleeFrogState::MeleeFrogActionList::Idle:
	{
		if (MeleeFrogAnimation)
		{
			if (MeleeFrogAnimation->GetAniClipName().compare(L"idle") != 0)
			{
				MeleeFrogAnimation->SetAnimation(L"meleefrog", L"idle");
			}
		}
	}
	break;
	case MeleeFrogState::MeleeFrogActionList::Damaged:
	{
		if (MeleeFrogAnimation)
		{
			if (MeleeFrogAnimation->GetAniClipName().compare(L"damaged") != 0)
			{
				MeleeFrogAnimation->SetAnimation(L"meleefrog", L"damaged");
			}
		}
	}
	break;
	case MeleeFrogState::MeleeFrogActionList::Dead:
	{
		if (MeleeFrogAnimation)
		{
			if (MeleeFrogAnimation->GetAniClipName().compare(L"dead") != 0)
			{
				MeleeFrogAnimation->SetAnimation(L"meleefrog", L"dead", 0, true);
			}
		}
	}
	break;
	case MeleeFrogState::MeleeFrogActionList::Attak1:
	{
		if (MeleeFrogAnimation)
		{
			if (MeleeFrogAnimation->GetAniClipName().compare(L"attack0") != 0)
			{
				MeleeFrogAnimation->SetAnimation(L"meleefrog", L"attack0");
			}
		}
	}
	break;
	case MeleeFrogState::MeleeFrogActionList::Attak2:
	{
		if (MeleeFrogAnimation)
		{
			if (MeleeFrogAnimation->GetAniClipName().compare(L"attack1") != 0)
			{
				MeleeFrogAnimation->SetAnimation(L"meleefrog", L"attack1");
			}
		}
	}
	break;
	case MeleeFrogState::MeleeFrogActionList::Move:
	{
		if (MeleeFrogAnimation)
		{
			if (MeleeFrogAnimation->GetAniClipName().compare(L"move") != 0)
			{
				MeleeFrogAnimation->SetAnimation(L"meleefrog", L"move");
			}
		}
	}
	break;
	default:
		break;
	}
}

void MeleeFrogState::OnEnable()
{
	NowAction = MeleeFrogActionList::Idle;

	HealthPoint = MaxHealthPoint;

	AnimationTimeScale = 1.0f;

	IsAnimationEnd = false;
}

void MeleeFrogState::Render()
{
	//int x = 200;
	//int y = 200;
	//int font = 14;

	//g_Renderer->_DebugText(x, y + 60, font, hos::Vector4(1, 1, 1, 1), L"Frog Forward : %f, %f, %f", m_GameObject->transform->GetForward().x, m_GameObject->transform->GetForward().y, m_GameObject->transform->GetForward().z);
}

void MeleeFrogState::PlayerSmashTime()
{

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
	{
		AnimationTimeScale = MeleeFrogAnimation->GetTimeScale();

		// 스매시이고 TimeScale 이 같은지 확인한다.
		if (AnimationTimeScale != m_PlayerState->GetSmashTimeScale())
		{
			AnimationTimeScale = m_PlayerState->GetSmashTimeScale();
			MeleeFrogAnimation->SetTimeScale(AnimationTimeScale);
		}

	}
	else
	{
		// 스매시가 아니면 TimeScale 을 되돌린다.
		AnimationTimeScale = 0.83f;

		if (MeleeFrogAnimation->GetTimeScale() != AnimationTimeScale)
		{
			MeleeFrogAnimation->SetTimeScale(AnimationTimeScale);
		}

	}


}

void MeleeFrogState::PlayerFeverTime()
{
	if (m_PlayerState->GetIsFever())
	{
		AnimationTimeScale = MeleeFrogAnimation->GetTimeScale();

		// 스매시이고 TimeScale 이 같은지 확인한다.
		if (AnimationTimeScale != m_PlayerState->GetFeverTimeScale())
		{
			AnimationTimeScale = m_PlayerState->GetFeverTimeScale();
			MeleeFrogAnimation->SetTimeScale(AnimationTimeScale);
		}
	}
}

void MeleeFrogState::PlayerDetect()
{
	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Dead)
	{
		IsInDetect = false;
		IsInAttack = false;

		return;
	}

	if (!IsActivation)
	{
		IsInDetect = false;
		IsInAttack = false;

		return;
	}


	hos::com::Transform* _PlayerTF = m_PlayerState->m_GameObject->transform;
	hos::com::Transform* _FrogTF = m_GameObject->transform;

	float _Length = 0.0f;

	_Length = (_PlayerTF->GetPosition() - _FrogTF->GetPosition()).Length();

	// 탐지 거리
	if (_Length <= 30.f)
	{
		IsInDetect = true;
	}
	else
	{
		IsInDetect = false;
	}


	// 공격 거리
	if (_Length <= 2.5f)
	{
		IsInAttack = true;
	}
	else
	{
		IsInAttack = false;
	}

}

void MeleeFrogState::ShowUI()
{
	hos::com::Transform* _PlayerTF = m_PlayerState->m_GameObject->transform;
	hos::com::Transform* _FrogTF = m_GameObject->transform;

	float _Length = 0.0f;

	_Length = (_PlayerTF->GetPosition() - _FrogTF->GetPosition()).Length();

	if (_Length <= 3.f)
	{
		hos::Vector4 _HPOffTil = hos::Vector4::Zero;

		_HPOffTil.x = 0;
		_HPOffTil.y = (HealthPoint / MaxHealthPoint);
		_HPOffTil.z = 0;
		_HPOffTil.w = 1.0f;

		HPUI->SetOffTil(_HPOffTil);

		if (!HPBar->GetActive())
		{
			HPBar->SetActive(true);
		}

		if (IsDamaged)
		{
			DamageUI->SetActive(true);
			DamageUIBack->SetActive(true);

			DamageUI->SetNowTick(0.f);
			DamageUIBack->SetNowTick(0.f);
			AddDamage += NowHitValue;

			int _temp = (int)(floor(AddDamage + 0.5f));

			hos::string _strDamage = std::to_wstring(_temp);

			DamageUI->SetText(_strDamage);
			DamageUIBack->SetText(_strDamage);

		}

		if (DamageUI->GetIsAnimationEnd())
		{
			AddDamage = 0.f;

			hos::string _strDamage = std::to_wstring(AddDamage);

			DamageUI->SetText(_strDamage);
			DamageUIBack->SetText(_strDamage);

			DamageUI->SetActive(false);
			DamageUIBack->SetActive(false);

		}

	}
	else
	{
		DamageUI->SetActive(false);
		DamageUIBack->SetActive(false);

		AddDamage = 0.f;

		HPBar->SetActive(false);
	}
}
