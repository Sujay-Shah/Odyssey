//
//  NPC_SideMovementComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "NPC_SideMovementComponent.h"


NPC_SideMovementComponent::NPC_SideMovementComponent(void)
	: Component(STRINGIFY(NPC_SideMovementComponent))
	, m_range(0.0f)
	,m_speed(0.0f)
{
	
}

NPC_SideMovementComponent::NPC_SideMovementComponent(float a_range, float a_speed)
	: Component(STRINGIFY(NPC_SideMovementComponent))
	, m_range(a_range)
	, m_speed(a_speed)
{

}


NPC_SideMovementComponent::~NPC_SideMovementComponent(void)
{

}

void NPC_SideMovementComponent::Initialize(void)
{

}

void NPC_SideMovementComponent::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
	m_range = 0.0f;
	m_speed = 0.0f;
	//m_pRigidBody = nullptr;
}

void NPC_SideMovementComponent::Update(float a_dt)
{
	//For Debugging
	//cout << "tick : " << ticks << std::endl;

	m_pRigidBody = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));

	
	ticks +=a_dt;
	float vel;
	if (x)
		 vel = m_range * sin(ticks * m_speed * PI) + m_initialPos.x;

	if (y)
		 vel = m_range * sin(ticks * m_speed * PI) + m_initialPos.y;


	//Tick Reset Statement
	//if(sin(ticks> 2*PI))
		//ticks += 0.0f;

	if(x)
		m_initialPos.x = vel;

	if (y)
		m_initialPos.y = vel;

	m_p->SetPosition(m_initialPos);
	m_pRigidBody->setPosition(m_initialPos);
}

void NPC_SideMovementComponent::Serialize(const json & a_j)
{
	
	m_range = ParseFloat(a_j, "Range");

	m_speed = ParseFloat(a_j, "Speed");

	x = ParseBool(a_j, "x");

	y = ParseBool(a_j, "y");
}

void NPC_SideMovementComponent::HandleEvent(Event * a_pEvent)
{
}

void NPC_SideMovementComponent::SetSpeed(float a_speed)
{
	m_speed = a_speed;
}

void NPC_SideMovementComponent::SetRange(float a_range)
{
	m_range = a_range;
}

float NPC_SideMovementComponent::GetSpeed(void)
{
	return m_speed;
}

float NPC_SideMovementComponent::GetRange(void)
{
	return m_range;
}

void NPC_SideMovementComponent::OverrideParams(const json&a_j,float offset)
{
	m_p = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_initialPos = m_p->GetPosition();

	if (ValueExists(a_j, "Range"))
		m_range = ParseFloat(a_j, "Range");

	if (ValueExists(a_j, "Speed"))
		m_speed = ParseFloat(a_j, "Speed");

	if (ValueExists(a_j, "x"))
		x = ParseBool(a_j, "x");

	if (ValueExists(a_j, "y"))
		y = ParseBool(a_j, "y");

}



