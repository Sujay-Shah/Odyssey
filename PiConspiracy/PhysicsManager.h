//
//  PhysicsManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "CollisionManager.h"

class PhysicsManager : public Singleton<PhysicsManager>
{
public:

	~PhysicsManager();

	void Init();
	void Update(float deltaTime);
	void ClearAll(bool a_isLevel);
	void ClearAll(void);
	
	void CollisionTrigger();
	
	void AddRigidBodyToPhyManager(RigidBodyComponent* a_rigidBody);
	
	void SetCollisionData(real friction_ = (real)0.9, real restitution_ = (real)0.1, real tolerance_ = (real)0.1);
	
	Contact& GetFractureContact() { return cData.CollisionInfoPool[fracture_contact]; }

	void RemoveRigidBodyComponent(RigidBodyComponent* a_rbcomp);

	void HandleEvents(Event* a_event);

	RigidBodyComponent* m_playerRigidBodyComponent;
private:
	//this bool is to know if the player is in a level or a buffer zone so update will be done accordingly
	//this bool will be set by the trigger events sent to managers
	// if TRUE we are in a level if FALSE
	bool m_isLevel;

	vector<RigidBodyComponent*> m_rigidBodyList;
	vector<RigidBodyComponent*> m_bufferRigidBodyList;

	CollisionManager m_collisionManager;
	real m_CollisionData[3];
	// collision info
	const static unsigned maxContacts = 1024;
	Contact contacts[maxContacts];
	CollisionInfo cData;
	SolveContactSysterm resolver = SolveContactSysterm(maxContacts * 8);

	// fracture
	unsigned fracture_contact;

	//TODO: double check everything and make sure thread clean up is working
	alignas(64) bool flags[2];
	std::thread* workers[2];
	bool isRunning[2];

	float m_deltaTime;

	void ThreadTask(size_t id);

	//for safe thread access
	std::shared_mutex m_mutex;
};