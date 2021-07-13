#pragma once
#include "hosEngine/NetDefine.h"
#include "hosEngine/Component.h"	
#include "Group.h"

// [2020/02/10 김효곤]
// 그룹을 제어하기 위한 컴포넌트, 그룹 로직, 그룹 샌드가 들어간다.
// 그룹을 제어하기 위해서 그룹 객체를 소유하고 있다.

// [2020/02/22 김효곤]
// 그룹을 통해 데이터를 내보낼 때 이 컴포넌트를 통하자.

namespace hos::com
{
	class NetworkToken;
	class GroupCallBackcomponent : public Component, public GroupCallBack
	{
		double SendTime;
		double CheckTime;

	protected:

	public:
		GroupCallBackcomponent() = delete;
		GroupCallBackcomponent(Group* group);
		virtual ~GroupCallBackcomponent();

	public:
		void Reset()override;		
		void OnEnable()override;
		void OnDisable()override;
		void LateUpdate() override;
		
		virtual void OnUpateFunc() {};													// 일정시간 마다 Update 호출
	
	private:
		void Bind(GameObject* gameObject) override;
		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view) override;
	};
}