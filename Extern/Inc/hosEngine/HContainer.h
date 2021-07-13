// <Concept>
// Source Name		: HContainer.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.02.09

// <Explanation>
// 계층구조를 구현하기 위한 탬플릿 클래스

// Working Log
// 2021.02.09.Hosik	: 생성

#pragma once
#include <vector>

namespace hos
{
	template <typename Data>
	class HContainer
	{
	public:
		typedef class HContainer<Data> Container;
		Data* m_pParent = nullptr;
		Data* m_pCurData = nullptr;

		std::vector<Data*> m_pChildList;
		int	m_ChildCnt = 0;

	public:
		void Release();					// 벡터 삭제

		bool AddChild(Data* pChild);							//자식으로 등록하기.
		void SetParent(Data* pParent) { m_pParent = pParent; }	//부모 설정하기.
		const Data* GetParent() { return m_pParent; }			//부모 포인터 얻기.
		std::vector<Data*>* GetChildList() { return &m_pChildList; }		//자식 리스트 얻기.★

	public:
		HContainer();
		HContainer(Data* pCurData);
		~HContainer();
	};

	template<typename Data>
	inline void HContainer<Data>::Release()
	{
		// Container만 제거해줍니다.
		/// 객체는 모델이 지워질 때 전부 지워준다.
		m_pParent = nullptr;

		m_pChildList.clear();
		m_pChildList.shrink_to_fit();
	}

	template<typename Data>
	inline bool HContainer<Data>::AddChild(Data* pChild)
	{
		m_pChildList.push_back(pChild);
		m_ChildCnt++;

		return true;
	}

	template<typename Data>
	inline HContainer<Data>::HContainer()
	{
		m_pChildList.reserve(10);
	}

	template<typename Data>
	inline HContainer<Data>::HContainer(Data* pCurData)
	{
	}

	template<typename Data>
	inline HContainer<Data>::~HContainer()
	{
	}
}