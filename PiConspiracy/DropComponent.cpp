//
//  DropComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "DropComponent.h"

#include "Renderer.h"
#include "Camera.h"


DropComponent::DropComponent(void)
	: Component(STRINGIFY(DropComponent))
	,m_speed(0.0f),isDrop(false), shake(false)
{
	
}

DropComponent::DropComponent(float a_speed)
	: Component(STRINGIFY(DropComponent))
	, m_speed(a_speed),isDrop(false), shake(false)
{

}


DropComponent::~DropComponent(void)
{

}

void DropComponent::Initialize(void)
{
	shake = true;
}

void DropComponent::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
	m_pTrans = false;
	m_rigidBody = nullptr;

	m_speed = 0.0f;
}

void DropComponent::Update(float a_dt)
{
	if (isDrop)
	{
		m_pos = m_pTrans->GetPosition();

		if (m_pos.y < 0.0f)
		{
			m_rigidBody->setPosition(m_pos.x, 1000.0f, m_pos.z);
		}

		//m_pos = m_pTrans->GetPosition();
		else if (m_pos.y > scaleValue.y)
		{

			//m_pTrans->SetPosition(m_pos.x, m_pos.y + m_speed, m_pos.z);
			m_rigidBody->setPosition(m_pos.x, m_pos.y - m_speed, m_pos.z);
		}

		if (m_rigidBody->getPosition().y < scaleValue.y)
		{
			//m_pTrans->SetPosition(m_pos.x, scaleValue.y, m_pos.z);
			m_rigidBody->setPosition(m_pos.x, scaleValue.y, m_pos.z);
			/*Camera* pCamera = PiRenderer->GetCamera(false);
			pCamera->SetCameraShake(100);*/

		}

		if (shake)
		{
			//PiRenderer->SetDebugCamMode(false);
			Camera* pCamera = PiRenderer->GetCamera();
			pCamera->SetCameraShake(100);
			shake = false;
		}

	}
}

void DropComponent::Serialize(const json & a_j)
{
	m_pTrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_rigidBody = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
	m_speed = ParseFloat(a_j, "Speed");

}

void DropComponent::HandleEvent(Event * a_pEvent)
{
	/*if (a_pEvent->GetEventType()==EEventType::Triggered)
	{

	}*/
}

void DropComponent::SetSpeed(float a_speed)
{
	m_speed = a_speed;
}


float DropComponent::GetSpeed(void)
{
	return m_speed;
}

void DropComponent::OverrideParams(const json &, float offset)
{
	scaleValue = m_pTrans->GetScaleVec();
}




