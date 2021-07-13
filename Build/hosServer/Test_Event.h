#pragma once
#include "hosEngine/Bundle_Com_System.h"

class Test_Event : public hos::com::Component
{
	Component* com;
	Component* t;
	float Counter = 0;
public:
	void Awake()override;
	void Reset()override;
	void Update()override;
};