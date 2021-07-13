#pragma once
#include "Defines.h"
#include <atomic>

// [2020/02/04 ��ȿ��]
// �ϳ��� SceneManager �� ���� ���� ��ü�� ������ �� �ֵ��� ����� ���´�.

// [2020/02/10 ��ȿ��]
// �׷��� SceneManager�� �����ϱ� ���� �����̴�. 

namespace hos::com
{
	class GroupSystem;
	class GroupCallBack;

	enum class GROUP_STATE : unsigned char
	{
		NONE,
		ENABLE,	 // Ȱ��ȭ
		DISABLE, // ��Ȱ��ȭ		
		LOAD,	 // �غ� �Ϸ�
		WAIT, 	 // �ε�
		GAME,	 // ������
		RELEASE, // ���� ���� �� ����	
		STANDBY  // ������ ���� ����
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

		std::mutex MemberLock;									// ��� ������ �ʿ��� lock
		std::mutex SceneMutex;									// Scene�� ������ �Ҹ꿡 �ʿ��� lock
		std::vector<NetworkToken*> Members;						// �ش� �׷쿡 ���� session id
		std::deque<int> MemberIdQueue;							// ���Ǯ ������ ���� ���̵� �����ϴ� deque
		string Name;											
		int Id;													// group id

		int MaxMember;											// �ִ� �׷��ο���
		GroupCallBack* CallBack;								// �׷쿡 ���ε� �׷� callback

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
		void Send(PACKET& packet); // �׷� �� ��� �������� Send

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
		virtual void OnAddToken(NetworkToken* token, int tokenIndex, std::wstring name, int uuid) = 0;	// �׷쿡 ��ū�� �����Ǿ��� �� (��������)
		virtual void OnDelToken(NetworkToken* token, int tokenIndex) = 0;								// �׷쿡 ��ū�� ���ŵǾ��� �� (��������)
		virtual void OnStateUpdate() = 0;
	};
}
