#pragma once
#pragma comment(lib, "pdh.lib")
#include <Windows.h>
#include <Pdh.h>
#include <psapi.h>

// Working Set (작업세트)
// 작업관리자에서 메모리로 보이는 항목으로 현재 프로그램이 사용하고 있는 물리 메모리의 양을 나타낸다.
// 윈도우는 현재 불필요하거나 현재 여유가 없을 경우 프로그램 메모리를 디스크에 저장한다. 소위 SWAP이라고 표현되는 부분이다
// 즉 작업세트는 해당 프로그램에서 스왑되지 않고 현재 물리메모리에서 사용되는 양이다.

// 보통 윈도우 프로그램은 실행 프로그램이 다른 공유 라이브러리를 참조하는 경우가 많고 그럴경우 작업 세트의 메모리는 참조하는 공유 라이브러리를 위하여
// 할당한 메모리도 포함하고 있을 수 있다.
// 거기에 윈도우가 현재 메모리 상태와 프로그램 상태에 따라 작업세트의 양을 조절하는 관계로 차지하는 크기는 항상 일정하지 않을 수 잇다.
// 물론 메모리 릭이 발생하는 경우에는 증가하는게 눈에 보인다.


// Private Bytes
// 프로그램에 할당된 메모리의 양을 나타냅니다. 이것은 물리메모리 + 스왑된 메모리를 나타냅니다. Working Set과는 다르게 공유 라이브러리를 위하여
// 할당한 메모리는 포함하지 않으나 해당 메모리 중에 프로그램 개발 시 링크한 라이브러리로 인하여 할당한 메모리를 별도로 구별할 수 없습니다. 그냥 프로그램 자체가 대략
// 이정도 사용한다고 보면 됩니다.

// Virtual Bytes
// 프로그램에 할당 된 메모리로 물리메모리 + 스왑된 메모리;를 나타내며 이것은 프로그램에 할당된 메모리에 공유 라이브러리를 위하여 할당된 메모리를 포함하고 있습니다.
// 이런 관계로 이 메모리는 실제 머신이 가진 메모리 보다 더 많은 메모리를 표시할 수 있습니다.

namespace gzz
{
	class Monitoring
	{
		PDH_HQUERY CpuQuery;
		PDH_HCOUNTER CpuTotal;

		ULARGE_INTEGER LastCpu;
		ULARGE_INTEGER LastKernelCpu;
		ULARGE_INTEGER LastUserCpu;

		HANDLE ProcessHandle;
		int ProcessCount;

	public:
		Monitoring();
		~Monitoring();

		double ThisProcessCpuUsage();		// 이 프로세스가 사용중인 cpu 사용량	
		double CpuUsage();					// 현재 사용중인 cpu 사용량

		SIZE_T WorkingSet();				// Working Set
		SIZE_T PhysicMemory();				// 이 프로세스가 사용중인 메모리 사용량 
	};
}
