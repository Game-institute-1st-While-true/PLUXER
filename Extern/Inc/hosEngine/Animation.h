#ifndef _ANIMATION_H
#define _ANIMATION_H

// [2021/03/23 신호식]
// 애니메이션 컴포넌트

// LOG
// [2021/03/23] 신호식 : 생성 및 기능 구현 시작.
// [2021/04/05] 노종원 : Serialize/ Deserialize 추가
// [2021/04/06] 박용의 : Animation Bundle, Clip Get 추가
// [2021/05/12] 신호식 : Animation Blend 추가.

#include "Component.h"

namespace hos
{
	class AnimationBundle;
	class AnimationClip;
}

namespace hos::com
{

	class Animation :public Component
	{
	private:
		// Animation Information
		bool bAnimation;
		hos::string BundleName;
		hos::string ClipName;

		// Data Adress
		AnimationBundle* AniBundle;
		AnimationClip* AniClip;

		// Animation State
		bool bPlay;
		bool bPlayOnce;
		bool bReversePlay;

		// Animation Time
		hos::F32 NowTick;
		hos::F32 TimeScale;
		
		// Bone and Animation Matrix
		hos::U32 BoneCount;
		hos::Matrix AnimationTM[MAX_BONE];
		hos::Matrix BoneOffTM[MAX_BONE];

		// Blend State and Data Adress
		bool bBlendState;
		float BlendInTime;
		float BlendPointTick;

		hos::string BlendInClipName;
		AnimationClip* BlendInClip;

		////////////////////////////////
		// 저장 안해도 됨(프레임 마다 갱신)
		hos::F32 StartTick;
		hos::F32 LastTick;
		bool IsAnimationEnd;
		bool bBlendIn;
		///////////////////////////////

	public:
		Animation();
		~Animation();

	protected:
		Animation(const Animation& dest);

	public:
		// Class's Function //
		void SetAnimation(hos::string bundleName, hos::string clipName, hos::F32 startPercent = 0.0f, bool bPlayOnce = false);
		void SetReverseAnimation(hos::string bundleName, hos::string clipName, hos::F32 startPercent = 100.0f, hos::F32 endPercent = 0.0f, bool bPlayOnce = true);
		void SetBlendAnimation(hos::string bundleName, hos::string clipName, hos::string_view blendClipName, float blendPoint, float blendTime, bool bPlayOnce = false);
		void RemoveAnimation();

		// Component Function //
		Animation* Clone() const override;
		void Update() override;
		void Reset() override {};
		void OnEnable() override {};
		void OnDisable() override {};


		// Getter //
		// Bone Info
		hos::U32 GetBoneCount() const { return BoneCount; };

		// Animation Info
		AnimationBundle* GetAniBundle() const { return AniBundle; };
		AnimationClip* GetAniClip() const { return AniClip; };
		string GetAniBundleName() const { return BundleName; };
		string GetAniClipName() const { return ClipName; };

		// Matrix Info
		hos::Matrix* GetAnimationTM() { return AnimationTM; };
		hos::Matrix* GetBoneOffTM() { return BoneOffTM; };
		hos::Matrix GetAniTMbyBoneName(hos::string boneName);

		// Time Info
		hos::F32 GetNowTick() const { return NowTick; };
		hos::F32 GetLastTick() const { return AniClip->GetClipEndTime(); };
		hos::F32 GetTimeScale() const { return TimeScale; };

		// State Info
		bool GetIsAnimationEnd() const { return IsAnimationEnd; };
		hos::F32 GetBlendPoint() { return (this->NowTick / this->LastTick); };

		// Bounding Box Info
		hos::Vector3* GetBBoxAddress() { return this->AniClip->GetBBoxAddress(); };

		
		// Setter //
		// Tick Calculate
		void ResetAniTick() { NowTick = 0.0f; };
		void SetAnitTick(hos::F32 tick) { NowTick = tick; };
		void SetTimeScale(hos::F32 value) { TimeScale = value; };

		// Set PlayState
		void SetPlayState(bool bPlay) { this->bPlay = bPlay; };
		bool GetPlayState() { return this->bPlay; };


		// Data Function //
		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view data) override;

	private:
		void ProcessTickCount();
		void DetectionBlendState();
		void AnimateMainClip();
		void AnimateBlendIn();
		void ChangeBoneTransform(int boneIndex);
	};
}

#endif //!_ANIMATION_H
