#pragma once
#include "hosEngine/Com_Client.h"
#include "NoteManager.h"
#include "ShieldFrogState.h"
#include "BTNode.h"

class ShieldFrogIdle;
class ShieldFrogDamaged;
class ShieldFrogDead;
class ShieldFrogDetect;

class ShieldFrogAction : public hos::com::Script
{
public:
	NoteManager* m_NoteManager;
	ShieldFrogState* m_ShieldFrogState;
	hos::com::NavAgent* m_NavAgent;
	hos::com::Transform* PlayerTransform;
	hos::com::Transform* ShieldFrogTransform;

	BTRoot* RootNode;
	BTSelector* StartSelector;
	ShieldFrogIdle* NodeIdle;
	ShieldFrogDamaged* NodeDamaged;
	ShieldFrogDead* NodeDead;

public:
	ShieldFrogAction();
	virtual ~ShieldFrogAction();

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void Render() override;

};

class ShieldFrogIdle : public BTNode
{
public:
	ShieldFrogIdle(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~ShieldFrogIdle() {};

public:
	virtual SearchResult Invoke() override;
};

class ShieldFrogDamaged : public BTNode
{
public:
	ShieldFrogDamaged(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~ShieldFrogDamaged() {};

public:
	virtual SearchResult Invoke() override;
};

class ShieldFrogDead : public BTNode
{
private:
	float DeadTimer = 0.0f;

public:
	ShieldFrogDead(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~ShieldFrogDead() {};

public:
	virtual SearchResult Invoke() override;
};

class ShieldFrogMeleeDetect : public BTNode
{
public:
	ShieldFrogMeleeDetect(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~ShieldFrogMeleeDetect() {};

public:
	virtual SearchResult Invoke() override;
};

class ShieldFrogMeleeAttack : public BTNode
{
public:
	ShieldFrogMeleeAttack(hos::com::Script* blackBoard) : BTNode(blackBoard) {};
	virtual ~ShieldFrogMeleeAttack(){};

public:
	virtual SearchResult Invoke() override;
};