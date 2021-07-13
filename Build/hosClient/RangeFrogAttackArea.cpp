#include "RangeFrogAttackArea.h"

RangeFrogAttackArea::RangeFrogAttackArea() : hos::com::Script(L"RangeFrogAttackArea")
{
	ExecutionOrder = 7;
}

RangeFrogAttackArea::~RangeFrogAttackArea()
{

}

void RangeFrogAttackArea::Reset()
{

}

void RangeFrogAttackArea::Awake()
{
	m_RangeFrogState = m_GameObject->GetParent()->GetComponent<RangeFrogState>();

	AttackCollision = m_GameObject->GetComponent<hos::com::SphereCollision>();

	HitSound = m_GameObject->GetComponent<hos::com::AudioSource>();

	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\mob_range_atk_impact.sclip");

}

void RangeFrogAttackArea::OnTriggerEnter(hos::com::Collision* other)
{
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		//// Enmey 로 바꾸고 데미지를 주자

		other->m_GameObject->GetComponent<PlayerState>()->GetDamaged(AttackDamage, 1, m_RangeFrogState->m_GameObject->transform);

		HitSound->Stop();
		HitSound->SetAudioClip(L"mob_range_atk_impact");
		HitSound->Play();

	}
}
