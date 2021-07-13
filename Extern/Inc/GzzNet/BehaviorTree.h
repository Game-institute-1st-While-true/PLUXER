#pragma once
#include <functional>
#include <vector>

namespace gzz
{
	enum class  RETURN_TYPE { ABORT = -1, FAIL = 0, SUCESS = 1, PROCESS = 0xff };
	using Behavior = std::function<int()>;

	template<class C>
	using CMF = int(C::*)();

	struct Node
	{
		Node* Up;
		std::vector<Node*> Nodes;
		int Seed;
		int Route;

		Node() = delete;
		Node(int seed)
			:Seed(seed), Route(0), Up(nullptr)
		{
			Nodes.reserve(20);
		}

		virtual ~Node()
		{
			__int64 size = Nodes.size();
			for (int i = 0; i < size; i++)
			{
				if (Nodes[i])
				{
					delete Nodes[i];
					Nodes[i] = nullptr;
				}
			}
		}

		Node* SearchNode(int seed)
		{
			if (seed == Seed)
			{
				return this;
			}
			for (auto node : Nodes)
			{
				if (node)
				{
					if (node->Seed == seed)
					{
						return node;
					}
					else
					{
						Node* ret = node->SearchNode(seed);
						if (ret != nullptr)
						{
							return ret;
						}
					}
				}
			}
			return nullptr;
		};

		void ResultStateUpdate(int ret, int index)
		{
			switch (static_cast<RETURN_TYPE>(ret))
			{
			case RETURN_TYPE::ABORT:
				Route = 0;
				break;
			case RETURN_TYPE::FAIL:
				Route = 0;
				break;
			case RETURN_TYPE::SUCESS:
				Route = 0;
				break;
			case RETURN_TYPE::PROCESS:
				Route = index;
				break;
			}
		}

		virtual int SelectInvoke()
		{
			if (!Nodes.empty())
			{
				int ret = 0;
				int index = 0;

				if (Nodes[Route])
				{
					return Nodes[Route]->SelectInvoke();
					ResultStateUpdate(ret, index++);

				}
				else
				{
					return -1;
				}
			}
			else
			{
				return Invoke();
			}
		}

		virtual int Invoke()
		{
			int ret = 0;
			int index = 0;

			for (auto node : Nodes)
			{
				ret = node->Invoke();
				ResultStateUpdate(ret, index++);
				if (ret == 0xff) break;
			}
			return ret;
		};
	};

	struct BehaviorNode : public Node
	{
		using FuncName = wchar_t[20];

		BehaviorNode() : Node(-1), Func(nullptr) {};
		BehaviorNode(int seed) : Node(seed), Func(nullptr) {};
		virtual ~BehaviorNode() = default;

		FuncName funcname = L"";
		Behavior Func;

	public:
		template<class C>
		void AddFunc(C* this_, CMF<C> function)
		{
			Func = [=]() { return(this_->*function)(); };
		}

		virtual int Invoke()
		{
			return Func();
		};
	};

	struct SequenceNode : public BehaviorNode
	{
		virtual ~SequenceNode() = default;
		virtual int Invoke()
		{
			int ret = 0;
			int index = 0;

			auto iter = Nodes.begin();

			for (; iter < Nodes.end(); iter++)
			{
				ret = (*iter)->Invoke();
				ResultStateUpdate(ret, index++);
				if (ret != 1) break;
			}
			return ret;
		};

		virtual int SelectInvoke()
		{
			int ret = 0;
			int index = Route;

			auto iter = Nodes.begin() + index;
			ret = (*iter)->SelectInvoke();

			if (ret != 0xff)
			{
				if (ret == -1)
				{
					return ret;
				}
				++iter; ++index;
				for (; iter < Nodes.end(); iter++)
				{
					ret = (*iter)->Invoke();
					ResultStateUpdate(ret, index++);
					if (ret != 1) break;
				}
			}
			return ret;
		};
	};

	struct SelectNode : public BehaviorNode
	{
		virtual ~SelectNode() = default;

		virtual int Invoke()
		{
			int ret = 0;
			int index = 0;
			auto iter = Nodes.begin();

			for (; iter < Nodes.end(); iter++)
			{
				ret = (*iter)->Invoke();
				ResultStateUpdate(ret, index++);
				if (ret != static_cast<int>(RETURN_TYPE::FAIL)) break;
			}
			return ret;
		};

		virtual int SelectInvoke()
		{
			int ret = 0;
			int index = Route;

			auto iter = Nodes.begin() + index;
			ret = (*iter)->SelectInvoke();

			if (ret != 0xff)
			{
				if (ret == -1)
				{
					return ret;
				}
				++iter; ++index;
				for (; iter < Nodes.end(); iter++)
				{
					ret = (*iter)->Invoke();
					ResultStateUpdate(ret, index++);
					if (ret != 0) break;
				}
			}
			return ret;
		};
	};

	struct DecoratorNode : public BehaviorNode
	{
		virtual ~DecoratorNode() = default;

		virtual int Invoke()
		{
			int ret = Func();

			if (ret > 0)
			{
				int index = 0;
				for (auto node : Nodes)
				{
					ret = node->Invoke();
					ResultStateUpdate(ret, index++);
					if (ret == 0xff) break;
				}
			}
			else
			{
				return ret;
			}
			return ret;
		};

		virtual int SelectInvoke()
		{
			int ret = Func();

			if (ret > 0)
			{
				int index = Route;

				auto iter = Nodes.begin() + index;
				ret = (*iter)->SelectInvoke();
				ResultStateUpdate(ret, index);
			}
			else
			{
				return ret;
			}
			return ret;
		}
	};

	struct ServiceNode : public BehaviorNode
	{
		virtual ~ServiceNode() = default;

		virtual int Invoke()
		{
			int index = 0;
			int ret = 0;

			Func();

			for (auto node : Nodes)
			{
				ret = node->Invoke();
				ResultStateUpdate(ret, index++);
				if (ret == 0xff) break;
			}
			return ret;
		};

		virtual int SelectInvoke()
		{
			Func();

			int index = Route;
			int ret = 0;
			auto iter = Nodes.begin() + index;
			ret = (*iter)->SelectInvoke();
			ResultStateUpdate(ret, index);
			return ret;
		}
	};

	class BehaviorTree
	{
	public:
		BehaviorTree();
		~BehaviorTree();

	private:
		Node* Root;
		std::vector<wchar_t*> FuncNames;
		int BtSeed;
		int PreRet;

		std::function<int()> RunFunc;

	public:
		bool Insert(BehaviorNode* node, int dst_node);
		bool Erase(Node* node);
		int Run();
	};
}