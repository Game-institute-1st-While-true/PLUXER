#ifndef _SCRIPT_H
#define _SCRIPT_H

// [2020/12/10 노종원]
// 스크립트
// 유저가 제작하는 스크립트 컴포넌트 이후 설계변경에 따라 사용하지 않을 가능성도 있다
// 임시 선언만 해놓는다

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/03/27] 노종원 : 복사 생성자 추가, 가상 복사 생성 함수 추가

#include "Component.h"

namespace hos::com
{
	class Script : public Component
	{
	private:
		string ScriptName;
		int EditerIndex;

	public:
		Script();
		Script(string_view scriptName);
		~Script();

	protected:
		Script(const Script& dest);

	public:
		string GetScriptName();
		void SetScriptName(string_view scriptName);
		void SetExecutionOrder(int order);
		int GetEditerIndex() const { return EditerIndex; };
		void SetEditerIndex(int index) { EditerIndex = index; };

		Script* Clone() const override;
		void Reset()override;
		void OnEnable()override;
		void OnDisable()override;

		const std::vector<U8> Serialize()final;
		bool Deserialize(mbstring_view data)final;
	};
}

#endif // !_SCRIPT_H