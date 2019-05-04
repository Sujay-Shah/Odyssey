//
//  ObjectFactory.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ObjectFactory.h"
#include "ResourceManager.h"
#include "MemoryManager.h"
#include "GameObjectManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"

ObjectFactory::ObjectFactory()
{
}


ObjectFactory::~ObjectFactory()
{
}

GameObject* ObjectFactory::CreateGameObject(string a_gameObjectName, bool a_isLevel)
{
	//To know if current Go belongs to lvl or buffer
	if (a_isLevel)
	{
		m_belongsTo = "level";
	}
	else
	{
		m_belongsTo = "buffer";
	}
	json* j= PiResource->GetArchetype(a_gameObjectName);
	if (j!=nullptr)
	{
		//create new game Object container
		GameObject* pGO = PiMemory->GetGameObjectFromMemory();
		pGO->SetGameObjectName(a_gameObjectName);
		if (pGO == nullptr)
		{
            LOG_DEBUG("No Objects left in object pool, more alloc needed");
			return nullptr;
		}
		string type = ParseString(*j, "type");
		pGO->SetGameObjectType(type);
		
		//read and set its type
		size_t size = (*j)["COMPONENTS"].size();
		for (size_t i = 0; i < size; ++i) 
		{
			string compName = ParseString((*j)["COMPONENTS"][i], "component");
			Component* pComponent = PiMemory->GetComponentFromMemory(compName);
			pGO->AddComponent(pComponent);
			pComponent->Initialize();
			pComponent->Serialize((*j)["COMPONENTS"][i]);

			//pComponent->OverrideParams()
		}

		if (/*type != "Player"*/ pGO->GetGameObjectType()==EGameObjectType::Player )
		{
			PiGameObject->m_Player = pGO;
			PiPhysics->m_playerRigidBodyComponent = pGO->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));
			
		}
		else if(pGO->GetGameObjectType() == EGameObjectType::Menu || 
			pGO->GetGameObjectType() == EGameObjectType::Button || 
			pGO->GetGameObjectType() == EGameObjectType::UI ||
			pGO->GetGameObjectType() == EGameObjectType::HUD ||
			pGO->GetGameObjectType() == EGameObjectType::SUN ||
			pGO->GetGameObjectType() == EGameObjectType::CheckBox)
		{ 
			PiUI->AddUIGameObjects(pGO);
			return pGO;
		}
		else
		{
			PiGameObject->AddGameObjectToActiveList(pGO, a_isLevel);
		}
		return pGO;
	}
	else
	{
        LOG_ERROR_ARG("Json for {} not found", a_gameObjectName);
        return nullptr;
	}
	return nullptr;
}


