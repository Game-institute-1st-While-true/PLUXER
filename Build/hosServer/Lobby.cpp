#include "Lobby.h"
#include "PacketDefine.h"

Lobby::Lobby(hos::com::Group* group)
	:GroupCallBackcomponent(group)
{
}

Lobby::~Lobby()
{
}

void Lobby::Awake()
{
}

void Lobby::AddPlayer(Player* player)
{
	Players.push_back(player);
	player->m_GameObject->network->Mapping(m_Group, static_cast<int>(PROTOCOL::GROUPSEND), &hos::com::Group::GroupSend);
	player->m_GameObject->network->Mapping(m_Group, static_cast<int>(PROTOCOL::EXPORT_ME_GROUPSEND), &hos::com::Group::ExceptMeGroupSend);
}

void Lobby::OnLoad()
{
}

void Lobby::OnAddToken(hos::com::NetworkToken* token, int tokenIndex, std::wstring name, int uuid)
{
	if (Players[tokenIndex]->m_GameObject->network)
	{
		Players[tokenIndex]->m_GameObject->network->RequestToken(token->GetSessionId());
		Players[tokenIndex]->SetId(name);
		Players[tokenIndex]->SetUuid(uuid);
		//Req_RoomInEvent(Players[tokenIndex]->GetId().c_str(), tokenIndex);
		if (token->IsConnection())
		{
		}
		if (m_Group->GetCurrentMenber() == m_Group->GetMaxMember())
		{
		}
	}
}

void Lobby::OnDelToken(hos::com::NetworkToken* token, int tokenIndex)
{
	if (Players[tokenIndex]->m_GameObject->network)
	{
		Players[tokenIndex]->m_GameObject->network->RemoveToken();
		//DelTokenEventSend(Users[tokenIndex]);
	}
}

void Lobby::OnStateUpdate()
{

}