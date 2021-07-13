#include "hosEngine/Group.h"
#include "LobbyGroup.h"
#include "../packets/PacketDefine.h"

using namespace hos::Packet;

LobbyGroup::LobbyGroup(GroupSystem* engine, Group* group, GameObject* obj, float sendtime)
	:GroupCallBackcomponent(group, sendtime)
{
	// �׷쿡 �ش��ϴ� �ݹ��� ��������ݴϴ�.
	group->SetCallBack(this);

	// ���ӿ�����Ʈ�� �׷� ������Ʈ�� ��������ݴϴ�.
	obj->AddComponent(this);

	for (int i = 0; i < group->GetMaxMember(); i++)
	{
		// �׷��� �����ϴ� ������Ʈ�� Ư���ϰ� ���ӿ�����Ʈ�� ���� �����մϴ�. (���� �ٲ� ���� ����)
		// ���ӿ�����Ʈ���� ���� ������Ʈ, ��Ʈ��ũ ������Ʈ�� ���Ե˴ϴ�.
		GameObject* user = new GameObject(L"user");
		User* usercom = new User();

		Networkcomponent* netcom = new Networkcomponent(engine->Network);
		Users.push_back(usercom);

		// ���� ������Ʈ�� ���̵� �������ݴϴ�.
		// ���� ������Ʈ�� ��Ʈ��ũ ������Ʈ�� �������ݴϴ�.
		usercom->SetId(i);
		usercom->SetNetwork(netcom);

		// ��Ʈ��ũ ������Ʈ�� �̸� ��ū�� �ο��޽��ϴ�. --> �κ� �׷��̱� �����Դϴ�. 
		netcom->GetToken(i);
		// �׷� ���� �Լ��� �������ݴϴ�.
		netcom->MapingReqGroupFuncs(engine);

		// ���� �� �������� ��Ʈ��ũ ������Ʈ, ���� ������Ʈ�� ��������ݴϴ�.
		user->AddComponent(usercom);
		user->AddComponent(netcom);

		// �ڽ��� ���� ���� ���� ������Ʈ�� ������ݴϴ�.
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
	// �� ��� ���� �̺�Ʈ. ��û�Ҷ��� �۵��ϴ°� �´µ�.
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