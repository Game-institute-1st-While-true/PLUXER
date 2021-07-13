#pragma once
#include "hosEngine/NetDefine.h"
#include "hosEngine/Component.h"	
#include "Group.h"

// [2020/02/10 ��ȿ��]
// �׷��� �����ϱ� ���� ������Ʈ, �׷� ����, �׷� ���尡 ����.
// �׷��� �����ϱ� ���ؼ� �׷� ��ü�� �����ϰ� �ִ�.

// [2020/02/22 ��ȿ��]
// �׷��� ���� �����͸� ������ �� �� ������Ʈ�� ������.

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
		
		virtual void OnUpateFunc() {};													// �����ð� ���� Update ȣ��
	
	private:
		void Bind(GameObject* gameObject) override;
		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view) override;
	};
}