#pragma once
#include "GroupSystem.h"
#include "NoteInfo.h"

class NoteManager : public hos::com::Script
{
public:
	enum class NoteErrorCheck
	{
		Wait, Success, Fail
	};

	enum class NoteErrorRangeCheck
	{
		CP, P, C, N, M, W		// Critical Perfect, Perfect, Cool, Nice, Miss, Wait
	};

private:
	NoteInfo* m_NoteInfo = nullptr;
	NoteErrorCheck ErrorCheck = NoteErrorCheck::Wait;
	NoteErrorRangeCheck ErrorRangeCheck = NoteErrorRangeCheck::W;

	double NowBGMTime = 0.0f;
	double ErrorRange = 0.0f;

	double OneBeatTime = 0.0f;
	double HalfBeatTime = 0.0f;
	double InputDelayRange = 0.f;
	double InputDelayScale = 0.25f;

	int NowNote = 0;
	int PrevNote = 0;

	bool IsBGMStart = false;
	bool IsInputSuccess = false;
	bool IsPressButton = false;
	bool IsChangeNote = false;
	bool IsHalfBeat = false;

	int* ViewNoteIndex;

	bool* IsViewNoteIndexEnd;

	int LoopCount = 0;

public:
	NoteManager();
	virtual ~NoteManager();

private:
	void NoteStateUpdate();
	void StartNote();
	void StopNote();

public:
	NoteInfo* GetNoteInfo() const { return m_NoteInfo; };
	bool GetIsBGMStart() const { return IsBGMStart; };
	double GetOneBeatTime() const { return OneBeatTime; };
	double GetInputDelayRange() const { return InputDelayRange; };
	int GetNowNote() const { return NowNote; };


public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void MusicStart();
	void Update() override;
};

