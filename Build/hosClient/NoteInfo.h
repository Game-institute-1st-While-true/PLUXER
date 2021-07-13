#pragma once

#include <Windows.h>
#include <string>
#include <fstream>

//#include <list>
//////////////////////////////////////////////////////////////////////////
// [2021/01/07 �ڿ���]
// 
// � ���� ������ �����ϱ� ���� ������
// 
//////////////////////////////////////////////////////////////////////////

struct NoteData
{
	double BPM = 0;

	float Volume = 1.0f;

	// ��Ʈ �Է� �ð� (��Ʈ �ð��� ���缭 ������, ���е�)
	double NoteInputTime = 0.0f;

	// ���� ����
	double SoundLength = 0.0f;

	// ��ü ��Ʈ ��
	int NoteCount = 0;

	// ǥ�� �� ��Ʈ ��
	int ViewNoteCount = 1;

	// ��Ʈ UI ����
	float NoteUIDistance = 10.0f;

};

class NoteInfo
{
private:
	std::wstring FileName;

	//int BPM = 0;

	//float Volume = 0.0f;
	
	// ��Ʈ �Է� �ð� (��Ʈ �ð��� ���缭 ������, ���е�)
	//double NoteInputTime = 0.0f;

	// ��Ʈ ������ (���� ��Ʈ �߰��� ���ο� ��Ʈ�� �߰� �� �� �ֱ⿡ ����Ʈ�� ��� ��)
	//std::list<double> NoteList;
	double* NoteList;

	// ���� ����
	//double SoundLength = 0.0f;

	// ��ü ��Ʈ ��
	//int NoteCount = 0;

public:
	NoteData m_NoteData;

	// bgm �� ��ü �ð�?

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

