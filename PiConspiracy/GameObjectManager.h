//
//  GameObjectManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "ObjectFactory.h"
class GameObjectManager : Singleton<GameObjectManager>
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Initialize(void);
	void Update(float a_dt/*, bool isPaused*/);
	void AddGameObjectToActiveList(GameObject* a_pGo, bool a_isLevel);
	void DeActivateAllGameObjects();
	void DeActivateGosInRegion(bool);
	void DeActivateGameObject(GameObject* a_pGo);

	void UnloadSceneObjectsforNumber(int level_number);


	ObjectFactory* GetObjectFactoryInstance(void);
	void HandleEvents(Event* a_event);
	GameObject* GetGameObjectbyName(string go_name, bool a_isLevel);
	
	GameObject* m_Player;

	inline vector<GameObject*> GetGameObjectsActiveList()
	{
		return m_activeGameObjects;
	}

	bool GetIsLevel(void) { return m_isLevel; }
private:

	//this bool is to know if the player is in a level or a buffer zone so update will be done accordingly
	//this bool will be set by the trigger events sent to managers
	// if TRUE we are in a level if FALSE
	bool m_isLevel;

	ObjectFactory* m_Factory;
	vector<GameObject*> m_activeGameObjects;
	vector<GameObject*> m_bufferActiveGameObjects;
	uint64 numOftotalComponents;

	//TODO: double check everything and make sure thread clean up is working
	alignas(64) bool flags[2];
	std::thread* workers[2];
	bool isRunning[2];

	float m_deltaTime;

	void ThreadTask(size_t id);
	std::shared_mutex m_mutex;
};

