#pragma once
#include <iostream>
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF", "EndOfFile")

static constexpr int DB_PARAM_SIZE = 1024;

typedef ADODB::_CommandPtr  	CmdPtr;
typedef ADODB::_ConnectionPtr	ConnPtr;
typedef ADODB::_RecordsetPtr	RecPtr;

