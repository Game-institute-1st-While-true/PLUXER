#include "GameGroup.h"
#include "hosEngine/Group.h"
#include "Move.h"
#include "../packets/PacketDefine.h"
#include "hosEngine/BoxCollision.h"
#include "hosEngine/Rigidbody.h"

using namespace hos::Packet;
GameGroup::GameGroup(GroupSystem* engine, Group* group, GameObject* obj, float sendtime)
	: GroupCallBackcomponent(group, sendtime), LoadingTime(0), GameTime(0)
{
	// 그룹에 해당하는 콜백을 적용시켜줍니다.
	group->SetActive(false);
	group->SetCallBack(this);

	// 게임오브젝트에 그룹 컴포넌트를 적용시켜줍니다.
	obj->AddComponent(this);
	updatefunc = &GameGroup::LoadingSceneUpdate;

	for (int i = 0; i < m_Group->GetMaxMember(); i++)
	{
		// 그룹을 관리하는 컴포넌트는 특이하게 게임오브젝트를 직접 생성합니다. (추후 바뀔 여지 있음)
		// 게임오브젝트에는 유저 컴포넌트, 네트워크 컴포넌트가 포함됩니다.

		// 해당 컴포넌트는 게임에 관련된 신에 포함되었으므로 Move 컴포넌트 또한 포함 됩니다.

		GameObject* user = new GameObject(L"user");
		User* usercom = new User();
		PlayerBehaivor* move = new PlayerBehaivor();
		BoxCollision* box = new BoxCollision();
		Rigidbody* rigidbody = new Rigidbody();

		Networkcomponent* netcom = new Networkcomponent(engine->Network);
		Users.push_back(usercom);

		// 여기에서의 네트워크 컴포넌트는 토큰을 부여받지 않습니다. --> 유저가 로비에서 방을 선택하면 토큰을 부여받게 됩니다.
		// 그룹관련 함수를 매핑해줍니다.
		netcom->MapingReqGroupFuncs(engine);

		// 그룹에 토큰을 포함시키지 않습니다. 역시 상위의 이유와 동일 합니다.
		// 게임 그룹의 프로토콜을 매핑해줍니다.
		MappingProtocol(netcom);

		// 유저의 아이디를 적용해줍니다.
		usercom->SetId(i);

		// 유저컴포넌트, 네트워크 컴포넌트, 무브 컴포넌트를 추가해줍니다.
		user->AddComponent(usercom);
		user->AddComponent(netcom);
		user->AddComponent(move);
		user->AddComponent(rigidbody);
		user->AddComponent(box);

		// 자신이 속한 씬에 게임 컴포넌트를 등록해줍니다.
		m_GameObject->m_Scene->AddGameObject(user);
		rigidbody->SetUseGravity(false);
	}

	GameObject* Obstacle = new GameObject(L"Obstacle");
	BoxCollision* obc = new BoxCollision();
	Obstacle->AddComponent(obc);
	Obstacle->GetComponent<Transform>()->SetPosition(Vector3(3, 0, 0));
	m_GameObject->m_Scene->AddGameObject(Obstacle);
}

GameGroup::~GameGroup()
{

}

void GameGroup::AddSceneUser()
{
	for (auto user : Users)
	{
		m_GameObject->m_Scene->AddGameObject(user->m_GameObject);
	}
}

void GameGroup::OnEnable()
{
}

void GameGroup::OnDisable()
{
}

void GameGroup::Awake()
{
	int i = 0;
	for (auto user : Users)
	{
		if (user != nullptr)
		{
			user->m_GameObject->GetComponent<Transform>()->SetPosition(Vector3(i * -2, 1, 0));

			if (user->IsReady() == true)
			{
				user->m_GameObject->SetActive(true);
				i++;
			}
		}
	}

	updatefunc = &GameGroup::NotImplemented;
	GameTime = 0;
	LoadingTime = 0;
}

void GameGroup::Reset()
{
}

void GameGroup::Req_FourceLobby()
{
	flatbuffers::FlatBufferBuilder builder;

	int id = -1;
	auto name = builder.CreateString("");
	int event = ROOM_CLOSE;
	int certification = false;

	auto protocol = CreateUserEventProtocol(builder, id, name, certification, event, 0);
	builder.Finish(protocol);

	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = event;

	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	m_Group->Send(pack);
}

void GameGroup::Req_SetUp()
{
	flatbuffers::FlatBufferBuilder builder;
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = POSITION;
	std::vector<flatbuffers::Offset<PositionProtocol>> pos_vec;

	for (int i = 0; i < Users.size(); i++)
	{
		auto tr = Users[i]->GetTransform();

		Vec3 pos(tr->GetPosition().x, tr->GetPosition().y, tr->GetPosition().z);
		Vec3 rot(tr->GetRotation().x, tr->GetRotation().y, tr->GetRotation().z);

		auto position = CreatePositionProtocol(builder, i, &pos, &rot, Users[i]->m_GameObject->GetActive());
		pos_vec.push_back(position);
	}

	auto positions = builder.CreateVector(pos_vec);
	auto protocol = CreatePositionListProtocol(builder, positions);

	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	m_Group->Send(pack);
}

void GameGroup::Req_LoadScene(int scene_number)
{
	flatbuffers::FlatBufferBuilder builder;

	int id = -1;
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
	m_Group->Send(pack);
}

void GameGroup::Req_LoadScene(NetworkToken* token, int scene_number)
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

void GameGroup::Update()
{
	switch (m_Group->GetState())
	{
	case hos::com::GROUP_STATE::LOAD:
		LoadingSceneUpdate();
		break;
	case hos::com::GROUP_STATE::PLAY:
		GameCloseCheck();
		break;
	case hos::com::GROUP_STATE::RELEASE:
		break;
	}
}

void GameGroup::LateUpdate()
{
	GroupCallBackcomponent::LateUpdate();
}

void GameGroup::OnAddToken(NetworkToken* token, int tokenIndex)
{
	Users[tokenIndex]->GetNetwork()->GetToken(token->GetSessionId());
	if (token->IsConnection())
	{
		Req_LoadScene(token, 2);
		AddTokenEventSend(Users[tokenIndex]);
	}
}

void GameGroup::OnDelToken(NetworkToken* token, int tokenIndex)
{
	Networkcomponent* network = Users[tokenIndex]->GetNetwork();

	if (network != nullptr)
	{
		Users[tokenIndex]->GetNetwork()->RemoveToken();
		DelTokenEventSend(Users[tokenIndex]);
	}
}

void GameGroup::OnStateUpdate()
{
	switch (m_Group->GetState())
	{
	case hos::com::GROUP_STATE::SLEEP:
		break;
	case hos::com::GROUP_STATE::WAIT:
		LoadingTime = 0;
		break;
	case hos::com::GROUP_STATE::LOAD:

		break;
	case hos::com::GROUP_STATE::PLAY:
		for (auto user : Users)
		{
			user->m_GameObject->SetActive(true);
		}

		Req_LoadScene(1);
		updatefunc = &GameGroup::PositionUpdate;
		break;
	case hos::com::GROUP_STATE::RELEASE:
		break;
	default:
		break;
	}
}

void GameGroup::OnUpateFunc()
{
	updatefunc(*this);
}

void GameGroup::MappingProtocol(Networkcomponent* network)
{
	network->Mapping(this, static_cast<unsigned char>(HOS_PROTOCOL::MOVE), &GameGroup::Ans_Move);
	network->Mapping(this, static_cast<unsigned char>(HOS_PROTOCOL::REQ_ID), &GameGroup::Ans_ReqID);
	network->Mapping(this, static_cast<unsigned char>(HOS_PROTOCOL::READY), &GameGroup::Ans_Ready);
	network->Mapping(this, static_cast<unsigned char>(HOS_PROTOCOL::SETUP), &GameGroup::Ans_SetUp);

}

void GameGroup::PositionUpdate()
{
	flatbuffers::FlatBufferBuilder builder;
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = POSITION;
	std::vector<flatbuffers::Offset<PositionProtocol>> pos_vec;

	for (int i = 0; i < Users.size(); i++)
	{
		auto tr = Users[i]->GetTransform();

		Vec3 pos(tr->GetPosition().x, tr->GetPosition().y, tr->GetPosition().z);
		Vec3 rot(tr->GetRotation().x, tr->GetRotation().y, tr->GetRotation().z);

		auto position = CreatePositionProtocol(builder, i, &pos, &rot, Users[i]->m_GameObject->GetActive());
		pos_vec.push_back(position);
	}

	auto positions = builder.CreateVector(pos_vec);
	auto protocol = CreatePositionListProtocol(builder, positions);

	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());

	for (auto user : Users)
	{
		auto network = user->GetNetwork();
		if (network != nullptr)
		{
			user->GetNetwork()->SendMsg(pack);
		}
	}
}

void GameGroup::LoadingSceneUpdate()
{
	LoadingTime = LoadingTime + m_GameObject->m_Scene->GetTimer()->DeltaTime();
	if (LoadingTime >= 5)
	{
		m_Group->SetState(GROUP_STATE::PLAY);
		LoadingTime = 0.f;
	}
}

void GameGroup::GameCloseCheck()
{
	GameTime += m_GameObject->m_Scene->GetTimer()->DeltaTime();
	if (GameTime >= 100)
	{
		GameTime = 0.f;
		m_Group->SetState(GROUP_STATE::RELEASE);
	}
}

void GameGroup::ReadyCheck()
{
	int cnt = 0;
	for (auto user : Users)
	{
		if (user != nullptr)
		{
			if (user->GetReady() == true)
			{
				++cnt;
			}
		}
	}

	if (cnt == m_Group->GetCurrentMenber())
	{
		m_Group->SetState(GROUP_STATE::WAIT);
	}
}

void GameGroup::NotImplemented()
{
}

void GameGroup::AddTokenEventSend(User* user)
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

void GameGroup::DelTokenEventSend(User* user)
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

void GameGroup::Ans_ReqID(PACKAGE& package)
{
	flatbuffers::FlatBufferBuilder builder;
	auto event_protocol = GetEventProtocol(package.packet.memory + PACKET_HEADER_SIZE);

	int id = -1;
	int event = event_protocol->event();
	int result = m_Group->FindToken(package.id);

	auto data = CreateEventProtocol(builder, id, event, result);
	builder.Finish(data);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned char>(event);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());

	if (result >= 0)
	{
		Users[result]->GetNetwork()->SendMsg(pack);
	}
}

void GameGroup::Ans_Ready(PACKAGE& package)
{
	flatbuffers::FlatBufferBuilder builder;
	auto event_protocol = GetEventProtocol(package.packet.memory + PACKET_HEADER_SIZE);

	int id = event_protocol->id();
	int event = event_protocol->event();
	int result = event_protocol->id();

	auto data = CreateEventProtocol(builder, id, event, result);
	builder.Finish(data);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned char>(event);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());

	Users[result]->SetReady(true);
	m_Group->Send(pack);
	ReadyCheck();
}

void GameGroup::Ans_Move(PACKAGE& package)
{
	auto move_protocol = GetMoveProtocol(package.packet.memory + PACKET_HEADER_SIZE);
	Users[move_protocol->id()]->MoveUpdate(move_protocol);

	for (auto user : Users)
	{
		auto network = user->GetNetwork();
		if (network != nullptr)
		{
			network->SendMsg(package.packet);
		}
	}
}

void GameGroup::Ans_SetUp(PACKAGE& package)
{
	auto position = GetPositionListProtocol(package.packet.memory + PACKET_HEADER_SIZE);

	auto pos_vec = position->Positions();
	auto len = pos_vec->size();

	for (int i = 0; i < len; i++)
	{
		auto data = pos_vec->Get(i);
		auto tr = Users[data->id()]->m_GameObject->GetComponent<Transform>();

		Users[data->id()]->PositionUdpate
		(
			Vector3(data->position()->x(), data->position()->y(), data->position()->z()),
			Rotator(data->rotation()->x(), data->rotation()->y(), data->rotation()->z())
		);
		Users[data->id()]->m_GameObject->SetActive(data->active());
	}
	Req_SetUp();
}
