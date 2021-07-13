#ifndef _SINGLETON_H
#define _SINGLETON_H

// [2020/10/15 노종원]
// 싱글톤 클래스를 만드는 매크로

//LOG
// [2020/10/23 노종원] 템플릿클래스 삭제 매크로로 변경한다. 템플릿클래스로 만든 싱글턴의 단점이 너무나도 많다.
// [2020/10/23 노종원] 클래스 선언의 최상단에 매크로를 사용하도록 하자

#define SINGLETON_CLASS(name)							\
private:												\
	name();												\
	~name();											\
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

#endif // !_SINGLETON_H