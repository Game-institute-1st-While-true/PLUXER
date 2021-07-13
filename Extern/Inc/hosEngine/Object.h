#ifndef _OBJECT_H
#define _OBJECT_H

// [2020/10/14 노종원]
// 기본 오브젝트 클래스
// 오브젝트의 이름과 기본적인 생성, 삭제, 복사 등등을 정의 한다.

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/03/15] 신호식 : 그래픽엔진 헤더 추가
#include "NetDefine.h"
#include "NetworkManager.h"
#include "hosUtility\hosUtility.h"
#include "hosUtility\hosMath.h"

#include "I_Callable.hpp"

#include "flatbuffers\flexbuffers.h"
#undef min	//define되있지않지만 undef하지않으면 컴파일에러가 발생한다
#include "flatbuffers\idl.h"

#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib, "Flatbuffers_64D")
#else
#pragma comment(lib, "Flatbuffers_86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib, "Flatbuffers_64R")
#else
#pragma comment(lib, "Flatbuffers_86D")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)

#include "PhysX.h"
#include "WindowsInput.h"

// 그래픽 엔진용 헤더
#include "Ang3DEngine/Ang3DEngine.h"
#include "DataManager.h"
#include "Navigation.h"

extern hos::string g_Path;
extern hos::ut::Logger* Debug;
extern hos::ut::GameTimer* Time;
extern hos::cg::Renderer* g_Renderer;
extern hos::com::DataManager* g_DataManager;
extern DirectX::AudioEngine* g_Audio;
extern hos::PhysX* g_Px;
extern hos::nv::Navigation* g_Navigation;
extern hos::com::NetworkManager* Network;

namespace hos::com
{
	class GameObject;
	class Transform;
	class Object abstract :public ICallable
	{
	private:
		string Name;
		bool IsActive;
	public:
		Object();
		Object(string_view name, bool active = true);
		Object(const Object&) = default;
		Object& operator=(const Object&) = default;
		Object(Object&&) = default;
		Object& operator=(Object&&) = default;
		virtual ~Object();

	public:
		string GetName() const;
		void SetName(string_view);
		bool GetActive() const;
		void SetActive(bool active);
		string ToString() const;
		unsigned long long GetHashCode() const;

		virtual const std::vector<U8> Serialize();
		virtual bool Deserialize(mbstring_view data);
	};

	__forceinline static bool operator==(const Object& lhs, const Object& rhs)
	{
		if (lhs.GetHashCode() == rhs.GetHashCode())
		{
			if (lhs.GetName() == rhs.GetName())
			{
				return true;
			}
		}

		return false;
	}

	__forceinline static bool operator!=(const Object& lhs, const Object& rhs)
	{
		if (lhs.GetHashCode() == rhs.GetHashCode())
		{
			if (lhs.GetName() == rhs.GetName())
			{
				return false;
			}
		}

		return true;
	}
}

#endif // !_OBJECT_H