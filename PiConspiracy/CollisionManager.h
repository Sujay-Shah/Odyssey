//
//  CollisionManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

//class Contact;
struct CollisionInfo
{
	void reset(collision_flag maxContacts)
	{
		FlagOfcon = maxContacts;
		currentcount = 0;
		CollisionInfo = CollisionInfoPool;
	}
	bool hasMoreContacts()
	{
		return FlagOfcon > 0;
	}

	Contact *CollisionInfoPool;
	real toleranceForce;
	real frictionForce;
	real restitution;
	int FlagOfcon;
	collision_flag currentcount;
	Contact *CollisionInfo;
	Contact *indvi_pool;
	real ownflag;
	void addContacts(collision_flag count);
};

class CrossLine
{
public:

	static bool CollisionBeBallAHPlane(const SphereCollider &sphere, const PlaneCollider &plane);

	static bool CollisionBeBallABall(const SphereCollider &first, const SphereCollider &sec);

	static bool CollisionBeBoxABox(const BoxCollider &first, const BoxCollider &sec);

	static bool CollisionBeBoxAPlane(const BoxCollider &box, const PlaneCollider &plane);
};
class BaseColldier;
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	//static collision_flag ColCheck_BallAPlane(const SphereCollider &sphere,	const PlaneCollider &plane,	CollisionInfo *info);

	//static collision_flag ColCheck_BoxABox(const BoxCollider &fisrt, const BoxCollider &sec, CollisionInfo *info, int static_flag = 0);

	//double check this
	//static collision_flag ColCheck_BallAHPlane(const SphereCollider &sphere, const PlaneCollider &plane, CollisionInfo *info);

	//static collision_flag ColCheck_BallABall(const SphereCollider &first, const SphereCollider &sec, CollisionInfo *info, int static_flag = 0);

	//double check this
	//static collision_flag ColCheck_BallAHPlane(const BoxCollider &box, const PlaneCollider &plane, CollisionInfo *info);

	//static collision_flag ColCheck_BallABox(const BoxCollider &box,	const SphereCollider &sphere, CollisionInfo *info, int static_flag = 0);

	static collision_flag ColCheckBoxToPoint(const BoxCollider &box, const Vector3D &point, CollisionInfo *info);

	collision_flag CheckCollisionGenerateContact(const BaseCollider &a_collider1, const BaseCollider &a_collider2, CollisionInfo *info);

	collision_flag(*CollisionCheckFunctions[EColliderShape::NUM][EColliderShape::NUM])(const BaseCollider &a_collider1, const BaseCollider &a_collider2, CollisionInfo *info);

};