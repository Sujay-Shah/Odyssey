//
//  Rotations.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Rotations.h"


Rotations::Rotations(void)
	: Component(STRINGIFY(Rotations))

{
	
}


Rotations::~Rotations(void)
{

}

void Rotations::Initialize(void)
{
	m_pTrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_rotation = m_pTrans->GetEulerAngles();

	m_rotRate = 2.0f;
}

void Rotations::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
	m_axis = 0;
	m_rotRate = 0.0f;
}

void Rotations::Update(float a_dt)
{
		
	if (m_axis == 1)
	{
		m_rotation.x += m_rotRate;
		m_pTrans->SetEulerAngles(m_rotation);
	}
	else 	
		if (m_axis == 2)
	{
		m_rotation.y += m_rotRate;
		m_pTrans->SetEulerAngles(m_rotation);
	}
	else	
		if (m_axis == 3)
	{
		m_rotation.z += m_rotRate;
		m_pTrans->SetEulerAngles(m_rotation);
	}
}

void Rotations::Serialize(const json & a_j)
{
	
	m_rotRate = ParseFloat(a_j, "angle");

	m_axis = ParseInt(a_j, "axis");

}

void Rotations::HandleEvent(Event * a_pEvent)
{
}

void Rotations::OverrideParams(const json& j,float offset)
{
	if (ValueExists(j, "angle"))
		m_rotRate = ValueExists(j, "angle") ? ParseInt(j, "angle") : m_rotRate;

	if (ValueExists(j, "axis"))
		m_axis = ValueExists(j, "axis") ? ParseInt(j, "axis") : m_axis;
}



