#ifndef _TEST_SYSTEM_H
#define _TEST_SYSTEM_H

// [2021/04/08 ������]
// Ŭ���̾�Ʈ �׽�Ʈ�� ��ũ��Ʈ

// LOG

#include "hosEngine\Com_Client.h"

class TestSystem : public hos::com::Script
{
public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
};

#endif // !_TEST_SYSTEM_H