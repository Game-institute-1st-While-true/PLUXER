#include "NoteInfo.h"

NoteInfo::NoteInfo()
{

}

NoteInfo::~NoteInfo()
{

}

void NoteInfo::RemoveNoteData()
{
	if (m_NoteData.NoteCount > 0)
	{
		if (NoteList)
		{
			delete[] NoteList;

			NoteList = nullptr;
		}
	}
}

void NoteInfo::CreateNoteData()
{
	if (!NoteList)
	{
		if (m_NoteData.NoteCount > 0)
		{
			NoteList = new double[m_NoteData.NoteCount];
		}
	}
}

void NoteInfo::Save(std::wstring filePath)
{
	HANDLE _file;
	DWORD val;

	_file = CreateFile(filePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(_file, (LPVOID)&m_NoteData, sizeof(NoteData), &val, NULL);

	for (int i = 0; i < m_NoteData.NoteCount; i++)
	{
		WriteFile(_file, &NoteList[i], sizeof(double), &val, NULL);
	}

	CloseHandle(_file);
}

void NoteInfo::Load(std::wstring filePath)
{
	HANDLE _file;
	DWORD val;

	_file = CreateFile(filePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (_file != INVALID_HANDLE_VALUE)
	{
		ReadFile(_file, (LPVOID)&m_NoteData, sizeof(NoteData), &val, NULL);

		RemoveNoteData();
		CreateNoteData();

		for (int i = 0; i < m_NoteData.NoteCount; i++)
		{
			ReadFile(_file, &NoteList[i], sizeof(double), &val, NULL);
		}

		CloseHandle(_file);
	}
}
