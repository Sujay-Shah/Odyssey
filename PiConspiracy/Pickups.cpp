//
//  Pickups.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Pickups.h"


Pickups::Pickups():Component(STRINGIFY(Pickups))
{
}


Pickups::~Pickups()
{
}

void Pickups::Initialize()
{
	m_id = 0;
}


void Pickups::Update(float a_dTime)
{
}

void Pickups::Serialize(const json &j)
{
	if (ValueExists(j, "multiplier"))
	{
		m_id = 0;
	}

	else if (ValueExists(j, "jump"))
	{
		m_id = 1;
	}
	
	else if (ValueExists(j, "pierce"))
	{
		m_id = 2;
	}
	else if (ValueExists(j, "life"))
	{
		m_id = 3;
	}
}

void Pickups::HandleEvent(Event * a_pEvent)
{
}

void Pickups::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
	m_id = 0;
}

void Pickups::OverrideParams(const json &,float offset)
{
}

int Pickups::GetId(void)
{
	return m_id;
}