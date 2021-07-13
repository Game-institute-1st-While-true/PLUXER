#ifndef _LOGGER_H
#define _LOGGER_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <mutex>
#include <fstream>

#include "BaseTypes.h"
#include "Singleton.h"

// [2020/11/16 노종원]
// 로그를 찍어주는 로거
// 콘솔, 파일 아웃풋에 로깅할 수 있도록

// LOG
// [2020/12/31] 노종원 : ut네임스페이스 추가

// LOG
// [2020/01/20] 김효곤 : MsgConsole 추가

// LOG
// [2020/02/03] 김효곤 : logger mutex 추가

// LOG
// [2020/02/05] 김효곤 : Write 함수 추가

namespace hos::ut
{
	class Logger
	{
		SINGLETON_CLASS(Logger)
	public:
		inline static string TIME_FORMAT = L"[%Y-%m-%d %X]";
		inline static string FILE_NAME_TIME_FORMAT = L"[%F %H%M%S]";
		constexpr static int TIME_STRING_LENGTH = 64;
	private:
		DebugConsole Console;
		std::wfstream File;
		std::mutex LogLock;
	private:
		void FileOpen()
		{
			if (!File.is_open())
			{
				using namespace std::chrono;

				time_t time = system_clock::to_time_t(std::chrono::system_clock::now());
				struct tm t;
				gmtime_s(&t, &time);
				wchar_t timeString[TIME_STRING_LENGTH];
				wcsftime(timeString, TIME_STRING_LENGTH, FILE_NAME_TIME_FORMAT.c_str(), &t);

				string filename(timeString);
				filename.append(L".txt");
				File.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::trunc);
			}
		}
	public:
		void   OutSideLog(string_view log)
		{
			std::lock_guard<std::mutex> lock(LogLock);
			Console << log;
			Console << L"\n";
		}
		void   ConsoleReset()
		{
			Console.Reset();
		}

		string Timestamp() const
		{
			using namespace std::chrono;

			time_t time = system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm t;
			gmtime_s(&t, &time);
			wchar_t timeString[TIME_STRING_LENGTH];
			wcsftime(timeString, TIME_STRING_LENGTH, TIME_FORMAT.c_str(), &t);

			return timeString;
		}

		template<typename T>
		void MakeWord(std::wstringstream& sstream, T word)
		{
			sstream << word;
		}

		template<typename ...Args>
		void Write(string_view catagory, string funcsig, string line, Args... message)
		{
			std::wstringstream text;
			string log = funcsig + L" : " + line + L" <";
			log.append(catagory);
			log.append(L"> ");
			(MakeWord(text, message), ...);
			log.append(text.str());
			log.append(L"\n");

			std::lock_guard<std::mutex> lock(LogLock);
			Console << log;
		}

		void LogOption(string_view catagory, string_view comment, string funcsig, string line, bool isConsole = false, bool isFile = false, bool isMsgBox = false)
		{
			string time = Timestamp();
			string log = funcsig + L" : " + line + L" <";
			log.append(catagory);
			log.append(L"> ");
			log.append(comment);

			if (isMsgBox)
			{
				MessageBox(NULL, log.c_str(), time.c_str(), MB_OK);
			}
			log = time + log + L"\n";
			OutputDebugStringW((funcsig + L" (" + line + L") : " + catagory.data() + L" : " + comment.data() + L"\n").c_str());

			// TODO : 파일에 안써진다
			if (isFile)
			{
				FileOpen();
				//File.write(log.c_str(), log.length());
				//File << log << std::endl;
			}
			if (isConsole)
			{
				std::lock_guard<std::mutex> lock(LogLock);
				Console << log;
			}
		}
	#define Log(catagory, comment) LogOption(catagory, comment, TEXT(__FILE__), std::to_wstring(__LINE__));
	#define LogConsole(catagory, comment) LogOption(catagory, comment, TEXT(__FILE__), std::to_wstring(__LINE__), true);
	#define MsgConsole(catagory, comment) LogOption(catagory, comment, TEXT(""), TEXT(""), true);
	#define LogFile(catagory, comment) LogOption(catagory, comment, TEXT(__FILE__), std::to_wstring(__LINE__), false, true);
	#define LogMsgBox(catagory, comment) LogOption(catagory, comment, TEXT(__FILE__), std::to_wstring(__LINE__), false, false, true);
	#define LogAll(catagory, comment) LogOption(catagory, comment, TEXT(__FILE__), std::to_wstring(__LINE__), true, true, true);

		template<typename ...T>
		void LogData(string_view catagory, T... args)
		{
			string str = Timestamp() + L" <";
			str.append(catagory);
			str.append(L"> ");
			OutputDebugStringW(str.c_str());

			(OutputDebugStringW((std::to_wstring(args) + L" ").c_str()), ...);

			OutputDebugStringW(L"\n");
		}
	};

	__forceinline Logger::Logger()
	{
	}

	__forceinline Logger::~Logger()
	{
		if (File.is_open())
		{
			File.close();
		}
	}
}
#endif // !_LOGGER_H