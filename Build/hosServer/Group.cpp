#include "Group.h"
#include "hosEngine/NetworkManager.h"
#include "hosEngine/Networkcomponent.h"

#include "PacketDefine.h"
#include "ScriptFactory.h"
#include "GroupCallBackcomponent.h"
#include "DB_Connection.h"

#include "BossRoom.h"

using namespace hos::Packet;
hos::com::Group::Group(const int id)
	: MaxMember(0), Id(id), m_Scene(nullptr), Name(L"Empty_Group"), CallBack(nullptr), State(GROUP_STATE::NONE), Type(GROUP_TYPE::NONE), GroupLifeTime(30), GroupLifeCheck(0), Playing(false), RoomDestroy(false)
{
}

hos::com::Group::~Group()
{
	SafeDelete(m_Scene);
}

void hos::com::Group::SetUpMembers(const int max)
{
	if (!Members.empty())
	{
		NetLogger::GetIns()->Write(L"members not empty");
		Members.clear();
		MemberIdQueue.clear();
	}

	MaxMember = max;
	for (int i = 0; i < MaxMember; i++)
	{
		Members.push_back(nullptr);
		MemberIdQueue.push_back(i);
	}
}

void hos::com::Group::SetCallBack(GroupCallBack* callback)
{
	SafeDelete(CallBack);
	CallBack = callback;
}

void hos::com::Group::SetState(GROUP_STATE state)
{

	if (DB_Connection::DBC == nullptr) return;
	if (State == state) return;
	
	State = state;

	switch (State)
	{
	case hos::com::GROUP_STATE::ENABLE:

		break;
	case hos::com::GROUP_STATE::DISABLE:
		DB_Connection::DBC->Req_DeActiveRoom(Id);
		break;
	case hos::com::GROUP_STATE::GAME:
		DB_Connection::DBC->Req_StateUpdateGameRoom(Id);
		break;
	case hos::com::GROUP_STATE::WAIT:
		DB_Connection::DBC->Req_StateUpdateWaitRoom(Id);
		break;
	case hos::com::GROUP_STATE::RELEASE:
		//AllDisconnect();	
		GroupLifeCheck = 0;
		break;
	}

	CallBack->OnStateUpdate();
}

bool hos::com::Group::AddMember(NetworkToken* token, std::wstring name, int uuid)
{
	std::lock_guard<std::mutex> lock(MemberLock);
	if (MemberIdQueue.empty() == true ||
		static_cast<int>(State) == static_cast<int>(GROUP_STATE::DISABLE))
	{
		return false;
	}
	int id = MemberIdQueue.front();
	Members[id] = token;
	MemberIdQueue.pop_front();
	token->SetGroupId(Id);
	CallBack->OnAddToken(token, id, name, uuid);
	return true;
}

bool hos::com::Group::DelMember(NetworkToken* token)
{
	std::lock_guard<std::mutex> lock(MemberLock);
	int i = 0;
	for (; i < MaxMember; ++i)
	{
		if (Members[i] == token)
		{
			Members[i]->SetGroupId(-1);
			MemberIdQueue.push_back(i);
			CallBack->OnDelToken(token, i);
			Members[i] = nullptr;
			break;
		}
	}
	if (MemberIdQueue.size() == MaxMember)
	{
		SetState(GROUP_STATE::RELEASE);
	}
	return true;
}

void hos::com::Group::GroupSend(PACKAGE& package)
{
	Send(package.packet);
}

void hos::com::Group::ExceptMeGroupSend(PACKAGE& package)
{
	for (auto token : Members)
	{
		if (token != nullptr)
		{
			if (token->GetSessionId() != package.id)
			{
				token->SendMsg(package.packet);
			}
		}
	}
}

void hos::com::Group::Send(PACKET& packet)
{
	for (auto token : Members)
	{
		if (token != nullptr)
		{
			token->SendMsg(packet);
		}
	}
}

void hos::com::Group::MemberDisconnect(NetworkToken* token)
{
	for (auto token : Members)
	{
		if (token == token)
		{
			token->Disconnect();
		}
	}
}

void hos::com::Group::AllDisconnect()
{
	for (auto token : Members)
	{
		if (token)
		{
			token->Disconnect();
		}
	}
}

void hos::com::Group::Load(string_view scene_name)
{
	SafeDelete(m_Scene);
	m_Scene = g_DataManager->GetScene(scene_name.data());
	if (m_Scene)
	{
		g_Factory->BindObject(m_Scene);
		m_Scene->ExecutionOrderRebuild();
	}
	else
	{
		NetLogger::GetIns()->Write(L"group", L"Scene Is Null!!!");
	}
	State = GROUP_STATE::DISABLE;
	GameObject* net_go = new GameObject(m_Scene->GetName());
	net_go->AddComponent(static_cast<GroupCallBackcomponent*>(CallBack));
	m_Scene->AddGameObject(net_go);
	SceneName = m_Scene->GetName();
}

void hos::com::Group::Load()
{
	SafeDelete(m_Scene);
	m_Scene = g_DataManager->GetScene(SceneName);
	if (m_Scene)
	{
		g_Factory->BindObject(m_Scene);
		m_Scene->ExecutionOrderRebuild();
	}
	else
	{
		NetLogger::GetIns()->Write(L"group", L"Scene Is Null!!!");
	}
	SetState(GROUP_STATE::DISABLE);

	GameObject* net_go = new GameObject(m_Scene->GetName());
	net_go->AddComponent(static_cast<GroupCallBackcomponent*>(CallBack));
	m_Scene->AddGameObjectRecursive(net_go);
	SceneName = m_Scene->GetName();
}

void hos::com::Group::LifeCycleCheck()
{
#pragma region OldCode
	//if (static_cast<int>(State) > static_cast<int>(GROUP_STATE::RELEASE)) return;
	//if (!RoomDestroy) return;

	//double delta = m_Scene->SceneTime->DeltaTime();

	//if (GroupLifeCheck >= GroupLifeTime)
	//{
	//	GroupLifeCheck = 0;
	//	SetState(GROUP_STATE::RELEASE);
	//}
	//else
	//{
	//	GroupLifeCheck += delta;
	//}
#pragma endregion
}

void hos::com::Group::Loop()
{
	m_Scene->SceneTime->Tick();

	m_Scene->FixedUpdate();
	m_Scene->Update();
	m_Scene->LateUpdate();

	LifeCycleCheck();
}