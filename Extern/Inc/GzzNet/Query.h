#pragma once
#include "DBDefine.h"
#include "QueryCommand.h"
#include "QueryRecord.h"

namespace gzz
{
	class Query
	{
	protected:
		QueryCommand* Cmd;
		QueryRecord Rec;

	public:
		Query();
		virtual ~Query();

	public:
		void SetRecord(QueryRecord record) { Rec = record; }
		QueryRecord& GetRecord() { return Rec; }
		void SetCommand(QueryCommand* command) { Cmd = command; }
		QueryCommand* GetCommand() { return Cmd; }
	};
}