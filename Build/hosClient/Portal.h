#pragma once

#include "hosEngine/COM_Client.h"

class Portal: public hos::com::Script
{
private:
	bool IsPlayerIn = false;

public:
	Portal();

	void OnTriggerEnter(hos::com::Collision* other)override;

public:
	bool GetIsPlayerIn() const { return IsPlayerIn; };

};

