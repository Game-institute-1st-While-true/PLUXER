#ifndef _DUMP_H
#define _DUMP_H
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <Windows.h>
#include <DbgHelp.h>

class Dump
{
private:
	static LPCTSTR AppName;
	static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS* pException);

public:
	Dump(LPCTSTR name);
};

#endif // !_DUMP_H