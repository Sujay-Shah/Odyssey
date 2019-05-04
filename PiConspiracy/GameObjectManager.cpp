//
//  GameObjectManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "GameObjectManager.h"
#include "MemoryManager.h"
#include "ObjectFactory.h"


GameObjectManager::GameObjectManager() : m_Factory(nullptr),numOftotalComponents(0)
{
}


GameObjectManager::~GameObjectManager()
{
	flags[0] = false;
	flags[1] = false;

	isRunning[0] = false;
	isRunning[1] = false;

	workers[0]->join();
	workers[1]->join();

	SAFE_DELETE(workers[0]);
	SAFE_DELETE(workers[1]);

	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	m_activeGameObjects.clear();
	m_bufferActiveGameObjects.clear();

	SAFE_DELETE(m_Factory);//mem leak add
}

void GameObjectManager::ThreadTask(size_t a_id)
{
	while (isRunning[a_id])
	{
		if (flags[a_id])
		{
			if (m_isLevel && !m_activeGameObjects.empty())
			{
				size_t size = m_activeGameObjects.size();
				size_t elements = m_activeGameObjects.size() / 2;
				size_t offset = a_id == 0 ? 0 : elements;
				size_t count = a_id == 0 ? elements : size;

				for (size_t i = offset; i < count; ++i)
				{
					m_activeGameObjects[i]->Update(m_deltaTime);
					
				}
			}
			else if (!m_isLevel && !m_bufferActiveGameObjects.empty())
			{
				size_t size = m_bufferActiveGameObjects.size();
				size_t elements = m_bufferActiveGameObjects.size() / 2;
				size_t offset = a_id == 0 ? 0 : elements;
				size_t count = a_id == 0 ? elements : size;

				for (size_t i = offset; i < count; ++i)
				{
					m_bufferActiveGameObjects[i]->Update(m_deltaTime);
				}
			}
			flags[a_id] = false;
		}
	}
}

void GameObjectManager::Initialize(void)
{
	m_activeGameObjects.reserve(400);
	m_bufferActiveGameObjects.reserve(50);

	m_isLevel = false;
	m_Factory = new ObjectFactory();

	flags[0] = false;
	flags[1] = false;

	isRunning[0] = true;
	isRunning[1] = true;

	m_deltaTime = 0.0f;
	
	workers[0] = new std::thread(std::bind(&GameObjectManager::ThreadTask, this, 0));
	workers[1] = new std::thread(std::bind(&GameObjectManager::ThreadTask, this, 1));

}

void GameObjectManager::Update(float a_dt/*,bool isPaused*/)
{
	//loop all gos and call their respective updates

	m_deltaTime = a_dt;
	memset(flags, true, sizeof(bool) * 2);
	while (flags[0] && flags[1]) 
	{
	}
	if (m_Player != nullptr)
	{
		m_Player->Update(a_dt);
	}
}

void GameObjectManager::AddGameObjectToActiveList(GameObject * a_pGo , bool a_isLevel)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	numOftotalComponents += a_pGo->m_activeComponents.size();
	//load level for next region, so next region will be inverse of current region's flag
	if (a_isLevel)
	{
		m_activeGameObjects.push_back(a_pGo);
	}
	else
	{
		m_bufferActiveGameObjects.push_back(a_pGo);
	}
}

void GameObjectManager::DeActivateAllGameObjects(void)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	size_t count = m_activeGameObjects.size();
	for (size_t i = 0; i < count; ++i)
	{
		m_activeGameObjects[i]->DeActivate();
		PiMemory->SetGameObjectBackInMemory(m_activeGameObjects[i]);
	}

	m_activeGameObjects.clear();

	size_t count1 = m_bufferActiveGameObjects.size();
	
	for (size_t i = 0; i < count1; ++i)
	{
		m_bufferActiveGameObjects[i]->DeActivate();
		PiMemory->SetGameObjectBackInMemory(m_bufferActiveGameObjects[i]);
	}
	m_bufferActiveGameObjects.clear();

	if (m_Player)
	{
		//PiMemory->SetGameObjectBackInMemory(m_Player);
		m_Player->GetComponent<InputComponent>(STRINGIFY(InputComponent))->ResetPos();
	}
	m_isLevel = false;
}

void GameObjectManager::DeActivateGosInRegion(bool a_isLevel)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	//if current is level, than remove bufferList
	if (!a_isLevel)
	{
		size_t count = m_activeGameObjects.size();
		for (size_t i = 0; i < count; ++i)
		{
			m_activeGameObjects[i]->DeActivate();
			PiMemory->SetGameObjectBackInMemory(m_activeGameObjects[i]);
		}

		m_activeGameObjects.clear();
	}
	else
	{
		size_t count = m_bufferActiveGameObjects.size();

		for (size_t i = 0; i < count; ++i)
		{
			m_bufferActiveGameObjects[i]->DeActivate();
			PiMemory->SetGameObjectBackInMemory(m_bufferActiveGameObjects[i]);
		}
		m_bufferActiveGameObjects.clear();
	}

}

void GameObjectManager::DeActivateGameObject(GameObject * a_pGo)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	if (m_isLevel)
	{
		vector<GameObject*>::iterator it = std::find(m_activeGameObjects.begin(), m_activeGameObjects.end(), a_pGo);
		a_pGo->DeActivate();
		m_activeGameObjects.erase(it);
	}
	else
	{
		vector<GameObject*>::iterator it = std::find(m_bufferActiveGameObjects.begin(), m_bufferActiveGameObjects.end(), a_pGo);
		a_pGo->DeActivate();
		m_bufferActiveGameObjects.erase(it);
	}
	PiMemory->SetGameObjectBackInMemory(a_pGo);
}

ObjectFactory * GameObjectManager::GetObjectFactoryInstance(void)
{
	return m_Factory;
}

void GameObjectManager::HandleEvents(Event* a_event)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	if (a_event->GetEventType() == EEventType::TriggerLevel)
	{
		LoadLevelEvent *levelEvent = static_cast<LoadLevelEvent*>(a_event);
		m_isLevel = levelEvent->isLevel;
	}

	else if (a_event->GetEventType() == EEventType::StartBufferLevel)
	{
		StartBufferRegionEvent *levelEvent = static_cast<StartBufferRegionEvent*>(a_event);
		m_isLevel = levelEvent->isLevel;
	}
	
	if (m_Player)
	{
		m_Player->HandleEvents(a_event);
	}

	if (m_isLevel)
	{
		for (size_t i = 0; i < m_activeGameObjects.size(); ++i)
		{
			m_activeGameObjects[i]->HandleEvents(a_event);
		}
	}
	else
	{
		for (size_t i = 0; i < m_bufferActiveGameObjects.size(); ++i)
		{
			m_bufferActiveGameObjects[i]->HandleEvents(a_event);
		}
	}
}

GameObject * GameObjectManager::GetGameObjectbyName(string go_name,bool a_isLevel)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	if (a_isLevel)
	{
		size_t size = m_activeGameObjects.size();
		for (int i = 0; i < size; ++i)
		{
			if (go_name == m_activeGameObjects[i]->GetGameObjectName())
			{
				return m_activeGameObjects[i];
			}
		}
	}
	else
	{
		size_t size = m_bufferActiveGameObjects.size();
		for (int i = 0; i < size; ++i)
		{
			if (go_name == m_bufferActiveGameObjects[i]->GetGameObjectName())
			{
				return m_bufferActiveGameObjects[i];
			}
		}
	}

    LOG_DEBUG_ARG("{} Not found in active Go List", go_name);
	return nullptr;
}
