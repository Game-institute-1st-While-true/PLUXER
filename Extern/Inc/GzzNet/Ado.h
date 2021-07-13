#pragma once
#include "IDataBase.h"
#include "DBDefine.h"
#include "Query.h"

#include <comutil.h>
#include <thread>
#include <atomic>

namespace gzz
{
	class Ado : public IDataBase
	{
		friend class DBManager;

		static constexpr long DEFAULT_TIMEOUT = 30;
	protected:
		ConnPtr	Connection;
		std::wstring Name;
		std::thread Woker;
		std::atomic<bool> IsRun;

	public:
		Ado() = delete;
		Ado(int id, DBManager* manager);
		virtual ~Ado();

	public:
		virtual bool Connect() override;
		virtual bool IsConnect() override;
		virtual bool Disconnect() override;
		virtual void StartThread() override;
		virtual void DestroyThread() override;
		virtual void Process() override;

		virtual void Run() override;

	public:
		void ErrorReport(const wchar_t* name, _com_error& e);
		HRESULT SetConnectTimeOut(long second);
		void Excute();
	};
}
