//
//  AIComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "AIComponent.h"
#include "GameObjectManager.h"
#include "Engine.h"

AIComponent::AIComponent():Component(STRINGIFY(AIComponent)),m_ySpeed(0.0f)
{

}


AIComponent::~AIComponent()
{
}

void AIComponent::Initialize()
{
	m_rigid = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
	_isDrop = true;
}

void AIComponent::Update(float a_dTime)
{
	if (PiEngine->GetIsPaused())
	{
		return;
	}
	++m_count;
	if (m_count > 1000000)
		m_count = 0;
	//fly past a certain distance in front of player
	m_playerZ = PiGameObject->m_Player->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition().z;

	m_AiZ = m_owner->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition().z;
	//once on that distance execute logic below
	if ( (m_AiZ < m_playerZ)  &&  (m_playerZ - m_AiZ >= m_offset ) )
	{
		//reduce z speed to match with players
		m_zSpeed = PiGameObject->m_Player->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent))->getVelocity().z;
	}
	//m_rigid->setVelocity( sinf(m_count*0.01)*1700.0f, 0.0f, -1300.0f );
	m_rigid->setVelocity( sinf(m_count*0.01f)*m_xSpeed, m_ySpeed, m_zSpeed );
	//set speed & movement
	if (_isDrop && (m_count %50==0))
	{
		GameObject* pGo = PiGameObject->GetObjectFactoryInstance()->CreateGameObject("AIPickUps", PiGameObject->GetIsLevel());
		//Transform* parentTrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
		//glm::vec3 temp = parentTrans->GetPosition();
		Vector3D temp = m_rigid->getPosition();
		glm::vec3 pos(temp.x,temp.y - 10.0f,temp.z);
		pGo->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent))->setPosition(pos);
	}
}

void AIComponent::Serialize(const json &j)
{
	m_isFriendly = ParseBool(j, "isFriendly");
	m_zSpeed = ParseFloat(j,"zSpeed");
	m_xSpeed = ParseFloat(j, "xSpeed");
	m_offset = ParseFloat(j, "offset");
}

void AIComponent::HandleEvent(Event * a_pEvent)
{
	if (a_pEvent->GetEventType() == EEventType::Collision)
	{
		CollisionEvent* cevent = static_cast<CollisionEvent*>(a_pEvent);
		if (cevent->m_gameObject->GetGameObjectName() == "STARTLEVEL_TRIGGER")
		{
			//m_rigid->setVelocity(sinf(m_count*0.01)*1700.0f, 1000.0f, 0.0f);
			m_ySpeed = 2000.0f;
			m_rigid->CollisionLayer = -1;
			LOG_DEBUG("collision with start buffer\n");
		}
		_isDrop = false;
	}
}

void AIComponent::Deactivate(void)
{
	_isDrop = true;
}

void AIComponent::OverrideParams(const json &, float)
{
}

void AIComponent::ImguiUpdate(float a_dt)
{
	/*START_WINDOW("AiComp Params");
	PiImgui->AddSliderFloat("z_speed", -2300.0f, -3000.0f, m_zSpeed);
	END_WINDOW;*/
}
