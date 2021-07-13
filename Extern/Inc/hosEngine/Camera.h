#ifndef _CAMERA_H
#define _CAMERA_H

// [2020/10/28 노종원]
// 카메라
// 오브젝트들이 화면상에 보일수 있도록 하는 컴포넌트

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/03/27] 노종원 : 복사 생성자 추가, 가상 복사 생성 함수 추가, GetProjMatrix 오타수정
// [2021/04/20] 신호식 : ViewPort관련 변수 2개 => 4개로 변경, Serialize 관련 부분 수정, GetSet 함수 정리.

#include "Component.h"

//Warning : Windows.h의 전역메크로 제거
#ifdef far
#undef far
#endif // far

#ifdef near
#undef near
#endif // near

namespace hos::com
{
	class Camera : public Component
	{
	public:
		enum class eMode
		{
			DEFAULT,
			FOLLOW,
		};
		enum class eViewMode
		{
			PERSPECTIVE,
			ORTHOGRAPHIC,
		};
		enum class eClearFlag
		{
			SKYBOX,
			SOLID_COLOR,
			NONE
		};
	private:
		float ViewPortLeft;
		float ViewPortRight;
		float ViewPortTop;
		float ViewPortBottom;

		float AspectRatio;
		U32 ScreenWidth;
		U32 ScreenHeight;

		float FOV;
		float Near;
		float Far;

		Vector3 FollowTarget;
		Color ClearColor;

		Matrix ViewMatrix;
		Matrix ProjMatrix;

		eMode Mode;
		eViewMode ViewMode;
		eClearFlag ClearFlag;

		hos::string SkyBoxName;
		
		hos::U32 MortionBlurSampleCount;
		bool RadialBlurState;

		bool bViewCull;

	public:
		Camera() = delete;
		//팔로우 카메라를 만들때 타겟을 지정하지 않으면 원점을 바라보게 됩니다.
		Camera(eMode mode = eMode::DEFAULT, Vector3 followTarget = Vector3(), eViewMode view = eViewMode::PERSPECTIVE, eClearFlag flag = eClearFlag::SOLID_COLOR, float fov = 60, float far = 500, float near = 1);
		~Camera();

	protected:
		Camera(const Camera& dest);

	public:
		float GetViewPortLeft() const;
		float GetViewPortRight() const;
		float GetViewPortTop() const;
		float GetViewPortBottom() const;
		void SetViewPortLeft(float ratioValue);
		void SetViewPortRight(float ratioValue);
		void SetViewPortTop(float ratioValue);
		void SetViewPortBottom(float ratioValue);
		void SetViewPort(float left, float right, float top, float bottom);

		float GetFOV() const;
		void SetFOV(float fov);
		float GetNear() const;
		void SetNear(float near);
		float GetFar() const;
		void SetFar(float far);

		Vector3 GetFollowTarget() const;
		void SetFollowTarget(const Vector3& followTarget);
		Color GetClearColor() const;
		void SetClearColor(const Color& color);

		Matrix GetViewMatrix() const;
		Matrix GetProjMatrix() const;

		void ChangeMode(eMode mode);
		void ChangeView(eViewMode view);
		void ChangeClearFlag(eClearFlag flag);

		hos::string GetSkyBoxName() const;
		void SetSkyBox(hos::string_view skyBoxname);

		eMode GetMode() const { return Mode; };
		eViewMode GetViewMode() const { return ViewMode; };
		eClearFlag GetClearFlag() const { return ClearFlag; };

		// MortionBlur On/Off
		void SetMortionBlurState(unsigned int sampleCount) { this->MortionBlurSampleCount = sampleCount; };
		void SetRadialBlurState(bool bRadial, float uv_x = 0.5f, float uv_y = 0.5f, float distance = 0.3f, float strength = 3.0f);

	public:
		Camera* Clone() const override;
		void Reset()override;
		void Bind(GameObject* gameObject) override;

		//ICallable
		void Start() override;
		void PreRender() override;
		void Render() override;
		void PostRender() override;
		void FixedUpdate() override;
		void OnEnable() override;
		void OnDisable() override;

		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view) override;
	};
}

#endif // !_CAMERA_H