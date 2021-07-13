#ifndef _UTILITY_H
#define _UTILITY_H

// [2020/10/15 노종원]
// 유틸리티 헤더
// 유틸리티 라이브러리에서 외부로 노출하게될 헤더입니다.

// warning : 유틸리티 라이브러리에 새로운 작업을 할 경우 모든 헤더는 이곳에 포함시켜 놓아야 합니다.

// LOG
// [2020/10/22] 노종원 : STL 컨테이너 템플릿사용으로 인해 정적 라이브러리로 변경
// [2020/11/16] 노종원 : 스트링 부분 BaseString.h 파일로 이동
// [2020/12/10] 노종원 : Utility에서 DX종속성을 제거하기위해 MathHelper.h제거
// [2020/12/20] 김효곤 : format utility 추가
// [2020/12/32] 노종원 : BaseString.h 제거 BaseTypes.h추가 기존내용은 BaseTypes.h로 병합
// [2021/03/10] 노종원 : 라이브러리 따로 추가하지않도록 헤더에 매크로 추가

#include "BaseTypes.h"
#include "FormatUtils.h"
#include "Singleton.h"
#include "Delegate.hpp"
#include "Console.h"
#include "Logger.hpp"
#include "Timer.hpp"
#include "Memory.h"

#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib,"hosUtility_x64D")
#else
#pragma comment(lib,"hosUtility_x86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib,"hosUtility_x64R")
#else
#pragma comment(lib,"hosUtility_x86R")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)


//[2020/10/23 노종원] max와 min 메크로를 제거하기위한 전처리문 가장아래에 선언할 것
//#ifdef max
//#undef max
//#endif // max
//
//#ifdef min
//#undef min
//#endif // min

//#ifdef far
//#undef far
//#endif // !far
//
//#ifdef near
//#undef near
//#endif // near

#endif // !_UTILITY_H