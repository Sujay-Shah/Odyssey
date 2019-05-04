//
//  TumblingCube.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "TumblingCube.h"

#include "GameObject.h"
#include "GameObjectManager.h"

TumblingCube::TumblingCube(void) : Component(STRINGIFY(TumblingCube)), m_counter(0), m_turn(0.0f)
{
}

TumblingCube::~TumblingCube()
{
}

void TumblingCube::Initialize()
{
	m_player = PiGameObject->GetGameObjectbyName("Player",true);
	m_counter = 0;
	m_turn = 0.0f;
}

void TumblingCube::Update(float a_dTime)
{
	
	//float dist = glm::distance(m_pRBodyComp->mp_newbox->body->getPosition().toVec3().z, m_playerRBComp->mp_newbox->body->getPosition().toVec3().z);
	vec3 first = m_playerRBComp->getPosition().toVec3();
	vec3 second = m_pRBodyComp->getPosition().toVec3();

	float dist = first.z - second.z;

	if (abs(dist) <= 1000.0f && m_counter == 0.0f)
	{
		vec3 rot = vec3(-80000.0f, 4000.0f, -80000.0f);
		vec3 point;
		point.y = m_pRBodyComp->getPosition().toVec3().y + m_pTrans->GetScale().y / 2;
		point.x = m_pRBodyComp->getPosition().toVec3().x;
		point.z = m_pRBodyComp->getPosition().toVec3().z;

		//m_pRBodyComp->mp_newbox->body->setOrientation(glm::quat(rot));
		m_pRBodyComp->addTorque(rot);
		m_counter++;
	}
}

void TumblingCube::Serialize(const json & a_j)
{
	m_pTrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_pRBodyComp = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));

	m_playerRBComp = m_player->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
	m_playerTrans = m_player->GetComponent<Transform>(STRINGIFY(Transform));
}

void TumblingCube::HandleEvent(Event * a_pEvent)
{

}

void TumblingCube::Deactivate(void)
{
}

void TumblingCube::OverrideParams(const json &,float)
{
}
