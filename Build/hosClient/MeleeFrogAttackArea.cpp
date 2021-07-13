#include "MeleeFrogAttackArea.h"

MeleeFrogAttackArea::MeleeFrogAttackArea() : hos::com::Script(L"MeleeFrogAttackArea")
{
	ExecutionOrder = 7;
}

MeleeFrogAttackArea::~MeleeFrogAttackArea()
{

}

void MeleeFrogAttackArea::Reset()
{

}

void MeleeFrogAttackArea::Awake()
{
	m_MeleeFrogState = m_GameObject->GetParent()->GetComponent<MeleeFrogState>();
	AttackCollision = m_GameObject->GetComponent<hos::com::BoxCollision>();
	//AttackArea->SetActive(false);
	IsAttack = false;

	AttackCollision = m_GameObject->GetComponent<hos::com::BoxCollision>();

	HitSound = m_GameObject->GetComponent<hos::com::AudioSource>();

	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_melee_tailatk_impact.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_melee_headatk_impact.sclip");

}

void MeleeFrogAttackArea::Start()
{
}

void MeleeFrogAttackArea::OnTriggerEnter(hos::com::Collision* other)
{
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		//// Enmey 로 바꾸고 데미지를 주자

		IsAttack = true;

		if (m_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Attak1)
		{
			HitSound->Stop();
			HitSound->SetAudioClip(L"mob_melee_tailatk_impact");
			HitSound->Play();
			other->m_GameObject->GetComponent<PlayerState>()->GetDamaged(Attak0Damage, 0, m_MeleeFrogState->m_GameObject->transform);
		}
		
		if (m_MeleeFrogState->GetNowAction() == MeleeFrogState::MeleeFrogActionList::Attak2)
		{
			HitSound->Stop();
			HitSound->SetAudioClip(L"mob_melee_headatk_impact");
			HitSound->Play();
			other->m_GameObject->GetComponent<PlayerState>()->GetDamaged(Attak1Damage, 1, m_MeleeFrogState->m_GameObject->transform);
		}

		//AttackCollision->SetActive(false);
	}
}

void MeleeFrogAttackArea::Render()
{
	//int x = 200;
	//int y = 300;
	//int font = 14;

	//if (IsAttack)
	//{
	//	g_Renderer->_DebugText(x, y, font, hos::Vector4(1, 1, 1, 1), L"Is Attack True");
	//}
	//else
	//{
	//	g_Renderer->_DebugText(x, y, font, hos::Vector4(1, 1, 1, 1), L"Is Attack False");
	//}
}
