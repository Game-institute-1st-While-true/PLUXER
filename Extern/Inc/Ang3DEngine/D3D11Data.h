// <Concept>
// Source Name		: D3D11Data.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.01.10

// <Explanation>
// D3D11 API에서 사용할 enum list 등의 상태값

// Working Log
// 2021.01.10.Hosik	: 생성
// 2021.04.20.Hosik : RenderTargetList 추가.

#pragma once

namespace hos::cg
{

	//enum class RenderMode
	//{
	//	FORWARD,
	//	DEFERRED,

	//	RENDERMODE_MAX,
	//};

	enum class ConstBufferType
	{
		CB_DEFAULT,
		CB_MATERIAL,
		CB_BONE,
		CB_LIGHT,
		CB_SCREEN,

		CB_MAX,
	};

	// 뎁스 스텐실 상태값
	enum
	{
		DS_DEPTH_ON,		//깊이버퍼 ON! (기본값), 스텐실버퍼 OFF.
		DS_DEPTH_OFF,		//깊이버퍼 OFF!, 스텐실 OFF.
		DS_DEPTH_WRITE_OFF,	//깊이버퍼 쓰기 끄기, 스텐실 OFF. 

		//스텐실 버퍼 연산용.
		DS_DEPTH_ON_STENCIL_ON,				//깊이버퍼 On (Write On), 스텐실버퍼 ON (참조값 쓰기) : "깊이/스텐실 기록".
		DS_DEPTH_ON_STENCIL_EQUAL_KEEP,		//깊이버퍼 On (Write On), 스텐실버퍼 ON (동일비교, 성공시 유지) : "지정 위치에만 그리기".
		DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP,	//깊이버퍼 On (Write On), 스텐실버퍼 ON (다름비교, 성공시 유지) : "지정 위치 이외에 그리기".

		DS_DEPTH_OFF_STENCIL_ON,			//깊이버퍼 Off, 스텐실버퍼 ON (참조값 쓰기) : "스텐실만 기록"
		DS_DEPTH_ON_STENCIL_EQUAL_INCR,		//깊이버퍼 On (Write On), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지".
		DS_DEPTH_WRITE_OFF_STENCIL_ON,		//깊이버퍼 On (Write Off), 스텐실버퍼 ON (항상기록, 성공시 증가) :" 스텐실만 기록".
		DS_DEPTH_OFF_STENCIL_EQUAL_INCR,	//깊이버퍼 Off (Write Off), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지.

		DS_MAX,						// 최대값 처리용

		DS_DEFAULT = DS_DEPTH_ON,	// 기본은 뎁스만 켜진 상태.
	};


	// 레스터라이져 상태 객체 Rasterizer State Objects
	// 
	enum {
		RS_SOLID,				//삼각형 채우기 : Fill Mode - Soild.
		RS_WIREFRM,				//삼각형 채우기 : Fill Mode - Wireframe.
		RS_CULLBACK,			//뒷면 컬링 (ON) : Back-Face Culling - "CCW" 
		RS_CULLFRONT,			//정면 컬링 (ON) : Front-Face Culling - "CW" 	
		RS_WIRECULLBACK,		//와이어 프레임 + 뒷면 컬링 (ON) 
		RS_WIRECULLFRONT,		//와이어 프레임 + 앞면 컬링 (ON) 	
		RS_NODEPTH,				//뎁스 클립 하지 않기

		//열거 상수 최대값.
		RS_MAX_,

		//상수 재정의. MAX 값 뒷쪽에 정의해야 함. 주의.
		RS_CULL_CCW = RS_CULLBACK,
		RS_CULL_CW = RS_CULLFRONT,
		RS_WIRE_CULL_CCW = RS_WIRECULLBACK,
		RS_WIRE_CULL_CW = RS_WIRECULLFRONT,

	};

	// 색상/알파 혼합 상태 객체 Blending State Objects 
	//
	enum {
		BS_DEFAULT,				//기본혼합 객체.
		BS_ALPHA_BLEND,			//알파 블랜딩. (Src.a + (1-Src.a))
		BS_COLOR_BLEND,			//색상 블랜딩. ( Src.Color + (1-Src.Color))
		BS_COLOR_BLEND_ONE,		//1:1 블랜딩. ( Src.Color + Dest.Color) 

		BS_MAX_,

		BS_AB_OFF = BS_DEFAULT,
		BS_AB_ON = BS_ALPHA_BLEND,
		//BS_AB_ALPHA = BS_ALPHA_BLEND,
		//BS_AB_COLOR = BS_COLOR_BLEND,
	};

}