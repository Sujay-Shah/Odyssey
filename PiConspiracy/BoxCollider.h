//
//  BoxCollider.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#pragma once
class BoxCollider : public BaseCollider
{
public:
	BoxCollider(void);
	~BoxCollider(void);

	void SetRBOwnerState(float mass, vec3 position, vec3 velocity, glm::quat rotation,
		vec3 Acceleration, vec3 halfSize_,
		float LinearDamping = 0.95f, float AngularDamping = 0.8f, bool ActiveGravity_ = true,
		bool BodyStatic_ = false, int CollisionLayer = -1);

	void CalculateMassProperties(real invDensity);

	inline void SetBreaker(bool a_breaker) { m_breaker = a_breaker; }
	inline bool GetBreaker(void) { return m_breaker; }

	inline void SetCanBeBroken(bool a_canBeBroken) { m_canBeBroken = a_canBeBroken; }
	inline bool GetCanBeBroken(void) { return m_canBeBroken; }

	inline void SetHit(bool a_hit) { m_hit = a_hit; }
	inline bool GetHit(void) { return m_hit; }
	
	Vector3D halfSize;

private:
	// make private
	void SetRBOwnerState(float mass, Vector3D position, Vector3D velocity, Quaternion rotation,
		Vector3D Acceleration, Vector3D halfSize_,
		float LinearDamping = 0.95f, float AngularDamping = 0.8f, bool ActiveGravity_ = true,
		bool BodyStatic_ = false, int CollisionLayer = -1);

	// use for fractureComponent 
	bool m_breaker = false;
	bool m_canBeBroken = false;
	bool m_hit = false;
};

