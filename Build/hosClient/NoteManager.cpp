#include "NoteManager.h"


NoteManager::NoteManager() : hos::com::Script(L"NoteManager")
{
	ExecutionOrder = 0;
}

NoteManager::~NoteManager()
{
	while (RightNoteUI.size() != 0)
	{
		RightNoteUI.pop_back();
		LeftNoteUI.pop_back();
	}

	while (BGMNoteInfo.size() != 0)
	{
		NoteInfo* _DeleteNoteInfo = BGMNoteInfo[BGMNoteInfo.size() - 1];
		BGMNoteInfo.pop_back();
		_DeleteNoteInfo->RemoveNoteData();
		delete _DeleteNoteInfo;
		_DeleteNoteInfo = nullptr;
	}

	delete[] ViewNoteIndex;
	delete[] IsViewNoteIndexEnd;
}

void NoteManager::Reset()
{

}

void NoteManager::Awake()
{
	/// 하나부터 하자

	// 비 전투구간, 전투 구간, 보스 구간

	// 노트 데이터를 불러와야 함
	//m_NoteInfo->Load(g_Path + L"\\Assets\\Data\\BossZone.bnd");


	NoteInfo* _NewNoteInfo = new NoteInfo();
	BGMNoteInfo.push_back(_NewNoteInfo);

	for (int i = 0; i < (int)BGMList::MAX; i++)
	{
		NoteInfo* _NewNoteInfo = new NoteInfo();
		BGMNoteInfo.push_back(_NewNoteInfo);
	}

	BGMNoteInfo[(int)BGMList::PeaceZone]->Load(g_Path + L"\\Assets\\Data\\PeaceZone.bnd");
	//BGMNoteInfo[(int)BGMList::PeaceZone]->Load(g_Path + L"\\Assets\\Data\\BossZone.bnd");
	BGMNoteInfo[(int)BGMList::CombatZone]->Load(g_Path + L"\\Assets\\Data\\CombatZone.bnd");
	BGMNoteInfo[(int)BGMList::BossZone]->Load(g_Path + L"\\Assets\\Data\\BossZone.bnd");

	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\peacezone_loop_100bpm.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\combatzone_loop_100bpm.sclip");
	g_DataManager->LoadAudioClip(g_Path + L"\\Assets\\Sound\\bgm_boss_2_loop.sclip");


	// 오디오 소스도 가지고 있어야할까
	m_AudioSource = m_GameObject->GetComponent<hos::com::AudioSource>();

	m_AudioSource->SetAudioClip(L"peacezone_loop_100bpm");
	m_AudioSource->Stop();

	// 중앙 판정 위치 추가
	CenterPosition = m_GameObject->m_Scene->FindGameObjectWithName(L"CenterNote")->GetComponent<hos::com::Transform>();

	InputDelayRange = BGMNoteInfo[NowBGM]->GetNoteList()[0] * InputDelayScale;

	OneBeatTime = BGMNoteInfo[NowBGM]->GetNoteList()[0];

	HalfBeatTime = OneBeatTime * 0.5f;

	CenterUIMeshRenderer = CenterPosition->GetChild(0)->m_GameObject->transform->GetChild(0)->m_GameObject->GetComponent<hos::com::MeshRenderer>();

	g_DataManager->LoadTexture(L"UI6_HitPoint_Emissive.tga");
	g_DataManager->LoadTexture(L"UI6_HitPoint_Emissive_faild.tga");
	g_DataManager->LoadTexture(L"UI6_HitPoint_Emissive_success.tga");

}

void NoteManager::Start()
{
	// 노트 데이터에 맞게 UI를 배치해야 함
	// NoteInfo에 맞게 좌우 UI 생성해서 배치

	hos::com::Transform* _NoteTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"LeftHalfNote")->GetComponent<hos::com::Transform>();

	LeftNoteUI.push_back(_NoteTransform);

	_NoteTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"LeftNote")->GetComponent<hos::com::Transform>();

	LeftNoteUI.push_back(_NoteTransform);

	_NoteTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"RightHalfNote")->GetComponent<hos::com::Transform>();

	RightNoteUI.push_back(_NoteTransform);

	_NoteTransform = m_GameObject->m_Scene->FindGameObjectWithName(L"RightNote")->GetComponent<hos::com::Transform>();

	RightNoteUI.push_back(_NoteTransform);

	//for (int i = 0; i < m_NoteInfo.m_NoteData.ViewNoteCount; i++)
	//{
	//	hos::com::GameObject* _RightNoteUIObject = new hos::com::GameObject();
	//	_RightNoteUIObject->Load(L"NoteUI.prefab");
	//	m_GameObject->m_Scene->AddGameObject(_RightNoteUIObject, m_GameObject);
	//	_RightNoteUIObject->SetName(L"RigthNoteUI");
	//	RightNoteUI.push_back(_RightNoteUIObject->GetComponent<hos::com::Transform>());
	//	_RightNoteUIObject->GetComponent<hos::com::Transform>()->SetLocalPosition(CenterPosition->GetLocalPosition());

	//	hos::com::GameObject* _LeftNoteUIObject = new hos::com::GameObject();
	//	_LeftNoteUIObject->Load(L"NoteUI.prefab");
	//	m_GameObject->m_Scene->AddGameObject(_LeftNoteUIObject, m_GameObject);
	//	_LeftNoteUIObject->SetName(L"LeftNoteUI");
	//	LeftNoteUI.push_back(_LeftNoteUIObject->GetComponent<hos::com::Transform>());
	//	_LeftNoteUIObject->GetComponent<hos::com::Transform>()->SetLocalPosition(CenterPosition->GetLocalPosition());
	//}

	ViewNoteIndex = new int[BGMNoteInfo[NowBGM]->m_NoteData.ViewNoteCount];
	IsViewNoteIndexEnd = new bool[BGMNoteInfo[NowBGM]->m_NoteData.ViewNoteCount];

	for (int i = 0; i < BGMNoteInfo[NowBGM]->m_NoteData.ViewNoteCount; i++)
	{
		ViewNoteIndex[i] = i;
		IsViewNoteIndexEnd[i] = false;
	}

}

void NoteManager::Update()
{

	NoteStateUpdate();

	// 노트 UI를 갱신하여 움직이게 한다.
	MoveNoteUI();

	ChangeCenterUI();

}

void NoteManager::MusicStart()
{
	IsBGMStart = !IsBGMStart;

	//if (IsBGMStart)
	//{
	//	m_AudioSource->Play();
	//}
	//else
	//{
	//	m_AudioSource->Stop();
	//}
}


void NoteManager::Render()
{
	//int x = 700;
	//int y = 200;
	//int font = 12;

	//g_Renderer->_DebugText(x, y, font, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"BGM Time : %f", NowBGMTime);
	//g_Renderer->_DebugText(x, y + 15, font, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"End BGM: %f", m_NoteInfo->m_NoteData.SoundLength);
	/*


	switch (ErrorCheck)
	{
	case NoteManager::NoteErrorCheck::Wait:
		g_Renderer->_DrawText(x, y + 30, font, DirectX::SimpleMath::Vector4(1, 0.3f, 1, 1), L"Wait");
		break;
	case NoteManager::NoteErrorCheck::Success:
		g_Renderer->_DrawText(x, y + 30, font, DirectX::SimpleMath::Vector4(0, 1, 0, 1), L"Success");
		break;
	case NoteManager::NoteErrorCheck::Fail:
		g_Renderer->_DrawText(x, y + 30, font, DirectX::SimpleMath::Vector4(1, 0, 0, 1), L"Fail");
		break;
	default:
		break;
	}

	switch (ErrorRangeCheck)
	{
	case NoteManager::NoteErrorRangeCheck::CP:
		g_Renderer->_DrawText(x, y + 45, font, DirectX::SimpleMath::Vector4(1, 1, 0, 1), L"Critical Perfect!!!");
		break;
	case NoteManager::NoteErrorRangeCheck::P:
		g_Renderer->_DrawText(x, y + 45, font, DirectX::SimpleMath::Vector4(1, 0.7f, 0, 1), L"Perfect!!");
		break;
	case NoteManager::NoteErrorRangeCheck::C:
		g_Renderer->_DrawText(x, y + 45, font, DirectX::SimpleMath::Vector4(1, 0.4f, 0.3f, 1), L"Cool!");
		break;
	case NoteManager::NoteErrorRangeCheck::N:
		g_Renderer->_DrawText(x, y + 45, font, DirectX::SimpleMath::Vector4(1, 0.2f, 0.5f, 1), L"Nice");
		break;
	case NoteManager::NoteErrorRangeCheck::M:
		g_Renderer->_DrawText(x, y + 45, font, DirectX::SimpleMath::Vector4(1, 0, 0, 1), L"Miss");
		break;
	case NoteManager::NoteErrorRangeCheck::W:
		g_Renderer->_DrawText(x, y + 45, font, DirectX::SimpleMath::Vector4(1, 0.3f, 1, 1), L"Wait");
		break;
	default:
		break;
	}

	//g_Renderer->_DrawText(x, y + 60, font, DirectX::SimpleMath::Vector4(1, 0.3f, 1, 1), L"%d", LoopCount);

	//for (int i = 0; i < m_NoteInfo->m_NoteData.ViewNoteCount; i++)
	//{
	//	g_Renderer->_DrawText(x + 300, y + (i * 15), font, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"%d", ViewNoteIndex[i]);
	//}
	*/
}

void NoteManager::NoteStateUpdate()
{
	if (IsBGMStart)
	{
		m_AudioSource->Play();
		StartNote();
	}
	else
	{
		m_AudioSource->Stop();
		StopNote();
	}
}

NoteManager::NoteErrorRangeCheck NoteManager::CheckUserInput()
{
	//////////////////////////////////////////////////////////////////////////
	// 판정 확인

	double _RangeCheck = 0.f;

	_RangeCheck = NowBGMTime - BGMNoteInfo[NowBGM]->GetNoteList()[PrevNote];

	if (_RangeCheck < 0)
	{
		_RangeCheck = -_RangeCheck;
	}

	ErrorRange = _RangeCheck;

	if (ErrorRange <= HalfBeatTime * 0.1f)
	{
		ErrorRangeCheck = NoteErrorRangeCheck::CP;
	}
	else if (ErrorRange <= HalfBeatTime * 0.4f)
	{
		ErrorRangeCheck = NoteErrorRangeCheck::P;
	}
	else if (ErrorRange <= HalfBeatTime * 0.7f)
	{
		ErrorRangeCheck = NoteErrorRangeCheck::C;
	}
	else if (ErrorRange <= HalfBeatTime * 0.9f)
	{
		ErrorRangeCheck = NoteErrorRangeCheck::N;
	}
	else
	{
		ErrorRangeCheck = NoteErrorRangeCheck::M;
		IsMiss = true;
	}

	if (!IsMiss)
	{
		IsSuccess = true;
	}

	return ErrorRangeCheck;

	//////////////////////////////////////////////////////////////////////////
	// 판정에 따른 행동
	/*
	if (ErrorCheck != NoteErrorCheck::Fail)
	{
		if (!IsPressButton)
		{
			IsPressButton = true;

			if (NowBGMTime > m_NoteInfo->GetNoteList()[PrevNote] - m_NoteInfo->m_NoteData.NoteInputTime &&
				NowBGMTime < m_NoteInfo->GetNoteList()[PrevNote] + m_NoteInfo->m_NoteData.NoteInputTime)
			{
				ErrorCheck = NoteErrorCheck::Success;
				IsInputSuccess = true;
				IsHalfBeat = true;

				// TODO : 사운드 관련 스크립트 컴포넌트로 옮겨 갈 것
				SFXAudioSource->Stop();
				SFXAudioSource->Play();

				return true;
			}
			else
			{
				ErrorCheck = NoteErrorCheck::Fail;
				IsInputSuccess = false;
				IsHalfBeat = false;

				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	*/
}

void NoteManager::StartNote()
{
	NowBGMTime += Time->DeltaTime();

	IsChangeNote = false;

	if (NowBGMTime > BGMNoteInfo[NowBGM]->m_NoteData.SoundLength)
	{
		NowBGMTime = 0.0f;
		NowNote = 0;
		LoopCount++;
		//IsBGMEnd = false;
		for (int i = 0; i < BGMNoteInfo[NowBGM]->m_NoteData.ViewNoteCount; i++)
		{
			ViewNoteIndex[i] = i;
			IsViewNoteIndexEnd[i] = false;
		}

	}

	if (NowBGMTime > BGMNoteInfo[NowBGM]->GetNoteList()[NowNote])
	{
		NowNote++;
	}

	//if (NowBGMTime > BGMNoteInfo[NowBGM]->GetNoteList()[PrevNote] + BGMNoteInfo[NowBGM]->m_NoteData.NoteInputTime || NowNote < PrevNote)
	if (NowBGMTime >= BGMNoteInfo[NowBGM]->GetNoteList()[PrevNote] + HalfBeatTime || NowNote < PrevNote)
	{
		PrevNote = NowNote;
		IsChangeNote = true;

		if (IsHalfBeat)
		{
			IsHalfBeat = !IsHalfBeat;
		}
		else
		{
			if (ErrorCheck == NoteErrorCheck::Success && IsInputSuccess)
			{
				IsInputSuccess = false;
			}
			else
			{
				ErrorRangeCheck = NoteErrorRangeCheck::W;
				ErrorCheck = NoteErrorCheck::Wait;
				IsInputSuccess = false;
			}
		}
	}

	IsPressButton = false;

}



void NoteManager::StopNote()
{
	NowBGMTime = 0.0f;
	NowNote = 0;
	PrevNote = 0;
	ErrorRange = 0.0f;
	ErrorCheck = NoteErrorCheck::Wait;
	ErrorRangeCheck = NoteErrorRangeCheck::W;
	IsPressButton = false;
	IsChangeNote = false;
	IsInputSuccess = false;
	IsHalfBeat = false;
	LoopCount = 0;


	for (int i = 0; i < BGMNoteInfo[NowBGM]->m_NoteData.ViewNoteCount; i++)
	{
		ViewNoteIndex[i] = i;
		IsViewNoteIndexEnd[i] = false;
	}

}

void NoteManager::MoveNoteUI()
{
	for (int i = 0; i < BGMNoteInfo[NowBGM]->m_NoteData.ViewNoteCount; i++)
	{
		// Left
		float _lerp = GetNoteLerpPosition(i, true);

		hos::Vector3 _temp;

		_temp.x = _lerp;
		_temp.y = LeftNoteUI[i]->GetLocalPosition().y;
		_temp.z = LeftNoteUI[i]->GetLocalPosition().z;

		LeftNoteUI[i]->SetLocalPosition(_temp);

		// Right
		_lerp = GetNoteLerpPosition(i, false);

		_temp.x = _lerp;
		_temp.y = RightNoteUI[i]->GetLocalPosition().y;
		_temp.z = RightNoteUI[i]->GetLocalPosition().z;

		RightNoteUI[i]->SetLocalPosition(_temp);
	}
}

float NoteManager::GetNoteLerpPosition(int noteIndex, bool isLeft)
{
	double _a = 0;

	if (isLeft)
	{
		_a = (double)CenterPosition->GetLocalPosition().x - BGMNoteInfo[NowBGM]->m_NoteData.NoteUIDistance;
	}
	else
	{
		_a = (double)CenterPosition->GetLocalPosition().x + BGMNoteInfo[NowBGM]->m_NoteData.NoteUIDistance;
	}

	int j = 0;

	double _g = 0;



	// 끝에 다다랐을 때
	if (BGMNoteInfo[NowBGM]->m_NoteData.NoteCount - 1 < ViewNoteIndex[noteIndex] + BGMNoteInfo[NowBGM]->m_NoteData.ViewNoteCount)
	{
		// 넘어 갔을 때
		if (NowNote + BGMNoteInfo[NowBGM]->m_NoteData.ViewNoteCount > BGMNoteInfo[NowBGM]->m_NoteData.NoteCount)
		{
			// 넘어갔으니 앞으로 보내기
			if (NowNote > ViewNoteIndex[noteIndex])
			{
				ViewNoteIndex[noteIndex] = noteIndex;

				// 값이 바뀌지 않게 플래그 세우기
				IsViewNoteIndexEnd[noteIndex] = true;
			}
		}
	}
	else
	{
		// 현재 노트보다 작고, 플래그가 세워지지 않았으면 넘기기
		if (ViewNoteIndex[noteIndex] < NowNote && !IsViewNoteIndexEnd[noteIndex])
		{
			ViewNoteIndex[noteIndex] += BGMNoteInfo[NowBGM]->m_NoteData.ViewNoteCount;
		}
	}

	// 플래그가 세워져 있을 때
	if (IsViewNoteIndexEnd[noteIndex])
	{
		_g = (BGMNoteInfo[NowBGM]->GetNoteList()[ViewNoteIndex[noteIndex]] + BGMNoteInfo[NowBGM]->m_NoteData.SoundLength);
	}
	else
	{
		_g = BGMNoteInfo[NowBGM]->GetNoteList()[ViewNoteIndex[noteIndex]];
	}


	//////////////////////////////////////////////////////////////////////////
	/*
	// 넘어갔다
	if ((NowNote - 1) + m_NoteInfo->m_NoteData.ViewNoteCount > m_NoteInfo->m_NoteData.NoteCount - 1)
	{
		// 넘어 갈 놈 찾기
		if (NowNote + noteIndex > m_NoteInfo->m_NoteData.NoteCount - 1)
		{
			j = NowNote + noteIndex - m_NoteInfo->m_NoteData.NoteCount;

			_g = m_NoteInfo->GetNoteList()[j] + m_NoteInfo->m_NoteData.SoundLength;
		}
		// 나머지
		else
		{
			j = noteIndex;
			_g = m_NoteInfo->GetNoteList()[NowNote + j];
		}
	}
	else
	{
		j = noteIndex;
		_g = m_NoteInfo->GetNoteList()[NowNote + j];
	}
	*/
	//////////////////////////////////////////////////////////////////////////

	double _h = NowBGMTime;

	double _alpha = (1 - (_g - _h));

	double _lerp = _a * (1 - _alpha) + CenterPosition->GetLocalPosition().x * _alpha;

	return (float)_lerp;

}

void NoteManager::ChangeBGM(NoteManager::BGMList val)
{
	if (val == BGMList::MAX)
	{
		return;
	}

	m_AudioSource->Stop();
	StopNote();

	switch (val)
	{
	case NoteManager::BGMList::PeaceZone:
	{
		m_AudioSource->SetAudioClip(L"peacezone_loop_100bpm");
		NowBGM = (int)BGMList::PeaceZone;
	}
	break;
	case NoteManager::BGMList::CombatZone:
	{
		m_AudioSource->SetAudioClip(L"combatzone_loop_100bpm");
		NowBGM = (int)BGMList::CombatZone;
	}
	break;
	case NoteManager::BGMList::BossZone:
	{
		m_AudioSource->SetAudioClip(L"bgm_boss_2_loop");
		NowBGM = (int)BGMList::BossZone;
	}
	break;
	case NoteManager::BGMList::MAX:
	default:
		break;
	}

	InputDelayRange = BGMNoteInfo[NowBGM]->GetNoteList()[0] * InputDelayScale;

	OneBeatTime = BGMNoteInfo[NowBGM]->GetNoteList()[0];

	HalfBeatTime = OneBeatTime * 0.5f;

	m_AudioSource->Play();
	StartNote();

}

void NoteManager::ChangeCenterUI()
{
	if (!IsChangeNote)
	{
		if (IsMiss)
		{
			CenterUIMeshRenderer->GetMaterialRef()->SetEmissTex(L"UI6_HitPoint_Emissive_faild.tga");
		}

		if (IsSuccess)
		{
			CenterUIMeshRenderer->GetMaterialRef()->SetEmissTex(L"UI6_HitPoint_Emissive_success.tga");
		}
	}
	else
	{
		IsMiss = false;
		IsSuccess = false;
		CenterUIMeshRenderer->GetMaterialRef()->SetEmissTex(L"UI6_HitPoint_Emissive.tga");
	}
}