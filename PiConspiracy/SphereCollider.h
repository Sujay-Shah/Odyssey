//
//  SphereCollider.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#pragma once

class SphereCollider : public BaseCollider
{
public:
	SphereCollider(void);
	~SphereCollider(void);

	void SetRBOwnerState(float mass, float radius_, vec3 position, vec3 velocity,
		vec3 Acceleration, float LinearDamping = 0.95f, float AngularDamping = 0.8f,
		bool ActiveGravity_ = true, bool BodyStatic_ = false, int CollisionLayer = -1);

	inline void SetBreaker(bool a_breaker) { m_breaker = a_breaker; }
	inline bool GetBreaker(void) { return m_breaker; }

	inline void SetRadius(real a_radius) { radius = a_radius; }
	inline real GetRadius(void) const { return radius; }

private:
	void SetRBOwnerState(float mass, float radius_, Vector3D position, Vector3D velocity,
		Vector3D Acceleration, float LinearDamping = 0.95f, float AngularDamping = 0.8f,
		bool ActiveGravity_ = true, bool BodyStatic_ = false, int CollisionLayer = -1);
	
	real radius;
	// use for fractureComponent 
	bool m_breaker = false;
};