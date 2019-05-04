//
//  SphereCollider.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#include "PiEnginePreCompiled.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider(void)
	: BaseCollider(EColliderShape::Sphere)
{
}

SphereCollider::~SphereCollider(void)
{
}

void SphereCollider::SetRBOwnerState(float mass, float radius_, vec3 position, vec3 velocity,
	vec3 Acceleration, float LinearDamping, float AngularDamping, bool ActiveGravity_, bool BodyStatic_, int CollisionLayer_)
{
	SetRBOwnerState(mass, radius_, Vector3D(position), Vector3D(velocity),
		Vector3D(Acceleration), LinearDamping, AngularDamping, ActiveGravity_, BodyStatic_, CollisionLayer_);
}

void SphereCollider::SetRBOwnerState(float mass, float radius_, Vector3D position, Vector3D velocity,
	Vector3D Acceleration, float LinearDamping, float AngularDamping, bool ActiveGravity_, bool BodyStatic_, int CollisionLayer_)
{
	if (BodyStatic_)
	{
		m_rbOwner->m_type = ERigiBodyType::Static;
		ActiveGravity_ = false;
	}
	else
		m_rbOwner->m_type = ERigiBodyType::Dynamic;


	m_rbOwner->CollisionLayer = CollisionLayer_;
	
	m_rbOwner->setMass(mass);
	m_rbOwner->setVelocity(velocity.x, velocity.y, velocity.z);

	if (ActiveGravity_)
		m_rbOwner->setAcceleration(Vector3D::GRAVITY + Acceleration);
	else
		m_rbOwner->setAcceleration(Acceleration);

	m_rbOwner->setDamping(LinearDamping, AngularDamping);
	m_rbOwner->clearAcc();
	radius = radius_;

	Matrix3x3 tensor;
	real coeff = 0.4f*m_rbOwner->getMass()*radius*radius;
	tensor.setInertiaTensorCoeffs(coeff, coeff, coeff);
	m_rbOwner->setInertiaTensor(tensor);



	m_rbOwner->setCanSleep(false);
	m_rbOwner->setSleep();
	// Set the data common to all particle types
	m_rbOwner->setPosition(position.x, position.y, position.z);
	// Clear the force accumulators
	m_rbOwner->CalculateOriginate();
	CalPhyInner();
}