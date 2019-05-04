//
//  PhysicsManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "PhysicsManager.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "DebugRenderer.h"
#include "ImguiManager.h"
#include "GameObjectManager.h"
#include "ObjectFactory.h"
#include "Engine.h"
#include "RenderableContainer.h"

void PhysicsManager::ThreadTask(size_t id)
{
	while (isRunning[id])
	{	
		//TODO: check thread quit?
		if (flags[id])
		{
			if (m_isLevel && !m_rigidBodyList.empty())
			{
				size_t size = m_rigidBodyList.size();
				size_t elements = m_rigidBodyList.size() / 2;
				size_t offset = id == 0 ? 0 : elements;
				size_t count = id == 0 ? elements : size;
					
				for (size_t i = offset; i < count; ++i)
				{
					if (m_rigidBodyList[i]->m_type == ERigiBodyType::Static)
					{
						//count++;
						continue;
					}

					m_rigidBodyList[i]->calculateIntegrate(m_deltaTime);
					m_rigidBodyList[i]->m_collider->CalPhyInner();
				}
			}
			else if (!m_isLevel && !m_bufferRigidBodyList.empty())
			{
				size_t size = m_bufferRigidBodyList.size();
				size_t elements = m_bufferRigidBodyList.size() / 2;
				size_t offset = id == 0 ? 0 : elements;
				size_t count = id == 0 ? elements : size;

				for (size_t i = offset; i < count; ++i)
				{
					if (m_bufferRigidBodyList[i]->m_type == ERigiBodyType::Static)
					{
						//count++;
						continue;
					}

					m_bufferRigidBodyList[i]->calculateIntegrate(m_deltaTime);
					m_bufferRigidBodyList[i]->m_collider->CalPhyInner();
				}
			}
			flags[id] = false;
		}
	}
}

PhysicsManager::~PhysicsManager()
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
	m_rigidBodyList.clear();
	m_bufferRigidBodyList.clear();
}

void PhysicsManager::Init()
{
	m_rigidBodyList.reserve(400);
	m_bufferRigidBodyList.reserve(50);

	m_isLevel = false;
	SetCollisionData();
	cData.CollisionInfoPool = contacts;

	flags[0] = false;
	flags[1] = false;

	isRunning[0] = true;
	isRunning[1] = true;

	m_deltaTime = 0.0f;
	workers[0] = new std::thread(std::bind(&PhysicsManager::ThreadTask, this, 0));
	workers[1] = new std::thread(std::bind(&PhysicsManager::ThreadTask, this, 1));
}

void PhysicsManager::Update(float deltaTime)
{
	if (!PiEngine->GetInGameStatus())
	{
		return;
	}
  //int count = 0;
	if (PiEngine->GetIsPaused())
	{
		return;
	}
	m_deltaTime = deltaTime;
	memset(flags, true, sizeof(bool) * 2);
	
	while (flags[0] && flags[1]) 
	{
	}
	if (m_playerRigidBodyComponent)
	{
		m_playerRigidBodyComponent->calculateIntegrate(deltaTime);
		m_playerRigidBodyComponent->m_collider->CalPhyInner();
	}
	
	bool bIsDebugDraw = false;
	bIsDebugDraw = PiEngine->IsDebugDrawOn();

	if (bIsDebugDraw)
	{

		if (m_isLevel)
		{
			for (size_t i = 0; i < m_rigidBodyList.size(); ++i)
			{
				

				if (m_rigidBodyList[i]->m_collider->GetColliderShape() == EColliderShape::Plane)
				{
					continue;
				}
				else
				{
				
					DebugRenderer* pDebug = m_rigidBodyList[i]->GetDebugRenderer();
					

					if (m_rigidBodyList[i]->m_collider->GetColliderShape() == EColliderShape::Box)
					{
						
						pDebug->setTypeDraw(0);
						
					}

					if (m_rigidBodyList[i]->m_collider->GetColliderShape() == EColliderShape::Sphere)
					{
						
						pDebug->setTypeDraw(1);
						
					}
				}
			}
		}
		else
		{
			for (size_t i = 0; i < m_bufferRigidBodyList.size(); ++i)
			{
				if (m_bufferRigidBodyList[i]->m_collider->GetColliderShape() == EColliderShape::Plane)
				{
					continue;
				}
				else
				{
					
					DebugRenderer* pDebug = m_bufferRigidBodyList[i]->GetDebugRenderer();

					if (m_bufferRigidBodyList[i]->m_collider->GetColliderShape() == EColliderShape::Box)
					{
						
						pDebug->setTypeDraw(0);
						
					}

					if (m_bufferRigidBodyList[i]->m_collider->GetColliderShape() == EColliderShape::Sphere)
					{
				
						pDebug->setTypeDraw(1);
				
					}
				}
			}
		}
	
		if (m_playerRigidBodyComponent)
		{
			
			DebugRenderer* pDebug = m_playerRigidBodyComponent->GetDebugRenderer();

			
			if (m_playerRigidBodyComponent->m_collider->GetColliderShape() == EColliderShape::Box)
			{
				pDebug->setTypeDraw(0);
			}
		}
				
	}

  // Set up the collision data structure
  cData.reset(maxContacts);
  cData.frictionForce = m_CollisionData[0];
  cData.restitution = m_CollisionData[1];
  cData.toleranceForce = m_CollisionData[2];

  // loading collision
  CollisionTrigger();

  // solve collision
  resolver.resolveContacts(cData.CollisionInfoPool, cData.currentcount, deltaTime);
}

void PhysicsManager::ClearAll(bool a_isLevel)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	if (!a_isLevel)
	{
		m_rigidBodyList.clear();
	}
	else
	{
		m_bufferRigidBodyList.clear();
	}
}

void PhysicsManager::ClearAll(void)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	m_rigidBodyList.clear();
	m_bufferRigidBodyList.clear();

	SetCollisionData();
	cData.CollisionInfoPool = contacts;

	m_isLevel = false;
}



void PhysicsManager::CollisionTrigger()
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	if (m_isLevel)
	{
		//vector<RigidBodyComponent*>::iterator end = m_rigidBodyList.end();
		//vector<RigidBodyComponent*>::iterator it1 = m_rigidBodyList.begin();
		//vector<RigidBodyComponent*>::iterator it2;
		size_t count = m_rigidBodyList.size();
		for (int i = 0; i < count; ++i)
		{
			m_collisionManager.CheckCollisionGenerateContact(*m_playerRigidBodyComponent->m_collider, *m_rigidBodyList[i]->m_collider, &cData);
			if (m_rigidBodyList[i]->CollisionLayer == -1 || m_rigidBodyList[i]->BodyStatic)
			{
				continue;
			}
			for (int j = 0; j < count; ++j)
			{
				if (m_rigidBodyList[i] == m_rigidBodyList[j] || m_rigidBodyList[i]->CollisionLayer != m_rigidBodyList[j]->CollisionLayer || (m_rigidBodyList[i]->BodyStatic && m_rigidBodyList[j]->BodyStatic))
				{
					continue;
				}
				m_collisionManager.CheckCollisionGenerateContact(*m_rigidBodyList[i]->m_collider, *m_rigidBodyList[j]->m_collider, &cData);
			}
		}
	}
	else if (!m_isLevel)
	{
		//vector<RigidBodyComponent*>::iterator end = m_bufferRigidBodyList.end();
		//vector<RigidBodyComponent*>::iterator it1 = m_bufferRigidBodyList.begin();
		//vector<RigidBodyComponent*>::iterator it2;
		size_t count = m_bufferRigidBodyList.size();
		for (int i = 0; i < count; ++i)
		{
			m_collisionManager.CheckCollisionGenerateContact(*m_playerRigidBodyComponent->m_collider, *m_bufferRigidBodyList[i]->m_collider, &cData);
			if (m_bufferRigidBodyList[i]->CollisionLayer == -1 || m_bufferRigidBodyList[i]->BodyStatic)
			{
				continue;
			}
			for (int j = 0; j < count; ++j)
			{
				if (m_bufferRigidBodyList[i] == m_bufferRigidBodyList[j] || m_bufferRigidBodyList[i]->CollisionLayer != m_bufferRigidBodyList[j]->CollisionLayer || (m_bufferRigidBodyList[i]->BodyStatic && m_bufferRigidBodyList[j]->BodyStatic))
				{
					continue;
				}
				m_collisionManager.CheckCollisionGenerateContact(*m_bufferRigidBodyList[i]->m_collider, *m_bufferRigidBodyList[j]->m_collider, &cData);
			}
		}
	}
}

void PhysicsManager::AddRigidBodyToPhyManager(RigidBodyComponent * a_rigidBody)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	string currentRegion = PiGameObject->GetObjectFactoryInstance()->m_belongsTo;
	if (currentRegion == "level")
	{
		m_rigidBodyList.push_back(a_rigidBody);
	}
	else
	{
		m_bufferRigidBodyList.push_back(a_rigidBody);
	}
}


void PhysicsManager::SetCollisionData(real friction_, real restitution_, real tolerance_)
{
  m_CollisionData[0] = friction_;
  m_CollisionData[1] = restitution_;
  m_CollisionData[2] = tolerance_;
}

void PhysicsManager::RemoveRigidBodyComponent(RigidBodyComponent * a_rbcomp)
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	if (m_isLevel)
	{
		vector<RigidBodyComponent*>::iterator it = std::find(m_rigidBodyList.begin(), m_rigidBodyList.end(), a_rbcomp);
		if(it != m_rigidBodyList.end())
			m_rigidBodyList.erase(it);
	}
	else
	{
		vector<RigidBodyComponent*>::iterator it = std::find(m_bufferRigidBodyList.begin(), m_bufferRigidBodyList.end(), a_rbcomp);
		if (it != m_bufferRigidBodyList.end())
			m_bufferRigidBodyList.erase(it);
	}
}

void PhysicsManager::HandleEvents(Event * a_event)
{
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

}
