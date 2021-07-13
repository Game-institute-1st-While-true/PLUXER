#pragma once
#include "PacketDefine.h"

#include "hosEngine/Transform.h"
#include "Move.h"

using namespace hos;
using namespace com;
class User : public Component
{
	int uuid;
	bool certification;
	Transform* transform;
	Networkcomponent* network;
	PlayerBehaivor* move;
	bool Ready;

public:
	User();
	virtual ~User();

public:
	int GetId();
	void SetId(int id);
	void SetNetwork(Networkcomponent* network);
	void SetReady(bool btn) { Ready = btn; };
	bool GetReady() { return Ready; };
	Networkcomponent* GetNetwork() { return network; }
	Transform* GetTransform() { return transform; }	
	void PositionUdpate(Vector3 pos, Rotator rot);
	void MoveUpdate(const hos::Packet::MoveProtocol* protocol);
	bool IsCertification() { return certification; }
	bool IsReady() { return Ready; }

public:
	void Awake() override;
	void Start() override;
	void Reset() {};
	void OnEnable()override;
	void OnDisable()override;

private:
	void MakeMovePacket(hos::Packet::MOVE_DIR dir_toggle, bool dash);
};

