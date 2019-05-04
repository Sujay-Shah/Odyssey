//
//  EventManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class EventManager : public Singleton<EventManager>
{
public:
	EventManager(void);
	~EventManager(void);

	//broadcast events to systems and game objects to handle based on the need
	void BroadcastEvent(Event* a_event);
	void BroadcastManagersEvent(Event* a_event);
	
	//add all events into an event queue
	//TODO: make sure this is handled well when multi threading is in
	void AddEventToQueue(Event* a_event);
	void AddTimedEventToQueue(Event* a_event, float a_time);

	void AddEventToManagersQueue(Event* a_event);

	//timed events will depend on frames
	void Update(float a_dtime);

private:
	list<Event*> m_eventsQueue;
	list<Event*> m_managersEventQueue;

	std::shared_mutex m_mutex;
	float m_altDt;
};