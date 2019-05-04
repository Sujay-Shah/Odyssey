//
//  StatsManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class StatsManager : public Singleton<StatsManager>
{
public:
	StatsManager(void);
	~StatsManager(void);

	void Reset(void);
	void Update(float a_dt);

	inline int32 GetScore(void);
	inline int32 GetMultiplier(void);

		   
	inline int32 GetPickupCount(void);
	inline int32 GetJumpCount(void);
	inline int32 GetPierceCount(void);
	inline int32 GetLivesCount(void);
	inline bool GetifTutorialVisisted(void);
	

	//score variables
	int32	m_score;					
	int32	m_scoreCounter;				// used to get player distance to be used as score
	int32	m_multiplier;				// every 5 pick up multiplier will increase nultiplier by 1
	int32	m_highScore;				//TODO: still not in use, make it work

	//power ups variables
	//for now working on stacking
	//can add as needed					//Id in pick up component values
	int32   m_pickupMultiplier;			//0
	int32	m_jumpCount;				//1
	int32	m_pierceCount;				//2
	int32	m_extraLife;				//3

	bool m_running;

	bool is_TutorialDone;
};

inline int32 StatsManager::GetLivesCount()
{
	return m_extraLife;
}

inline int32 StatsManager::GetScore()
{
	return m_score;
}

inline int32 StatsManager::GetMultiplier()
{
	return m_multiplier;
}

inline int32 StatsManager::GetPickupCount()
{
	return m_pickupMultiplier;
}

inline int32 StatsManager::GetJumpCount()
{
	return m_jumpCount;
}

inline int32 StatsManager::GetPierceCount()
{
	return m_pierceCount;
}

inline bool StatsManager::GetifTutorialVisisted(void)
{
	return is_TutorialDone;
}

