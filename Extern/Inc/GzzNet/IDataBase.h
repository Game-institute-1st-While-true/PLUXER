#pragma once
#include <iostream>
#include "DBDefine.h"

namespace gzz
{
	class DBManager;
	class IDataBase
	{
	protected:
		enum DB_STATE
		{
			STOP,
			STANDBY,
			RUNNING
		};

		int Id;
		std::string ConnectString;
		DBManager* Manager;
		bool Connection;
		int State;

	public:
		IDataBase() = delete;
		IDataBase(int id, DBManager* manager) { Id = id; Connection = false; State = STOP; Manager = manager; }
		virtual ~IDataBase() = default;

	public:
		void SetConnectString(std::string name) { ConnectString = name; }
		std::string GetConnectString() { return ConnectString; }

		virtual bool Connect() = 0;
		virtual bool Disconnect() = 0;
		virtual bool IsConnect() = 0;
		virtual void Run() = 0;
		virtual void StartThread() = 0;
		virtual void DestroyThread() = 0;
		virtual void Process() = 0;
	};
}
