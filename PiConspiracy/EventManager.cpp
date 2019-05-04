//
//  EventManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "EventManager.h"
#include "GameObjectManager.h"
#include "Engine.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "PhysicsManager.h"
#include "ResourceManager.h"
#include "ScriptManager.h"
#include "AudioManager.h"

EventManager::EventManager(void):m_altDt(0.0f)
{
	
}

EventManager::~EventManager(void)
{

	//delete unprocessed events
	for (auto it : m_eventsQueue)
	{
		delete it;
	}


	for (auto it : m_managersEventQueue)
	{
		delete it;
	}
}

void EventManager::BroadcastEvent(Event* a_event)
{
	//TODO: make broadcast might need to broadcast to more than just game object manager
	//might need subscriber/listner for later
	PiGameObject->HandleEvents(a_event);
	PiUI->HandleEvent(a_event);
}

void EventManager::BroadcastManagersEvent(Event * a_event)
{
	PiResource->HandleEvent(a_event);
	//PLS DONT CHANGE THE ORDER!!
	PiEngine->HandleEvent(a_event);
	//DONT CHANGE THE ORDER!!
	//PiScript->HandleEvent(a_event);
	PiPhysics->HandleEvents(a_event);
	PiGameObject->HandleEvents(a_event);
	
	PiScene->HandleEvent(a_event);
	PiUI->HandleEvent(a_event);
	PiAudio->HandleEvent(a_event);
	//DONT CHANGE THE ORDER!!
	

}

void EventManager::AddEventToQueue(Event* a_event)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	m_eventsQueue.push_back(std::move(a_event));
}

void EventManager::AddTimedEventToQueue(Event * a_event, float a_time)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	a_event->SetEventTime(a_time);
	m_eventsQueue.push_back(std::move(a_event));
}

void EventManager::AddEventToManagersQueue(Event * a_event)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	m_managersEventQueue.push_back(a_event);
}

void EventManager::Update(float a_dtime)
{
	if (PiEngine->GetIsPaused())
	{
		m_altDt += (float)0.0016;
	}
	if (m_eventsQueue.empty() && m_managersEventQueue.empty())
	{
		return;
	}
	
	list<Event*>::iterator itBegin, itEnd, itTemp;
	list<Event*>::iterator itMBegin, itMEnd, itMTemp;
	itBegin = m_eventsQueue.begin();
	itEnd = m_eventsQueue.end();
	itMBegin = m_managersEventQueue.begin();
	itMEnd = m_managersEventQueue.end();
	
	while (itBegin != itEnd)
	{
		Event* eventHolder = *itBegin;

		eventHolder->SetEventTime(eventHolder->GetEventTime() - a_dtime);

		if (eventHolder->GetEventTime() <= 0.0f)
		{
			BroadcastEvent(eventHolder);
			SAFE_DELETE(eventHolder);
			itTemp = itBegin;
			itTemp++;
			m_eventsQueue.erase(itBegin);
			itBegin = itTemp;
		}
		else
		{
			itBegin++;
		}
	}
	while (itMBegin != itMEnd)
	{
		Event* managereventHolder = *itMBegin;

		if (PiEngine->GetIsPaused())
		{
			managereventHolder->SetEventTime(managereventHolder->GetEventTime() - m_altDt);
		}
		else
			managereventHolder->SetEventTime(managereventHolder->GetEventTime() - a_dtime);

		if (managereventHolder->GetEventTime() <= 0.0f)
		{
			BroadcastManagersEvent(managereventHolder);
			SAFE_DELETE(managereventHolder);
			itMTemp = itMBegin;
			itMTemp++;
			m_managersEventQueue.erase(itMBegin);
			itMBegin = itMTemp;

			if (PiEngine->GetIsPaused())
			{
				m_altDt = 0.0f;
			}
		}
		else
		{
			itMBegin++;
		}
	}
}