#ifndef _UI_TEXT_H
#define _UI_TEXT_H

// [2021/03/10 노종원]
// 텍스트를 출력하는 UI

// LOG

#include "Component.h"
#include "FontData.h"

namespace hos
{
	class BoneAnimation;
}

namespace hos::com
{
	class UIText :public Component
	{
	private:
		string Text;
		int FontSize;
		Color FontColor;
		Vector2 Origin;
		float FontRot;
		int LayerOrder;

		I32 AlignType;

		string FontName;
		FontData* Font;

		bool bAni;
		bool bAnimationEnd;
		hos::F32 NowTick;
		hos::string AnimationName;
		BoneAnimation* AnimationData;

		float Aspect_X;
		float Aspect_Y;

		bool bFollowText;
		bool IsLoop;

		hos::I32 CoordU;
		hos::I32 CoordV;

	public:
		UIText();
		~UIText();
	protected:
		UIText(const UIText& dest);

	public:
		void SetText(string_view text);
		void SetTextSafe(string_view text);
		void SetFont(string_view fontName);
		void SetFontSize(int size);
		void SetFontColor(Color color);
		void SetOrigin(Vector2 origin) { this->Origin = origin; };

		string GetText()const;
		string GetFontName()const;
		FontData* GetFontData()const;
		int GetFontSize()const;
		Color GetFontColor()const;
		Vector2 GetOrigin() const { return this->Origin; };

		I32 GetAlignType() { return this->AlignType; };
		void SetAlignType(I32 typeIndex) { this->AlignType = typeIndex; };

		float GetFontRot() const { return FontRot; }
		void SetFontRot(float val) { FontRot = val; }
		int GetLayerOrder() const { return LayerOrder; }
		void SetLayerOrder(int val) { LayerOrder = val; }

		BoneAnimation* GetAnimation() const { return AnimationData; };
		void SetAnimation(hos::string aniName);
		hos::string GetAnimationName() const { return AnimationName; };
		void RemoveAnimation();
		
		bool GetIsAnimation() const { return bAni; };
		void SetIsAnimation(bool isAnimation) { bAni = isAnimation; };
		bool GetIsLoop() const { return IsLoop; };
		void SetIsLoop(bool isLoop) { IsLoop = isLoop; };

		bool GetIsAnimationEnd() const { return bAnimationEnd; };

		hos::F32 GetNowTick() { return this->NowTick; };
		void SetNowTick(hos::F32 tick) { NowTick = tick; bAnimationEnd = false; };

		hos::Vector2 GetTextBoundSize();

		bool GetFollowText() const { return bFollowText; }
		void SetFollowText(bool val) { bFollowText = val; }

		hos::I32 GetCoordU() const { return CoordU; }
		void SetCoordU(hos::I32 val) { CoordU = val; }
		hos::I32 GetCoordV() const { return CoordV; }
		void SetCoordV(hos::I32 val) { CoordV = val; }

		// Component
		void Bind(GameObject* gameObject) override;
		void UnBind(int index)override;
		void OnGUI(float aspect_x, float aspect_y) override;
		void Update();
		UIText* Clone() const override;
		void Reset() override {};
		void OnEnable() override;
		void OnDisable() override {};

	private:
		void ProcessTickCount();

		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view data) override;
	};
}

#endif // _UI_TEXT_H