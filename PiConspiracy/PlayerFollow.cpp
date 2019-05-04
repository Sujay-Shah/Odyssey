//
//  PlayerFollow.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "PlayerFollow.h"
#include "Renderer.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "Engine.h"
#include "StatsManager.h"

PlayerFollow::PlayerFollow() : Component(STRINGIFY(PlayerFollow))
{

}


PlayerFollow::~PlayerFollow()
{
}

void PlayerFollow::Initialize()
{
	m_cam = PiRenderer->GetMainCamera();
	m_trans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	//m_offset = vec3(0.0f, 20.0f, 40.0f);
}

void PlayerFollow::Update(float a_dTime)
{
	if (!PiEngine->GetInGameStatus())
	{
		return;
	}
	if (m_isCam)
	{
		if(PiStats->m_running)
			m_cam->SetPosition(m_trans->GetPosition() + m_offset);
	}
	else
	{
		if (m_owner->GetGameObjectType()== EGameObjectType::SUN)
		{
		
			m_trans->SetPosition(PiGameObject->m_Player->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition() + glm::vec3(0, 2000.0f, -6000.0f)); //600,-1000
		}
		else
		{
			m_trans->SetPosition(PiGameObject->m_Player->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition() + m_offset);
		}

	}
	
	//m_cam->Set
}

void PlayerFollow::Serialize(const json &j)
{
	m_cam = PiRenderer->GetMainCamera();
	m_trans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_offset.x = ParseFloat(j, "xOffset");
	m_offset.y = ParseFloat(j, "yOffset");
	m_offset.z = ParseFloat(j, "zOffset");
	m_isCam = ParseBool(j, "isCam");

	if (!m_isCam && m_owner->GetGameObjectType() != EGameObjectType::SUN)
	{
		//m_playerTrans = PiGameObject->m_Player->GetComponent<Transform>(STRINGIFY(Transform));
	}
}

void PlayerFollow::HandleEvent(Event * a_pEvent)
{
}

void PlayerFollow::Deactivate(void)
{
	m_cam = nullptr;
	m_trans = nullptr;
}

void PlayerFollow::OverrideParams(const json &,float offset)
{
}

void PlayerFollow::ImguiUpdate(float a_dTime)
{
	/*if (!m_isCam && m_owner->GetGameObjectType() != EGameObjectType::SUN)
	{
		START_WINDOW("PlayerFollow Params");
		PiImgui->AddSliderFloat("xOffset", -30.0f, 10.0f, m_offset.x);
		PiImgui->AddSliderFloat("yOffset", -30.0f, 50.0f, m_offset.y);
		PiImgui->AddSliderFloat("zOffset", 0.0f, 100.0f, m_offset.z);
		END_WINDOW;
	}*/
	//uncomment to manipulate cam 
	START_WINDOW("PlayerFollow Params");
	PiImgui->AddSliderFloat("xOffset", -10.0f, 10.0f, m_offset.y);
	PiImgui->AddSliderFloat("yOffset", 10.0f, 1000.0f, m_offset.y);
	PiImgui->AddSliderFloat("zOffset", 10.0f, 1000.0f, m_offset.z);
	END_WINDOW;
}
