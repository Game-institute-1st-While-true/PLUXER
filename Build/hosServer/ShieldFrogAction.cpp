#include "ShieldFrogAction.h"

ShieldFrogAction::ShieldFrogAction() : hos::com::Script(L"ShieldFrogAction")
{
	ExecutionOrder = 6;
}

ShieldFrogAction::~ShieldFrogAction()
{
	RootNode->RemoveAllChild();
	delete RootNode;
	RootNode = nullptr;

}

void ShieldFrogAction::Reset()
{

}

void ShieldFrogAction::Awake()
{
	// Behavior Tree �� ������ �߰�

	m_NoteManager = m_GameObject->m_Scene->FindGameObjectWithName(L"NoteManager")->GetComponent<NoteManager>();
	m_ShieldFrogState = m_GameObject->GetComponent<ShieldFrogState>();
	m_NavAgent = m_GameObject->GetComponent<hos::com::NavAgent>();
	m_NavAgent->SetNavMeshId(m_GameObject->m_Scene->GetBuildIndex());
	m_NavAgent->RequestPathFind();

	PlayerTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<hos::com::Transform>();
	ShieldFrogTransform = m_GameObject->transform;

	RootNode = new BTRoot(this);
	StartSelector = new BTSelector(this);
	NodeIdle = new ShieldFrogIdle(this);
	NodeDamaged = new ShieldFrogDamaged(this);
	NodeDead = new ShieldFrogDead(this);

	RootNode->SetStartNode(StartSelector);

	StartSelector->AddChild(NodeDead);
	StartSelector->AddChild(NodeDamaged);
	StartSelector->AddChild(NodeIdle);
}

void ShieldFrogAction::Start()
{
	// �����ٰ� ������ ���� �ʱ�ȭ
}

void ShieldFrogAction::Update()
{

	if (m_NoteManager->GetIsBGMStart())
	{
		// Behavior Tree �� ������
		RootNode->Invoke();
	}

}

BTNode::SearchResult ShieldFrogIdle::Invoke()
{
	ShieldFrogAction* _ShildFrogAciton = static_cast<ShieldFrogAction*>(BlackBoard);

	if (!_ShildFrogAciton)
	{
		return SearchResult::Failure;
	}

	_ShildFrogAciton->m_ShieldFrogState->SetNowAction(ShieldFrogState::ShieldFrogActionList::Idle);

	//if (_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->GetAniClipName().compare(L"open_idle") != 0)
	//{
	//	_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->SetAnimation(L"prog4", L"open_idle");
	//}

	auto Navi = _ShildFrogAciton->m_NavAgent;

	Navi->SetPosition();
	Navi->SetDestination(_ShildFrogAciton->PlayerTransform->GetPosition());

	hos::Vector3 _NowPos = _ShildFrogAciton->ShieldFrogTransform->GetPosition();

	float _MoveSpeed = 0.5f;

	hos::Vector3 _NowDir = _ShildFrogAciton->m_NavAgent->GetDir();

	hos::Vector3 forward(_NowDir.x, 0.f, _NowDir.z);
	_ShildFrogAciton->ShieldFrogTransform->SetForward(forward);

	_ShildFrogAciton->ShieldFrogTransform->SetPosition(_NowPos + (_ShildFrogAciton->m_NavAgent->GetDir() * _MoveSpeed * Time->DeltaTime()));

	return BTNode::SearchResult::Success;
}

BTNode::SearchResult ShieldFrogDamaged::Invoke()
{
	ShieldFrogAction* _ShildFrogAciton = static_cast<ShieldFrogAction*>(BlackBoard);

	// Ȯ���� �Ϳ� ���ؼ��� ���̳��� ĳ��Ʈ�� �� �ʿ䰡 ����.
	if (!_ShildFrogAciton)
	{
		return SearchResult::Failure;
	}

	// ���к� ���� �����ΰ�?



	// ���к� ���°� Damaged �ΰ�?
	if (_ShildFrogAciton->m_ShieldFrogState->GetNowAction() == ShieldFrogState::ShieldFrogActionList::Damaged)
	{
		//if (_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->GetAniClipName().compare(L"open_damaged_front") != 0)
		//{
		//	_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->SetAnimation(L"prog4", L"open_damaged_front");
		//}

		//if (_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->GetIsAnimationEnd())
		//{
		//	return BTNode::SearchResult::Failure;
		//}
		//else
		//{
		//	return SearchResult::Success;
		//}
	}
	else
	{
		return BTNode::SearchResult::Failure;
	}

}

BTNode::SearchResult ShieldFrogDead::Invoke()
{
	ShieldFrogAction* _ShildFrogAciton = static_cast<ShieldFrogAction*>(BlackBoard);

	if (!_ShildFrogAciton)
	{
		return SearchResult::Failure;
	}

	if (_ShildFrogAciton->m_ShieldFrogState->GetNowAction() == ShieldFrogState::ShieldFrogActionList::Dead)
	{
	/*	if (_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->GetAniClipName().compare(L"open_dead") != 0)
		{
			_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->SetAnimation(L"prog4", L"open_dead");
		}*/

		return SearchResult::Success;
	}
	else
	{
		return BTNode::SearchResult::Failure;
	}

}
