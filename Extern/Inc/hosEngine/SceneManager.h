#ifndef _SCENE_MANAGER_H
#define _SCENE_MANAGER_H

// [2020/10/22 노종원]
// 게임내의 씬들을 관리할 매니저 클래스

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
#include "Scene.h"

namespace hos::com
{
	class SceneManager
	{
		friend class Group;
		friend class NetworkManager;
	private:
		enum { START_SCENE = 0 };

	public:
		SceneManager();
		~SceneManager();

	private:
		std::vector<Scene*> Scenes;
		int NowScene;
		bool IsChange;

	private:
		friend class COM_Client;
		friend class COM_Testbed;
		std::vector<Scene*>& GetScenes();

	public:
		void AddScene(Scene* scene);
		void RemoveScene(int buildIndex);
		void ChangeScene(int buildIndex);
		void ChangeScene(string_view name);
		bool HasScene();
		Scene* GetScene(int buildIndex);
		void Loop();

		void NonBockingLoop();
	};
}
extern hos::com::SceneManager* SceneManagement;

#endif // !_SCENE_MANAGER_H