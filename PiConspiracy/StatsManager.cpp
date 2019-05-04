//
//  StatsManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "StatsManager.h"
#include "Engine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"

StatsManager::StatsManager() :
	m_score(0), 
	m_multiplier(1), 
	m_pickupMultiplier(0), 
	m_jumpCount(0),
	m_pierceCount(0),
	m_extraLife(0),
	is_TutorialDone(false),
	m_running(true)
{
}


StatsManager::~StatsManager()
{
}

void StatsManager::Reset(void)
{
	
	

	m_score = 0;
	m_multiplier = 1;
	m_pickupMultiplier = 0;
	m_jumpCount = 0;
	m_pierceCount = 0;
	m_extraLife = 0;
	m_running = true;
}

void StatsManager::Update(float a_dt)
{
	
	if (m_running && !PiEngine->getIsPlayerDead())
	{
		if (m_pickupMultiplier == 5)
		{
			m_pickupMultiplier = 0;
			++m_multiplier;
		}
		m_score = m_scoreCounter * m_multiplier;
	}
}
