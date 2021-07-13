#ifndef _SINGLETON_H
#define _SINGLETON_H
#include <mutex>

// [2020/10/15 노종원]
// 싱글톤 클래스를 만드는 매크로

//LOG
// [2020/10/23 노종원] 템플릿클래스 삭제 매크로로 변경한다. 템플릿클래스로 만든 싱글턴의 단점이 너무나도 많다.
// [2020/11/16 노종원] 싱글턴 클래스는 상속할 수 없다

// warnning : 클래스 선언의 최상단에 매크로를 사용하도록 하자
#define SINGLETON_CLASS(name)							\
private:												\
	name();												\
	~name();											\
public:													\
	name(const name&) = delete;							\
	name& operator=(const name&) = delete;				\
	name(name&&) = delete;								\
	name& operator=(name&&) = delete;					\
														\
	inline static name* name##Ins;						\
public:													\
	static name* GetIns()								\
	{													\
		if (name##Ins == nullptr)						\
		{												\
			name##Ins = new name();						\
		}												\
		return name##Ins;								\
	}													\
	static void Destory()								\
	{													\
		if (name##Ins)									\
		{												\
			delete name##Ins;							\
			name##Ins = nullptr;						\
		}												\
	}													\
private:												\

#define THREAD_SAFE_SINGLETON_CLASS(name)					\
private:													\
	name();													\
	~name();												\
	inline static std::mutex InstanceMutex = std::mutex();	\
public:														\
	name(const name&) = delete;								\
	name& operator=(const name&) = delete;					\
	name(name&&) = delete;									\
	name& operator=(name&&) = delete;						\
															\
	inline static name* name##Ins;							\
public:														\
	static name* GetIns()									\
	{														\
		InstanceMutex.lock();								\
		if (name##Ins == nullptr)							\
		{													\
			name##Ins = new name();							\
		}													\
		InstanceMutex.unlock();								\
		return name##Ins;									\
	}														\
	static void Destory()									\
	{														\
		if (name##Ins)										\
		{													\
			delete name##Ins;								\
			name##Ins = nullptr;							\
		}													\
	}														\
private:													\

/*
namespace hos
{
	template<typename T>
	class Singleton
	{
	private:
		inline static T* Instance = nullptr;
	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

	private:
		//싱글턴 클래스의 복사 금지
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		//싱글턴 클래스의 이동 금지
		Singleton(Singleton&&) = delete;
		Singleton& operator=(Singleton&&) = delete;

	public:
		static T* GetIns()
		{
			if (Instance == nullptr)
			{
				Instance = new T;
			}
			return Instance;
		}

		void Destory()
		{
			if (Instance)
			{
				delete Instance;
				Instance = nullptr;
			}
		}
	};
}
//*/

#endif // !_SINGLETON_H