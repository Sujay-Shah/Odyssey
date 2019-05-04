//
//  Shoot.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Shoot.h"
#include "Renderer.h"
#include "Camera.h"
#include "EventManager.h"
#include "GameObjectManager.h"
#include "Vector3D.h"

Shoot::Shoot():Component(STRINGIFY(Shoot))
{

}


Shoot::~Shoot()
{
}

void Shoot::Initialize()
{
	half.x =  0.5f;
	half.y = 0.5f;
}

void Shoot::Update(float a_dTime)
{
	/*PiRenderer->GetCamera(false)->SetPosition(m_owner->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition() + glm::vec3(0, 3, 10));*/

	
}

void Shoot::Serialize(const json &j)
{
	m_forceXZ = ParseFloat(j, "forceXZ");
	m_forceY = ParseFloat(j, "forceY");
}

void Shoot::HandleEvent(Event * a_pEvent)
{
	if (a_pEvent->GetEventType() == EventType::Shoot)
	{
		ShootEvent* shoot = static_cast<ShootEvent*>(a_pEvent);
		cursorPos = shoot->mousePos;
		cursorPos.x = cursorPos.x / WINDOW_WIDTH;
		cursorPos.y = cursorPos.y / WINDOW_HEIGHT;

		//cout << "x : " << cursorPos.x << " y: " << cursorPos.y<<"\n";
		GameObject* pGo = PiGameObject->GetObjectFactoryInstance()->CreateGameObject("Bullet",PiGameObject->GetIsLevel());
		Transform* parentTrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
		pGo->GetComponent<Transform>(STRINGIFY(Transform))->SetPosition(parentTrans->GetPosition());

		RigidBodyComponent* rigid = pGo->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
	
		if (rigid->m_collider->GetColliderShape() == EColliderShape::Box)
		{
			vec3 force(0, 10, 10);
			Vector3D v(force);
			rigid->addForce(v);
		}
		else if (rigid->m_collider->GetColliderShape() == EColliderShape::Sphere)
		{
			vec3 force; 
			//rigid->mp_newSphere->setState();
			float diffX = 0.0f ,diffY = 0.0f;
			if (cursorPos.x < half.x)
			{
				diffX = half.x - cursorPos.x;
				force=glm::vec3(-m_forceXZ * diffX, m_forceY*cursorPos.y, -m_forceXZ);
			}
			else
			{
				diffX =  cursorPos.x - half.x;
				force = glm::vec3(m_forceXZ * diffX, m_forceY*cursorPos.y, -m_forceXZ);
			}
			//cout << "x : " << diffX << " y: " << cursorPos.y << "\n";
			//force=force	* vec3(cursorPos.x,cursorPos);
			Vector3D v(force);
			rigid->addForce(v);
		}
	}
}

void Shoot::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
}

void Shoot::OverrideParams(const json &, float offset)
{
}
