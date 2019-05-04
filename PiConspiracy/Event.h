//
//  Event.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

struct EventType
{
	enum Enum
	{
		//TODO: add event types as need
		DropBall,
		DestroyBall,
		ChangeScene,
		Collision,
		PlayerHit,
		MultiplierPicked,
		PiercePowerUp,
		JumpPowerUp,
		PowerUpPicked,
		BoostPicked,
		JumpPicked,
		RespawnPicked,
		PowerUpUsed,
		OnPause,
		OnResume,
		OnQuit,
		OnRestart,
		TriggerLevel,
		StartBufferLevel,
		CubeFallTop,
		Triggered,
		UnloadBufferRegion,
		TutorialTexture,
		OnDeath,
		OnStart,
		OnDestrcutiveAction,
		OnControlsPressed,
		ReloadScripts,
		GameStart,
		Story,
		ControllerRec,
		OnCurrentRestart,
		ReloadPrefabs,
		Dash,
		ResetPickupHUD,
		IncreaseSpeed,
		BackEvent,
		Exit2Main,
		OnDeathCamera,
		OnCreditsPressed,
		OnOptionsPressed,
		Minimized,
		Maximized,
		NUM
	};
};

TYPEDEF_ENUM(EventType);

//base class for events to derive from as needed for each system
class Event
{
public:
	Event(void) {}
	Event(EEventType a_eventType)
		: m_eventType(a_eventType)
		, m_eventTimer(0.0f)
	{
	}
	virtual ~Event() {}

	inline void SetEventTime(float a_time) { m_eventTimer = a_time; }
	inline float GetEventTime(void) { return m_eventTimer; }
	inline EEventType GetEventType(void) { return m_eventType; }
	inline void SetEventType(EEventType a_type) { m_eventType = a_type; }
	
protected:
	EEventType m_eventType;
	//by default events will have a timer of 0 as instant events 
	//if timed events are needed set a time
	//the manager will handle all that in the update 
	float m_eventTimer;
};

struct ChangeEvent : public Event
{
	ChangeEvent() :Event(EventType::ChangeScene)
	{
	}
	
};

struct BackEvent : public Event
{
	BackEvent() :Event(EventType::BackEvent)
	{
	}
	int m_changeTo;
	int m_changeFrom;
};

struct CollisionEvent : public Event
{
	CollisionEvent() : Event(EEventType::Collision) {}
	~CollisionEvent() {}

	class GameObject* m_gameObject;
};

struct PlayerHitEvent : public Event 
{
	PlayerHitEvent() : Event(EEventType::PlayerHit) {}
	~PlayerHitEvent() {};

	float m_damage;
};

struct OnPauseEvent :public Event
{
	OnPauseEvent() :Event(EEventType::OnPause) {}
	~OnPauseEvent() {};
};

struct OnResumeEvent :public Event
{
	OnResumeEvent() :Event(EEventType::OnResume) {}
	~OnResumeEvent() {};
};

struct OnQuitEvent :public Event
{
	OnQuitEvent() :Event(EEventType::OnQuit) {}
	~OnQuitEvent() {};
};

struct OnRestartEvent :public Event
{
	OnRestartEvent() :Event(EEventType::OnRestart) {}
	~OnRestartEvent() {};
};

struct ReloadPrefab :public Event
{
	ReloadPrefab() :Event(EEventType::ReloadPrefabs) {}
	~ReloadPrefab() {};
};

struct OnCurrentRestartEvent :public Event
{
	OnCurrentRestartEvent() :Event(EEventType::OnCurrentRestart) {}
	~OnCurrentRestartEvent() {};
};

struct LoadLevelEvent :public Event
{
	LoadLevelEvent() : Event(EEventType::TriggerLevel) {}
	~LoadLevelEvent() {};

	float index;
	float offset;
	//this bool is to work with game object manager check there for better description
	bool isLevel;
};

struct StartBufferRegionEvent : public Event
{
	StartBufferRegionEvent() : Event(EEventType::StartBufferLevel) {}
	~StartBufferRegionEvent() {};

	//this bool is to work with game object manager check there for better description
	bool isLevel;

};

struct CubeFallEvent :public Event
{
	CubeFallEvent() : Event(EEventType::CubeFallTop) {}
	~CubeFallEvent() {};

	int id;
};

struct TriggeredEvent :public Event
{
	TriggeredEvent() : Event(EEventType::Triggered) {}
	~TriggeredEvent() {};

	int id;
	string behaviour;
};

struct OnMultiplierPickup : public Event
{
	OnMultiplierPickup() : Event(EEventType::MultiplierPicked) {}
	~OnMultiplierPickup() {};

	int m_hudId;
};

struct UnloadBufferRegionEvent : public Event 
{
	UnloadBufferRegionEvent() : Event(EEventType::UnloadBufferRegion) {}
	~UnloadBufferRegionEvent() {};

};

struct TutorialTextureEvent : public Event 
{
	TutorialTextureEvent() : Event(EEventType::TutorialTexture) {}
	~TutorialTextureEvent() {};

	std::string TutorialTexture_Type;

};

struct OnDeathEvent : public Event
{
	OnDeathEvent() : Event(EEventType::OnDeath) {}
	~OnDeathEvent() {};

	int m_score;
};

struct OnDeathCameraEvent : public Event
{
	OnDeathCameraEvent() : Event(EEventType::OnDeathCamera) {}
	~OnDeathCameraEvent() {};

};

struct OnStart : public Event
{
	OnStart() : Event(EEventType::OnStart) {}
	~OnStart() {};

};

struct OnDestructiveAction : public Event
{
	OnDestructiveAction() : Event(EEventType::OnDestrcutiveAction) {}
	~OnDestructiveAction() {};

	EEventType m_destructiveEvent;
	bool m_isActionConfirmed;
};

struct OnControlsPressed : public Event
{
	OnControlsPressed() : Event(EEventType::OnControlsPressed) {}
	~OnControlsPressed() {};

};

struct OnCreditsPressed : public Event
{
	OnCreditsPressed() : Event(EEventType::OnCreditsPressed) {}
	~OnCreditsPressed() {};

};

struct ReloadScripts: public Event
{
	ReloadScripts() : Event(EEventType::ReloadScripts) {}
	~ReloadScripts() {};
};

struct GameStartEvent : public Event
{
	GameStartEvent() : Event(EEventType::GameStart) {}
	~GameStartEvent() {};
};

struct StorytEvent : public Event
{
	StorytEvent() : Event(EEventType::Story) {}
	~StorytEvent() {};
};

struct ControllerRecEvent : public Event
{
	ControllerRecEvent() : Event(EEventType::ControllerRec) {}
	~ControllerRecEvent() {};
};

struct DashEvent :public Event
{
	DashEvent() : Event(EEventType::Dash) {}
	~DashEvent() {};

	float m_coolDownVal;
};

struct OnPowerUpPicked : public Event
{
	OnPowerUpPicked() : Event(EEventType::PowerUpPicked) {}
	~OnPowerUpPicked() {};

	string m_powerUpType;
};

struct OnPowerUpUsed : public Event
{
	OnPowerUpUsed(): Event(EEventType::PowerUpUsed) {}
	~OnPowerUpUsed() {};

	string m_powerUpType;
};
//struct RiseEvent : public Event, public TriggeredEvent
//{
//
//};

//struct PickUpEvent : public Event 
//{
//	PickUpEvent() : Event(EEventType::PickUpHit) {}
//	~PickUpEvent() {};
//
//	float m_points;
//};