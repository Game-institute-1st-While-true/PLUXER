#pragma once
#include <Windows.h>
#include "IPaper.h"

namespace gzz
{
	class PDebug : public IPaper
	{
	public:
		PDebug() = default;
		virtual ~PDebug() = default;

	public:
		virtual void Print(std::wstring text)
		{
			std::wstring str = L"";

			for (int i = 0; i < text.size(); i++)
			{
				if (text[i] != L'\0')
					str += text[i];
			}
			str += L'\n';
			OutputDebugString(str.c_str());
		}
	};
}