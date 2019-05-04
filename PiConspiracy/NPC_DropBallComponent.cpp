//
//  NPC_DropBallComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "NPC_DropBallComponent.h"

#include "GameObjectManager.h"
#include "ObjectFactory.h"
#include "EventManager.h"


NPC_DropBallComponent::NPC_DropBallComponent(void)
	:Component(STRINGIFY(NPC_DropBallComponent))
	, m_rateOfDrop(0.0f)
{
}

NPC_DropBallComponent::NPC_DropBallComponent(float a_rateOfDrop)
	:Component(STRINGIFY(NPC_DropBallComponent))
	, m_rateOfDrop(a_rateOfDrop)
{
	
	
}


NPC_DropBallComponent::~NPC_DropBallComponent(void)
{
	
}


void NPC_DropBallComponent::Initialize(void)
{

	Event* m_timedDrop = new Event(EEventType::DropBall);
	PiEvent->AddTimedEventToQueue(m_timedDrop, m_rateOfDrop);
	//pos = m_owner->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition();

}

void NPC_DropBallComponent::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
	m_rateOfDrop = 0.0f;
}

void NPC_DropBallComponent::Update(float a_dt)
{

}

void NPC_DropBallComponent::Serialize(const json& a_j)
{
	m_rateOfDrop = ParseFloat(a_j, "RateOfDrop");

}

void NPC_DropBallComponent::HandleEvent(Event* a_pEvent)
{
	if (a_pEvent->GetEventType() == EEventType::DropBall)
	{
		//cout << "ball spawned " << std::endl;
		m_ball = PiGameObject->GetObjectFactoryInstance()->CreateGameObject("Ball",PiGameObject->GetIsLevel());
		pos = m_owner->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition();
		m_ball->GetComponent<Transform>(STRINGIFY(Transform))->SetPosition(pos);
		vec3 s = m_ball->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition();

		ballCreated = true;

		Event* m_timedDrop = new Event(EEventType::DropBall);
		PiEvent->AddTimedEventToQueue(m_timedDrop, m_rateOfDrop);

		Event* m_des = new Event(EEventType::DestroyBall);
		PiEvent->AddTimedEventToQueue(m_des, m_rateOfDrop-2.0f);

	}
	if (a_pEvent->GetEventType() == EEventType::DestroyBall)
	{
		if(m_ball)
		{
			PiGameObject->DeActivateGameObject(m_ball);
			
			//m_ball->SetActiveFlag(false);

		//cout << "ball destroyed " << std::endl;
		}
	}
}

void NPC_DropBallComponent::SetRateOfDrop(float a_rateOfDrop)
{
	m_rateOfDrop = a_rateOfDrop;
}

float NPC_DropBallComponent::GetRateOfDrop()
{
	return m_rateOfDrop;
}

void NPC_DropBallComponent::OverrideParams(const json&,float offset)
{
}
