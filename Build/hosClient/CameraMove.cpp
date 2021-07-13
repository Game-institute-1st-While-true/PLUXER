#include "CameraMove.h"

using namespace hos;
using namespace com;


void CameraMove::Awake()
{
	CameraTransform = m_GameObject->GetComponent<Transform>();
	//PlayerViewCamera = m_GameObject->GetComponent<Camera>();
	//PlayerTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"Player")->GetComponent<Transform>();

	//ThetaY = 0;
	//PosY = 3;
	//Distance = 5;
	//MoveSpeed = 5;

	//PrevMouseMove = 0.f;
	//NowMouseMove = 0.f;

}

void CameraMove::Update()
{

	if (Input->GetKeyUp(KeyCode::NumPad0))
	{
		//IsDebugCamera = !IsDebugCamera;
	}

	if (IsDebugCamera)
	{
		m_GameObject->GetComponent<Camera>()->ChangeMode(Camera::eMode::DEFAULT);

		Vector3 pos = m_GameObject->transform->GetPosition();
		Rotator q = m_GameObject->transform->GetRotation();

		float force = 10;
		if (Input->GetKey(KeyCode::NumPad8))
		{
			Vector3 dir = m_GameObject->transform->GetForward();
			pos = pos - (dir * force * (float)Time->DeltaTime());
			m_GameObject->transform->SetPosition(pos);
		}
		if (Input->GetKey(KeyCode::NumPad5))
		{
			Vector3 dir = m_GameObject->transform->GetForward();
			pos = pos + (dir * force * (float)Time->DeltaTime());
			m_GameObject->transform->SetPosition(pos);
		}
		if (Input->GetKey(KeyCode::NumPad6))
		{
			Vector3 dir = m_GameObject->transform->GetLeft();
			pos = pos - (dir * force * (float)Time->DeltaTime());
			m_GameObject->transform->SetPosition(pos);
		}
		if (Input->GetKey(KeyCode::NumPad4))
		{
			Vector3 dir = m_GameObject->transform->GetLeft();
			pos = pos + (dir * force * (float)Time->DeltaTime());
			m_GameObject->transform->SetPosition(pos);
		}

		if (Input->GetKey(KeyCode::NumPad7))
		{
			q.y -= 100 * (float)Time->DeltaTime();
			m_GameObject->transform->SetRotation(q);
		}

		if (Input->GetKey(KeyCode::NumPad9))
		{
			q.y += 100 * (float)Time->DeltaTime();
			m_GameObject->transform->SetRotation(q);
		}

		if (Input->GetKey(KeyCode::PageUp))
		{
			q.x -= 100 * (float)Time->DeltaTime();
			m_GameObject->transform->SetRotation(q);
		}

		if (Input->GetKey(KeyCode::PageDown))
		{
			q.x += 100 * (float)Time->DeltaTime();
			m_GameObject->transform->SetRotation(q);
		}
	}
	/*//
	else
	{
		NowMouseMove = Input->GetAxis(Axis::MouseX);

		float _DeltaMouseMove = PrevMouseMove - NowMouseMove;

		if (_DeltaMouseMove < 0)
		{
			ThetaY += Time->DeltaTime() * MoveSpeed;
		}

		if (_DeltaMouseMove > 0)
		{
			ThetaY -= Time->DeltaTime() * MoveSpeed;
		}

		PrevMouseMove = NowMouseMove;

		if (PlayerViewCamera->GetMode() != Camera::eMode::FOLLOW)
		{
			PlayerViewCamera->ChangeMode(Camera::eMode::FOLLOW);
		}

		// 캐릭터의 방향
		//Vector3 _Forward = PlayerTransform->GetForward();
		//Vector3 _Forward = Vector3(0,0,1);
		Matrix mRotY = Matrix::CreateRotationY(ThetaY);

		DirectX::SimpleMath::Vector3 _result = XMVector3Transform(Vector3::Forward, mRotY);

		_result.Normalize();
		_result = _result * Distance;

		DirectX::SimpleMath::Vector3 playerPosition = PlayerTransform->GetPosition();
		DirectX::SimpleMath::Vector3 cameraPosition = playerPosition - _result;

		cameraPosition.y = PosY;

		CameraTransform->SetPosition(cameraPosition);

		PlayerViewCamera->SetFollowTarget(PlayerTransform->GetPosition());

		//////////////////////////////////////////////////////////////////////////
		// 카메라를 플레이어 방향 쪽으로 회전시키자

		float degreeY = DirectX::XMConvertToDegrees(ThetaY);

		CameraTransform->SetRotation(Rotator(0, degreeY, 0));

		
		//Vector3 cameraForward = cameraPosition;
		////Vector3 cameraForward = playerPosition - cameraPosition;

		//cameraForward.y = 0;

		//cameraForward.Normalize();

		//Vector3 cameraRotation;

		//float cameraRotationY = cameraForward.Dot(CameraTransform->GetForward());

		//cameraRotationY = DirectX::XMConvertToDegrees(cameraRotationY);

		//cameraRotation.y = cameraRotationY;

		//CameraTransform->SetRotation(cameraRotation);
		
	}
	//*/
}


void CameraMove::Render()
{
	//int x = 1000;
	//int y = 0;
	//int font = 12;

	//g_Renderer->_DrawText(x, y, font, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"Camera Position : %f %f %f", CameraTransform->GetPosition().x, CameraTransform->GetPosition().y, CameraTransform->GetPosition().z);
	//g_Renderer->_DrawText(x, y + 15, font, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"Camera Rotation : %f %f %f", CameraTransform->GetRotation().x, CameraTransform->GetRotation().y, CameraTransform->GetRotation().z);
	//g_Renderer->_DrawText(x, y + 30, font, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"Camera Forward : %f %f %f", CameraTransform->GetForward().x, CameraTransform->GetForward().y, CameraTransform->GetForward().z);
	//g_Renderer->_DrawText(x, y + 45, font, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"Player Position : %f %f %f", PlayerTransform->GetPosition().x, PlayerTransform->GetPosition().y, PlayerTransform->GetPosition().z);
	//g_Renderer->_DrawText(x, y + 60, font, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"Player Rotation : %f %f %f", PlayerTransform->GetRotation().x, PlayerTransform->GetRotation().y, PlayerTransform->GetRotation().z);
	//g_Renderer->_DrawText(x, y + 75, font, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"ThetaY : %f", ThetaY); 
}

void CameraMove::Reset()
{
}
