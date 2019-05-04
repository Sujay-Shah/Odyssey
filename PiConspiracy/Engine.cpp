//
//  Engine.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//


//bool EDITOR_ = true;

#include "PiEnginePreCompiled.h"
#include "Engine.h"
#include "LogManager.h"
#include "Renderer.h"
#include "FrameRateController.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "ImguiManager.h"
#include "MemoryManager.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "ThreadManager.h"
#include "PhysicsManager.h"
#include "StatsManager.h"
#include "ScriptManager.h"
#include "UIManager.h"
#include "ProjectConfig.h"
#include "Camera.h"



//DEBUG
#include "ObjectFactory.h"
// Required for SDL to compile because of references to fprintf
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
    return _iob;
}

#pragma comment(lib, "legacy_stdio_definitions.lib")


Engine::Engine() :
	m_pWindow(nullptr),
	m_bApplicationIsRunning(false),
	m_uWindowWidth(WINDOW_WIDTH),
	m_uWindowHeight(WINDOW_HEIGHT),
	m_bFullScreen(false),
	m_fDeltaTime(0.0f),
	m_dFPS(0.0),
	m_paused(false),
	m_pFrameRateController(nullptr),
	m_resourceManager(nullptr),
	m_inputManager(nullptr),
	m_gameObjectManager(nullptr),
	m_sceneManager(nullptr),
	m_pRenderer(nullptr),
	m_pImguiManager(nullptr),
	m_physicsManager(nullptr),
	//m_levelEditor(nullptr),
	m_statsManager(nullptr),
	m_uiManager(nullptr),
	m_scriptManager(nullptr),
	m_isInGame(false),
	m_bIsDebugDraw(false),
	m_IsPlayerDead(false),
	m_isGameSlowed(false),
	m_loadMmenu(false),
	m_digLogoDone(false)
{
    m_pLogManager = new LogManager;
    LOG_INFO_ARG("Welcome to PiEngine! Build date: {} {}", __DATE__, __TIME__);
}

Engine::~Engine()
{
    SAFE_DELETE(m_pLogManager);
   //SAFE_DELETE(m_eventManager);//Added to rem mem leaks

		//add mem errors
		SAFE_DELETE(m_pFrameRateController);
		SAFE_DELETE(m_resourceManager);
		SAFE_DELETE(m_pMemoryManager);
		SAFE_DELETE(m_eventManager);
		SAFE_DELETE(m_inputManager);
		SAFE_DELETE(m_gameObjectManager);
		SAFE_DELETE(m_sceneManager);
		SAFE_DELETE(m_audioManager);
		SAFE_DELETE(m_pRenderer);
		SAFE_DELETE(m_pImguiManager);
		SAFE_DELETE(m_physicsManager);
		
		SAFE_DELETE(m_statsManager);
		SAFE_DELETE(m_uiManager);
}

bool Engine::Initialize()
{
#ifdef _DEBUG
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle("Pi Console");
	}
#endif // _DEBUG




	srand(uint32(time(NULL)));

	// Initialize Systems/ Managers
	m_pFrameRateController	= new FrameRateController(60);
	m_resourceManager		= new ResourceManager();
	m_pMemoryManager		= new MemoryManager();
	m_eventManager			= new EventManager();
	m_inputManager			= new InputManager();
	m_gameObjectManager		= new GameObjectManager();
	m_sceneManager			= new SceneManager();
	m_audioManager			= new AudioManager();
	m_pRenderer				= new Renderer();
	m_pImguiManager			= new ImguiManager();
	m_physicsManager		= new PhysicsManager();
	//m_levelEditor			= new LevelEditor();
	m_statsManager			= new StatsManager();
	m_uiManager				= new UIManager();
	
	

	//m_scriptManager = new ScriptManager();
    if (!InitializeWindow())
    {
        return false;
    }

#if EDITOR
    m_levelEditor->Initialize(m_pWindow, &m_glContext);
#else
	m_pImguiManager->Initialize(m_pWindow, &m_glContext);
#endif

	// Initializing all managers
	// Make sure AudioManager Initializes before ResourceManager
	m_audioManager->Initialize();

	m_resourceManager->Initialize();
	m_pMemoryManager->Initialize();
	m_gameObjectManager->Initialize();
#if EDITOR
#else
  m_physicsManager->Init();
#endif

	//m_scriptManager->Initialize();
	m_sceneManager->Initialize("SceneCollection.json");
    m_pRenderer->Initialize();
	

	

	//DEBUG
	//m_gameObjectManager->m_Factory->CreateGameObject("object1");

#if EDITOR
    PiScene->ChangeScene(3);
#else
	//TODO: uncomment to include digipen logo
	if (PiResource->GetConfig()->m_presentation)
	{
		PiScene->ChangeScene(0);


		ControllerRecEvent* ct = new ControllerRecEvent;
		ct->SetEventTime(3.0f);
		PiEvent->AddEventToManagersQueue(ct);


		StorytEvent* st = new StorytEvent;
		st->SetEventTime(5.0f);
		PiEvent->AddEventToManagersQueue(st);

		


	}
	else
	{
		//TODO:specify scene number to change,remove after development
		if (PiResource->GetConfig()->m_withPlayer)
		{
			m_isInGame = true;
		}
		int start = PiResource->GetConfig()->m_startScene;
		PiScene->ChangeScene(start);
	}
	
#endif
	m_uiManager->Initialize("MenuCollection.json");
	m_audioManager->PlaySounds("MenuMusic", vec3(0.0f), 1.0f, false, false, false);
    return true;
}

bool Engine::Finalize()
{
    return true;
}

void GetWindowResol(int &horizontal, int &vertical)
{
	RECT desk;
	const HWND	hdesk = GetDesktopWindow();
	GetWindowRect(hdesk, &desk);

	horizontal = desk.right;
	vertical = desk.bottom;
}

bool Engine::InitializeWindow()
{
	//SDL_DisplayMode DM;
	//SDL_GetCurrentDisplayMode(0, &DM);
	//	std::cout << DM.w << "Width" << std::endl;
	//	std::cout << DM.h << "Heigth" << std::endl;
	int x, y;
	SDL_GetWindowSize(m_pWindow, &x, &y);
	
	GetWindowResol(x, y);

	m_uWindowWidth	= x;
	m_uWindowHeight = y;

	std::cout << x << "Width" << std::endl;
	std::cout << y << "Heigth" << std::endl;
	

	m_pWindow = CreateSDLWindow(x,y);// (m_uWindowWidth, m_uWindowHeight);
#ifdef _DEBUG
	SDL_SetWindowFullscreen(m_pWindow, 0);
#else
	SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN);
	m_bFullScreen = true;
#endif // _DEBUG

    if (m_pWindow == NULL)
    {
        return false;
    }

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	m_glContext = SDL_GL_CreateContext(m_pWindow);
	
    if (glewInit() != GLEW_OK)
    {
        LOG_DEBUG("Error Initializing GLEW");
        return false;
    }

    // Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
        LOG_ERROR_ARG("Warning: Unable to set VSync! SDL Error : {}", SDL_GetError());
	}
	else
	{
        LOG_DEBUG("V-Sync Setup Properly");
	}

    return true;
}

SDL_Window* Engine::CreateSDLWindow(uint32 a_uWidth, uint32 a_uHeight)
{
    // Initialize SDL
    int init_return = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    if (init_return < 0) 
    {
        LOG_ERROR_ARG("Couldn't initialize SDL error: {}", init_return);
        return nullptr;
    }

    // Enable AntiAliasiing
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    if (m_bFullScreen)
    {
        // Create SDL window
        m_pWindow = SDL_CreateWindow(	"PiEngine Launcher",		                    // window title
										 0,						                    // initial x position
										 0,						                    // initial y position
										 m_uWindowWidth,			                    // width, in pixels
										 m_uWindowHeight,			                    // height, in pixels
										 SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN );

		
    }
    else
    {
        // Create SDL window
		m_pWindow = SDL_CreateWindow(	"Odyssey",									// window title
										SDL_WINDOWPOS_CENTERED,						// initial x position
										SDL_WINDOWPOS_CENTERED,						// initial y position
			m_uWindowWidth,										// width, in pixels
			m_uWindowHeight,										// height, in pixels
										SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

        SDL_SetWindowPosition(m_pWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }	

    if (m_pWindow == nullptr) 
    {
        LOG_ERROR_ARG("Could not create window : {}", SDL_GetError());
        return nullptr;
    }
    return m_pWindow;
}

void Engine::Run()
{
	SDL_Event event;
	

    // Game Loop
    m_bApplicationIsRunning = true;
	if(PiResource->GetConfig()->m_isMusicOn)
		PiAudio->PlaySounds("PiConspiracy", vec3(0.0f), 10.0f, false);

    while (m_bApplicationIsRunning)
    {

		if (m_loadMmenu == true)
		{
			//add 2 sec event to load start menu
			GameStartEvent* gs = new GameStartEvent;
			//gs->SetEventTime(6.0f);
			PiEvent->AddEventToManagersQueue(gs);
			m_loadMmenu = false;
		}

		m_pFrameRateController->FrameStart();
        m_fDeltaTime = m_pFrameRateController->GetDeltaTime();

		PiImgui->StartFrame(m_pWindow);

		// Input Manager Needs to be Updated before all other updates
		// So let this next statement be here.
		m_inputManager->Update(m_fDeltaTime);
        
		
		SDL_ShowCursor(SDL_DISABLE);
		while (SDL_PollEvent(&event) != 0)
        {
			m_inputManager->ProcessEvent(&event);
			ImGui_ImplSDL2_ProcessEvent(&event);
			
			//TODO : fix the crash if controller isn't connected
            if (event.type == SDL_QUIT )// || event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
            {
                m_bApplicationIsRunning = false;
				
            }

			if (PiResource->GetConfig()->m_enableWindowEvents)
			{
				 if (event.type == SDL_WINDOWEVENT)
				 {
					if ( event.window.event == SDL_WINDOWEVENT_FOCUS_LOST /*|| SDL_WINDOWEVENT_HIDDEN*/)
					{ 
						if (m_isInGame)
						{

							m_paused = !m_paused;
							if (m_paused)
							{
								//m_pFrameRateController->SetTimeScale(0);
								//broadcast pause events to respective managers
								OnPauseEvent pause;
								PiUI->HandleEvent(&pause);
								this->HandleEvent(&pause);
								PiAudio->HandleEvent(&pause);

								Event* e = new Event;
								e->SetEventType(EEventType::Minimized);
								PiEvent->AddEventToManagersQueue(e);


							}
						}
						m_isMinimized = true;
						LOG_DEBUG("window minimized, free mouse enabled\n");
						
					}
					else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
					{
						
						Event* e = new Event;
						e->SetEventType(EEventType::Maximized);
						PiEvent->AddEventToManagersQueue(e);
						m_isMinimized = false;
						LOG_DEBUG("window maximized, free mouse disabled\n");
					}
				 }
			}
			
        }
		if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_ESCAPE) || PiInput->IsControllerButtonDown(EControllerButton::ButtonStart))
		{
			//TODO: turn this into event based?
			if (m_isInGame && !m_IsPlayerDead)
			{ 
				m_paused = !m_paused;
				if (m_paused)
				{
					//m_pFrameRateController->SetTimeScale(0);
					//broadcast pause events to respective managers
					OnPauseEvent pause;
					PiUI->HandleEvent(&pause);
					this->HandleEvent(&pause);
					PiAudio->HandleEvent(&pause);
				}
				else
				{
					OnResumeEvent resume;
					//m_pFrameRateController->SetTimeScale(1.0f);
					PiUI->HandleEvent(&resume);
					this->HandleEvent(&resume);
					PiAudio->HandleEvent(&resume);
				}
			}
		}
		if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_R))
		{
			/*ReloadScripts* reload = new ReloadScripts;
			PiEvent->AddEventToManagersQueue(reload);*/
			OnCurrentRestartEvent* onRes = new OnCurrentRestartEvent();
			PiEvent->AddEventToManagersQueue(onRes);
		}
		if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_Q))
		{
			/*ReloadScripts* reload = new ReloadScripts;
			PiEvent->AddEventToManagersQueue(reload);*/
			ReloadPrefab* onRes = new ReloadPrefab();
			PiEvent->AddEventToManagersQueue(onRes);
		}
		if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_F11))
		{
			m_bFullScreen = !m_bFullScreen;
			setIsFullScreen(m_bFullScreen);
			/*
			if (m_bFullScreen)
			{
				SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN);
			}
			else if (!m_bFullScreen)
			{
				SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_OPENGL);

			}*/
		}

		if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_E)/*PiImgui->IsDebugDrawOn()*/)
		{
			m_bIsDebugDraw = !m_bIsDebugDraw;
		}


		if ((PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_RETURN) || PiInput->IsControllerButtonDown(EControllerButton::ButtonStart) || PiInput->IsControllerButtonDown(EControllerButton::ButtonA)) && m_digLogoDone == true)
		{
			m_loadMmenu = true;
			m_digLogoDone = false;
		}

        Update();
		
		PiImgui->EndFrame();

		SDL_GL_SwapWindow(m_pWindow);
		m_pFrameRateController->FrameEnd();
    }
	
    ShutDown();
}

void Engine::Update()
{
	//Play Main Theme
    // Manager Updates
	vec3 vColor;	// = vec3(0.0f, 0.0f, 0.0f);
#if EDITOR
  vColor = vec3(1.0f);
#else
  vColor = m_pImguiManager->getFogColor();
#endif
	vColor = m_pRenderer->GetFogParameters().m_vColor;
	//m_pRenderer->Clear(vColor);
	ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame(m_pWindow);

	// EventManager Update
	m_eventManager->Update(m_fDeltaTime);

	//other systems update here
#if EDITOR
#else
  m_physicsManager->Update(m_fDeltaTime);
#endif
	

	m_gameObjectManager->Update(m_fDeltaTime);
	m_uiManager->Update(m_fDeltaTime);
	m_statsManager->Update(m_fDeltaTime);
	

	//=====================================
	// update Renderer
	//=====================================
#ifdef FWD_RENDERING
	
	m_pRenderer->Draw();

#endif // FWD_RENDERING


//#if EDITOR
//    m_levelEditor->Update(m_pWindow, &m_glContext);
//#else
	if (PiResource->GetConfig()->m_presentation)
	{
#ifdef _DEBUG
		m_pImguiManager->Update(m_pWindow, &m_glContext);
		// ImguiManager
#endif // DEBUG
	}
	else
	{
		m_pImguiManager->Update(m_pWindow, &m_glContext);
	}	
}

void Engine::HandleEvent(Event * a_pEvent)
{
	if (a_pEvent->GetEventType() == EEventType::OnResume)
	{
		m_paused = false;
		m_pFrameRateController->SetTimeScale(1);
	}
	else if (a_pEvent->GetEventType() == EEventType::OnPause)
	{
		m_paused = true;
		m_pFrameRateController->SetTimeScale(0);
	}
	else if (a_pEvent->GetEventType() == EEventType::OnQuit)
	{
		m_bApplicationIsRunning = false;
		//cout << "in engine quit event";
	}
	else if (a_pEvent->GetEventType() == EEventType::OnRestart)
	{
		//reset states
		m_paused = false;
		m_PanCamera = false;
		m_pFrameRateController->SetTimeScale(1);
		//PiScene->ChangeScene(PiScene->GetCurrentSceneIndex());
		//PiPhysics->ClearAll();
	}
	else if (a_pEvent->GetEventType() == EEventType::OnDeath)
	{
		//Stop the game
		m_paused = true;
		PiStats->m_running = false;
		m_PanCamera = true;
		m_pFrameRateController->SetTimeScale(0);

		//Camera Behaviour on Death
		OnDeathCameraEvent* camera_behaviour = new OnDeathCameraEvent();
		camera_behaviour->SetEventTime(2.0f);
		PiEvent->AddEventToManagersQueue(camera_behaviour);
		
	}
	else if (a_pEvent->GetEventType() == EEventType::OnStart)
	{
		PiFPS->SetTimeScale(1.0f);
		m_paused = false;
		m_PanCamera = false;
		m_isInGame = true;
		Event* e = new Event;
		e->SetEventType(EEventType::ResetPickupHUD);
		PiEvent->AddEventToQueue(e);
		PiStats->Reset();
		m_audioManager->PlaySounds("PiConspiracy", vec3(0.0f), 1.0f, false, false, false);
	}
	else if (a_pEvent->GetEventType() == EEventType::GameStart)
	{
		PiScene->ChangeScene(1);
		

	}
	else if (a_pEvent->GetEventType() == EEventType::Story)
	{
		PiScene->ChangeScene(17);
		m_digLogoDone = true;
	}
	else if (a_pEvent->GetEventType() == EEventType::ControllerRec)
	{
		PiScene->ChangeScene(18);
	}
	else if (a_pEvent->GetEventType() == EEventType::Exit2Main)
	{
		m_isInGame = false;
		m_audioManager->PlaySounds("MenuMusic", vec3(0.0f), 1.0f, false, false, false);
	}
	else if (a_pEvent->GetEventType() == EEventType::OnCurrentRestart)
	{
		PiScene->ChangeScene(PiScene->GetCurrentSceneIndex());
		if (PiFPS->GetTimeScale() == 0.0f)
		{
			PiFPS->SetTimeScale(1.0f);
		}
	}
}

void Engine::ShutDown()
{
	//TODO optimize deletion
    SAFE_DELETE(m_pFrameRateController);
	SAFE_DELETE(m_resourceManager);
	SAFE_DELETE(m_pMemoryManager);
	SAFE_DELETE(m_inputManager);
	SAFE_DELETE(m_gameObjectManager);
	SAFE_DELETE(m_sceneManager);//mem leak add
	SAFE_DELETE(m_eventManager);
	SAFE_DELETE(m_audioManager);
	SAFE_DELETE(m_pImguiManager);
    SAFE_DELETE(m_pRenderer);
    //SAFE_DELETE(m_levelEditor);
    SAFE_DELETE(m_statsManager);
	SAFE_DELETE(m_physicsManager);
	SAFE_DELETE(m_uiManager);
	//SAFE_DELETE(m_scriptManager);
	SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_pWindow);

    SDL_Quit();
}
