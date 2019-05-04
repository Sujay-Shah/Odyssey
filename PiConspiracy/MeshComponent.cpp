//
//  MeshComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/30/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "GameObjectManager.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "RenderableContainer.h"
#include "ShaderManager.h"
#include "Material.h"
#include "MemoryManager.h"
#include "StatsManager.h"
#include "Engine.h"

MeshComponent::MeshComponent() :
Component(STRINGIFY(MeshComponent)),
m_pMesh(nullptr),
m_pTransform(nullptr),
m_Type(0),   // default
m_strModelName(""),
m_strMaterialName("PiDefault.json"),
m_bReceiveShadows(true),
m_bIsRenderable(true),
_hudID(-1)
{

}


MeshComponent::~MeshComponent()
{
	
//	SAFE_DELETE(m_pTransform);
	//SAFE_DELETE(m_pMesh);
}

void MeshComponent::Initialize()
{

}

void MeshComponent::Serialize(const json &read_value)
{
	m_Type = ParseInt(read_value, "type");

	
	_hudID = ParseInt(read_value, "hudID");

    if (ValueExists(read_value, "modelName"))
    {
	    string modelName    = ParseString(read_value, "modelName");
	    m_strModelName      = modelName.c_str();
    }

	if (ValueExists(read_value, "materialName"))
	{
		string materialName = ParseString(read_value, "materialName");
		m_strMaterialName = materialName.c_str();
	}

	m_pMesh = static_cast<Mesh*>(PiMemory->GetRenderableFromMemory("Mesh"));
    m_pMesh->Initialize((Mesh::eType)m_Type, m_strMaterialName,  m_strModelName);

	if (ValueExists(read_value, "isRenderable"))
	{
		bool isRenderable = ParseBool(read_value, "isRenderable");
		m_bIsRenderable = isRenderable;
	}

	m_pMesh->SetRenderable(m_bIsRenderable);

	/*if (m_owner->GetGameObjectType()==EGameObjectType::HUD)
	{
		ShaderManager* shader = m_pMesh->GetMaterial()->GetShader();
		shader->Use();
		shader->SetInt(shader->GetShaderID(), "isCoolDown", 1);
		PiRenderer->GetShapeGenerator()->SetIsCoolDown(true);
	}*/
	//initialize cooldown
	if(m_owner->GetGameObjectName()=="UI_dashLogo"|| m_owner->GetGameObjectName() == "UI_dashCoolDownLogo")
	{ 
		m_pMesh->GetMaterial()->m_customAlpha = true;
		m_pMesh->GetMaterial()->m_alpha = 1.0f;
	}
	else
		m_pMesh->GetMaterial()->m_isCoolDown = true;
}

void MeshComponent::Update(float a_dTime)
{
	_UpdatePowerUpHuD();
	m_pTransform = m_owner->GetComponent<Transform>(STRINGIFY(Transform));

	m_pMesh->SetModelMatrix(m_pTransform->GetModelingMatrix());


	if (m_owner->GetGameObjectName() == "SlowMoQuad")
	{

		if (PiEngine->getIsGameSlowed())
		{
			m_pMesh->SetRenderable(true);
		}
		else
		{
			m_pMesh->GetMaterial()->SetAllFalse();
			m_pMesh->SetRenderable(false);
		}
	}


	if (m_pMesh->IsRenderable())
	{
		RenderableContainer* pContainer = PiRenderer->GetRenderableContainer();
		pContainer->PushRenderable(m_pMesh);
	}
	//m_pMesh->SetRenderable(true);
}


void MeshComponent::HandleEvent(Event * a_pEvent)
{
	if (a_pEvent->GetEventType() == EEventType::ReloadPrefabs)
	{
		m_pMesh->Initialize((Mesh::eType)m_Type, m_strMaterialName, m_strModelName);
	}
	else if (a_pEvent->GetEventType() == EEventType::MultiplierPicked)
	{
		OnMultiplierPickup* m = static_cast<OnMultiplierPickup*>(a_pEvent);
		//_hudIDbyEvent = m->m_hudId;
		if (_hudID == m->m_hudId)
		{
			m_pMesh->GetMaterial()->m_isCoolDown = false;
		}
	}
	else if (a_pEvent->GetEventType() == EEventType::ResetPickupHUD /*|| a_pEvent->GetEventType() == EEventType::OnStart*/)
	{
		if (_hudID >0 && _hudID<6)
		{
			m_pMesh->GetMaterial()->m_isCoolDown = true;
		}
	}
	else if (a_pEvent->GetEventType() == EEventType::PowerUpUsed)
	{
		/*ShaderManager* shader = m_pMesh->GetMaterial()->GetShader();
		shader->Use();
		shader->SetInt(shader->GetShaderID(),"isCoolDown", 1);*/
		//PiRenderer->GetShapeGenerator()->SetIsCoolDown(true);

		OnPowerUpUsed* p = static_cast<OnPowerUpUsed*>(a_pEvent);
		if (m_owner->GetGameObjectName() == "UI_boostLogo" && p->m_powerUpType == "boost" /*||*/
			/*m_owner->GetGameObjectName() == "UI_jumpLogo" && p->m_powerUpType == "jump" ||
			m_owner->GetGameObjectName() == "UI_respawnLogo" && p->m_powerUpType == "respawn" */
			)
		{
			m_pMesh->GetMaterial()->m_isCoolDown = true;
		}
		if (m_owner->GetGameObjectName() == "UI_jumpLogo" && p->m_powerUpType == "jump")
		{
			m_pMesh->GetMaterial()->m_isCoolDown = true;
		}
		if (m_owner->GetGameObjectName() == "UI_respawnLogo" && p->m_powerUpType == "respawn")
		{
			m_pMesh->GetMaterial()->m_isCoolDown = true;
		}
		

	}

	else if (a_pEvent->GetEventType() == EEventType::TutorialTexture)
	{
		TutorialTextureEvent* p = static_cast<TutorialTextureEvent*>(a_pEvent);

		if (m_owner->GetGameObjectName() == "SlowMoQuad" && p->TutorialTexture_Type == "Jump")
		{
			//Enable Jump Texture
			m_pMesh->GetMaterial()->m_jumpTexture = true;

		}

		else if (m_owner->GetGameObjectName() == "SlowMoQuad" && p->TutorialTexture_Type == "Boost")
		{
			m_pMesh->GetMaterial()->m_BoostTexture = true;

		}

		else if ( m_owner->GetGameObjectName() == "SlowMoQuad" &&  p->TutorialTexture_Type == "Left")
		{
			m_pMesh->GetMaterial()->m_leftTexture =  true;
		}

		else if (m_owner->GetGameObjectName() == "SlowMoQuad" && p->TutorialTexture_Type == "Right")
		{
			m_pMesh->GetMaterial()->m_rightTexture = (true);
		}
		
		else if (m_owner->GetGameObjectName() == "SlowMoQuad" && p->TutorialTexture_Type == "PickUp")
		{
			m_pMesh->GetMaterial()->m_PickupTexture = (true);
		}

		else if (m_owner->GetGameObjectName() == "SlowMoQuad" && p->TutorialTexture_Type == "ELife")
		{
			m_pMesh->GetMaterial()->m_ElifeTexture = (true);
		}

	}
	else if (a_pEvent->GetEventType() == EEventType::PowerUpPicked || a_pEvent->GetEventType() == EEventType::OnStart /*|| a_pEvent->GetEventType() == EEventType::BoostPicked*/)
	{
		
		/*ShaderManager* shader = m_pMesh->GetMaterial()->GetShader();
		shader->Use();
		shader->SetInt(shader->GetShaderID(), "isCoolDown", 0);*/
		if (a_pEvent->GetEventType() == EEventType::PowerUpPicked)
		{
			OnPowerUpPicked* p = static_cast<OnPowerUpPicked*>(a_pEvent);
			if (m_owner->GetGameObjectName() == "UI_boostLogo" && p->m_powerUpType=="boost" /*||*/
				/*m_owner->GetGameObjectName() == "UI_jumpLogo" && p->m_powerUpType == "jump" ||
				m_owner->GetGameObjectName() == "UI_respawnLogo" && p->m_powerUpType == "respawn" */
				)
			{
				m_pMesh->GetMaterial()->m_isCoolDown = false;
			}
			if (m_owner->GetGameObjectName() == "UI_jumpLogo" && p->m_powerUpType == "jump")
			{
				m_pMesh->GetMaterial()->m_isCoolDown = false;
			}
			if (m_owner->GetGameObjectName() == "UI_respawnLogo" && p->m_powerUpType == "respawn")
			{
				m_pMesh->GetMaterial()->m_isCoolDown = false;
			}
		}
	}
}

void MeshComponent::Deactivate(void)
{
	//m_owner = nullptr;
	//m_activeFlag = false;

	PiMemory->SetRendrableBackInMemory(m_pMesh, "Mesh");

	m_pMesh = nullptr;
}

void MeshComponent::OverrideParams(const json&j,float offset)
{
	if (ValueExists(j, "hudID"))
	{
		_hudID = ParseInt(j, "hudID");
	}
}

void MeshComponent::_UpdatePowerUpHuD()
{
	if (PiStats->GetJumpCount()<=0 && m_owner->GetGameObjectName() == "UI_jumpLogo" ||
		PiStats->GetPierceCount() <=0 && m_owner->GetGameObjectName() == "UI_boostLogo"||
		PiStats->GetLivesCount() <=0 && m_owner->GetGameObjectName() == "UI_respawnLogo")
	{
		m_pMesh->GetMaterial()->m_isCoolDown = true;
	}

	/*if (_hudID==_hudIDbyEvent)
	{
		m_pMesh->GetMaterial()->m_isCoolDown = false;
	}*/
}
