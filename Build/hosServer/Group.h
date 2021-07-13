#pragma once
#include "Defines.h"
#include <atomic>

// [2020/02/04 김효곤]
// 하나의 SceneManager 에 여러 유저 객체가 접근할 수 있도록 만들어 놓는다.

// [2020/02/10 김효곤]
// 그룹은 SceneManager을 제어하기 위한 수단이다. 

namespace hos::com
{
	class GroupSystem;
	class GroupCallBack;

	enum class GROUP_STATE : unsigned char
	{
		NONE,
		ENABLE,	 // 활성화
		DISABLE, // 비활성화		
		LOAD,	 // 준비 완료
		WAIT, 	 // 로딩
		GAME,	 // 게임중
		RELEASE, // 게임 종료 후 정리	
		STANDBY  // 유저가 들어온 상태
	};

	enum class GROUP_TYPE : unsigned char
	{
		NONE,
		LOBBY,
		BOSS_ROOM
	};

	class Group
	{
		friend class GroupSystem;

	private:
		Scene* m_Scene;											// Scene
		string SceneName;										// SceneName

		double GroupLifeTime;									
		double GroupLifeCheck;

		GROUP_STATE State;
		GROUP_TYPE Type;

		std::mutex MemberLock;									// 멤버 관리에 필요한 lock
		std::mutex SceneMutex;									// Scene의 생성과 소멸에 필요한 lock
		std::vector<NetworkToken*> Members;						// 해당 그룹에 속한 session id
		std::deque<int> MemberIdQueue;							// 멤버풀 관리를 위해 아이디를 저장하는 deque
		string Name;											
		int Id;													// group id

		int MaxMember;											// 최대 그룹인원수
		GroupCallBack* CallBack;								// 그룹에 매핑된 그룹 callback

		bool Playing;
		bool RoomDestroy;

	public:
		Group() = delete;
		Group(const int id);
		~Group();

	public:

		// getter and setter
		int GetId() const { return Id; }
		string GetName() { return Name; }
		void SetName(string name) { Name = name; }
		void SetUpMembers(const int max);
		int	GetMaxMember() const { return MaxMember; };
		int GetCurrentMenber() const { return MaxMember - static_cast<int>(MemberIdQueue.size()); }
		void SetCallBack(GroupCallBack* callback);

		GROUP_STATE GetState() { return State; }
		void SetState(GROUP_STATE state);

		bool IsPlaying() { return Playing; }

		Scene* GetScene() { return m_Scene; }

		// user management
	public:
		bool AddMember(NetworkToken* token, std::wstring name, int uuid);
		bool DelMember(NetworkToken* token);
		void GroupSend(PACKAGE& package);
		void ExceptMeGroupSend(PACKAGE& package);
		void Send(PACKET& packet); // 그룹 내 모든 유저에게 Send

		void MemberDisconnect(NetworkToken* token);
		void AllDisconnect();

		int FindMember(NetworkToken* token)
		{
			std::scoped_lock<std::mutex> lock(MemberLock);
			int cnt = 0;
			for (auto other : Members)
			{
				if (other == token)
				{
					return cnt;
				}
				cnt++;
			}
			return -1;
		}
		int FindMember(int token_id)
		{
			std::scoped_lock<std::mutex> lock(MemberLock);
			int cnt = 0;
			for (auto other : Members)
			{
				if (other != nullptr)
				{
					if (other->GetSessionId() == token_id)
					{
						return cnt;
					}
				}
				cnt++;
			}
			return -1;
		}
		NetworkToken* GetMember(int index)
		{
			std::scoped_lock<std::mutex> lock(MemberLock);
			if (index < 0 || Members.size() < index)
			{
				return nullptr;
			}
			return Members[index];
		};

		// load 
	public:
		void Load(string_view scene_name);
		void Load();

	private:
		void LifeCycleCheck();

	private:
		void Loop();
	};

	class GroupCallBack
	{
	protected:
		Group* m_Group;

	public:
		GroupCallBack() = delete;
		GroupCallBack(Group* group) { m_Group = group; };
		virtual ~GroupCallBack() = default;

		virtual void OnLoad() = 0;
		virtual void OnAddToken(NetworkToken* token, int tokenIndex, std::wstring name, int uuid) = 0;	// 그룹에 토큰이 생성되었을 때 (유저입장)
		virtual void OnDelToken(NetworkToken* token, int tokenIndex) = 0;								// 그룹에 토큰이 제거되었을 때 (유저퇴장)
		virtual void OnStateUpdate() = 0;
	};
}
