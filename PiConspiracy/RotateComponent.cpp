//
//  RotateComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "RotateComponent.h"


RotateComponent::RotateComponent(void)
	: Component(STRINGIFY(RotateComponent))
{
}

RotateComponent::~RotateComponent(void)
{
}

void RotateComponent::Initialize()
{
	m_rigidBody = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
}

void RotateComponent::Update(float a_dTime)
{
	m_rigidBody->AddToRotation(m_x, m_y, m_z);
}

void RotateComponent::Serialize(const json & a_j)
{
	if (ValueExists(a_j, "x"))
	{
		m_x = ParseFloat(a_j, "x");
	}
	if (ValueExists(a_j, "y"))
	{
		m_y = ParseFloat(a_j, "y");
	}
	if (ValueExists(a_j, "z"))
	{
		m_z = ParseFloat(a_j, "z");
	}
}

void RotateComponent::HandleEvent(Event * a_pEvent)
{
}

void RotateComponent::Deactivate(void)
{
}

void RotateComponent::OverrideParams(const json &j, float offset)
{
	if (ValueExists(j, "x"))
	{
		m_x = ParseFloat(j, "x");
	}
	if (ValueExists(j, "y"))
	{
		m_y = ParseFloat(j, "y");
	}
	if (ValueExists(j, "z"))
	{
		m_z = ParseFloat(j, "z");
	}
}
