#pragma once
#include <iostream>
#include <fstream>	
#include <chrono>
#include <iomanip>
#include <Windows.h>
#include"IPaper.h"

namespace gzz
{
	const wchar_t* FILE_NAME_FORMAT = L"D%Y-%m-%d";

	class PFile : public IPaper
	{
	private:
		std::wfstream out;

	public:
		PFile()
		{	
			// 오늘 날짜를 기준으로 파일 생성.
			std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			tm _tm;

			localtime_s(&_tm, &now);
			wchar_t time_stamp[20];
			ZeroMemory(time_stamp, sizeof(time_stamp));
			wcsftime(&time_stamp[0], 20, FILE_NAME_FORMAT, &_tm);

			std::wstring filename;
			filename += time_stamp;
			filename += L".txt";
			out.open(filename, std::ios::app);
		}
		virtual ~PFile()
		{
			out.close();
		}

	public:
		virtual void Print(std::wstring text)
		{
			out << text;
			out << std::endl;
		}
	private:
	};
}