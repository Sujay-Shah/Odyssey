//
//  GameObject.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "MemoryManager.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"


GameObject::GameObject(size_t a_size)
	: m_activeComponents(),
	m_isRenderable(false),
	m_activeFlag(false),
	m_type(EGameObjectType::None)
{
	m_activeComponents.reserve(a_size);
}


GameObject::~GameObject()
{
	m_activeComponents.clear();
	m_activeComponents.shrink_to_fit();
}

void GameObject::AddComponent(Component* a_component)
{
	a_component->SetParent(this);
	m_activeComponents.push_back(a_component);
}

void GameObject::SetGameObjectType(string a_tagName)
{
	auto it = TAG_NAMES.find(a_tagName);
	if (it != TAG_NAMES.end())
	{
		m_type = it->second;
	}
	else
	{
		m_type = EGameObjectType::None;
		LOG_DEBUG_ARG("Tagname not found\n",a_tagName);
	}
}

EGameObjectType GameObject::GetGameObjectType(void) const
{
	return m_type;
}

void GameObject::Update(float a_dt)
{
	//assert(m_activeComponents);
	//m_gameObjectName;

	for (Component* it : m_activeComponents)
	{
		it->Update(a_dt);
		if (PiResource->GetConfig()->m_presentation)
		{
#ifdef _DEBUG
			it->ImguiUpdate(a_dt);
#endif
		}
		else
		{
			it->ImguiUpdate(a_dt);
		}

	}
}

void GameObject::HandleEvents(Event* a_event)
{
	for (Component* component : m_activeComponents)
	{
		component->HandleEvent(a_event);
	}
}

void GameObject::OverrideComponents(const json& a_j,float offset)
{
	for (Component* it : m_activeComponents)
	{
		it->OverrideParams(a_j, offset);
	}
}

const string GameObject::GetGameObjectName(void) const
{
	return m_gameObjectName;
}
void GameObject::SetGameObjectName(string a_name)
{
	m_gameObjectName = a_name;
}
void GameObject::DeActivate(void)
{
	m_gameObjectName = "";
	m_isRenderable = false;
	m_type = EGameObjectType::None;

	//de-activate its components as well
	for (Component* component : m_activeComponents)
	{
		component->Deactivate();
		PiMemory->SetComponentBackInMemory(component);
	}

	m_activeComponents.clear();
}

