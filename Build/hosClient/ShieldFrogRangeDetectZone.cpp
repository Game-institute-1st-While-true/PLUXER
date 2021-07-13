#include "ShieldFrogRangeDetectZone.h"

ShieldFrogRangeDetectZone::ShieldFrogRangeDetectZone() : hos::com::Script(L"ShieldFrogRangeDetectZone")
{

}

ShieldFrogRangeDetectZone::~ShieldFrogRangeDetectZone()
{

}

void ShieldFrogRangeDetectZone::Awake()
{

}

void ShieldFrogRangeDetectZone::OnTriggerEnter(hos::com::Collision* other)
{
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		IsPlayerDetect = true;
	}
}

void ShieldFrogRangeDetectZone::OnTriggerExit(hos::com::Collision* other)
{
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		IsPlayerDetect = false;
	}
}

void ShieldFrogRangeDetectZone::Reset()
{

}
