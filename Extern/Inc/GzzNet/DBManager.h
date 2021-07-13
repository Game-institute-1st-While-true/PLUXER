#pragma once
#include <vector>

#include "DBDefine.h"
#include "JobQueue.h"

namespace gzz
{
	class Query;
	class Ado;
	class IDataBase;

	class DBManager
	{
	private:
		JobQueue<Query*> QueryJobQueue;
		std::vector<IDataBase*> DbPool;
		int DBCount;
		std::string ConnectString;
		
	public:
		DBManager();
		~DBManager();

	public:
		void Init(int count, std::string constr);
		void Release();
		void Run();

		int	GetDbCount() { return DBCount; }
		std::string GetString() { return ConnectString; }

	public:
		bool AddQuery(Query* query);
		bool PopQuery(Query** query);
		int QueryCount();
	};
}
