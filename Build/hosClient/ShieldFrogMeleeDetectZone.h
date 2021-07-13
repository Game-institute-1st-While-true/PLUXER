#pragma once
#include "hosEngine/Com_Client.h"


class ShieldFrogMeleeDetectZone : public hos::com::Script
{
private:
	bool IsPlayerDetect;

public:
	ShieldFrogMeleeDetectZone();
	virtual ~ShieldFrogMeleeDetectZone();

public:
	void Reset() override;
	void Awake() override;
	void OnTriggerEnter(hos::com::Collision* other) override;
	void OnTriggerExit(hos::com::Collision* other) override;

public:
	bool GetIsPlayerDetect() const { return IsPlayerDetect; };

};

