#pragma once
#include <string>
#include <sstream>
#include "DBDefine.h"

// [2021-03-24 김효곤]
// 지정된 데이터베이스에 질의하는 클래스

static constexpr int MAX_PARAM = 100;
namespace gzz
{
	class QueryCommand
	{
		friend class Query;
		friend class Ado;
		friend class DBManager;

	public:
		enum class TYPE : unsigned char
		{
			RETURN,		// 리턴 있는 쿼리 올 때 까지 대기함 (adAsyncFetch : 앞의 행을 가져올 때 가지 대기 이후엔 비동기)
			CALL_BACK,	// 콜백 쿼리 (리턴을 예약한 쿼리, 대기 x) (adAsyncFetchNonBlocking)
			NO_RETURN,	// 리턴 없는 쿼리 (adExecuteNoRecords)
		};

	private:
		CmdPtr Cmd;
		std::wstring QueryText;
		int Type;
		int ParamCount;

	public:
		QueryCommand();
		~QueryCommand();

	public:
		void SetQuery(std::wstring query, int type);									// 직접 쿼리문을 설정합니다.
		std::wstring_view GetQuery() { return QueryText; }								// 쿼리문을 확인 합니다.

		template<typename ... Args>
		void Call_Procedure(TYPE type, std::wstring name, Args... message);				// 저장 프로시저를 위한 쿼리문을 생성합니다.

	private:
		template<typename T>
		void AddParam(T word, int& count);
	};

	template<typename ...Args>
	inline void QueryCommand::Call_Procedure(TYPE type, std::wstring name, Args... message)
	{
		Type = static_cast<int>(type);
		QueryText.clear();

		QueryText += L"CALL ";
		QueryText += name;
		QueryText += L"(";

		(AddParam(message, ParamCount), ...);
		if (ParamCount != 0)
		{
			QueryText.pop_back();
			QueryText.pop_back();
		}
		QueryText += L")";
	}

	template<typename T>
	inline void QueryCommand::AddParam(T param, int& count)
	{
		std::wstringstream text;
		text << param;
		text << ", ";

		QueryText += text.str();
		count++;
	}
}
