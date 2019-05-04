//
//  RiseComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "RiseComponent.h"

#include "Renderer.h"
#include "Camera.h"

RiseComponent::RiseComponent(void)
	: Component(STRINGIFY(RiseComponent))
	,m_speed(0.0f),isRise(false), shake(false)
{
	
}

RiseComponent::RiseComponent(float a_speed)
	: Component(STRINGIFY(RiseComponent))
	, m_speed(a_speed),isRise(false), shake(false)
{

}


RiseComponent::~RiseComponent(void)
{

}

void RiseComponent::Initialize(void)
{
	shake = true;
}

void RiseComponent::Deactivate(void)
{
	m_owner = nullptr;
	m_transform = nullptr;
	m_RigidBody = nullptr;
	m_activeFlag = false;

	m_speed = 0.0f;
}

void RiseComponent::Update(float a_dt)
{
	if (isRise)
	{
		m_pos = m_transform->GetPosition();
		if (m_transform->GetPosition().y < scaleValue.y)
		{

			//m_pTrans->SetPosition(m_pos.x, m_pos.y + m_speed, m_pos.z);
			m_RigidBody->setPosition(m_pos.x, m_pos.y + m_speed, m_pos.z);
		}

		else if (m_transform->GetPosition().y > scaleValue.y)
		{
			//m_pTrans->SetPosition(m_pos.x, scaleValue.y, m_pos.z);
			m_RigidBody->setPosition(m_pos.x, scaleValue.y+5.0f, m_pos.z);
			
		}
		if (shake)
		{
			Camera* pCamera = PiRenderer->GetMainCamera();
			pCamera->SetCameraShake(100);
			shake = false;
		}
	}
}

void RiseComponent::Serialize(const json & a_j)
{
	m_transform = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_RigidBody = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
	m_speed = ParseFloat(a_j, "Speed");
}

void RiseComponent::HandleEvent(Event * a_pEvent)
{
	/*if (a_pEvent->GetEventType()==EEventType::Triggered)
	{

	}*/
}

void RiseComponent::SetSpeed(float a_speed)
{
	m_speed = a_speed;
}


float RiseComponent::GetSpeed(void)
{
	return m_speed;
}

void RiseComponent::OverrideParams(const json &a_j, float offset)
{
	scaleValue = m_transform->GetScaleVec();

	if (ValueExists(a_j, "Speed"))
		m_speed = ParseFloat(a_j, "Speed");
}




