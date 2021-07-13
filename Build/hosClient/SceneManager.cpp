#include "SceneManager.h"
#include "hosUtility/hosUtility.h"

using namespace hos;

SceneManager* SceneManagement = nullptr;

SceneManager::SceneManager() : NowScene(nullptr), PrevScene(nullptr), IsLoopCondition(true)
{
}

SceneManager::~SceneManager()
{
	bool nowSceneisDontDestory = false;
	for (com::Scene* scene : Scenes)
	{
		if (scene == NowScene)
		{
			nowSceneisDontDestory = true;
		}
		SafeDelete(scene);
	}
	Scenes.clear();

	if (!nowSceneisDontDestory)
	{
		SafeDelete(NowScene);
	}

	for (com::GameObject* obj : GameObjects)
	{
		SafeDelete(obj);
	}
	GameObjects.clear();
}

std::vector<hos::com::Scene*>& SceneManager::GetScenes()
{
	return Scenes;
}

void SceneManager::ChangeScene(int buildIndex)
{
	hos::com::Scene* ChangeScene = nullptr;
	for (com::Scene* scene : Scenes)
	{
		if (scene->GetBuildIndex() == buildIndex)
		{
			ChangeScene = scene;
		}
	}

	if (nullptr == ChangeScene)
	{
		ChangeScene = g_DataManager->GetScene(buildIndex);
	}

	if (ChangeScene)
	{
		PrevScene = NowScene;
		NowScene = ChangeScene;
		SF.BindObject(NowScene);
		NowScene->ExecutionOrderRebuild();

		NowScene->Awake();
		NowScene->Start();
	}
}

void hos::SceneManager::ChangeScene(hos::string_view name)
{
	int index = g_DataManager->GetSceneBulidIndex(name);
	if (index >= 0)
	{
		ChangeScene(index);
	}
	else
	{
		hos::ut::Logger::GetIns()->LogConsole(L"scene manager", L"Change Scene Fail");
	}
}

hos::com::Scene* SceneManager::GetScene()
{
	return NowScene;
}

void hos::SceneManager::Quit()
{
	IsLoopCondition = false;
}

bool hos::SceneManager::IsLoop() const
{
	return IsLoopCondition;
}

void hos::SceneManager::CheckScene()
{
	if (PrevScene)
	{
		PrevScene->OnDisable();
		hos::SafeDelete(PrevScene);
	}
}
