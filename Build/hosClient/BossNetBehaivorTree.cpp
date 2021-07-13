#include "BossFrogAction.h"

void BossFrogAction::BossNetBehaivortree()
{
	ServiceNode* debug_service_node = new ServiceNode();

	SelectNode* no_ai_select = new SelectNode();

	BehaviorTask* task_dead = new BehaviorTask();
	BehaviorTask* task_move = new BehaviorTask();
	BehaviorTask* task_idle = new BehaviorTask();
	BehaviorTask* task_tongue = new BehaviorTask();
	BehaviorTask* task_backmove = new BehaviorTask();
	BehaviorTask* task_backjump = new BehaviorTask();
	BehaviorTask* task_rotate_to_target = new BehaviorTask();
	BehaviorTask* task_jump_att = new BehaviorTask();
	BehaviorTask* task_rush_att = new BehaviorTask();
	BehaviorTask* task_range_att = new BehaviorTask();
	BehaviorTask* task_search_target = new BehaviorTask();

	debug_service_node->Func = [=]()
	{
#ifdef _DEBUG
		if (Input->GetKey(KeyCode::D1))
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::Idle);
		}
		if (Input->GetKey(KeyCode::D2))
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::Move);
		}
		if (Input->GetKey(KeyCode::D3))
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::TongueAttack);
		}
		if (Input->GetKey(KeyCode::D4))
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::Dead);
		}
		if (Input->GetKey(KeyCode::D5))
		{
			m_BossFrogState->SetNowAction(BossFrogState::BossFrogActionList::RangeAttack);
		}
		if (Input->GetKey(KeyCode::D6))
		{
			m_ProjectileManager->ActiveProjectile(0, 0, hos::Vector3(0, 8, 0), true);
		}
		if (Input->GetKey(KeyCode::D7))
		{
			m_ProjectileManager->ActiveProjectile(0, 0, hos::Vector3(0, 8, 0), false);
		}

#endif // DEBUG
		return static_cast<int>(ACTION_RESULT::SUCESS);
	};

	task_dead->Func = [=]()
	{
		if (m_BossFrogState->GetAiType() != 1)
		{
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}

		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"die") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"die", 0, true);

				MelodySound->SetLoop(false);
				MelodySound->Stop();
				MelodySound->SetAudioClip(L"boss_die");
				MelodySound->Play();
			}
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}

		else
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};

	task_move->Func = [=]()
	{
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Move)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_forward") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_forward");
			}
			if (!m_BossFrogState->GetStop())
			{
				if (MelodySound->GetState() == DirectX::SoundState::PLAYING)
				{
					MelodySound->Stop();
					MelodySound->SetAudioClip(L"boss_die");
				}
				else
				{
					MelodySound->SetAudioClip(L"boss_die");
				}
				BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (BossFrogTransform->GetForward() * (float)Time->DeltaTime() * 1.f * m_BossFrogState->GetAnimationTimeScale()));
				IsSoundChange = false;
				m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Green);
			}
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};

	task_idle->Func = [=]()
	{
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Idle)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"idle") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"idle");
				if (MelodySound->GetState() == DirectX::SoundState::PLAYING)
				{
					MelodySound->Stop();
					MelodySound->SetAudioClip(L"boss_die");
				}
				else
				{
					MelodySound->SetAudioClip(L"boss_die");
				}
				IsSoundChange = false;
				PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(false);
				m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Green);
			}
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};

	task_tongue->Func = [=]()
	{
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::TongueAttack)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk2_tongue_combined") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk2_tongue_combined");
				m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);
			}
			AttackArea->AttackCollision->SetActive(false);
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
		else
		{
			AttackArea->AttackCollision->SetActive(false);
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};

	task_backmove->Func = [=]()
	{
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::BackMove)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_forward") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_forward");
				IsSoundChange = false;
			}
			if (!m_BossFrogState->GetStop())
			{
				if (MelodySound->GetState() == DirectX::SoundState::PLAYING)
				{
					MelodySound->Stop();
					MelodySound->SetAudioClip(L"boss_die");
				}
				else
				{
					MelodySound->SetAudioClip(L"boss_die");
				}
				BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (-BossFrogTransform->GetForward() * (float)Time->DeltaTime() * MoveSpeed));
			}
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};

	task_backjump->Func = [=]()
	{
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::BackJump)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"backjump_xy0") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"backjump_xy0");
				MelodySound->Stop();
				MelodySound->SetAudioClip(L"boss_backjump_110bpm");
				MelodySound->Play();
			}
			if (!m_BossFrogState->GetStop())
			{
				BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (-BossFrogTransform->GetForward() * (float)Time->DeltaTime() * BackJumpMoveSpeed));
			}
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};

	task_rotate_to_target->Func = [=]()
	{
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::RotateToTarget)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_left") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_left");
			}
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};

	//task_jump_att->Func = [=]()
	//{
	//	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::JumpAttack)
	//	{
	//		if (!IsSetJumpAttack)
	//		{
	//			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk1_jump_combined") != 0)
	//			{
	//				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk1_jump_combined");
	//				PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(false);
	//				IsSetJumpAttack = true;
	//				JumpAttackTimer = 0.f;
	//				//JumpAttackMoveSpeed = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length() * 2;
	//			}
	//		}
	//		if (!m_BossFrogState->GetStop())
	//		{
	//			m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);
	//			//BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (BossFrogTransform->GetForward() * JumpAttackMoveSpeed * (float)Time->DeltaTime()));
	//		}

	//		if (m_BossFrogState->BossFrogAnimation->GetIsAnimationEnd())
	//		{
	//			PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(false);

	//			//IsSetJumpAttack = false;
	//			//JumpAttackTimer = 0.f;
	//			//IsJumpAttackReady = false;
	//			//return static_cast<int>(ACTION_RESULT::FAILURE);
	//		}
	//		else
	//		{
	//			if (JumpAttackTimer > JumpAttackEnable && JumpAttackTimer < JumpAttackDisable)
	//			{
	//				float _Distance = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length();

	//				if (_Distance < JumpAttackShakeDistance)
	//				{
	//					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(true);
	//					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeTime(JumpAttackShakeTime);

	//					if (_Distance <= 2)
	//					{
	//						_Distance = 2.f;
	//					}
	//					JumpAttackShakeValue = 1 / _Distance;
	//					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeValue(JumpAttackShakeValue);
	//				}
	//			}

	//			if (JumpAttackTimer > JumpAttackDisable)
	//			{
	//			}

	//			JumpAttackTimer += (float)Time->DeltaTime();
	//			return static_cast<int>(ACTION_RESULT::PROCESS);
	//		}
	//	}
	//	else
	//	{
	//		PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(false);
	//		return static_cast<int>(ACTION_RESULT::FAILURE);
	//	}
	//};

	task_jump_att->Func = [=]()
	{
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::JumpAttack)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk1_jump_combined") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk1_jump_combined");
			}
			if (!m_BossFrogState->GetStop())
			{
				//BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (BossFrogTransform->GetForward() * JumpAttackMoveSpeed * (float)Time->DeltaTime()));
				m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);

			}
		
			if (JumpAttackTimer > JumpAttackEnable && JumpAttackTimer < JumpAttackDisable)
			{
				float _Distance = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length();

				if (_Distance < JumpAttackShakeDistance)
				{
					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(true);
					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeTime(JumpAttackShakeTime);

					if (_Distance <= 2)
					{
						_Distance = 2.f;
					}
					JumpAttackShakeValue = 1 / _Distance;
					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeValue(JumpAttackShakeValue);
				}
			}

			JumpAttackTimer += (float)Time->DeltaTime();
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};

	task_rush_att->Func = [=]()
	{
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::RushAttack)
		{
			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk3_rush") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk3_rush");
			}
			if (!m_BossFrogState->GetStop())
			{
				BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (BossFrogTransform->GetForward() * RushMoveSpeed * (float)Time->DeltaTime()));
				m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);
			}

			float _Distance = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length();

			if (!PlayerTransform->m_GameObject->GetComponent<PlayerState>()->GetIsShakeCamera())
			{
				if (_Distance < RushAttackShakeDistance)
				{
					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(true);
					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeTime(RushAttackShakeTime);

					if (_Distance <= 2)
					{
						_Distance = 2.f;
					}
					RushAttackShakeValue = 1 / _Distance;
					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeValue(RushAttackShakeValue);
				}
			}
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
		else
		{
			PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(false);
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};
	//task_rush_att->Func = [=]()
	//{
	//	if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::RushAttack)
	//	{
	//		if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk3_rush") != 0)
	//		{
	//			m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk3_rush");

	//			if (!m_BossFrogState->GetStop())
	//			{
	//				BossFrogTransform->SetPosition(BossFrogTransform->GetPosition() + (BossFrogTransform->GetForward() * RushMoveSpeed * (float)Time->DeltaTime()));
	//				m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);


	//				float _Distance = (BossFrogTransform->GetPosition() - PlayerTransform->GetPosition()).Length();

	//				if (_Distance < RushAttackShakeDistance)
	//				{
	//					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(true);
	//					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeTime(RushAttackShakeTime);

	//					if (_Distance <= 2)
	//					{
	//						_Distance = 2.f;
	//					}

	//					RushAttackShakeValue = 1 / _Distance;

	//					PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetShakeValue(RushAttackShakeValue);
	//				}
	//			}
	//			return static_cast<int>(ACTION_RESULT::SUCESS);
	//		}
	//	}

	//	else
	//	{
	//		PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(false);
	//		return static_cast<int>(ACTION_RESULT::FAILURE);
	//	}
	//};

	task_range_att->Func = [=]()
	{
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::Dead)
		{
			return static_cast<int>(ACTION_RESULT::AVOID);
		}
		else if (m_BossFrogState->GetNowAction() != BossFrogState::BossFrogActionList::RangeAttack)
		{
			IsSetRangeAttack = false;
			IsRangeAttackReady = false;
			RangeAttackReadyTimer = 0.f;
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}

		if (!IsSetRangeAttack)
		{
			// Cast Animation 실행

			IsSetRangeAttack = true;

			if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"atk0_projectile_combined") != 0)
			{
				m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk0_projectile_combined", 0, true);
			}
			m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Red);
			RangeAttackCount = 3;
		}

		if (m_BossFrogState->BossFrogAnimation->GetIsAnimationEnd())
		{
			if (RangeAttackCount != 0)
			{
				if (!IsReverse)
				{
					// 발사

					//m_ProjectileManager->SetProjectile(PlayerTransform, 0);
					ReadySound->Stop();
					ReadySound->SetAudioClip(L"boss_atk_projectile_2_cast");
					ReadySound->Play();

					IsReverse = true;
					RangeAttackCount--;

					if (RangeAttackCount <= 0)
					{
						m_BossFrogState->BossFrogAnimation->SetBlendAnimation(L"boss", L"atk0_projectile_recover", L"atk0_projectile_combined", 1.2f, 0.15f, true);
					}
					else
					{
						m_BossFrogState->BossFrogAnimation->SetReverseAnimation(L"boss", L"atk0_projectile_combined", 100.f, 69.f, true);
					}

				}
				else
				{
					m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"atk0_projectile_combined", 69.f, true);
					IsReverse = false;
				}
			}
			else
			{
				RangeAttackCount = 3;
				IsSetRangeAttack = false;
				m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Green);
				IsRangeAttackReady = false;
				RangeAttackReadyTimer = 0.f;
				IsSoundChange = false;
			}
		}
		return static_cast<int>(ACTION_RESULT::PROCESS);
	};

	task_search_target->Func = [=]()
	{
		PlayerTransform->m_GameObject->GetComponent<PlayerState>()->SetIsShakeCamera(false);
		if (m_BossFrogState->GetNowAction() == BossFrogState::BossFrogActionList::SearchTarget)
		{
			auto state = m_BossFrogState->GetNextAction();

			// 멜로디 플레이 1회 호출.
			switch (state)
			{
			case BossFrogState::BossFrogActionList::JumpAttack:

				JumpAttackTimer = 0.f;



			if (MelodySound)
			{
				if (MelodySound->GetAudioClip()->GetName().compare(L"boss_atk_jump_combined_110bpm") != 0)
				{
					MelodySound->Stop();
					MelodySound->SetAudioClip(L"boss_atk_jump_combined_110bpm");
					MelodySound->Play();
				}
				else
				{
					if (MelodySound->GetState() == DirectX::SoundState::STOPPED)
					{
						MelodySound->Play();
					}
				}
			}
			break;
			case BossFrogState::BossFrogActionList::RangeAttack:
				if (MelodySound)
				{
					if (MelodySound->GetAudioClip()->GetName().compare(L"boss_atk_projectile_1_melodyready_110bpm") != 0)
					{
						MelodySound->Stop();
						MelodySound->SetAudioClip(L"boss_atk_projectile_1_melodyready_110bpm");
						MelodySound->Play();
					}
					else
					{
						if (MelodySound->GetState() == DirectX::SoundState::STOPPED)
						{
							MelodySound->Play();
						}
					}
				}
				break;
			case BossFrogState::BossFrogActionList::TongueAttack:
				if (MelodySound)
				{
					if (MelodySound->GetAudioClip()->GetName().compare(L"boss_atk_tongue_combined_110bpm") != 0)
					{
						MelodySound->Stop();
						MelodySound->SetAudioClip(L"boss_atk_tongue_combined_110bpm");
						MelodySound->Play();
						IsSoundChange = true;
					}
					else
					{
						if (MelodySound->GetState() == DirectX::SoundState::STOPPED)
						{
							MelodySound->Play();
						}
					}
				}
				break;
			case BossFrogState::BossFrogActionList::RushAttack:
				if (MelodySound)
				{
					if (MelodySound->GetAudioClip()->GetName().compare(L"boss_atk_rush_110bpm_withmelody") != 0)
					{
						MelodySound->Stop();
						MelodySound->SetAudioClip(L"boss_atk_rush_110bpm_withmelody");
						MelodySound->Play();
						IsSoundChange = true;
					}
					else
					{
						if (MelodySound->GetState() == DirectX::SoundState::STOPPED)
						{
							MelodySound->Play();
						}
					}
				}
				break;
			}
			m_BossFrogState->SetTrafficLightColor(BossFrogState::BossFrogTrafficLight::Yellow);

			hos::Vector3 _RotDir = BossFrogTransform->GetForward().Cross(BossFrogTransform->GetPosition());
			hos::Rotator _Rot = BossFrogTransform->GetRotation();

			float _RotY = 0.0f;
			if (_RotDir.y < -0.1f)
			{
				if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_left") != 0)
				{
					m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_left");
				}

				_RotY = _Rot.y;
				_RotY -= (float)Time->DeltaTime() * (RotSpeed * 2.5f) * m_BossFrogState->GetAnimationTimeScale();
				_Rot.y = _RotY;
				BossFrogTransform->SetRotation(_Rot);
			}
			else if (_RotDir.y > 0.1f)
			{
				if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"walk_right") != 0)
				{
					m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"walk_right");
				}

				_RotY = _Rot.y;
				_RotY += (float)Time->DeltaTime() * (RotSpeed * 2.5f) * m_BossFrogState->GetAnimationTimeScale();
				_Rot.y = _RotY;
				BossFrogTransform->SetRotation(_Rot);
			}
			else
			{
				if (m_BossFrogState->BossFrogAnimation->GetAniClipName().compare(L"idle") != 0)
				{
					m_BossFrogState->BossFrogAnimation->SetAnimation(L"boss", L"idle");
				}
			}
			return static_cast<int>(ACTION_RESULT::SUCESS);
		}
		else
		{
			return static_cast<int>(ACTION_RESULT::FAILURE);
		}
	};

	Bt->Insert(debug_service_node, static_cast<int>(ON_AI_TYPE::ROOT));
	Bt->Insert(no_ai_select, static_cast<int>(NO_AI_TYPE::DEBUG_SERVICE_NODE));
	Bt->Insert(task_move, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_idle, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_dead, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_tongue, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_backmove, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_backjump, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_rotate_to_target, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_jump_att, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_rush_att, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_range_att, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
	Bt->Insert(task_search_target, static_cast<int>(NO_AI_TYPE::NO_AI_SELECT));
}