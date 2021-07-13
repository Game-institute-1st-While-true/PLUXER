// <Concept>
// Source Name		: Ang3DEngine.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.03.15

// <Explanation>
// Graphics Engine 라이브러리에서 외부로 노출하게될 헤더입니다.
// warning : 라이브러리에 새로운 작업을 할 경우 모든 헤더는 이곳에 포함시켜 놓아야 합니다.

// Working Log
// 2021.03.15.Hosik	: 생성

// Header List
#include "Renderer.h"
#include "RendererDesc.h"
#include "VertexDataType.h"
#include "RenderQue.h"

#ifndef _3DENGINE_H
#define _3DENGINE_H

#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib,"Ang3DEngine_x64D")
#else
#pragma comment(lib,"Ang3DEngine_x86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib,"Ang3DEngine_x64R")
#else
#pragma comment(lib,"Ang3DEngine_x86R")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)


#endif // !_3DENGINE_H