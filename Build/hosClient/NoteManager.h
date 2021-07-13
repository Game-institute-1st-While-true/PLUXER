#pragma once
#include "hosEngine/COM_Client.h"
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

	enum class BGMList
	{
		PeaceZone, CombatZone, BossZone, MAX
	};

private:
	std::vector<NoteInfo*> BGMNoteInfo;

	NoteErrorCheck ErrorCheck = NoteErrorCheck::Wait;

	NoteErrorRangeCheck ErrorRangeCheck = NoteErrorRangeCheck::W;

	double NowBGMTime = 0.0f;
	double ErrorRange = 0.0f;

	double OneBeatTime = 0.0f;
	double HalfBeatTime = 0.0f;
	double InputDelayRange = 0.f;
	double InputDelayScale = 0.75f;

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

	int NowBGM = 0;

	bool IsMiss = false;
	bool IsSuccess = false;

public:
	hos::com::AudioSource* m_AudioSource = nullptr;
	hos::com::AudioSource* SFXAudioSource = nullptr;
	hos::com::Transform* CenterPosition = nullptr;

	std::vector<hos::com::Transform*> RightNoteUI;
	std::vector<hos::com::Transform*> LeftNoteUI;
	
	hos::com::MeshRenderer* CenterUIMeshRenderer = nullptr;


public:
	NoteManager();
	virtual ~NoteManager();

private:
	void NoteStateUpdate();
	void StartNote();
	void StopNote();
	void MoveNoteUI();
	float GetNoteLerpPosition(int noteIndex, bool isLeft);
	void ChangeCenterUI();

public:
	NoteErrorRangeCheck CheckUserInput();
	bool GetIsPressButton() const { return IsPressButton; };
	bool GetIsInputSuccess() const { return IsInputSuccess; };
	bool GetIsChangeNote() const { return IsChangeNote; };
	bool GetIsBGMStart() const { return IsBGMStart; };
	double GetOneBeatTime() const { return OneBeatTime; };
	double GetInputDelayRange() const { return InputDelayRange; };
	int GetNowNote() const { return NowNote; };
	BGMList GetNowBGM() const { return (BGMList)NowBGM; };
	void ChangeBGM(NoteManager::BGMList val);

public:
	void Reset() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void MusicStart();
	void Render() override;

};

