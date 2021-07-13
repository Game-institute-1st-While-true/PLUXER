#include "User.h"

User::User()
	:uuid(-1), Ready(false), transform(nullptr), move(nullptr), network(nullptr)
{
}

User::~User()
{
}

void User::SetId(int id)
{
	uuid = id;
}

int User::GetId()
{
	return uuid;
}

void User::PositionUdpate(Vector3 pos, Rotator rot)
{
	transform->SetPosition(pos);
	transform->SetRotation(rot);
}

void User::SetNetwork(Networkcomponent* network)
{
	this->network = network;
}

void User::MakeMovePacket(hos::Packet::MOVE_DIR dir_toggle, bool dash)
{
	if (network == nullptr) return;

	flatbuffers::FlatBufferBuilder builder;

	Vec3 foward(transform->GetForward().x, transform->GetForward().y, transform->GetForward().z);
	Vec3 rotation(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z);
	auto data = CreateMoveProtocol(builder, uuid, &foward, &rotation, dir_toggle, dash);
	builder.Finish(data);

	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned char>(HOS_PROTOCOL::MOVE);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	network->SendMsg(pack);
	builder.Clear();
}

void User::MoveUpdate(const hos::Packet::MoveProtocol* move_protocol)
{
	Vector3 foward(move_protocol->foward()->x(), move_protocol->foward()->y(), move_protocol->foward()->z());
	Rotator rotation(move_protocol->rotation()->x(), move_protocol->rotation()->y(), move_protocol->rotation()->z());

	if (move->IsDir(move_protocol->direction()) == true)
	{
		move->SetMoveData(move_protocol->direction(), false, move_protocol->dash(), foward, rotation);
	}
	else
	{
		move->SetMoveData(move_protocol->direction(), true, move_protocol->dash(), foward, rotation);
	}
}

void User::Awake()
{
	network = m_GameObject->GetComponent<Networkcomponent>();
	if (network == nullptr)
	{
		Logger::GetIns()->LogConsole(L"data", L"network component is null");
	}
	transform = m_GameObject->GetComponent<Transform>();
	if (transform == nullptr)
	{
		Logger::GetIns()->LogConsole(L"data", L"transform component is null");
	}
	move = m_GameObject->GetComponent<PlayerBehaivor>();
	if (move == nullptr)
	{
		Logger::GetIns()->LogConsole(L"data", L"move component is null");
	}

	m_GameObject->GetComponent<Transform>()->Reset();
	m_GameObject->GetComponent<Transform>()->SetPosition(Vector3(uuid * -2, 0, 0));
	Ready = false;
}

void User::Start()
{

}

void User::OnEnable()
{
}

void User::OnDisable()
{
}

