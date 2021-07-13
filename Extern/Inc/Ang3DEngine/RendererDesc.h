// <Concept>
// Source Name		: RenderderDesc.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2020.11.05

// <Explanation>
// 렌더상태정보를 파라미터로 넘겨주기위한 클래스.
// Class의 Value Instance에 대한 복사생성자와 대입연산자를 지원합니다.

// Working Log
// 2021.01.08.Hosik	: 생성 // 복사생성자, 대입연산자 구현, 1단계 테스트.

#pragma once

#include "framework.h"
#include <string>

#define RENDERER_SCREENWIDTH 640
#define RENDERER_SCREENHEIGHT 480

namespace hos::cg
{
	class RendererDesc
	{
	public:
		// 드라이버 타입
		enum class DriverType
		{
			DRIVER_DIRECT3D11,
			DRIVER_NULL,

			DRIVER_MAX,
		};

		// 렌더 타입
		enum class RenderModeList
		{
			RENDER_PBR,
			RENDER_LAMBERT,

			RENDER_MAX,

			RENDER_DEFAULT = RENDER_LAMBERT,
		};

		// 카메라 타입
		enum class CameraModeList
		{
			CAMERA3D_DEFAULT,
			CAMERA3D_FOLLOW,
			CAMERA2D_DEFAULT,
			CAMERA2D_FOLLOW,
			CAMERA_SCREENSPACE,
		};

		// SHADOWMAP 타입
		enum class ShadowMapCullList
		{
			SHADOWMAP_SOLID,
			SHADOWMAP_CULLBACK,
			SHADOWMAP_CULLFRONT,
		};

	public:
		// 하드웨어 정보
		// TODO: 윈도우가 아닐 땐? HWND이 아니라 딴거라면??
		DriverType Driver;			// 그래픽 드라이버 타입
		HWND Hwnd;					// 윈도우 핸들

		// 렌더 설정
		bool Fullscreen;
		bool Vsync;					// 수직동기화
		unsigned int ScreenWidth;
		unsigned int ScreenHeight;
		float ScreenDepth;
		float ScreenNear;			// 근평면 (최소 0보단 커야한다)
		unsigned int ValueAA;		// 안티 앨리어싱 (1:OFF ~ 8:최대)
		unsigned int ValueAF;		// 비등방성 필터링 (1:OFF ~ 16:최대)

		// VGA 정보
		unsigned int VideoCardMemory_MB;
		std::wstring VideoCardName;
		unsigned int Numerator;
		unsigned int Denominator;

		// 게임 옵션
		bool MotionBlurState;
		bool FogState;

	public:
		RendererDesc();										// 기본생성자
		RendererDesc(const RendererDesc& desc);				// 복사생성자
		RendererDesc& operator=(const RendererDesc& rhs);	// 대입연산자
		~RendererDesc();									// 소멸자

	public:
		void CorrectDescValue();	// 근/원평면과 AA,AF단계가 잘못된 경우 교정하는 함수
	};
}