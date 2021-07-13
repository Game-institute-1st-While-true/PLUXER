#include "ArcadePlayer.h"
#include "ArcadeNetworkEvent.h"

using namespace hos;
using namespace com;

ArcadePlayer::ArcadePlayer()
	: Script(L"ArcadePlayer"), Net(nullptr)
{
	ExecutionOrder = 4;
}

ArcadePlayer::~ArcadePlayer()
{
}

void ArcadePlayer::Start()
{
	GameObject* obj = m_GameObject->m_Scene->FindGameObjectWithName(L"Network");
	if (obj)
	{
		Net = obj->GetComponent<ArcadeNetworkEvent>();
	}
	prevTime = std::chrono::steady_clock::now();
}

void ArcadePlayer::FixedUpdate()
{
	std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - prevTime);
	if (delta.count() > 15)
	{
		if (PreDir != NowDir)
		{
			
		}
	}
}