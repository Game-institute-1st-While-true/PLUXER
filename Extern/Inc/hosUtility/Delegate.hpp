#ifndef _DELEGATE_H
#define _DELEGATE_H

// [2020/10/20 노종원]
// 델리게이트클래스
// 씬매니저, 오브젝트등에 필요해서 만들었다.

// LOG
// [2020/12/31] 노종원 : ut네임스페이스 추가

#include <functional>
#include <vector>

namespace hos::ut
{
	template<typename Ret, typename... Args>
	class Delegate
	{
		using FUNC_TYPE = std::function<Ret(Args...)>;
		using FUNC_TYPE_PTR = Ret(*)(Args...);
		template<class C>
		using FUNC_TYPE_MEM_FN = Ret(C::*)(Args...);

	private:
		std::vector<FUNC_TYPE> FuncList;

	public:
		explicit Delegate() = default;
		Delegate(const Delegate& delegate)
		{
			//벡터 복사 제대로 되는지 확인 필요
			FuncList = delegate.FuncList;
		}
		Delegate& operator=(const Delegate&) = default;
		Delegate(Delegate&&) = default;
		Delegate& operator=(Delegate&&) = default;
		~Delegate() = default;

	public:
		void Add(FUNC_TYPE function)
		{
			if (function)
			{
				FuncList.emplace_back(function);
			}
		}

		//작동하는것 확인 virtual table을 이용한 this pointer 계산으로 파라메터를 줄여서 +=연산자 오버로딩 해보자
		template<class C>
		void Add(FUNC_TYPE_MEM_FN<C> function, C* this_)
		{
			if (function != nullptr && this_ != nullptr)
			{
				FUNC_TYPE _func = [=](Args... args) 
				{
					(this_->*function)(args...); 
				};
				FuncList.emplace_back(_func);
			}
		}

		void operator+=(FUNC_TYPE function)
		{
			Add(function);
		}

		/*template<class C>
		void operator += (FUNC_TYPE_MEM_FN<C> function, C* obj)
		{
			FUNC_TYPE _func = [&](Args... args) { (obj->*function)(args...); };
			FuncList.emplace_back(_func);
		}*/

		void Remove(FUNC_TYPE function)
		{
			auto _funcptr = function.template target<Ret(*)(Args...)>();
			for (auto it = FuncList.begin(); it != FuncList.end(); ++it)
			{
				auto _ptr = (*it).template target<Ret(*)(Args...)>();
				if (*_funcptr == *_ptr)
				{
					FuncList.erase(it);
					return;
				}
			}
		}

		//TODO : 정상 작동하는지 검증 필요
		template<class C>
		void Remove(FUNC_TYPE_MEM_FN<C> function, C* this_)
		{
			FUNC_TYPE _func = [=](Args... args) { (this_->*function)(args...); };
			auto _funcptr = _func.template target<Ret(*)(Args...)>();
			for (auto it = FuncList.begin(); it != FuncList.end(); ++it)
			{
				auto _ptr = (*it).template target<Ret(*)(Args...)>();
				if (*_funcptr == *_ptr)
				{
					FuncList.erase(it);
					return;
				}
			}
		}

		void Remove(int index)
		{
			if (index <= 0 || index > FuncList.size())
			{
				return;
			}
			auto iter = FuncList.begin();
			for (int i = 0; i < index; i++)
			{
				++iter;
			}
			FuncList.erase(iter);
		}

		void operator-=(FUNC_TYPE function)
		{
			Remove(function);
		}

		void operator()(Args... args)
		{
			std::vector<FUNC_TYPE> tmpList = FuncList;
			for (auto _func : tmpList)
			{
				_func(args...);
			}
		}
	};

	template<typename Ret, typename... Args>
	class  BindDelegate
	{
		using FUNC_TYPE = std::function<Ret(Args...)>;
		using BIND_FUNC_TYPE = std::function<Ret()>;

	private:
		std::vector<BIND_FUNC_TYPE> FuncList;

	public:
		BindDelegate() = default;
		BindDelegate(const BindDelegate& bindDelegate)
		{
			FuncList = bindDelegate.FuncList;
		}
		BindDelegate(BindDelegate&&) = delete;
		~BindDelegate() = default;

	public:
		void Add(FUNC_TYPE function, Args... args)
		{
			auto holder = std::bind(function, std::forward<Args>(args)...);
			FuncList.emplace_back(holder);
		}

		void operator+=(FUNC_TYPE function)
		{
			Add(function);
		}

		void Remove(FUNC_TYPE function)
		{
			auto _funcptr = function.template target<Ret(*)(Args...)>();
			for (auto it = FuncList.begin(); it != FuncList.end(); ++it)
			{
				auto _ptr = (*it).template target<Ret(*)(Args...)>();
				if (*_funcptr == *_ptr)
				{
					FuncList.erase(it);
					return;
				}
			}
		}

		void operator-=(FUNC_TYPE function)
		{
			Remove(function);
		}

		void operator()()
		{
			for (auto _func : FuncList)
			{
				_func();
			}
		}
	};
}

#endif // !_DELEGATE_H