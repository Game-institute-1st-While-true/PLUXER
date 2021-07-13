#pragma once
#include <afxwin.h>
#include<locale.h>

#include "Note.h"

namespace gzz
{
	class PListBox : public IPaper
	{
		CListBox* Paper;
		bool	  IsScroll = false;
	public:
		PListBox(CListBox* paper) { Paper = paper; };
		virtual ~PListBox() = default;

	public:
		virtual void Print(std::wstring text)
		{
			setlocale(LC_ALL, "");
			CString str = L"";
			for (int i = 0; i < text.size(); i++)
			{
				if (text[i] != L'\0')
					str += text[i];
			}
			Paper->AddString(str);
			Paper->SetTopIndex(Paper->GetCount() - 1);
		}
	public:
		CListBox* GetListBox() { return Paper; };
		void	  OnScroll() { IsScroll = true; }
		void	  OffScroll() { IsScroll = false; }
	};
}