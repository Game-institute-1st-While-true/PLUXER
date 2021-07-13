#include "hosEngine/Group.h"
#include "LobbyGroup.h"
#include "../packets/PacketDefine.h"

using namespace hos::Packet;

LobbyGroup::LobbyGroup(GroupSystem* engine, Group* group, GameObject* obj, float sendtime)
	:GroupCallBackcomponent(group, sendtime)
{
	// 그룹에 해당하는 콜백을 적용시켜줍니다.
	group->SetCallBack(this);

	// 게임오브젝트에 그룹 컴포넌트를 적용시켜줍니다.
	obj->AddComponent(this);

	for (int i = 0; i < group->GetMaxMember(); i++)
	{
		// 그룹을 관리하는 컴포넌트는 특이하게 게임오브젝트를 직접 생성합니다. (추후 바뀔 여지 있음)
		// 게임오브젝트에는 유저 컴포넌트, 네트워크 컴포넌트가 포함됩니다.
		GameObject* user = new GameObject(L"user");
		User* usercom = new User();

		Networkcomponent* netcom = new Networkcomponent(engine->Network);
		Users.push_back(usercom);

		// 유저 컴포넌트에 아이디를 지정해줍니다.
		// 유저 컴포넌트에 네트워크 컴포넌트를 지정해줍니다.
		usercom->SetId(i);
		usercom->SetNetwork(netcom);

		// 네트워크 컴포넌트는 미리 토큰을 부여받습니다. --> 로비 그룹이기 때문입니다. 
		netcom->GetToken(i);
		// 그룹 관련 함수를 매핑해줍니다.
		netcom->MapingReqGroupFuncs(engine);

		// 생성 될 유저에게 네트워크 컴포넌트, 게임 컴포넌트를 적용시켜줍니다.
		user->AddComponent(usercom);
		user->AddComponent(netcom);

		// 자신이 속한 씬에 게임 컴포넌트를 등록해줍니다.
		m_GameObject->m_Scene->AddGameObject(user);
	}
}

LobbyGroup::~LobbyGroup()
{
}

void LobbyGroup::OnEnable()
{
}

void LobbyGroup::OnDisable()
{
}

void LobbyGroup::Reset()
{
}

void LobbyGroup::Req_LoadScene(NetworkToken* token, int scene_number)
{
	flatbuffers::FlatBufferBuilder builder;

	int id = m_Group->FindToken(token);
	auto name = builder.CreateString("");
	int event = LOAD_SCENE;
	int certification = false;
	int sceneNumber = scene_number;

	auto protocol = CreateUserEventProtocol(builder, id, name, certification, event, sceneNumber);
	builder.Finish(protocol);

	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = event;

	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	token->SendMsg(pack);
}


void LobbyGroup::OnAddToken(NetworkToken* token, int tokenIndex)
{
	Users[tokenIndex]->GetNetwork()->GetToken(token->GetSessionId());
	
	if (token->IsConnection())
	{
		Req_LoadScene(token, 0);
		AddTokenEventSend(Users[tokenIndex]);
	}
}

void LobbyGroup::OnDelToken(NetworkToken* token, int tokenIndex)
{
	Networkcomponent* network = Users[tokenIndex]->GetNetwork();

	if (network != nullptr)
	{
		Users[tokenIndex]->GetNetwork()->RemoveToken();
		DelTokenEventSend(Users[tokenIndex]);
	}
}

void LobbyGroup::OnUpateFunc()
{
	// 룸 목록 갱신 이벤트. 요청할때만 작동하는게 맞는듯.
}

void LobbyGroup::OnStateUpdate()
{
}

void LobbyGroup::AddTokenEventSend(User* user)
{
	flatbuffers::FlatBufferBuilder builder;

	int id = user->GetId();
	auto u8_name = UTF16ToUTF8(user->GetName());
	auto name = builder.CreateString(u8_name);

	bool certification = user->IsCertification();
	int event = GROUP_IN_EVENT;

	auto protocol = CreateUserEventProtocol(builder, id, name, certification, event, 1);

	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned char>(event);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	m_Group->Send(pack);
}

void LobbyGroup::DelTokenEventSend(User* user)
{
	flatbuffers::FlatBufferBuilder builder;

	int id = user->GetId();
	auto u8_name = UTF16ToUTF8(user->GetName());
	auto name = builder.CreateString(u8_name);

	bool certification = user->IsCertification();
	int event = GROUP_OUT_EVENT;

	auto protocol = CreateUserEventProtocol(builder, id, name, certification, event, 1);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned char>(event);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	m_Group->Send(pack);
}