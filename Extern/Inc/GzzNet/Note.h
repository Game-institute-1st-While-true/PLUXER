#pragma once
#include "Singleton.h"
#include <iostream>	
#include <sstream>

#include <string>
#include <string_view>

#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "IPaper.h"

// [2020/10/18 김효곤]
// 로거 템플릿, 싱글턴을 상속받습니다.
// Visual Studio -> 출력창에 필요 데이터를 그려줍니다.
// 사용 키워드에 따라 다르게 표현할 수 있습니다.

// [2020/10/26 김효곤]
// 싱글턴, 키워드 부분떼고 다시만듭니다.
// 편하게 확인하기 위해 Utily.lib 에서도 뗍니다.

// [2020/11/06 김효곤]
// 싱글턴 매크로에서 버그 확인되어 싱글턴 추후 추가 예정. -- > [2020/11/08 김효곤] 확인하고 처리함.
// IPaper클래스를 상속받아서 로그를 써내려 가도록 수정.

namespace gzz
{
	constexpr int MAX_PAPER_SIZE = 100;
	class Note
	{
		SINGLETON_CLASS(Note);

	private:
		std::vector<IPaper*> Exercisebook;
		int LogNumber = 0;

	public:
		void AddPage(IPaper* Item)
		{
			Exercisebook.push_back(Item);
		};

		void StartStamp()
		{
			Write(L"note : ", L"Logger Start !!");
		}

		void ReleasePage()
		{
			Write(L"note : ", L"Logger End !!");
			if (Exercisebook.empty() == false)
			{
				for (unsigned int i = 0; i < Exercisebook.size(); i++)
				{
					if (Exercisebook[i] != nullptr)
					{
						delete Exercisebook[i];
					}
				}
			}
		}

		template<typename ... Args>
		void Write(std::wstring keyWord, Args...);
		inline std::wstring Clock(const wchar_t* format);

	private:
		template<typename T>
		void MakeWord(std::wstringstream& sstream, T word);

		template<typename ... Args>
		void Print(std::wstring keyWord, std::wstring text);
	};

	inline Note::Note()
	{
	}

	inline Note::~Note()
	{
		ReleasePage();
	}

	template<typename T>
	void Note::MakeWord(std::wstringstream& sstream, T word)
	{
		sstream << word;
	}

	template<typename ...Args>
	void Note::Print(std::wstring keyWord, std::wstring text)
	{
		if (Exercisebook.empty() == true)
		{
			std::wcout << L"There is nowhere to write." << std::endl;
		}

		for (auto page : Exercisebook)
		{
			page->Print(text);
		}
	}

	template<typename ...Args>
	void Note::Write(std::wstring keyWord, Args... message)
	{
		std::wstringstream text;
		text << LogNumber++ << L" ## " << keyWord << L">> " << Clock(L"D%Y-%m-%dT%H:%M:%S") << "  ";
		(MakeWord(text, message), ...);

		std::wstring text_ = text.str();
		text_.erase(std::remove(text_.begin(), text_.end(), L'\0'));
		text_[text_.size() - 1] = L'\0';
		Print(keyWord, text_);
	}

	inline std::wstring Note::Clock(const wchar_t* format)
	{
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		tm _tm;
		localtime_s(&_tm, &now);

		std::wstring result(19, L'\0');
		wcsftime(&result[0], result.size(), format, &_tm);
		return result;
	}
}