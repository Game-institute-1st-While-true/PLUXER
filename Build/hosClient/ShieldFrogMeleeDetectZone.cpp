#include "ShieldFrogMeleeDetectZone.h"

ShieldFrogMeleeDetectZone::ShieldFrogMeleeDetectZone() : hos::com::Script(L"ShieldFrogMeleeDetectZone")
{

}

ShieldFrogMeleeDetectZone::~ShieldFrogMeleeDetectZone()
{

}

void ShieldFrogMeleeDetectZone::Awake()
{

}

void ShieldFrogMeleeDetectZone::OnTriggerEnter(hos::com::Collision* other)
{
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		IsPlayerDetect = true;
	}
}

void ShieldFrogMeleeDetectZone::OnTriggerExit(hos::com::Collision* other)
{
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		IsPlayerDetect = false;
	}
}

void ShieldFrogMeleeDetectZone::Reset()
{

}
