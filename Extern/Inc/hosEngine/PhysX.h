#ifndef _PHYS_X_H
#define _PHYS_X_H

// [2021/1/7 노종원]
// 피직스 라이브러리를 사용하기위한 클래스

// LOG
// [2021-02-19 김효곤] physx 구조 변경 작업
// [2021/02/19 노종원] physx 구조 변경 작업

#include "hosUtility\hosUtility.h"
#include "hosUtility\hosMath.h"
#include "PxPhysicsAPI.h"
#include "vehicle\PxVehicleSDK.h"

#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib,"PhysX_64D")
#pragma comment(lib,"PhysXCharacterKinematic_static_64D")
#pragma comment(lib,"PhysXCommon_64D")
#pragma comment(lib,"PhysXCooking_64D")
#pragma comment(lib,"PhysXExtensions_static_64D")
#pragma comment(lib,"PhysXFoundation_64D")
#pragma comment(lib,"PhysXPvdSDK_static_64D")
#pragma comment(lib,"PhysXVehicle_static_64D")
#else
#pragma comment(lib,"PhysX_86D")
#pragma comment(lib,"PhysXCharacterKinematic_static_86D")
#pragma comment(lib,"PhysXCommon_86D")
#pragma comment(lib,"PhysXCooking_86D")
#pragma comment(lib,"PhysXExtensions_static_86D")
#pragma comment(lib,"PhysXFoundation_86D")
#pragma comment(lib,"PhysXPvdSDK_static_86D")
#pragma comment(lib,"PhysXVehicle_static_86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib,"PhysX_64")
#pragma comment(lib,"PhysXCharacterKinematic_static_64")
#pragma comment(lib,"PhysXCommon_64")
#pragma comment(lib,"PhysXCooking_64")
#pragma comment(lib,"PhysXExtensions_static_64")
#pragma comment(lib,"PhysXFoundation_64")
#pragma comment(lib,"PhysXPvdSDK_static_64")
#pragma comment(lib,"PhysXVehicle_static_64")
#else
#pragma comment(lib,"PhysX_86")
#pragma comment(lib,"PhysXCharacterKinematic_static_86")
#pragma comment(lib,"PhysXCommon_86")
#pragma comment(lib,"PhysXCooking_86")
#pragma comment(lib,"PhysXExtensions_static_86")
#pragma comment(lib,"PhysXFoundation_86")
#pragma comment(lib,"PhysXPvdSDK_static_86")
#pragma comment(lib,"PhysXVehicle_static_86")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)


namespace hos
{
	namespace com
	{
		class Scene;
	}

	class PhysX
	{
		friend class com::Scene;
	public:
		inline static const mbstring_literal PVD_HOST = "127.0.0.1";
		static constexpr int PVD_PORT = 5425;
		static constexpr int PVD_TIMEOUT = 10;

	private:
		physx::PxDefaultAllocator Allocator;
		physx::PxDefaultErrorCallback ErrorCallback;
		physx::PxFoundation* Foundation;
		physx::PxPvd* PVD;
		physx::PxDefaultCpuDispatcher* Dispatcher;

		U32 NumDispatcherThread;
		bool IsDebugMemory;

	public:
		physx::PxPhysics* Physics;
		physx::PxMaterial* DefaultMaterial;

	public:
		PhysX();
		~PhysX();

	public:

		physx::PxCooking* CreateCooking(const physx::PxCookingParams& params);
	};
}

#endif // !_PHYS_X_H