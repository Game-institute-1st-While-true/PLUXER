#include "NetworkEvent.h"
#include "hosEngine/Group.h"

using namespace hos;
using namespace com;

NetworkEvent::NetworkEvent(Group* now_group)
	: synctime(0)
{
	group = now_group;

	auto objects = group->GetSceneManager()->GetScene(0)->GetObjects();
	objcount = 3;
	for (int i = 0; i < objcount; i++)
	{
		users.push_back(objects[i]->GetComponent<User>());
	}
}

NetworkEvent::~NetworkEvent()
{
}

void NetworkEvent::Awake()
{
	for (int i = 0; i < objcount; i++)
	{
		users[i]->SetId(i);
		auto network = users[i]->m_GameObject->GetComponent<Networkcomponent>();
		network->GetToken(i);
		MappingProtocol(network);
	}
}

void NetworkEvent::Update()
{
}

void NetworkEvent::LateUpdate()
{
	synctime += Time->DeltaTime();
	if (synctime >= 0.01f)
	{
		synctime = 0;
		MakeActiveUserPosition();
	}
}

void NetworkEvent::MappingProtocol(Networkcomponent* network)
{
	network->Mapping(this, static_cast<unsigned char>(HOS_PROTOCOL::MOVE), &NetworkEvent::MoveUpdate);
	network->Mapping(this, static_cast<unsigned char>(HOS_PROTOCOL::REQ_ID), &NetworkEvent::EventUpdate);
	network->Mapping(this, static_cast<unsigned char>(HOS_PROTOCOL::ACTIVE), &NetworkEvent::EventUpdate);
}

void NetworkEvent::MakeEventPacket(int id, int event_types, int result)
{
	flatbuffers::FlatBufferBuilder builder;

	auto data = CreateEventProtocol(builder, id, event_types, result);
	builder.Finish(data);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned char>(event_types);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());

	switch (event_types)
	{
	case hos::Packet::ACTIVE:
		for (auto user : users)
		{
			user->m_GameObject->GetComponent<Networkcomponent>()->SendMsg(pack);
		}
		break;
	case hos::Packet::REQ_ID:
		users[id]->m_GameObject->GetComponent<Networkcomponent>()->SendMsg(pack);
		break;
	default:
		break;
	}
	builder.Clear();
}

void NetworkEvent::MakeActiveUserPosition()
{
	flatbuffers::FlatBufferBuilder builder;
	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = POSITION;
	std::vector<flatbuffers::Offset<PositionProtocol>> pos_vec;

	for (int i = 0; i < objcount; i++)
	{
		auto tr = users[i]->m_GameObject->GetComponent<Transform>();
		Vec3 pos(tr->GetPosition().x, tr->GetPosition().y, tr->GetPosition().z);
		Vec3 rot(tr->GetRotation().x, tr->GetRotation().y, tr->GetRotation().z);
		auto position = CreatePositionProtocol(builder, i, &pos, &rot, users[i]->m_GameObject->GetActive());
		pos_vec.push_back(position);
	}

	auto positions = builder.CreateVector(pos_vec);
	auto protocol = CreatePositionListProtocol(builder, positions);

	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());

	for (auto user : users)
	{
		user->m_GameObject->GetComponent<Networkcomponent>()->SendMsg(pack);
	}
}

void NetworkEvent::EventUpdate(PACKAGE& package)
{
	flatbuffers::FlatBufferBuilder builder;
	auto data = GetEventProtocol(package.packet.memory + PACKET_HEADER_SIZE);

	switch (data->event())
	{
	case hos::Packet::ACTIVE:
		MakeEventPacket(users[data->id()]->GetId(), ACTIVE, users[package.id]->GetId());
		users[data->id()]->m_GameObject->SetActive(true);
		break;
	case hos::Packet::REQ_ID:
		MakeEventPacket(users[package.id]->GetId(), REQ_ID, users[package.id]->GetId());
		break;
	}
}

void NetworkEvent::MoveUpdate(PACKAGE& package)
{
	auto move_protocol = GetMoveProtocol(package.packet.memory + PACKET_HEADER_SIZE);
	users[move_protocol->id()]->MoveUpdate(move_protocol);

	for (auto user : users)
	{
		user->m_GameObject->GetComponent<Networkcomponent>()->SendMsg(package.packet);
	}
}
