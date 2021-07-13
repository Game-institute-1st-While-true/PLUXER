#include "BTNode.h"

BTNode::BTNode(hos::com::Script* blackBoard) : BlackBoard(blackBoard)
{

}

BTNode::~BTNode()
{

}

//////////////////////////////////////////////////////////////////////////

BTRoot::BTRoot(hos::com::Script* blackBoard) : BTNode(blackBoard)
{

}


BTRoot::~BTRoot()
{

}

void BTRoot::SetStartNode(BTCompositeNode* node)
{
	StartNode = nullptr;
	StartNode = node;
}

void BTRoot::RemoveAllChild()
{
	StartNode->RemoveAllChild();
	delete StartNode;
	StartNode = nullptr;
}

BTNode::SearchResult BTRoot::Invoke()
{
	if (StartNode)
	{
		// 반환 값이 Continue 이면?
		// 해당 경로 저장?

		// 반환 값이 Abort 이면?
		// Countinue 해제

		return StartNode->Invoke();
	}
	else
	{
		return BTNode::SearchResult::Failure;
	}


}

//////////////////////////////////////////////////////////////////////////

BTCompositeNode::BTCompositeNode(hos::com::Script* blackBoard) : BTNode(blackBoard)
{

}

BTCompositeNode::~BTCompositeNode()
{

}

void BTCompositeNode::AddChild(BTNode* node)
{
	Children.push_back(node);
}

void BTCompositeNode::RemoveAllChild()
{
	while (!Children.empty())
	{
		BTNode* _RemoveBtNode = Children.back();
		Children.pop_back();

		BTCompositeNode* _RemoveCompositeNode = dynamic_cast<BTCompositeNode*>(_RemoveBtNode);

		if (_RemoveCompositeNode)
		{
			_RemoveCompositeNode->RemoveAllChild();
		}

		delete _RemoveBtNode;
		_RemoveBtNode = nullptr;
	}
}

const std::vector<BTNode*>& BTCompositeNode::GetChildren()
{
	return Children;
}

//////////////////////////////////////////////////////////////////////////

BTSelector::BTSelector(hos::com::Script* blackBoard) : BTCompositeNode(blackBoard)
{

}

BTSelector::~BTSelector()
{

}

BTNode::SearchResult BTSelector::Invoke()
{
	for (auto node : GetChildren())
	{
		if (node->Invoke() != BTNode::SearchResult::Failure)
		{
			return node->Invoke();
		}
	}

	return BTNode::SearchResult::Failure;
}

//////////////////////////////////////////////////////////////////////////

BTSequence::BTSequence(hos::com::Script* blackBoard) : BTCompositeNode(blackBoard)
{

}

BTSequence::~BTSequence()
{

}

BTNode::SearchResult BTSequence::Invoke()
{
	for (auto node : GetChildren())
	{
		if (node->Invoke() != BTNode::SearchResult::Success)
		{
			return node->Invoke();
		}
	}

	return BTNode::SearchResult::Success;
}
