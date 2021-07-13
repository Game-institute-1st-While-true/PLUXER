// <Concept>
// Source Name		: DebugGrid.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.02.19

// <Explanation>
// 디버그 환경에서 모델 가시성 및 좌표계 확인을 위한 디버그 그리드.
// 일단 D3D11 환경용입니다.


// Working Log
// 2021.02.19.Hosik	: 생성

#pragma once

#include <d3d11_4.h>
#include "DXTK\Simplemath.h"

#define GRID_SIZE 20

namespace hos::cg
{

	class DebugGrid
	{
	public:
		struct VTX_GRID
		{
			DirectX::SimpleMath::Vector3 vPos;
			DirectX::SimpleMath::Vector3 vColor;
		};

		struct INDEX_GRID
		{
			UINT a;
			UINT b;
		};

		// 내부 멤버 인스턴스
	private:
		// Renderer
		ID3D11Device& DeviceRef;
		ID3D11DeviceContext& ContextRef;

		// 그리드
		VTX_GRID m_VtxList[(GRID_SIZE + 1) * (GRID_SIZE + 1)];
		INDEX_GRID m_IndexList[(GRID_SIZE + 1) * 2];

		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		unsigned int m_VertexCount = 0;
		unsigned int m_IndexCount = 0;

		// Axis
		VTX_GRID AxisVtxList[6];
		INDEX_GRID AxisIndexList[3];

		ID3D11Buffer* AxisVertexBuffer = nullptr;
		ID3D11Buffer* AxisIndexBuffer = nullptr;
		unsigned int AxisVertexCount = 0;
		unsigned int AxisIndexCount = 0;

		// 생성&소멸자
	public:
		DebugGrid() = delete;
		DebugGrid(ID3D11Device& device, ID3D11DeviceContext& context);
		~DebugGrid();

		// 외부 사용 가능 함수
	public:
		void RenderGrid();
		unsigned int GetGridIndexCount();

		void RenderAxis();
		unsigned int GetAxisIndexCount();

		// 내부용 함수
	private:
		void CreateGrid();
		void CreateAxis();

		HRESULT CreateVB(void* pBuff, UINT size, ID3D11Buffer** ppVB);
		HRESULT CreateIB(void* pBuff, UINT size, ID3D11Buffer** ppIB);
	};
}