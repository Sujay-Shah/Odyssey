#pragma once
#include "Event.h"
#include "GameObject.h"

class CollisionEvent : public Event
{
public:
	CollisionEvent() : Event(EEventType::Collision) {}
	~CollisionEvent() {}

	GameObject* m_gameObject;
};