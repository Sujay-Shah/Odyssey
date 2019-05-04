//
//  BoxCollider.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#include "PiEnginePreCompiled.h"
#include "BoxCollider.h"


BoxCollider::BoxCollider(void)
	: BaseCollider(EColliderShape::Box)
{
}


BoxCollider::~BoxCollider(void)
{
}

void BoxCollider::SetRBOwnerState(float mass, vec3 position, vec3 velocity, glm::quat rotation,
	vec3 Acceleration, vec3 halfSize_,
	float LinearDamping, float AngularDamping, bool ActiveGravity_, bool BodyStatic_
	, int CollisionLayer_)
{
	SetRBOwnerState(mass, Vector3D(position), Vector3D(velocity), Quaternion(rotation),
		Vector3D(Acceleration), Vector3D(halfSize_), LinearDamping, AngularDamping, ActiveGravity_, BodyStatic_
		, CollisionLayer_);
}

void BoxCollider::SetRBOwnerState(float mass, Vector3D position, Vector3D velocity, Quaternion rotation,
	Vector3D Acceleration, Vector3D halfSize_,
	float LinearDamping, float AngularDamping, bool ActiveGravity_, bool BodyStatic_, int CollisionLayer_)
{
	if (BodyStatic_)
	{
		m_rbOwner->m_type = ERigiBodyType::Static;
		ActiveGravity_ = false;
	}
	else
		m_rbOwner->m_type = ERigiBodyType::Dynamic;

	m_rbOwner->CollisionLayer = CollisionLayer_;

	m_rbOwner->setPosition(position.x, position.y, position.z);
	//Quaternion temp;
	//body->setOrientation(temp.DegreeToQuaternion(Vector3D(10, 10, 15)));
	//temp = temp.DegreeToQuaternion(Vector3D(10, 10, 15));


	//glm::quat angle = glm::quat(vec3(0));
	//body->setOrientation(glm::quat(vec3(0)));
	//body->setOrientation(1,0,0,0);
	m_rbOwner->setOrientation(rotation);

	m_rbOwner->setVelocity(velocity.x, velocity.y, velocity.z);
	//body->setRotation(rotation);
	m_rbOwner->setRotation(0, 0, 0);

	//body->


	halfSize = halfSize_;
	real mass_ = mass;
	m_rbOwner->setMass(mass_);

	Matrix3x3 tensor;
	tensor.setBlockInertiaTensor(halfSize, mass_);
	m_rbOwner->setInertiaTensor(tensor);

	m_rbOwner->setLinearDamping(LinearDamping);
	m_rbOwner->setAngularDamping(AngularDamping);
	m_rbOwner->clearAcc();
	if (ActiveGravity_)
		m_rbOwner->setAcceleration(Vector3D::GRAVITY + Acceleration);
	else
		m_rbOwner->setAcceleration(Acceleration);
	//body->setAcceleration(Acceleration);
	m_rbOwner->setCanSleep(false);
	m_rbOwner->setSleep();

	m_rbOwner->CalculateOriginate();
	CalPhyInner();
}

void BoxCollider::CalculateMassProperties(real invDensity)
{
	// Check for infinite mass
	if (invDensity <= 0)
	{
		// Just set zeros for both mass and inertia tensor
		m_rbOwner->setInverseMass(0);
		m_rbOwner->setInverseTensor(Matrix3x3(0.f, 0.f, 0.f,
			0.f, 0.f, 0.f,
			0.f, 0.f, 0.f));
	}
	else
	{
		// Otherwise we need to calculate the mass
		real volume = halfSize.Length() * 2.0f;
		real mass = volume / invDensity;

		m_rbOwner->setMass(mass);

		// And calculate the inertia tensor from the mass and size
		mass *= 0.333f;
		Matrix3x3 tensor;
		tensor.setInertiaTensorCoeffs(
			mass * halfSize.y*halfSize.y + halfSize.z*halfSize.z,
			mass * halfSize.y*halfSize.x + halfSize.z*halfSize.z,
			mass * halfSize.y*halfSize.x + halfSize.z*halfSize.y
		);
		m_rbOwner->setInertiaTensor(tensor);
	}
}