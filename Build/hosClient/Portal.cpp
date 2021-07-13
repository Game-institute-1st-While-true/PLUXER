#include "Portal.h"

using namespace hos;
using namespace com;

Portal::Portal() : hos::com::Script(L"Portal")
{
}

void Portal::OnTriggerEnter(hos::com::Collision* other)
{
	if (other->m_GameObject->GetTag() == L"Player")
	{
		g_Renderer->SetFogOption(0.5f, 0.5f, 0.5f, 0.008f);

		// 빛을 전부 찾아와야 한다.
		hos::com::Light* MainDirectionLight = m_GameObject->m_Scene->FindGameObjectWithName(L"MainDirectionLight")->GetComponent<Light>();
		MainDirectionLight->SetColor(Color(0.15f, 0.2f, 0.45f, 1));

		hos::com::Light* SubDirectionLight_0 = m_GameObject->m_Scene->FindGameObjectWithName(L"SubDirectionLight000")->GetComponent<Light>();
		SubDirectionLight_0->SetActive(false);

		hos::com::Light* SubDirectionLight_1 = m_GameObject->m_Scene->FindGameObjectWithName(L"SubDirectionLight001")->GetComponent<Light>();
		SubDirectionLight_1->SetActive(false);

		hos::com::Light* SubDirectionLight_2 = m_GameObject->m_Scene->FindGameObjectWithName(L"SubDirectionLight002")->GetComponent<Light>();
		SubDirectionLight_2->SetActive(false);

		hos::com::Light* AmbientLight = m_GameObject->m_Scene->FindGameObjectWithName(L"AmbientLight")->GetComponent<Light>();
		AmbientLight->SetColor(Color(0.05f, 0.1f, 0.25f));

		Transform* point = m_GameObject->transform->GetChild(0);
		other->m_GameObject->transform->SetPosition(point->GetPosition());
		hos::com::GameObject* BossUI = m_GameObject->m_Scene->FindGameObjectWithName(L"BossUI");
		if (BossUI)
		{
			BossUI->SetActive(true);
		}

		IsPlayerIn = true;

		g_Renderer->SetShadowMapCullMode(hos::cg::RendererDesc::ShadowMapCullList::SHADOWMAP_CULLFRONT);

	}
}