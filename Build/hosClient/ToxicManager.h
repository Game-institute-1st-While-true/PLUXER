#pragma once
#include "hosEngine/Com_Client.h"
#include "ToxicFloor.h"

class ToxicManager : public hos::com::Script
{

private:
	std::vector<ToxicFloor*> ToxicFloorList;

public:
	ToxicManager();
	virtual ~ToxicManager();

public:
	void Reset() override;
	void Awake() override;
	void Update() override;

public:
	void SetToxicFloorPosition(hos::Vector3 pos);
	void ActiveToxicFloorPosition(int index, hos::Vector3 pos, bool btn);
};

