#include "hosUtility\Dump.h"
#include "hosEngine\COM_Client.h"
#include "hosEngine\COM_Testbed.h"
#include "SceneManager.h"
#include <filesystem>
#include <thread>
#include "resource.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(419467);
#endif
	Dump minidump(L"minidump");

	HANDLE process;
	process = CreateSemaphore(NULL, 1, 1, L"HOS_CLIENT");

	if (NULL == process)
	{
		return 1;
	}

	DWORD result = WaitForSingleObject(process, 0);

	if (result == WAIT_TIMEOUT)
	{
		//ReleaseSemaphore(process,1,NULL);
		CloseHandle(process);
		return 2;
	}
	else if(result == WAIT_OBJECT_0)
	{
		//Main Client
		hos::com::COM_Client* system = new hos::com::COM_Client(hInstance);

		//Offline Client
		//hos::com::COM_Testbed* system = new hos::com::COM_Testbed(hInstance);

		HCURSOR cur = LoadCursorFromFileW((g_Path + L"\\Assets\\Data\\cursor.cur").c_str());
		if (cur != NULL)
		{
			Input->SetMouseCursor(cur);
		}
		HICON ico = LoadIconW(system->GetApp()->GetH_Ins(), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(system->GetApp()->GetH_Wnd(), WM_SETICON, ICON_SMALL, (LPARAM)ico);
		SendMessage(system->GetApp()->GetH_Wnd(), WM_SETICON, ICON_BIG, (LPARAM)ico);

	#pragma region loading
		HANDLE loadEnd = CreateEvent(NULL, TRUE, FALSE, NULL);
		std::thread loadthread = std::thread([&] {
			g_DataManager->LoadTexture(L"loading_background_back.png");
			g_DataManager->LoadTexture(L"loading_background.png");
			g_DataManager->LoadTexture(L"loading_circle.png");
			g_DataManager->LoadTexture(L"loading_circle_2.png");
			g_DataManager->LoadTexture(L"loading_circle_3.png");
			g_DataManager->LoadTexture(L"loading_font.png");
			g_DataManager->LoadTexture(L"loading_o.png");

			RECT rt = system->GetApp()->GetSize();
			hos::U32 width = rt.right - rt.left;
			hos::U32 height = rt.bottom - rt.top;

			hos::cg::RenderQue::RenderUIObejct loadingBack;
			loadingBack.Width = width;
			loadingBack.Height = height;
			loadingBack.bTexture = true;
			loadingBack.TextureName = L"loading_background_back.png";

			hos::cg::RenderQue::RenderUIObejct loadingBack2;
			loadingBack2.Width = width;
			loadingBack2.Height = height;
			loadingBack2.bTexture = true;
			loadingBack2.TextureName = L"loading_background.png";
			loadingBack2.LayerOrder = 1;

			hos::cg::RenderQue::RenderUIObejct loadingCircle;
			loadingCircle.Width = 446;
			loadingCircle.Height = 447;
			loadingCircle.bTexture = true;
			loadingCircle.TextureName = L"loading_circle.png";
			loadingCircle.LayerOrder = 1;
			loadingCircle.Align = 5;
			loadingCircle.Pos = hos::Vector3(width / 2.f, height / 2.f);

			hos::cg::RenderQue::RenderUIObejct loadingCircle2;
			loadingCircle2.Width = 371;
			loadingCircle2.Height = 371;
			loadingCircle2.bTexture = true;
			loadingCircle2.TextureName = L"loading_circle_2.png";
			loadingCircle2.LayerOrder = 1;
			loadingCircle2.Align = 5;
			loadingCircle2.Pos = hos::Vector3(width / 2.f, height / 2.f);

			hos::cg::RenderQue::RenderUIObejct loadingCircle3;
			loadingCircle3.Width = 338;
			loadingCircle3.Height = 333;
			loadingCircle3.bTexture = true;
			loadingCircle3.TextureName = L"loading_circle_3.png";
			loadingCircle3.LayerOrder = 1;
			loadingCircle3.Align = 5;
			loadingCircle3.Pos = hos::Vector3(width / 2.f, height / 2.f);

			hos::cg::RenderQue::RenderUIObejct loadingFont;
			loadingFont.Width = 272;
			loadingFont.Height = 44;
			loadingFont.bTexture = true;
			loadingFont.TextureName = L"loading_font.png";
			loadingFont.LayerOrder = 1;
			loadingFont.Align = 5;
			loadingFont.Pos = hos::Vector3(width / 2.f, height / 2.f);

			hos::cg::RenderQue::RenderUIObejct loadingO1;
			loadingO1.Width = 11;
			loadingO1.Height = 11;
			loadingO1.bTexture = true;
			loadingO1.TextureName = L"loading_o.png";
			loadingO1.LayerOrder = 1;
			loadingO1.Align = 5;

			hos::cg::RenderQue::RenderUIObejct loadingO2;
			loadingO2.Width = 11;
			loadingO2.Height = 11;
			loadingO2.bTexture = true;
			loadingO2.TextureName = L"loading_o.png";
			loadingO2.LayerOrder = 1;
			loadingO2.Align = 5;

			hos::cg::RenderQue::RenderUIObejct loadingO3;
			loadingO3.Width = 11;
			loadingO3.Height = 11;
			loadingO3.bTexture = true;
			loadingO3.TextureName = L"loading_o.png";
			loadingO3.LayerOrder = 1;
			loadingO3.Align = 5;

			bool isVisiable = false;
			double deltaTime;
			double TickCount = 0;
			float OPosUp = (float)(height / 2 + 50);
			float OPosDown = (float)(height / 2 + 70);
			float OLeft = (float)(width / 2 - 40);
			float OCenter = (float)(width / 2);
			float ORight = (float)(width / 2 + 40);
			int tick = 0;
			while (true)
			{
				if (system->Update() == false)
				{
					break;
				}

				deltaTime = Time->DeltaTime();
				TickCount += deltaTime;
				if (g_Renderer)
				{
					g_Renderer->AddRenderUIObejct(loadingBack);
					g_Renderer->AddRenderUIObejct(loadingBack2);
					loadingCircle.Rot.z += (float)(deltaTime * 0.1);
					g_Renderer->AddRenderUIObejct(loadingCircle);
					loadingCircle2.Rot.z -= (float)(deltaTime * 0.2);
					g_Renderer->AddRenderUIObejct(loadingCircle2);
					g_Renderer->AddRenderUIObejct(loadingCircle3);
					g_Renderer->AddRenderUIObejct(loadingFont);

					if (TickCount > 0.5)
					{
						tick++;
						if (tick >= 4)
						{
							tick = 0;
						}
						TickCount = 0;
					}
					switch (tick)
					{
					case 0:
						loadingO1.Pos = hos::Vector3(OLeft, OPosUp);
						loadingO2.Pos = hos::Vector3(OCenter, OPosDown);
						loadingO3.Pos = hos::Vector3(ORight, OPosDown);
						break;
					case 1:
						loadingO1.Pos = hos::Vector3(OLeft, OPosDown);
						loadingO2.Pos = hos::Vector3(OCenter, OPosUp);
						loadingO3.Pos = hos::Vector3(ORight, OPosDown);
						break;
					case 2:
						loadingO1.Pos = hos::Vector3(OLeft, OPosDown);
						loadingO2.Pos = hos::Vector3(OCenter, OPosDown);
						loadingO3.Pos = hos::Vector3(ORight, OPosUp);
						break;
					case 3:
						loadingO1.Pos = hos::Vector3(OLeft, OPosDown);
						loadingO2.Pos = hos::Vector3(OCenter, OPosDown);
						loadingO3.Pos = hos::Vector3(ORight, OPosDown);
						break;
					}
					g_Renderer->AddRenderUIObejct(loadingO1);
					g_Renderer->AddRenderUIObejct(loadingO2);
					g_Renderer->AddRenderUIObejct(loadingO3);

					RECT windowSize = system->GetApp()->GetSize();

					g_Renderer->ForwardRender_ToCameraBuffer(0.0f, 0.1f, hos::Vector3(), hos::Vector3(), 90.f, (float)width, (float)height, 0.1f, 100.f, hos::cg::RendererDesc::CameraModeList::CAMERA3D_DEFAULT, 0, 0, false, L"", hos::Vector4(), bDebug);
					g_Renderer->BeginDraw(windowSize.right, windowSize.bottom);
					g_Renderer->RenderUI_ToSwapChainBuffer();
					g_Renderer->EndDraw();
				}

				DWORD result = WaitForSingleObject(loadEnd, 1);
				if (result == WAIT_OBJECT_0)
				{
					break;
				}
			}
			});

		system->Load();
		SetEvent(loadEnd);
		loadthread.join();
	#pragma endregion

		g_Renderer->SetViewFrustum(false);
		SceneManagement = new hos::SceneManager();
		SceneManagement->ChangeScene(hos::SceneManager::START_SCENE);
		bool Valid = false;
		hos::com::Scene* scene = nullptr;
		while (true)
		{
			if (system->Update() == false)
			{
				break;
			}
			if (SceneManagement->IsLoop())
			{
				scene = SceneManagement->GetScene();
				if (nullptr == scene)
				{
					if (!Valid)
					{
						if (MessageBox(system->GetApp()->GetH_Wnd(), L"실행가능한 씬이 없습니다.\n종료하시겠습니까?", L"Error", MB_YESNO | MB_ICONERROR) == IDYES)
						{
							system->GetApp()->Destory();
							break;
						}
						else
						{
							Valid = true;
						}
					}
					continue;
				}

				RECT windowRECT = system->GetApp()->GetSize();
				float aspect_x = ((float)(windowRECT.right - windowRECT.left) / 1920.f);
				float aspect_y = ((float)(windowRECT.bottom - windowRECT.top) / 1080.f);

				scene->OnGUI(aspect_x, aspect_y);
				scene->FixedUpdate();
				scene->Update();
				scene->PhysicsUpdate();
				scene->LateUpdate();
				scene->PreRender();

			#pragma region ScreenMode Test
				//	RECT windowRECT = system->GetApp()->GetMonitorSize();
				//	g_Renderer->_DebugText(100, 100, 30, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"%d, %d", windowRECT.right, windowRECT.bottom);

				//	bool bFull = g_Renderer->GetFullScreenState();

				//	if (bFull)
				//	{
				//		g_Renderer->_DebugText(100, 200, 30, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"FullScreen Mode");
				//	}

				//	else
				//	{
				//		g_Renderer->_DebugText(100, 200, 30, DirectX::SimpleMath::Vector4(1, 1, 1, 1), L"Window Mode");
				//}
			#pragma endregion

				scene->Render();

				// Device RenderTarget Set
				if (g_Renderer)
				{
					RECT windowSize = system->GetApp()->GetSize();
					g_Renderer->BeginDraw(windowSize.right, windowSize.bottom);
				}
				scene->PostRender();

				g_Renderer->RenderUI_ToSwapChainBuffer();

				// Present
				if (g_Renderer)
				{
					g_Renderer->EndDraw();
				}

				SceneManagement->CheckScene();
			}
			else
			{
				break;
			}
		}
		hos::string dirpath = std::filesystem::current_path();
		hos::string iniName = L"\\cookie.ini";

		hos::string CookiePath = dirpath + iniName;

		WritePrivateProfileStringW(L"Login", L"CERTIFICATION", L"FALSE", CookiePath.c_str());
		WritePrivateProfileStringW(L"Login", L"ID", L"", CookiePath.c_str());
		WritePrivateProfileStringW(L"Login", L"PW", L"", CookiePath.c_str());
		WritePrivateProfileStringW(L"Login", L"CODE", L"-1", CookiePath.c_str());

		delete g_Navigation;
		g_Navigation = nullptr;

		delete SceneManagement;
		delete system;
	}

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	CloseHandle(process);
	return 0;
}