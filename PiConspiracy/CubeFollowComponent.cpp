//
//  DropComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "CubeFollowComponent.h"
#include "GameObjectManager.h"

#include "Renderer.h"
#include "Camera.h"


CubeFollowComponent::CubeFollowComponent(void)
	: Component(STRINGIFY(CubeFollowComponent))
	,isStart(false)
{
	
}




CubeFollowComponent::~CubeFollowComponent(void)
{

}

void CubeFollowComponent::Initialize(void)
{

}

void CubeFollowComponent::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
	m_pTrans = false;
	m_rigidBody = nullptr;


}

void CubeFollowComponent::Update(float a_dt)
{

	if (isStart)
	{
		m_pos = m_pTrans->GetPosition();

		if (findloc)
		{
			playerTransform = PiGameObject->m_Player->GetComponent<Transform>(STRINGIFY(Transform));
			playerPos = playerTransform->GetPosition();
			findloc = false;
		}

		
		

		if (initialCheck)
		{
			if (m_pos.x < playerPos.x)
			{
				fromLeft = true;
				initialCheck = false;
			}
			if (m_pos.x > playerPos.x)
			{
				fromRight = true;
				initialCheck = false;
			}
		}

		if (fromLeft)
		{
			m_rigidBody->setPosition(m_pos.x + 200.0f, m_pos.y, m_pos.z);
			m_pTrans->SetPosition(m_pos.x + 200.0f, m_pos.y, m_pos.z);

			if (m_rigidBody->getPosition().x >= playerPos.x)
			{
				m_rigidBody->setPosition(playerPos.x, m_pos.y, m_pos.z);
				m_pTrans->SetPosition(playerPos.x, m_pos.y, m_pos.z);
				fromLeft = false;
			}
		}

		if (fromRight)
		{
			m_rigidBody->setPosition(m_pos.x - 200.0f, m_pos.y, m_pos.z);
			m_pTrans->SetPosition(m_pos.x - 200.0f, m_pos.y, m_pos.z);

			if (m_rigidBody->getPosition().x <= playerPos.x)
			{
				m_rigidBody->setPosition(playerPos.x, m_pos.y, m_pos.z);
				m_pTrans->SetPosition(playerPos.x, m_pos.y, m_pos.z);
				fromRight = false;
			}
		}


	}
}

void CubeFollowComponent::Serialize(const json & a_j)
{
	m_pTrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_rigidBody = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
}

void CubeFollowComponent::HandleEvent(Event * a_pEvent)
{
	/*if (a_pEvent->GetEventType()==EEventType::Triggered)
	{

	}*/
}



void CubeFollowComponent::OverrideParams(const json &, float offset)
{
	
}




