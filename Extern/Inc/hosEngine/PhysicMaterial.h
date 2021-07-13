#ifndef _PHYSIC_MATERIAL_H
#define _PHYSIC_MATERIAL_H

// [2021/1/8 노종원]
// 물리 머테리얼

// LOG

#include "hosUtility\hosUtility.h"
#include "PxPhysicsAPI.h"

namespace hos
{
	class PhysicMaterial
	{
	public:
		static constexpr string_literal FILE_EXTENSION = L".pm";
		static constexpr string_literal FILE_PATH = L"\\Assets\\PhysMat\\";

		enum class eCombine
		{
			COMBINE_AVERAGE = physx::PxCombineMode::eAVERAGE,
			COMBINE_MIN = physx::PxCombineMode::eMIN,
			COMBINE_MULTIPLY = physx::PxCombineMode::eMULTIPLY,
			COMBINE_MAX = physx::PxCombineMode::eMAX,

			NUM_COMBINE = physx::PxCombineMode::eN_VALUES,
		};

		static constexpr float DEFAULT_DYNAMIC_FRICTION = 0.6f;
		static constexpr float DEFAULT_STATIC_FRICTION = 0.6f;
		static constexpr float DEFAULT_BOUNCINESS = 0.f;
		static constexpr eCombine DEFAULT_FRICTION_COMBINE = eCombine::COMBINE_AVERAGE;
		static constexpr eCombine DEFAULT_BOUNCE_COMBINE = eCombine::COMBINE_AVERAGE;

	private:
		physx::PxMaterial* Material;

		//동적 마찰계수 [ 0 ~ F32_MAX ]
		float DynamicFriction;
		//정적 마찰계수 [ 0 ~ F32_MAX ]
		float StaticFriction;
		//탄성 [ 0 ~ 1 ]
		float Bounciness;
		//다른 물체와 접촉했을때 마찰계수를 계산하는 방식
		eCombine FrictionCombine;
		eCombine BounceCombine;
		string Name;

	public:
		PhysicMaterial(
			float dynamicFriction = DEFAULT_DYNAMIC_FRICTION,
			float staticFriction = DEFAULT_STATIC_FRICTION,
			float bounciness = DEFAULT_BOUNCINESS,
			eCombine frictionCombine = DEFAULT_FRICTION_COMBINE,
			eCombine bounceCombine = DEFAULT_BOUNCE_COMBINE
		);
		~PhysicMaterial();

		physx::PxMaterial* Get() const;

		float GetDynamicFriction()const;
		float GetStaticFriction()const;
		float GetBounciness()const;
		eCombine GetFrictionCombine()const;
		eCombine GetBounceCombine()const;
		string GetName()const;

		void SetDynamicFriction(float friction);
		void SetStaticFriction(float friction);
		void SetBounciness(float bounce);
		void SetFrictionCombine(eCombine combine);
		void SetBounceCombine(eCombine combine);

		const std::vector<U8> Serialize();
		bool Deserialize(mbstring_view);
	};
}

#endif // !_PHYSIC_MATERIAL_H