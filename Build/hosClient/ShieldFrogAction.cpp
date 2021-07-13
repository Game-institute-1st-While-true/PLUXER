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
	// Behavior Tree 에 노드들을 추가

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
	// 꺼졌다가 켜지면 상태 초기화
}

void ShieldFrogAction::Update()
{

	if (m_NoteManager->GetIsBGMStart())
	{
		// Behavior Tree 를 돌리자
		RootNode->Invoke();
	}

}

void ShieldFrogAction::Render()
{
	g_Renderer->_DebugText(700, 200, 15, hos::Vector4(1, 1, 1, 1), L"Robot Pos : %f, %f, %f", ShieldFrogTransform->GetPosition().x, ShieldFrogTransform->GetPosition().y, ShieldFrogTransform->GetPosition().z);
	//g_Renderer->_DebugText(700, 200, 15, hos::Vector4(1, 1, 1, 1), L"Player Pos : %f, %f, %f", PlayerTransform->GetPosition().x, PlayerTransform->GetPosition().y, PlayerTransform->GetPosition().z);
	//g_Renderer->_DebugText(700, 250, 15, hos::Vector4(1, 1, 1, 1), L"Player Dir : %f, %f, %f", PlayerTransform->GetForward().x, PlayerTransform->GetForward().y, PlayerTransform->GetForward().z);
	//g_Renderer->_DebugText(700, 300, 15, hos::Vector4(1, 1, 1, 1), L"Nav Dir : %f, %f, %f", m_NavAgent->GetDir().x, m_NavAgent->GetDir().y, m_NavAgent->GetDir().z);

	g_Renderer->_DebugText(100, 160, 12, hos::Vector4(1, 1, 1, 1), L" ShieldFrog Pos : %f, %f, %f", ShieldFrogTransform->GetPosition().x, ShieldFrogTransform->GetPosition().y, ShieldFrogTransform->GetPosition().z);
}

BTNode::SearchResult ShieldFrogIdle::Invoke()
{
	ShieldFrogAction* _ShildFrogAciton = dynamic_cast<ShieldFrogAction*>(BlackBoard);

	if (!_ShildFrogAciton)
	{
		return SearchResult::Failure;
	}

	_ShildFrogAciton->m_ShieldFrogState->SetNowAction(ShieldFrogState::ShieldFrogActionList::Idle);

	if (_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->GetAniClipName().compare(L"open_idle") != 0)
	{
		_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->SetAnimation(L"prog4", L"open_idle");
	}

	auto Navi = _ShildFrogAciton->m_NavAgent;

	Navi->SetPosition();
	Navi->SetDestination(_ShildFrogAciton->PlayerTransform->GetPosition());

	hos::Vector3 _NowPos = _ShildFrogAciton->ShieldFrogTransform->GetPosition();

	float _MoveSpeed = 0.5f;

	hos::Vector3 _NowDir = _ShildFrogAciton->m_NavAgent->GetDir();

	hos::Vector3 forward(_NowDir.x, 0.f, _NowDir.z);
	_ShildFrogAciton->ShieldFrogTransform->SetForward(forward);

	_ShildFrogAciton->ShieldFrogTransform->SetPosition(_NowPos + (_ShildFrogAciton->m_NavAgent->GetDir() * _MoveSpeed * (float)Time->DeltaTime() * _ShildFrogAciton->m_ShieldFrogState->GetAnimationTimeScale()));

	return BTNode::SearchResult::Success;
}

BTNode::SearchResult ShieldFrogDamaged::Invoke()
{
	ShieldFrogAction* _ShildFrogAciton = dynamic_cast<ShieldFrogAction*>(BlackBoard);

	// 확실한 것에 대해서는 다이나믹 캐스트를 할 필요가 없다.
	if (!_ShildFrogAciton)
	{
		return SearchResult::Failure;
	}

	// 방패병 무적 상태인가?



	// 방패병 상태가 Damaged 인가?
	if (_ShildFrogAciton->m_ShieldFrogState->GetNowAction() == ShieldFrogState::ShieldFrogActionList::Damaged)
	{
		// 방패를 올린 상태인가?
		// 맞은 방향에 따라서 애니메이션이 다름


		if (_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->GetAniClipName().compare(L"open_damaged_front") != 0)
		{
			_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->SetAnimation(L"prog4", L"open_damaged_front");
		}

		if (_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->GetIsAnimationEnd())
		{
			return BTNode::SearchResult::Failure;
		}
		else
		{
			return SearchResult::Success;
		}
	}
	else
	{
		return BTNode::SearchResult::Failure;
	}

}

BTNode::SearchResult ShieldFrogDead::Invoke()
{
	ShieldFrogAction* _ShildFrogAciton = dynamic_cast<ShieldFrogAction*>(BlackBoard);

	if (!_ShildFrogAciton)
	{
		return SearchResult::Failure;
	}

	if (_ShildFrogAciton->m_ShieldFrogState->GetNowAction() == ShieldFrogState::ShieldFrogActionList::Dead)
	{
		if (_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->GetAniClipName().compare(L"open_dead") != 0)
		{
			_ShildFrogAciton->m_ShieldFrogState->ShieldFrogAnimation->SetAnimation(L"prog4", L"open_dead");
		}

		if (DeadTimer > 2.0f)
		{
			_ShildFrogAciton->m_GameObject->SetActive(false);
		}
		else
		{
			DeadTimer += (float)Time->DeltaTime();
		}

		return SearchResult::Success;
	}
	else
	{
		return SearchResult::Failure;
	}

}

BTNode::SearchResult ShieldFrogMeleeDetect::Invoke()
{
	ShieldFrogAction* _ShildFrogAciton = dynamic_cast<ShieldFrogAction*>(BlackBoard);

	if (!_ShildFrogAciton)
	{
		return SearchResult::Failure;
	}

	// 범위 내에 들어왔는가?

	// 방패 올리기

	// 앞으로 이동하면서 회전

	// 바라보고 있는가?

	return SearchResult::Failure;
}

BTNode::SearchResult ShieldFrogMeleeAttack::Invoke()
{
	// 박자에 맞춰서 공격해야 함 
	return SearchResult::Failure;
}
