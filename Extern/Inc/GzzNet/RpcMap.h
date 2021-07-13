#pragma once
#include <iostream>
#include <map>	
#include <functional>

/// <summary>
///	[2021-02-08 김효곤]
/// key -1 은 오류값 출력이다.
/// Mapping으로 커스텀 가능하다.
/// </summary>

namespace gzz
{
	template<typename Ret, typename... Args>
	class RpcMap
	{
		using Rpc = std::function<Ret(Args...)>;
		template<class C>
		using CM_Rpc = Ret(C::*)(Args...);
	public:
		std::map<int, Rpc> Map;
		RpcMap()
		{
			Rpc _func = [=](Args... args) { this->worng_approach(args...); };
			Map.insert(std::make_pair(-1, _func));
		}
		virtual ~RpcMap() = default;
		char err_code = -1;

	public:
		template<class C>
		void Mapping(CM_Rpc<C> function, int index, C* this_)
		{
			Rpc _func = [=](Args... args) { (this_->*function)(args...); };
			Map.insert(std::make_pair(index, _func));
		}

		void SetErrCode(int code)
		{
			err_code = code;
		}

		Ret worng_approach(Args... args)
		{
			std::wcout << L"worng approach" << std::endl;
		}

		Rpc& operator[](const int index)
		{
			auto iter = Map.find(index);
			if (iter != Map.end())
			{
				return iter->second;
			}
			else
			{
				return Map.find(err_code)->second;
			}
		}
	};
}