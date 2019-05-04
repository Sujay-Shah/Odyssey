//
//  TriggerComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "TriggerComponent.h"

TriggerComponent::TriggerComponent(void)
	: Component(STRINGIFY(TriggerComponent))
{
}

TriggerComponent::~TriggerComponent(void)
{
}

void TriggerComponent::Initialize()
{
	m_id = -1;
}

void TriggerComponent::Update(float a_dTime)
{
}

void TriggerComponent::Serialize(const json &a_j)
{
	if (ValueExists(a_j, "id"))
	{
		m_id = ParseInt(a_j, "id");
	}
}

void TriggerComponent::HandleEvent(Event * a_pEvent)
{
	if (a_pEvent->GetEventType() == EEventType::Triggered)
	{
		//TODO::Crude way, refactor
		TriggeredEvent* trig = static_cast<TriggeredEvent*>(a_pEvent);
		if (trig->id == m_id && trig->behaviour=="Rise")
		{
			//then react
			RiseComponent* rise = m_owner->GetComponent<RiseComponent>(STRINGIFY(RiseComponent));
			rise->isRise = true;
		}

		else if (trig->id == m_id && trig->behaviour == "Drop")
		{
			//then react
			DropComponent* drop = m_owner->GetComponent<DropComponent>(STRINGIFY(DropComponent));
			drop->isDrop = true;
		}

		else if (trig->id == m_id && trig->behaviour == "Tumble_Right")
		{
			//then react
			RigidBodyComponent* tumble = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
			tumble->setAcceleration(2000.0, 0.0, 0.0);
		}

		else if (trig->id == m_id && trig->behaviour == "Tumble_Left")
		{
			//then react
			RigidBodyComponent* tumble = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
			tumble->setAcceleration(-2000.0, 0.0, 0.0);
		
		}

		else if (trig->id == m_id && trig->behaviour == "Set_Dynamic")
		{
			//then react
			RigidBodyComponent* tumble = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
			tumble->BodyStatic = false;
			tumble->m_type = ERigiBodyType::Dynamic;
			tumble->CollisionLayer = 2;
		}

		else if (trig->id == m_id && trig->behaviour == "goUp_Tumble")
		{
			//then react
			RigidBodyComponent* tumble = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
			tumble->setAcceleration(0.0, 10000.0, 0.0);

		}
		
		else if (trig->id == m_id && trig->behaviour == "follow_cube")
		{
			//then react
			CubeFollowComponent* cubef = m_owner->GetComponent<CubeFollowComponent>(STRINGIFY(CubeFollowComponent));
			cubef->findloc = true;
			cubef->isStart = true;

		}

		else if ( trig->behaviour == "Slow_Motion")
		{
			LOG_DEBUG("Reached into the trigger event Hadle Event");
		}

	}
}

void TriggerComponent::Deactivate(void)
{
}

void TriggerComponent::OverrideParams(const json &j,float offset)
{
	if (ValueExists(j, "id"))
	{
		m_id = ParseInt(j, "id");
	}
}

