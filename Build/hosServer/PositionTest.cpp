#define WIN32_LEAN_AND_MEAN

#include "PositionTest.h"
#include "hosEngine/Transform.h"
#include "hosEngine/Networkcomponent.h"
#include "hosEngine/PacketDefine.h"

using namespace hos;
using namespace com;

PositionTest::PositionTest(bool ishost, int netid)
	:IsHost(ishost), NetId(netid), move(nullptr), network(nullptr), transfrom(nullptr)
{
}

void PositionTest::Awake()
{
	transfrom = m_GameObject->GetComponent<Transform>();
	network = m_GameObject->GetComponent<Networkcomponent>();
	move = m_GameObject->GetComponent<Move>();
	network->GetToken(NetId);

	if (IsHost == false)
	{
		network->Connect(L"127.0.0.1", 9000);
		network->Mapping(this, static_cast<unsigned char>(HOS_PROTOCOL::POSITION), &PositionTest::PositionUpdate);
	}
	else
		network->Mapping(this, static_cast<unsigned char>(HOS_PROTOCOL::MOVE), &PositionTest::MoveInputUpdate);
}

void PositionTest::Reset()
{
}

void PositionTest::Update()
{
	float force = 10;
	if (GetAsyncKeyState('W'))
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_N) == false)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_N, true);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_N, true);
		}
	}
	else
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_N) == true)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_N, false);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_N, false);
		}
	}

	if (GetAsyncKeyState('S'))
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_S) == false)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_S, true);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_S, true);
		}
	}
	else
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_S) == true)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_S, false);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_S, false);
		}
	}

	if (GetAsyncKeyState('A'))
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_E) == false)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_E, true);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_E, true);
		}
	}
	else
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_E) == true)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_E, false);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_E, false);
		}
	}

	if (GetAsyncKeyState('D'))
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_W) == false)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_W, true);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_W, true);
		}
	}
	else
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_W) == true)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_W, false);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_W, false);
		}
	}
	if (GetAsyncKeyState('Q'))
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_ROT_L) == false)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_ROT_L, true);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_ROT_L, true);
		}
	}
	else
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_ROT_L) == true)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_ROT_L, false);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_ROT_L, false);
		}
	}
	if (GetAsyncKeyState('E'))
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_ROT_R) == false)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_ROT_R, true);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_ROT_R, true);
		}
	}
	else
	{
		if (move->IsDir(MOVE_DIR::MOVE_DIR_ROT_R) == true)
		{
			move->SetDir(MOVE_DIR::MOVE_DIR_ROT_R, false);
			MakeMovePacket(MOVE_DIR::MOVE_DIR_ROT_R, false);
		}
	}
}

void PositionTest::LateUpdate()
{
	if (IsHost)
	{
		position_sync_time += Time->DeltaTime();
		if (position_sync_time >= POSSYNCTIME)
		{
			Note::GetIns()->Write(L"net", __FUNCTIONW__, __LINE__, L"POSUPDATE!!", 1);
			position_sync_time = 0; 
			MakePositionPacket();
			
		}
	}
}

void PositionTest::MakeMovePacket(hos::Packet::MOVE_DIR dir_toggle, bool dash)
{
	if (IsHost) return;

	Transform* trans = m_GameObject->GetComponent<Transform>();
	Networkcomponent* net = m_GameObject->GetComponent<Networkcomponent>();
	flatbuffers::FlatBufferBuilder builder;

	Vec3 foward(trans->GetForward().x, trans->GetForward().y, trans->GetForward().z);
	Vec3 rotation(trans->GetRotation().x, trans->GetRotation().y, trans->GetRotation().z);
	auto data = CreateMoveProtocol(builder, &foward, &rotation, dir_toggle, dash);
	builder.Finish(data);

	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned char>(HOS_PROTOCOL::MOVE);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	net->SendMsg(pack);
	builder.Clear();
}

void PositionTest::MakePositionPacket()
{
	if (!IsHost) return;

	flatbuffers::FlatBufferBuilder builder;

	Vector3 my_pos = transfrom->GetPosition();
	Vector3 my_rot = transfrom->GetRotation();

	Vec3 pos(my_pos.x, my_pos.y, my_pos.z);
	Vec3 rot(my_rot.x, my_rot.y, my_rot.z);

	auto data = CreatePositionProtocol(builder, &pos, &rot);
	builder.Finish(data);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned char>(HOS_PROTOCOL::POSITION);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	network->SendMsg(pack);
	builder.Clear();
}

void PositionTest::PositionUpdate(PACKAGE& package)
{
	flatbuffers::FlatBufferBuilder builder;

	auto position = GetPositionProtocol(package.packet.memory + PACKET_HEADER_SIZE);
	Vector3 pos(position->position()->x(), position->position()->y(), position->position()->z());
	transfrom->SetPosition(pos);

	Vector3 rot(position->rotation()->x(), position->rotation()->y(), position->rotation()->z());
	transfrom->SetRotation(rot);
}

void PositionTest::MoveInputUpdate(PACKAGE& package)
{
	flatbuffers::FlatBufferBuilder builder;

	auto move_protocol = GetMoveProtocol(package.packet.memory + PACKET_HEADER_SIZE);

	Vector3 foward(move_protocol->foward()->x(), move_protocol->foward()->y(), move_protocol->foward()->z());
	Vector3 rotation(move_protocol->rotation()->x(), move_protocol->rotation()->y(), move_protocol->rotation()->z());

	if (move->IsDir(move_protocol->direction()) == true)
	{
		move->SetMoveData(move_protocol->direction(), false, move_protocol->dash(), foward, rotation);
	}
	else
	{
		move->SetMoveData(move_protocol->direction(), true, move_protocol->dash(), foward, rotation);
	}
}
