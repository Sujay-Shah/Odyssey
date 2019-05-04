#pragma once
//
//  Engine.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#define EDITOR 0
class Engine : Singleton<Engine>
{

private:
	bool            InitializeWindow();
	SDL_Window*     CreateSDLWindow(uint32 a_uWidth, uint32 a_uHeight);

	SDL_GLContext   m_glContext;
	SDL_Window*     m_pWindow;

	bool            m_bApplicationIsRunning;
	uint32          m_uWindowWidth;
	uint32          m_uWindowHeight;
	bool            m_bFullScreen;
	float           m_fDeltaTime;
	double          m_dFPS;
	bool			m_paused;
	bool			m_IsPlayerDead;
	bool			m_isInGame;
	bool			m_bIsDebugDraw;
	bool			m_isGameSlowed;
	bool			m_loadMmenu;
	bool			m_digLogoDone;

	//Managers
    class LogManager* m_pLogManager;
	class FrameRateController* m_pFrameRateController;
	class ResourceManager* m_resourceManager;
	class MemoryManager* m_pMemoryManager;
	class EventManager* m_eventManager;
	class InputManager* m_inputManager;
	class ImguiManager* m_pImguiManager;
	class GameObjectManager* m_gameObjectManager;
	class AudioManager* m_audioManager;
    class Renderer*       m_pRenderer;
	class SceneManager*  m_sceneManager;
	class PhysicsManager*  m_physicsManager;


	//class LevelEditor* m_levelEditor;
	class StatsManager* m_statsManager;
	class UIManager* m_uiManager;

	class ScriptManager* m_scriptManager;
public:
                    Engine();
    virtual         ~Engine();

	bool            GetInGameStatus() { return m_isInGame; }
	void            SetInGameStatus(bool a_status) { m_isInGame = a_status; }
    bool            Initialize();
    bool            Finalize();

	bool            GetIsPaused(void) { return m_paused; }

    void            Run();
    void            Update();
	void		    HandleEvent(Event* a_pEvent);
    void            ShutDown();
	bool			m_PanCamera = false;
	bool            m_isMinimized;

    inline uint32   GetClientWidth();
    inline uint32   GetClientHeight();

	inline SDL_Window* GetWindow(void);
	inline bool		IsDebugDrawOn();
	inline FrameRateController* getFrameRateController()
	{
		return m_pFrameRateController;
	}
	inline bool getPanCamera()
	{
		return m_PanCamera;
	}
	inline void setPanCamera(bool a_PanCamera)
	{
		m_PanCamera = a_PanCamera;
	}
	inline const bool& getIsPlayerDead()
	{
		return m_IsPlayerDead;
	}

	inline void setIsPlayerDead(bool a_isPlayerDead)
	{
		m_IsPlayerDead = a_isPlayerDead;
	}

	inline bool GetIsFullScreen(void)
	{
		return m_bFullScreen;
	}

	inline void setIsFullScreen(bool a_fullscreen)
	{
		m_bFullScreen = a_fullscreen;
		if (m_bFullScreen)
		{
			SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN);
		}
		else 
		{
			SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_OPENGL);

		}
	}

	inline void SetClientWidth(uint32 a_width)
	{
		m_uWindowWidth = a_width;
	}

	inline void SetClientHeight(uint32 a_height)
	{
		m_uWindowHeight = a_height;
	}


	inline const bool& getIsGameSlowed() {
		return m_isGameSlowed;
	}

	void SetIsGameSlowed(bool a_condition)
	{
		m_isGameSlowed = a_condition;
	}



};

inline uint32 Engine::GetClientWidth()
{
    return m_uWindowWidth;
}

inline uint32 Engine::GetClientHeight()
{
    return m_uWindowHeight;
}

inline SDL_Window* Engine::GetWindow()
{
	return m_pWindow;
}

inline bool Engine::IsDebugDrawOn()
{
	return m_bIsDebugDraw;
}