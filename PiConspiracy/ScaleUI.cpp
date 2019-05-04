//
//  ScaleUI.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ScaleUI.h"


ScaleUI::ScaleUI():Component(STRINGIFY(ScaleUI))
{
	
}


ScaleUI::~ScaleUI()
{
}

void ScaleUI::Initialize()
{
	m_isValFull = true;
	m_cdtimer = 0.0f;
	//m_currHt = 0.0f;
	m_trans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_currentScale = m_trans->GetScale();
}

void ScaleUI::Update(float a_dTime)
{
	if (m_isValFull)
	{
		return;
	}
	//m_cdtimer += a_dTime;
	m_currHt -= (m_maxHt / m_maxCD) * a_dTime;

	m_currentScale.y = m_currHt;
	m_trans->SetScale(m_currentScale);
	std::cout <<"height : "<< m_currHt << std::endl;
	if (m_currHt <=0)
	{
		m_isValFull = true;
		m_cdtimer = 0.0f;
		m_currHt = m_maxHt;
	}
}

void ScaleUI::Serialize(const json &j)
{
	m_maxHt = ParseFloat(j, "maxHeight");
	m_currHt = m_maxHt;
}

void ScaleUI::HandleEvent(Event * a_pEvent)
{
	if (a_pEvent->GetEventType()==EventType::Dash)
	{
		DashEvent* d = static_cast<DashEvent*>(a_pEvent);
		m_isValFull = false;
		m_maxCD= d->m_coolDownVal;
	}
}

void ScaleUI::Deactivate(void)
{
}

void ScaleUI::OverrideParams(const json &, float)
{
}
