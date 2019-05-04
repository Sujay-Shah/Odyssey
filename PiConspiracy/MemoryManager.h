//
//  MemoryManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

#define REGISTER_COMPONENT(Comp) \
{ \
	RegisterComponent<Comp>(STRINGIFY(Comp)); \
} \

class MemoryManager : public Singleton<MemoryManager>
{
public:
	MemoryManager(void);
	~MemoryManager(void);

	void Initialize(void);
	void Exit(void);

	template <typename T>
	void RegisterComponent(const char* a_componentName)
	{
		T* component = new T();

		//check to avoid duplicate component registrations
		unordered_map<string, Component*>::iterator it = m_componentMap.find(a_componentName);
		if (it == m_componentMap.end())
		{
			m_componentMap.insert({ a_componentName , component });
		}
		else
		{
			cout << "Component already registered\n";
		}
	}


	GameObject* GetGameObjectFromMemory(void);
	void SetGameObjectBackInMemory(GameObject* a_gameObject);

	Component* GetComponentFromMemory(string a_componentType);
	void SetComponentBackInMemory(Component* a_component);
	
	//TODO: get for any other types, shapes are added next step if possible need to make more generic
	BaseCollider* GetColliderFromMemory(EColliderShape a_colliderShape);
	void SetColliderBackInMemory(BaseCollider* a_collider);

	Renderable* GetRenderableFromMemory(string a_renderableName);
	void SetRendrableBackInMemory(Renderable* a_renderable, string a_renderableName);

	Particle* GetParticleFromMemory(void);
	void SetParticleBackInMemory(Particle *a_particle);

private:
	list<GameObject*> m_gameObjectMemory;
	list<GameObject*>::iterator m_firstAvailableGameObject;

	unordered_map<string, Component*> m_componentMap; 
	unordered_map<string, list<Component*>> m_componentMemory;
	unordered_map<string, list<Component*>::iterator> m_firstAvailableComponent;

	//TODO: add models/shapes to memory as well, shapes are added next step if possible need to make more generic
	unordered_map<EColliderShape, list<BaseCollider*>> m_colliderMemory;
	unordered_map<EColliderShape, list<BaseCollider*>::iterator> m_firstAvailableCollider;

	//for renderable objects
	unordered_map<string, list<Renderable*>> m_renderableMemory;
	unordered_map<string, list<Renderable*>::iterator> m_firstAvailablRrenderable;

	//for particles
	list<Particle*> m_particleMemory;
	list<Particle*>::iterator m_firstAvailableParticle;

	size_t m_numberOfGameObjects;
	size_t m_numberOfComponents;
	size_t m_numberOfColliders;
	size_t m_numberOfPlanes;
	
	size_t m_numberOfRenderables;
	size_t m_numberOfParticles;
	//TODO: eventually for particles
};