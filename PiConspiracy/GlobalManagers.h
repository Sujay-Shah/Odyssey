//
//  GlobalManagers.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

#define PiEngine    Singleton<Engine>::GetInstance()
#define PiRenderer  Singleton<Renderer>::GetInstance()
#define PiFPS       Singleton<FrameRateController>::GetInstance()
#define PiEvent     Singleton<EventManager>::GetInstance()
#define PiResource  Singleton<ResourceManager>::GetInstance()
#define PiInput     Singleton<InputManager>::GetInstance()
#define PiMemory    Singleton<MemoryManager>::GetInstance()
#define PiAudio		Singleton<AudioManager>::GetInstance()
#define PiImgui     Singleton<ImguiManager>::GetInstance()
#define PiScene     Singleton<SceneManager>::GetInstance()
#define PiGameObject Singleton<GameObjectManager>::GetInstance()
#define PiPhysics	Singleton<PhysicsManager>::GetInstance()
#define PiStats		Singleton<StatsManager>::GetInstance()
#define PiUI		Singleton<UIManager>::GetInstance()
#define PiScript	Singleton<ScriptManager>::GetInstance()