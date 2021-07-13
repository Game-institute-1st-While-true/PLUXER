#pragma once

#include <Windows.h>
#include <string>
#include <fstream>

//#include <list>
//////////////////////////////////////////////////////////////////////////
// [2021/01/07 박용의]
// 
// 곡에 대한 정보를 저장하기 위한 데이터
// 
//////////////////////////////////////////////////////////////////////////

struct NoteData
{
	double BPM = 0;

	float Volume = 1.0f;

	// 노트 입력 시간 (노트 시간에 맞춰서 눌렀나, 정밀도)
	double NoteInputTime = 0.0f;

	// 음악 길이
	double SoundLength = 0.0f;

	// 전체 노트 수
	int NoteCount = 0;

	// 표시 할 노트 수
	int ViewNoteCount = 1;

	// 노트 UI 간격
	float NoteUIDistance = 10.0f;

};

class NoteInfo
{
private:
	std::wstring FileName;

	//int BPM = 0;

	//float Volume = 0.0f;
	
	// 노트 입력 시간 (노트 시간에 맞춰서 눌렀나, 정밀도)
	//double NoteInputTime = 0.0f;

	// 노트 데이터 (기존 노트 중간에 새로운 노트가 추가 될 수 있기에 리스트를 사용 함)
	//std::list<double> NoteList;
	double* NoteList;

	// 음악 길이
	//double SoundLength = 0.0f;

	// 전체 노트 수
	//int NoteCount = 0;

public:
	NoteData m_NoteData;

	// bgm 의 전체 시간?

public:
	NoteInfo();
	virtual ~NoteInfo();


	void RemoveNoteData();
	void CreateNoteData();

	void Save(std::wstring filePath);
	void Load(std::wstring filePath);

	std::wstring_view GetFileName() const { return FileName; }
	void SetFileName(std::wstring_view val) { FileName = val; }
	//int GetBPM() const { return BPM; }
	//void SetBPM(int val) { BPM = val; }
	//float GetVolume() const { return Volume; }
	//void SetVolume(float val) { Volume = val; }
	//double GetNoteInputTime() const { return NoteInputTime; }
	//void SetNoteInputTime(double val) { NoteInputTime = val; }
	//double GetSoundLength() const { return SoundLength; }
	//void SetSoundLength(double val) { SoundLength = val; }
	//int GetNoteCount() const { return NoteCount; }
	//void SetNoteCount(int val) { NoteCount = val; }
	double* GetNoteList() const { return NoteList; }
	void SetNote(int notePos, double val) { NoteList[notePos] = val; };
	double GetNote(int index) const { return NoteList[index]; };

};

