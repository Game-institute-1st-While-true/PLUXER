#ifndef _UI_INPUT_FIELD_H
#define _UI_INPUT_FIELD_H

// [2021/04/22 노종원]
// 사용자의 입력을 받는 컴포넌트

// LOG

#include "UIButton.h"

namespace hos::com
{
	class UIInputField : public UIButton
	{
	public:
		enum class eMode
		{
			Default,
			Password,
			Number,
		};

		static constexpr U32 DEFAULT_TEXT_LIMIT = 0;
		static constexpr string_literal CursorImage = L"login_cursor.png";
	private:
		eMode Mode;
		U32 TextLimit;

		//internal
		string Text;
		double BlinkTime;
		float AspectX;
		float AspectY;
	public:
		UIInputField();
		virtual ~UIInputField();

	protected:
		UIInputField(const UIInputField& dest);

	public:
		eMode GetMode()const;
		void SetMode(eMode mode);
		U32 GetTextLimit()const;
		void SetTextLimit(U32 limit);
		string GetText()const;

		void Bind(GameObject* gameObject)override;
		void UnBind(int index)override;
		UIInputField* Clone()const override;
		void Reset()override;

		void Start()override;
		void OnGUI(float aspect_x, float aspect_y)override;
		void PreRender()override;
		void OnEnable()override;
		void OnDisable()override;

		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view data) override;
	};
}

#endif // !_UI_INPUT_FIELD_H