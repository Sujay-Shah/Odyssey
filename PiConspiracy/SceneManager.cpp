//
//  SceneManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "DirectoryObject.h"
#include "ProjectConfig.h"
#include "ObjectFactory.h"
#include "GameObjectManager.h"
#include "PhysicsManager.h"
#include "FrameRateController.h"
#include "UIManager.h"
#include "StatsManager.h"
#include "Engine.h"
#include "Mesh.h"
#include "EventManager.h"
#include "InputManager.h"

SceneManager::SceneManager():m_currentScene(0), m_prevScene(-1)
{
}


SceneManager::~SceneManager()
{
}

void SceneManager::Initialize(string a_fileName)
{
	m_startScene = 0;
	_numZonesPlayed = 0;
	m_currentScene = m_startScene;
	const DirectoryObject* scene = PiResource->GetFilePath(a_fileName);
	m_j = OpenJsonFile(scene->m_path);
	
	m_sceneCount = m_j["SCENES"].size();

	maxSceneIndex=GetSceneIndex(m_j["SCENES"], "NUMSCENE");

	//rng
	rand.Initialize();
}

bool SceneManager::_isZonePlayed(int zoneIndex)
{
	auto it = std::find(m_playedLevels.begin(),m_playedLevels.end(),zoneIndex);

	if (it != m_playedLevels.end())
	{
		return true;
	}

	return false;
}

void SceneManager::LoadScene(uint32 a_sceneIndex)
{
	////get current json object index from the array of scenes
	//m_Scenes->m_currentParent = m_Scenes->GetJsonObject(m_Scenes->GetRoot(), "SCENES", a_sceneIndex);
	string sceneName = ParseString(m_j["SCENES"][a_sceneIndex], "sceneName");
	ProjectConfig* pc = Singleton<ProjectConfig>::GetInstance();

	string scenePath = pc->GetSceneDir() + sceneName + ".json";

	json scene = OpenJsonFile(scenePath);
	
	uint64 size = scene["GAMEOBJECTS"].size();

 	string prefab;
	//for number of game objects, create game objects
	
	ObjectFactory *objFactory = PiGameObject->GetObjectFactoryInstance();
	//==========================================
	float offset_pos = 0.0f;
	//==========================================
	for (int i = 0; i < size; ++i)
	{
		prefab = ParseString(scene["GAMEOBJECTS"][i], "prefab");
		
		//doesnt matter bool, player will be added in a seperate list
		GameObject* pGO = objFactory->CreateGameObject(prefab,false);	

		//if position exists, override it with level pos values
		pGO->OverrideComponents(scene["GAMEOBJECTS"][i], offset_pos);
		//m_gameObjectInCurrentScene.push_back(pGO);

		//cout << "";
	}
    //LOG_DEBUG("Loaded Successfully");
}

void SceneManager::LoadNextRegion(uint32 a_sceneIndex,float a_offset)
{
	LOG_DEBUG_ARG("Next scene index : {}",a_sceneIndex);
	////get current json object index from the array of scenes
	//m_Scenes->m_currentParent = m_Scenes->GetJsonObject(m_Scenes->GetRoot(), "SCENES", a_sceneIndex);
	string sceneName = ParseString(m_j["SCENES"][a_sceneIndex], "sceneName");
	ProjectConfig* pc = Singleton<ProjectConfig>::GetInstance();

	string scenePath = pc->GetSceneDir() + sceneName + ".json";

	json scene = OpenJsonFile(scenePath);

	uint64 size = scene["GAMEOBJECTS"].size();

	string prefab;
	//for number of game objects, create game objects
	//TODO: decide on the object factory singleton situation
	ObjectFactory *objFactory = PiGameObject->GetObjectFactoryInstance();
	//==========================================
	bool isLevel = !PiGameObject->GetIsLevel();
	GameObject* go; 
	float end_offset;
	float offset_pos_for_next_region = 0.0f;
	if (isLevel)
	{
		go = PiGameObject->GetGameObjectbyName("STARTLEVEL_TRIGGER", !isLevel);
		end_offset = go->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition().z;
		//offset_pos_for_next_region = a_offset - 6000.0f;
		offset_pos_for_next_region = /*a_offset*/ + end_offset - 6000.0f;
	}
	else
	{
		go = PiGameObject->GetGameObjectbyName("ENDLEVEL_TRIGGER", !isLevel);
		assert(go != nullptr);
		end_offset = go->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition().z;
		//offset_pos_for_next_region = a_offset - 30000.0f;
		offset_pos_for_next_region = /*a_offset*/ + end_offset ;
	}
	//==========================================
	
	for (int i = 0; i < size; ++i)
	{

		prefab = ParseString(scene["GAMEOBJECTS"][i], "prefab");

		GameObject* pGO = objFactory->CreateGameObject(prefab, isLevel);

		//if position exists, override it with level pos values
		pGO->OverrideComponents(scene["GAMEOBJECTS"][i], offset_pos_for_next_region);
		//m_gameObjectInCurrentScene.push_back(pGO);


	}
	//LOG_DEBUG_ARG("Region Loaded Successfully : {}", sceneName);
}

void SceneManager::UnloadCurrentScene(void)
{
	//TODO: this needs rework
	//Remove gameObjects from current scene
	PiGameObject->DeActivateAllGameObjects();
	//PiUI->DeActivateUIGOs();
	PiPhysics->ClearAll();
}

void SceneManager::UnloadRegion(bool a_isLevel)
{
	//PiGameObject->DeActivateAllGameObjects();
	PiGameObject->DeActivateGosInRegion(a_isLevel);
	//PiUI->DeActivateUIGOs();
	PiPhysics->ClearAll(a_isLevel);
}


void SceneManager::ChangeScene(uint32 a_sceneIndex)
{
	if (a_sceneIndex < m_sceneCount)
	{
		m_currentScene = a_sceneIndex;		
		UnloadCurrentScene();
		LoadScene(a_sceneIndex);
	}
	else
	{
		m_currentScene = 0;
		UnloadCurrentScene();
		LoadScene(m_currentScene);	
        LOG_DEBUG("Scene index out of bounds");
	}

	//TODO:Uncomment in game check after development
	if (PiEngine->GetInGameStatus() && PiGameObject->m_Player == nullptr)
	{
		
		PiGameObject->m_Player=PiGameObject->GetObjectFactoryInstance()->CreateGameObject("Player",false);
		PiGameObject->m_Player->GetComponent<MeshComponent>(STRINGIFY(MeshComponent))->GetMesh()->SetRenderable(true);
	}
}

void SceneManager::NextScene(void)
{
	m_currentScene += 1;
	ChangeScene(m_currentScene);
}

void SceneManager::HandleEvent(Event * a_event)
{
	//TODO:Randomize the level Selection
	
	if (a_event->GetEventType() == EEventType::UnloadBufferRegion)
	{
		UnloadCurrentScene();
	}

	else if (a_event->GetEventType() == EEventType::TriggerLevel)
	{
		LoadLevelEvent* e = static_cast<LoadLevelEvent*>(a_event);
		
		//Start Level trigger, unloads prev buffer region, loads next buffer region
		if (e->index == 0)
		{
			UnloadRegion(e->isLevel);			
			LoadNextRegion(2, e->offset);
		}

		//Buffer Start Trigger, loads next level, clear last buffer
		else if (e->index == 1)
		{
			m_prevScene = m_currentScene;
			//------------------------------------------------------------------------------------
			UnloadRegion(e->isLevel);
			
			//add prev level index in the Q, till threshold t
			if (m_playedLevels.size()<=8)
			{
				m_playedLevels.push_back(m_prevScene);
				
			}
			else
			{
				m_playedLevels.pop_front();
				m_playedLevels.push_back(m_prevScene);
			}
			//Randomize again, if its the same level in the queue
			
			do
			{
				m_currentScene = rand.GetRandomNumberBetween(3,maxSceneIndex);
				
				//Hard Coded Tutorial level value
				if (m_currentScene == 6 && PiInput->m_Skipcounter != 0)//||PiStats->GetifTutorialVisisted() == true)
					m_currentScene = rand.GetRandomNumberBetween(3, maxSceneIndex);//continue;
				
			} while (_isZonePlayed(m_currentScene));

			//m_currentScene++;
			++_numZonesPlayed;
			if (_numZonesPlayed > 1)
			{
				Event* e = new Event;
				e->SetEventType(EEventType::IncreaseSpeed);
				PiEvent->AddEventToQueue(e);
			}
			//TODO: make this automatic
			int sceneIndex = m_currentScene /*% (maxSceneIndex)*/;

			
			/*if (sceneIndex<3)
			{
				m_currentScene = 3;
				sceneIndex = 3;
			}*/
			//------------------------------------------------------------------------------------
			
			LoadNextRegion(sceneIndex,e->offset);	
		}
	}

	else if (a_event->GetEventType() == EEventType::OnRestart)
	{
		//DO stuff for Restart
		if (PiInput->m_Skipcounter == 0)
		{
			ChangeScene(6);//Whatever the tutorial level number is
			PiStats->Reset();
		}
		else
		{
			ChangeScene(2);
			PiStats->Reset();
		}
	}
	else if (a_event->GetEventType() == EEventType::OnStart)
	{
		//====================================================================================
		//If tutorial player already and player visits the main menu and starts game
		//====================================================================================

		if (PiInput->m_Skipcounter == 0)
		{
			ChangeScene(6);//Whatever the tutorial level number is
			PiStats->Reset();
		}
		else
		{
			ChangeScene(2);
			PiStats->Reset();

		}
		
	}
	else if (a_event->GetEventType() == EEventType::Exit2Main)
	{
		ChangeScene(1);
		PiStats->Reset();
	}

}

int SceneManager::GetCurrentSceneIndex(void)
{
	return m_currentScene;
}

int SceneManager::GetPreviousSceneIndex(void)
{
	return m_prevScene;
}