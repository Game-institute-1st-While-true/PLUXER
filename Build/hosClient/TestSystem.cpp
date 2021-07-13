#include "TestSystem.h"
#include "SceneManager.h"

using namespace hos;
using namespace com;

void TestSystem::Reset()
{
}

void TestSystem::Awake()
{
}

void TestSystem::Start()
{
}

void TestSystem::Update()
{
	if (Input->GetKeyDown(KeyCode::F1))
	{
		SceneManagement->ChangeScene(0);
	}
	if (Input->GetKeyDown(KeyCode::F2))
	{
		SceneManagement->ChangeScene(1);
	}
}
