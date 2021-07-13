#ifndef _I_CALLABLE
#define _I_CALLABLE

// [2020/10/23 노종원]
// ICallable 각 오브젝트, 씬등 엔진내부에서 호출할 수 있는 함수들을 가진 인터페이스
// ICallableData ICallableFunction을 담고있는 델리게이트

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/04/19] 노종원 : Deprecated 된 ICallableData를 주석

#include "hosUtility/hosUtility.h"

namespace hos::com
{
	class Collision;
	class ICallable abstract
	{
	public:
		ICallable() = default;
		virtual ~ICallable() = default;

	protected:
		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void FixedUpdate() = 0;
		virtual void OnTriggerEnter(Collision* other) = 0;
		virtual void OnTriggerStay(Collision* other) = 0;
		virtual void OnTriggerExit(Collision* other) = 0;
		virtual void OnCollisionEnter(Collision* other) = 0;
		virtual void OnCollisionStay(Collision* other) = 0;
		virtual void OnCollisionExit(Collision* other) = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;
		virtual void OnStateMachine() = 0;
		virtual void OnAnimation() = 0;
		virtual void PreRender() = 0;
		virtual void Render() = 0;
		virtual void PostRender() = 0;
		virtual void RenderImage() = 0;
		virtual void OnGUI(float aspect_x, float aspect_y) = 0;
		virtual void OnPause() = 0;
		virtual void OnClose() = 0;
		virtual void OnQuit() = 0;
		virtual void OnDestory() = 0;
		virtual void OnEnable() = 0;
		virtual void OnDisable() = 0;
	};

	///Deprecated
	/*class ICallableData abstract
	{
	protected:
		ut::Delegate<void> AwakeFunc;
		ut::Delegate<void> StartFunc;
		ut::Delegate<void> FixedUpdateFunc;
		ut::Delegate<void, Collision*> OnTriggerEnterFunc;
		ut::Delegate<void, Collision*> OnTriggerStayFunc;
		ut::Delegate<void, Collision*> OnTriggerExitFunc;
		ut::Delegate<void, Collision*> OnCollisionEnterFunc;
		ut::Delegate<void, Collision*> OnCollisionStayFunc;
		ut::Delegate<void, Collision*> OnCollisionExitFunc;
		ut::Delegate<void> UpdateFunc;
		ut::Delegate<void> LateUpdateFunc;
		ut::Delegate<void> OnStateMachineFunc;
		ut::Delegate<void> OnAnimationFunc;
		ut::Delegate<void> PreRenderFunc;
		ut::Delegate<void> RenderFunc;
		ut::Delegate<void> PostRenderFunc;
		ut::Delegate<void> RenderImageFunc;
		ut::Delegate<void> OnGUIFunc;
		ut::Delegate<void> OnPauseFunc;
		ut::Delegate<void> OnCloseFunc;
		ut::Delegate<void> OnQuitFunc;
		ut::Delegate<void> OnDestoryFunc;

	public:
		ICallableData() = default;
		ICallableData(const ICallableData&) = default;
		ICallableData& operator=(const ICallableData&) = default;
		ICallableData(ICallableData&&) = default;
		ICallableData& operator=(ICallableData&&) = default;
		virtual ~ICallableData() = default;

		virtual void ExecutionOrderAwake() {};
		virtual void ExecutionOrderStart() {};
		virtual void ExecutionOrderFixedUpdate() {};
	};*/
}

#endif // !_I_CALLABLE