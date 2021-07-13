#pragma once
#include <iostream>
#include <mutex>
#include "IPaper.h"

namespace gzz
{
	class PConsole : public IPaper
	{
	private:
		std::mutex PrintMutex;
	public:
		PConsole() = default;
		virtual ~PConsole() = default;

	public:
		virtual void Print(std::wstring text)
		{
			std::lock_guard<std::mutex> lock(PrintMutex);
			std::wcout << text << std::endl;
		}
	};
}