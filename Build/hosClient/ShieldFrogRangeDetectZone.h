#pragma once
#include "hosEngine/Com_Client.h"


class ShieldFrogRangeDetectZone : public hos::com::Script
{
private:
	bool IsPlayerDetect;

public:
	ShieldFrogRangeDetectZone();
	virtual ~ShieldFrogRangeDetectZone();

public:
	void Reset() override;
	void Awake() override;
	void OnTriggerEnter(hos::com::Collision* other) override;
	void OnTriggerExit(hos::com::Collision* other) override;

public:
	bool GetIsPlayerDetect() const { return IsPlayerDetect; };
};

