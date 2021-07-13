#pragma once

#include "hosEngine/COM_Client.h"
#include "EnemyManager.h"
#include "PlayerState.h"
#include "SceneManager.h"
#include "NoteManager.h"

class GameManager : public hos::com::Script
{
	// ÄÞº¸, Á¡¼ö, ·©Å©

private:
	bool IsGameEnd = false;

	float EndTime = 3.f;
	float EndTimer = 0.f;

public:
	EnemyManager* m_EnemyManager;
	PlayerState* m_PlayerState;
	NoteManager* m_NoteManager;

public:
	GameManager();
	virtual ~GameManager();

public:
	bool GetIsGameEnd() const { return IsGameEnd; };

public:
	void Awake()override;
	void Reset()override;
	void Start()override;
	void Update()override;
};