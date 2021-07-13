#ifndef _COM_SYSTEM_H
#define _COM_SYSTEM_H

// [2020/11/23 노종원]
// Component Object Model System
// 컴포넌트 라이브러리의 전반을 담당할 클래스, 라이브러리의 메인 헤더

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/03/10] 노종원 : 라이브러리 따로 추가하지않도록 헤더에 매크로 추가
// [2021/03/10] 노종원 : 버전2를 위해서 렌더러 임시 제거
// [2021/04/16] 김효곤 : Scene Seed 추가

#include <thread>
#include "NetworkManager.h"
#include "Navigation.h"
#include "DXTK/DirectXTK.h"
#include "PhysX.h"
#include "Scene.h"

//platform
#include "WindowsApplication.h"

//Components
#include "Transform.h"
#include "BoxCollision.h"
#include "CapsuleCollision.h"
#include "MeshCollision.h"
#include "SphereCollision.h"
#include "Rigidbody.h"
#include "Camera.h"
#include "Light.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "Script.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "NavAgent.h"
#include "Networkcomponent.h"
#include "Animation.h"
#include "UIImage.h"
#include "UI_Text.h"
#include "UIButton.h"
#include "UI_InputField.h"

#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib,"hosEngine_x64D")
#else
#pragma comment(lib,"hosEngine_x86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib,"hosEngine_x64R")
#else
#pragma comment(lib,"hosEngine_x86R")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)

namespace hos::com
{
	class COM_Client
	{
	private:
		std::thread MainThread;
		WindowsApplication* App;
	public:

	public:
		COM_Client(HINSTANCE ins);
		~COM_Client();

	public:
		void Load();
		bool Update();
		WindowsApplication* GetApp()const;

	public:
		ut::Delegate<void> OnIdleProc;
	};
}

#endif // !_COM_SYSTEM_H