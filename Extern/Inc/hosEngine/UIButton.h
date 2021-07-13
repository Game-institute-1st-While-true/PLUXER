#ifndef _UI_BUTTON_H
#define _UI_BUTTON_H

// [2021/04/19 신호식]
// UI 클릭을 체크하는 Button Component

// LOG
// [2021/04/19] 신호식 : 생성.
// [2021/04/22] 노종원 : MouseHover, Clicked등을 지원하기위해 UIImage를 상속받는다.

#include "UIImage.h"

namespace hos::com
{
	class UIButton : public Component
	{
	public:
		enum class eState
		{
			None,
			Hover,
			Pressed,
			Selected,
			Disable,
		};
		enum class eTransitionMode
		{
			None,
			Color,
			Sprite
		};

		static constexpr Color DEFAULT_NORMAL_COLOR = Color(1.f, 1.f, 1.f, 1.f);
		static constexpr Color DEFAULT_HOVER_COLOR = Color(0.96f, 0.96f, 0.96f, 1.f);
		static constexpr Color DEFAULT_PRESS_COLOR = Color(0.78f, 0.78f, 0.78f, 1.f);
		static constexpr Color DEFAULT_SELECTED_COLOR = Color(0.96f, 0.96f, 0.96f, 1.f);
		static constexpr Color DEFAULT_DISABLE_COLOR = Color(0.78f, 0.78f, 0.78f, 1.f);

	protected:
		bool Interactable;
		eTransitionMode Transition;

		Color NormalColor;
		Color HoverColor;
		Color PressColor;
		Color SelectedColor;
		Color DisableColor;

		string NormalSprite;
		string HoverSprite;
		string PressSprite;
		string SelectedSprite;
		string DisableSprite;

		//internal
		eState State;
		eState PrevState;
		bool bSelected;
		Color DefaultColor;
		string DefaultSprite;

	public:
		UIButton();
		virtual ~UIButton();

	protected:
		UIButton(const UIButton& dest);

	public:
		bool GetInteractable()const;
		void SetInteractable(bool b);
		Color GetNormalColor()const;
		void SetNormalColor(Color color);
		Color GetHoverColor()const;
		void SetHoverColor(Color color);
		Color GetPressColor()const;
		void SetPressColor(Color color);
		Color GetSelectedColor()const;
		void SetSelectedColor(Color color);
		Color GetDisableColor()const;
		void SetDisableColor(Color color);
		string GetNormalSprite()const;
		void SetNormalSprite(string name);
		string GetHoverSprite()const;
		void SetHoverSprite(string name);
		string GetPressSprite()const;
		void SetPressSprite(string name);
		string GetSelectedSprite()const;
		void SetSelectedSprite(string name);
		string GetDisableSprite()const;
		void SetDisableSprite(string name);
		eState GetState()const;
		void SetState(eState state);

		eTransitionMode GetTransition() const { return Transition; };
		void ChangeTransition(eTransitionMode mode);
		bool IsClick() const;
		bool OnHover() const;

		////// Componenet //////

		void Bind(GameObject* gameObject) override;
		void UnBind(int index)override;
		void Start();
		void OnGUI(float aspect_x, float aspect_y);
		UIButton* Clone() const override;
		void Reset() override;
		void OnEnable() override;
		void OnDisable() override;

		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view data) override;

	private:
	};

}

#endif //_UI_BUTTON_H