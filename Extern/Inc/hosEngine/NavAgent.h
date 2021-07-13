#pragma once
#include "Component.h"
#include "NavQuery.h"

namespace hos::com
{
	class NavAgent : public Component
	{
		using NavQuery = hos::nv::NavQuery;
	private:
		NavQuery* Query;

	public:
		NavAgent();
		virtual ~NavAgent();

	protected:
		NavAgent(const NavAgent& dest);

	public:
		void SetPosition();
		Vector3 GetStart() { return Query->GetStart(); }
		void SetDestination(Vector3 dest);
		Vector3 GetDestination() { return Query->GetEnd(); }
		void SetTryCount(int count) { Query->TryCount = count; }
		int GetTryCount() { return Query->TryCount; }
		void SetMaxPathCount(int count) { Query->MaxPathCount = count; }
		int GetMaxPathCount() { return Query->MaxPathCount; }

		bool RequestPathFind();
		bool ReleasePathFind();		

		void SetNavMeshId(int id);
		Vector3 GetDir();

	public:
		void Bind(GameObject* gameObject) override;
		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view data) override;

		NavAgent* Clone()const override;
		void Reset()override;
		void Awake()override;
		void OnEnable()override;
		void OnDisable()override;
		void OnClose()override;
	};
}
