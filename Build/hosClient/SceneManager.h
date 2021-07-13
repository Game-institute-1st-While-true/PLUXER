#ifndef _SCENE_MANAGEMENT_H
#define _SCENE_MANAGEMENT_H

// [2020/10/22 노종원]
// 게임내의 씬들을 관리할 매니저 클래스

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/04/12] 노종원 : 엔진 내부에서 클라이언트로 이동
// [2021/04/16] 김효곤 : Change Scene 수정 

#include "hosEngine\COM_Client.h"
#include "ScriptFactory.h"

namespace hos
{
	class SceneManager
	{
	public:
		enum { START_SCENE = 0 };

	public:
		SceneManager();
		~SceneManager();

	private:
		// don't Destoryed
		std::vector<hos::com::Scene*> Scenes;
		std::vector<hos::com::GameObject*> GameObjects;

		//Scene
		hos::com::Scene* NowScene;
		hos::com::Scene* PrevScene;

		ScriptFactory SF;

		bool IsLoopCondition;
	private:
		std::vector<hos::com::Scene*>& GetScenes();

	public:
		void ChangeScene(int buildIndex);
		void ChangeScene(hos::string_view name);
		hos::com::Scene* GetScene();
		void Quit();
		bool IsLoop() const;
		void CheckScene();
	};
}

extern hos::SceneManager* SceneManagement;

#endif // _SCENE_MANAGEMENT_H