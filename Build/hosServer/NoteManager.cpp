#include "NoteManager.h"

NoteManager::NoteManager() : hos::com::Script(L"NoteManager"), IsBGMStart(false)
{
	m_NoteInfo = new NoteInfo();
}

NoteManager::~NoteManager()
{
	m_NoteInfo->RemoveNoteData();

	delete m_NoteInfo;
	m_NoteInfo = nullptr;

	delete[] ViewNoteIndex;
	delete[] IsViewNoteIndexEnd;
}

void NoteManager::Reset()
{

}

void NoteManager::Awake()
{
	/// 하나부터 하자

	// 노트 데이터를 불러와야 함
	m_NoteInfo->Load(g_Path + L"\\Assets\\Data\\BossZone.bnd");

	InputDelayScale = 0.25f;

	InputDelayRange = m_NoteInfo->GetNoteList()[0] * InputDelayScale;

	OneBeatTime = m_NoteInfo->GetNoteList()[0];

	HalfBeatTime = OneBeatTime * 0.5f;

}

void NoteManager::Start()
{
}

void NoteManager::Update()
{
	NoteStateUpdate();
}

void NoteManager::NoteStateUpdate()
{
	if (IsBGMStart)
	{
		StartNote();
	}
	else
	{
		StopNote();
	}
}

void NoteManager::MusicStart()
{
	IsBGMStart = true;
}

void NoteManager::StartNote()
{
	double delta = m_GameObject->m_Scene->GetTimer()->DeltaTime();

	NowBGMTime += delta;
	IsChangeNote = false;

	if (NowBGMTime > m_NoteInfo->m_NoteData.SoundLength)
	{
		NowBGMTime = 0.0f;
		NowNote = 0;
		LoopCount++;
	}

	if (NowBGMTime > m_NoteInfo->GetNoteList()[NowNote])
	{
		NowNote++;
	}

	if (NowBGMTime > m_NoteInfo->GetNoteList()[PrevNote] + m_NoteInfo->m_NoteData.NoteInputTime
		|| NowNote < PrevNote)
	{
		PrevNote = NowNote;
		IsChangeNote = true;

		if (IsHalfBeat)
		{
			IsHalfBeat = !IsHalfBeat;
		}
		else
		{
			if (ErrorCheck == NoteErrorCheck::Success && IsInputSuccess)
			{
				IsInputSuccess = false;
			}
			else
			{
				ErrorRangeCheck = NoteErrorRangeCheck::W;
				ErrorCheck = NoteErrorCheck::Wait;
				IsInputSuccess = false;
			}
		}
	}
	IsPressButton = false;
}

void NoteManager::StopNote()
{
	NowBGMTime = 0.0f;
	NowNote = 0;
	PrevNote = 0;
	ErrorRange = 0.0f;
	ErrorCheck = NoteErrorCheck::Wait;
	ErrorRangeCheck = NoteErrorRangeCheck::W;
	IsPressButton = false;
	IsChangeNote = false;
	IsInputSuccess = false;
	IsHalfBeat = false;
	LoopCount = 0;
}