//
//  SceneManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class GameObject;
class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	void Initialize(string a_pathToSceneCollection);

	void UnloadCurrentScene(void);
	void UnloadRegion(bool);
	void HandleEvent(Event* a_pEvent);
	void ChangeScene(uint32 a_sceneIndex);

	void NextScene(void);


	int GetPreviousSceneIndex(void);
	int GetCurrentSceneIndex(void);

private:
	int _numLevelsForSpeedInc;
	//temp var
	int maxSceneIndex;
	int _numZonesPlayed;

	bool _isZonePlayed(int zoneIndex);
	//vector<GameObject*> m_gameObjectInCurrentScene;
	json m_j;
	void LoadScene(uint32 a_sceneIndex);
	void LoadNextRegion(uint32 a_sceneIndex, float a_offset);
	uint64 m_sceneCount;
	int32 m_currentScene;
	int32 m_prevScene;
	int32 m_startScene;
	std::list<int> m_playedLevels;
	RandomNumberGenerator rand;
};
