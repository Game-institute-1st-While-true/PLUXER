#include "Projectile.h"

Projectile::Projectile() : hos::com::Script(L"Projectile")
{
	ExecutionOrder = 8;
}

Projectile::~Projectile()
{

}

void Projectile::Reset()
{

}

void Projectile::Awake()
{

	AttackArea = m_GameObject->GetComponent<hos::com::CapsuleCollision>();

	m_PlayerState = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<PlayerState>();

	//AttackArea->SetActive(false);

	if (m_GameObject->m_Scene->FindGameObjectWithName(L"ToxicManager"))
	{
		m_ToxicManager = m_GameObject->m_Scene->FindGameObjectWithName(L"ToxicManager")->GetComponent<ToxicManager>();
	}

}

void Projectile::Update()
{
	// 타입에 따른 투사체 이동
	// 게임 오브젝트 활성화 확인

	if (!this->GetActive())
	{
		ProjectileDisableTimer = 0.f;

		return;
	}

	if (ProjectileDisableTimer > ProjectileDisableTime)
	{
		AttackArea->SetActive(false);
		m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(false);
		ProjectileDisableTimer = 0.f;
		this->SetActive(false);
	}
	else
	{
		ProjectileDisableTimer += (float)Time->DeltaTime();
	}

	if (m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash1 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash2 ||
		m_PlayerState->GetNowPlayerAction() == PlayerState::PlayerActionList::Smash3)
	{
		AnimationTimeScale = m_PlayerState->GetSmashTimeScale();
	}
	else
	{
		AnimationTimeScale = 1.f;
	}

	if (m_PlayerState->GetIsFever())
	{
		AnimationTimeScale = m_PlayerState->GetFeverTimeScale();
	}

	if (AttackType == 0)
	{
		m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetForward() * MoveSpeed * (float)Time->DeltaTime() * AnimationTimeScale));
	}

	if (AttackType == 1)
	{
		m_GameObject->transform->SetPosition(m_GameObject->transform->GetPosition() + (m_GameObject->transform->GetForward() * MoveSpeed * (float)Time->DeltaTime() * AnimationTimeScale));
	}
}

void Projectile::LateUpdate()
{

}

void Projectile::OnTriggerEnter(hos::com::Collision* other)
{
	// 공격 타입에 따라 데미지가 다르게 들어감
	if (other->m_GameObject->GetTag().compare(L"Player") == 0)
	{
		if (AttackType == 0)
		{
			other->m_GameObject->GetComponent<PlayerState>()->GetDamaged(BossAttackDamage, 0, m_GameObject->transform);
			// 해당하는 위치에 바닥 생성

			hos::Vector3 _pos = m_GameObject->transform->GetPosition();

			_pos.y = 0.01f;

			m_ToxicManager->SetToxicFloorPosition(_pos);
		}
		if (AttackType == 1)
		{
			other->m_GameObject->GetComponent<PlayerState>()->GetDamaged(AttackDamage, 0, m_GameObject->transform);
		}

		AttackArea->SetActive(false);
		m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(false);
		this->SetActive(false);
	}

	if (other->m_GameObject->GetTag().compare(L"Wall") == 0)
	{
		if (AttackType == 0)
		{
			// 해당하는 위치에 바닥 생성
			hos::Vector3 _pos = m_GameObject->transform->GetPosition();

			_pos.y = 0.01f;

			m_ToxicManager->SetToxicFloorPosition(_pos);
		}

		// 오브젝트 비활성화
		AttackArea->SetActive(false);
		m_GameObject->transform->GetChild(0)->m_GameObject->SetActive(false);
		this->SetActive(false);
	}

}
