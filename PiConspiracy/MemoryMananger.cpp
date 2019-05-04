//
//  MemoryMananger.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "MemoryManager.h"

#include "Renderable.h"
#include "Mesh.h"
#include "TextRenderer.h"
#include "DebugRenderer.h"
#include "Particle.h"

MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
	Exit();
}

void MemoryManager::Initialize()
{
	// TODO: Always Register any new components added to Engine
	REGISTER_COMPONENT(Transform);
	REGISTER_COMPONENT(LightComponent);
	REGISTER_COMPONENT(InputComponent);
	REGISTER_COMPONENT(MeshComponent);
	REGISTER_COMPONENT(RigidBodyComponent);
	REGISTER_COMPONENT(FractureComponent);
	REGISTER_COMPONENT(NPC_SideMovementComponent);
	//REGISTER_COMPONENT(CameraComponent);
	REGISTER_COMPONENT(NPC_DropBallComponent);
	REGISTER_COMPONENT(TumblingCube);
	REGISTER_COMPONENT(PlayerFollow);
	REGISTER_COMPONENT(Pickups);
	REGISTER_COMPONENT(Canvas);
	REGISTER_COMPONENT(TriggerComponent);
	REGISTER_COMPONENT(Button);
	REGISTER_COMPONENT(TextComponent);
	REGISTER_COMPONENT(NPC_RotateComponent);
	REGISTER_COMPONENT(RiseComponent);
	REGISTER_COMPONENT(Rotations);
	REGISTER_COMPONENT(DropComponent);
	REGISTER_COMPONENT(AIComponent);
	REGISTER_COMPONENT(RotateComponent);
	REGISTER_COMPONENT(CubeFollowComponent);
	REGISTER_COMPONENT(CheckBox);
	REGISTER_COMPONENT(ParticleComponent);
	//REGISTER_COMPONENT(Script);
	REGISTER_COMPONENT(UIController);
	REGISTER_COMPONENT(ScaleUI);
	//---------------------------end of register-----------------------------//

	// number of reserved objects and components in memory pool
	// can be changed based on need
	m_numberOfComponents = 2048;
	m_numberOfGameObjects = 2048;
	m_numberOfColliders = 2048;
	m_numberOfPlanes = 16;
	m_numberOfRenderables = 2048;
	m_numberOfParticles = 10000;

	// number of unique components in the engine
	size_t numOfComponents = m_componentMap.size();

	unordered_map<string, Component*>::iterator it = m_componentMap.begin();
	while(it != m_componentMap.end())
	{
		string type = it->second->GetComponentType();
		if (type == "InputComponent") //|| type == "UIController")
		{
			m_componentMemory[type].push_back(it->second->Create());
		}
		else
		{
			for (size_t i = 0; i < m_numberOfComponents; ++i)
			{
				m_componentMemory[type].push_back(it->second->Create());
			}
		}
		m_firstAvailableComponent[type] = m_componentMemory[type].begin();
		++it;
	}

	//m_gameObjectMemory.reserve(m_numberOfGameObjects);
	for (size_t j = 0; j < m_numberOfGameObjects; ++j)
	{
		m_gameObjectMemory.push_back(new GameObject(numOfComponents));
	}
	m_firstAvailableGameObject = m_gameObjectMemory.begin();

	//TODO: again reserving colliders, but if possible eventually work out a more generic way
	for (size_t k = 0; k < m_numberOfColliders; ++k)
	{
		m_colliderMemory[EColliderShape::Box].push_back(new BoxCollider());
		m_colliderMemory[EColliderShape::Sphere].push_back(new SphereCollider());
	}
	m_firstAvailableCollider[EColliderShape::Box] = m_colliderMemory[EColliderShape::Box].begin();
	m_firstAvailableCollider[EColliderShape::Sphere] = m_colliderMemory[EColliderShape::Sphere].begin();

	for (size_t l = 0; l < m_numberOfPlanes; ++l)
	{
		m_colliderMemory[EColliderShape::Plane].push_back(new PlaneCollider());
	}
	m_firstAvailableCollider[EColliderShape::Plane] = m_colliderMemory[EColliderShape::Plane].begin();

	for (size_t m = 0; m < m_numberOfRenderables; ++m)
	{
		m_renderableMemory["Mesh"].push_back(new Mesh());
		m_renderableMemory["TextRenderer"].push_back(new TextRenderer());
		m_renderableMemory["DebugRenderer"].push_back(new DebugRenderer());
	}
	m_firstAvailablRrenderable["Mesh"] = m_renderableMemory["Mesh"].begin();
	m_firstAvailablRrenderable["TextRenderer"] = m_renderableMemory["TextRenderer"].begin();
	m_firstAvailablRrenderable["DebugRenderer"] = m_renderableMemory["DebugRenderer"].begin();

	for (size_t n = 0; n < m_numberOfParticles; ++n)
	{
		m_particleMemory.push_back(new Particle());
	}
	m_firstAvailableParticle = m_particleMemory.begin();
}

void MemoryManager::Exit(void)
{
	for (Particle *it : m_particleMemory)
	{
		SAFE_DELETE(it);
	}
	m_particleMemory.clear();

	unordered_map<string, list<Renderable*>>::iterator itRenderable = m_renderableMemory.begin();
	while (itRenderable != m_renderableMemory.end())
	{
		list<Renderable*>::iterator renderableListIt = m_renderableMemory[itRenderable->first].begin();
		while (renderableListIt != m_renderableMemory[itRenderable->first].end())
		{
			Renderable* renderable = *renderableListIt;
			SAFE_DELETE(renderable);
			++renderableListIt;
		}
		m_renderableMemory[itRenderable->first].clear();

		++itRenderable;
	}
	m_renderableMemory.clear();

	unordered_map<EColliderShape, list<BaseCollider*>>::iterator itShape = m_colliderMemory.begin();
	while (itShape != m_colliderMemory.end())
	{
		list<BaseCollider*>::iterator shapeListIt = m_colliderMemory[itShape->first].begin();
		while (shapeListIt != m_colliderMemory[itShape->first].end())
		{
			BaseCollider* baseCollider = *shapeListIt;
			SAFE_DELETE(baseCollider);
			++shapeListIt;
		}
		m_colliderMemory[itShape->first].clear();

		++itShape;
	}
	m_colliderMemory.clear();

	unordered_map<string, Component*>::iterator it = m_componentMap.begin();
	while (it != m_componentMap.end())
	{
		list<Component*>::iterator compIterator = m_componentMemory[it->first].begin();
		while (compIterator != m_componentMemory[it->first].end())
		{
			Component* component = *compIterator;
			SAFE_DELETE(component);
			++compIterator;
		}
		m_componentMemory[it->first].clear();
		
		SAFE_DELETE(it->second);
		++it;
	}
	m_componentMap.clear();
	m_componentMemory.clear();

	for (GameObject* it : m_gameObjectMemory)
	{
		SAFE_DELETE(it);
	}
	m_gameObjectMemory.clear();
}

GameObject * MemoryManager::GetGameObjectFromMemory(void)
{
	GameObject* gameObject = *m_firstAvailableGameObject;
	if (gameObject != nullptr && !gameObject->GetActiveFlag())
	{
		gameObject->SetActiveFlag(true);
		++m_firstAvailableGameObject;
		return gameObject;
	}
	cout << "memory out of GameObjects\n";
	return nullptr;
}

void MemoryManager::SetGameObjectBackInMemory(GameObject * a_gameObject)
{
	a_gameObject->SetActiveFlag(false);
	list<GameObject*>::iterator gameObject = std::find(m_gameObjectMemory.begin(), m_gameObjectMemory.end(), a_gameObject);
	m_gameObjectMemory.splice(m_gameObjectMemory.end(), m_gameObjectMemory, gameObject);
}

Component* MemoryManager::GetComponentFromMemory(string a_componentType)
{
	Component* component = *m_firstAvailableComponent[a_componentType];
	if (component != nullptr && !component->GetActiveFlag())
	{
		component->SetActiveFlag(true);
		++m_firstAvailableComponent[a_componentType];
		return component;
	}
	cout << "No component of type " + a_componentType + " found\n";
	return nullptr;
}

void MemoryManager::SetComponentBackInMemory(Component * a_component)
{
	a_component->SetActiveFlag(false);
	a_component->SetParent(nullptr);
	string componentType = a_component->GetComponentType();
	list<Component*>::iterator component = std::find(m_componentMemory[componentType].begin(), m_componentMemory[componentType].end(), a_component);
	m_componentMemory[componentType].splice(m_componentMemory[componentType].end(), m_componentMemory[componentType], component);

}

BaseCollider * MemoryManager::GetColliderFromMemory(EColliderShape a_colliderShape)
{
	BaseCollider* collider = *m_firstAvailableCollider[a_colliderShape];
	if (collider != nullptr && !collider->GetActiveFlag())
	{
		collider->SetActiveFlag(true);
		++m_firstAvailableCollider[a_colliderShape];
		return collider;
	}
	return nullptr;
}

void MemoryManager::SetColliderBackInMemory(BaseCollider * a_collider)
{
	a_collider->SetActiveFlag(false);
	a_collider->SetRBOwner(nullptr);
	EColliderShape colliderShape = a_collider->GetColliderShape();
	list<BaseCollider*>::iterator collider = std::find(m_colliderMemory[colliderShape].begin(), m_colliderMemory[colliderShape].end(), a_collider);
	m_colliderMemory[colliderShape].splice(m_colliderMemory[colliderShape].end(), m_colliderMemory[colliderShape], collider);
}

Renderable * MemoryManager::GetRenderableFromMemory(string a_renderableName)
{
	Renderable* renderable = *m_firstAvailablRrenderable[a_renderableName];
	if (renderable != nullptr && !renderable->GetActiveFlag())
	{
		renderable->SetActiveFlag(true);
		++m_firstAvailablRrenderable[a_renderableName];
		return renderable;
	}
	return nullptr;
}

void MemoryManager::SetRendrableBackInMemory(Renderable * a_renderable, string a_renderableName)
{
	a_renderable->SetActiveFlag(false);
	list<Renderable*>::iterator renderable = std::find(m_renderableMemory[a_renderableName].begin(), m_renderableMemory[a_renderableName].end(), a_renderable);
	m_renderableMemory[a_renderableName].splice(m_renderableMemory[a_renderableName].end(), m_renderableMemory[a_renderableName], renderable);
}

Particle * MemoryManager::GetParticleFromMemory(void)
{
	Particle *particle = *m_firstAvailableParticle;
	if (particle != nullptr && !particle->GetActiveFlag())
	{
		particle->SetActiveFlag(true);
		++m_firstAvailableParticle;
		return particle;
	}
	return nullptr;
}

void MemoryManager::SetParticleBackInMemory(Particle * a_particle)
{
	a_particle->SetActiveFlag(false);
	list<Particle*>::iterator particle = std::find(m_particleMemory.begin(), m_particleMemory.end(), a_particle);
	m_particleMemory.splice(m_particleMemory.end(), m_particleMemory, particle);
}
