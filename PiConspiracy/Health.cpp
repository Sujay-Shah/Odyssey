//
//  Health.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Health.h"
#include "StatsManager.h"

Health::Health() : Component(STRINGIFY(Health)),m_currentHealth(0),m_maxHealth(0)
{
}


Health::~Health()
{
}

void Health::Initialize()
{
	
}

void Health::Update(float a_dTime)
{
}

void Health::Serialize(const json &j)
{
	m_maxHealth = ParseFloat(j, "maxHealth");
	m_currentHealth = m_maxHealth;
}

void Health::HandleEvent(Event * a_pEvent)
{
	if (a_pEvent->GetEventType() == EEventType::PlayerHit)
	{
		PlayerHitEvent* pHit = static_cast<PlayerHitEvent*>(a_pEvent);
		m_currentHealth -= pHit->m_damage;

		
		if (m_currentHealth <= 0)
		{
			PiStats->m_health = 0.0f;
			LOG_DEBUG( "player is dead\n");
			return;
		}
		PiStats->m_health = m_currentHealth;
	}
}

void Health::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
}

void Health::OverrideParams(const json &,float offset)
{
}
