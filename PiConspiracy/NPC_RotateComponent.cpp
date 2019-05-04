//
//  NPC_RotateComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "NPC_RotateComponent.h"


NPC_RotateComponent::NPC_RotateComponent(void)
	: Component(STRINGIFY(NPC_RotateComponent))
	,m_speed(0.0f)
{
	
}

NPC_RotateComponent::NPC_RotateComponent(float a_speed)
	: Component(STRINGIFY(NPC_RotateComponent))
	, m_speed(a_speed)
{

}


NPC_RotateComponent::~NPC_RotateComponent(void)
{

}

void NPC_RotateComponent::Initialize(void)
{
	//m_pTrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition();
}

void NPC_RotateComponent::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;

	m_speed = 0.0f;
}

void NPC_RotateComponent::Update(float a_dt)
{
	//m_pTrans->SetRotation
}

void NPC_RotateComponent::Serialize(const json & a_j)
{

	m_speed = ParseFloat(a_j, "Speed");


}

void NPC_RotateComponent::HandleEvent(Event * a_pEvent)
{
}

void NPC_RotateComponent::SetSpeed(float a_speed)
{
	m_speed = a_speed;
}


float NPC_RotateComponent::GetSpeed(void)
{
	return m_speed;
}

void NPC_RotateComponent::OverrideParams(const json &, float offset)
{
}




