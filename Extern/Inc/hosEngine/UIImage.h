#ifndef _UI_IMAGE_H
#define _UI_IMAGE_H

// [2021/04/06 신호식]
// 2D UI를 출력하는 컴포넌트

// LOG
// [2021/04/06] 신호식 : 생성.
// [2021/04/13] 박용의 : 에디터에서 사용하기 위해 GetSet 추가.

#include "Component.h"

namespace hos
{
	class BoneAnimation;
}

namespace hos::com
{
	class UIImage :public Component
	{
	private:
		Color BaseColor;
		Vector4 OffTil;
		I32 LayerOrder;
		I32 AlignType;

		U32 Width;
		U32 Height;

		bool bTexture;
		hos::string TextureName;

		bool bAni;
		bool bAnimationEnd;
		hos::F32 NowTick;
		hos::string AnimationName;
		BoneAnimation* AnimationData;

		bool bFollowUI;
		bool IsLoop;

		float Aspect_X;
		float Aspect_Y;

		hos::I32 CoordU;
		hos::I32 CoordV;

	public:
		UIImage();
		~UIImage();

	protected:
		UIImage(const UIImage& dest);

	public:
		hos::I32 GetLayerOrder() const { return LayerOrder; }
		void SetLayerOrder(hos::I32 val) { LayerOrder = val; }
		Color GetColor() const { return BaseColor; };
		void SetColor(hos::Color& color);
		Vector4 GetOffTil() const { return OffTil; };
		void SetOffTil(hos::Vector4& offTil);

		I32 GetAlignType() { return this->AlignType; };
		void SetAlignType(I32 typeIndex) { this->AlignType = typeIndex; };

		void SetTexture(hos::string textureName);
		hos::string GetTextureName() const { return TextureName; };
		void RemoveTexture();
		bool GetIsTexture() const { return bTexture; };
		void SetIsTexture(bool isTexture) { bTexture = isTexture; };

		void SetAnimation(hos::string aniName);
		hos::string GetAnimationName() const { return AnimationName; };
		void RemoveAnimation();
		bool GetIsAnimation() const { return bAni; };
		void SetIsAnimation(bool isAnimation) { bAni = isAnimation; };

		bool GetIsAnimationEnd() const { return bAnimationEnd; };

		hos::F32 GetNowTick() { return this->NowTick; };
		void SetNowTick(hos::F32 tick) { NowTick = tick; bAnimationEnd = false; };

		U32 GetWidthSize() { return this->Width; };
		U32 GetHeightSize() { return this->Height; };
		void SetWidth(U32 size) { this->Width = size; };
		void SetHeight(U32 size) { this->Height = size; };

		// FollowUI
		bool GetFollowUI() const { return bFollowUI; }
		void SetFollowUI(bool val) { bFollowUI = val; }
		bool GetIsLoop() const { return IsLoop; }
		void SetIsLoop(bool b) { IsLoop = b; }

		hos::I32 GetCoordU() const { return CoordU; }
		void SetCoordU(hos::I32 val) { CoordU = val; }
		hos::I32 GetCoordV() const { return CoordV; }
		void SetCoordV(hos::I32 val) { CoordV = val; }

		////// Componenet //////

		void Bind(GameObject* gameObject) override;
		void UnBind(int index)override;
		void OnGUI(float aspect_x, float aspect_y) override;
		void Update();
		UIImage* Clone() const override;
		void Reset() override {};
		void OnEnable() override;
		void OnDisable() override {};

		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view data) override;

	private:
		void ProcessTickCount();
	};
}

#endif // !_UI_IMAGE_H