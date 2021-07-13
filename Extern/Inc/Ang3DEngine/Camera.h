// <Concept>
// Source Name		: Camera.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.02.15

// <Explanation>
// 카메라 기능 클래스
// 외부에서 데이터를 전달 받아서 카메라 Matrix를 생성한 뒤, 행렬버퍼에 입력합니다.
// Renderer class에서 외부에서 Matrix를 입력 받아 파이프라인에 직접 입력할 수도 있습니다.

// <Working Log>
// 2021.02.15.Hosik	: 생성
// 2021.03.25.Hosik : 카메라모드, 시점 별 함수 추가. 2.5D는 필요할 때 다시 세팅.

// <Memo>
// 2021.03.25.Hosik : UI 카메라의 원-근평면 Value의 적절한 값 확인이 필요함.

#pragma once

#include "MatrixBuffer.h"

namespace hos::cg
{

	class Camera
	{
	public:
		enum class CameraType	// 카메라 타입 목록
		{
			CAMERA_NONETYPE,
			CAMERA_3D,
			CAMERA_2D,
		};

		enum class CameraMode	// 카메라 모드 목록
		{
			CAMERA_NONEMODE,
			CAMERA_DEFAULT,
			CAMERA_LOOKAT,
		};

	private:
		// 행렬 버퍼에 대한 참조
		MatrixBuffer& MatrixData;
		DirectX::SimpleMath::Matrix& ViewTMRef;
		DirectX::SimpleMath::Matrix& ProjTMRef;
		DirectX::SimpleMath::Matrix& UIViewTMRef;
		DirectX::SimpleMath::Matrix& UIProjTMRef;


	public:
		Camera(MatrixBuffer& matrixBuffer, DirectX::SimpleMath::Matrix& viewTM, DirectX::SimpleMath::Matrix& projTM, DirectX::SimpleMath::Matrix& uiViewTMRef, DirectX::SimpleMath::Matrix& uiProjTMRef);
		~Camera();

	public:
		void Set3DCamera_DefaultView(DirectX::SimpleMath::Vector3 vPos, DirectX::SimpleMath::Vector3 vRot,
									float fov, float screenWidth, float screenHeight, float screenNear, float screenDepth);

		void Set3DCamera_FollowView(DirectX::SimpleMath::Vector3 eyePos, DirectX::SimpleMath::Vector3 lookAt, DirectX::SimpleMath::Vector3 upVector,
									float fov, float screenWidth, float screenHeight, float screenNear, float screenDepth);

		void Set2DCamera_DefaultView(DirectX::SimpleMath::Vector3 vPos, DirectX::SimpleMath::Vector3 vRot,
									float screenWidth, float screenHeight, float screenNear, float screenDepth);

		void Set2DCamera_FollowView(DirectX::SimpleMath::Vector3 eyePos, DirectX::SimpleMath::Vector3 lookAt, DirectX::SimpleMath::Vector3 upVector,
									float screenWidth, float screenHeight, float screenNear, float screenDepth);

		void Set3DUICamera(float screenWidth, float screenHeight);

	private:
		void Set3DCamera(float fov, float screenWidth, float screenHeight, float screenNear, float screenDepth);
		void Set2DCamera(float screenWidth, float screenHeight, float screenNear, float screenDepth);

		void SetDefaultView(DirectX::SimpleMath::Vector3 vPos, DirectX::SimpleMath::Vector3 vRot);
		void SetFollowView(DirectX::SimpleMath::Vector3 eyePos, DirectX::SimpleMath::Vector3 lookAt, DirectX::SimpleMath::Vector3 upVector);
	};
}