#ifndef _COM_TESTBED_H
#define _COM_TESTBED_H

// [2021/03/18 노종원]
// Component Object Model System Testbed
// 실험실

// LOG
#include "NetworkManager.h"
#include <thread>
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
#include "NavAgent.h"

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
	class COM_Testbed
	{
	private:
		std::thread MainThread;
		WindowsApplication* App;
	public:

	public:
		COM_Testbed(HINSTANCE ins);
		~COM_Testbed();

	public:
		void Load();
		bool Update();
		WindowsApplication* GetApp()const;

	public:
		ut::Delegate<void> OnIdleProc;
	};
}

#endif // !_COM_TESTBED_H