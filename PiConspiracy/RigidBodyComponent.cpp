//
//  RigidBodyComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Engine.h"
#include "RigidBodyComponent.h"
#include "PhysicsManager.h"
#include "MemoryManager.h"
#include "GameObjectManager.h"
#include "Renderer.h"
#include "RenderableContainer.h"
#include "DebugRenderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"

RigidBodyComponent::RigidBodyComponent() : Component(STRINGIFY(RigidBodyComponent)),
BodyStatic(true),
m_type(ERigiBodyType::Static),
m_pDebugRenderer(nullptr)
{

}

RigidBodyComponent::~RigidBodyComponent()
{
	//SAFE_DELETE(m_pDebugRenderer);
}

void RigidBodyComponent::Initialize()
{
	m_pDebugRenderer = static_cast<DebugRenderer*>(PiMemory->GetRenderableFromMemory("DebugRenderer"));
	m_pDebugRenderer->Initialize("Assets/Shaders/Basic");
}

void RigidBodyComponent::Update(float a_dTime)
{
	vec3 rot;

	if (BodyStatic)
	{
		rot = RotationMatrixToEulerAngles(getTransform()).toVec3();
		mat4 m(1.0f);
		glm::quat rotation = glm::quat(rot);
		m_modelMatrix = glm::translate(m, getPosition().toVec3()) * (glm::toMat4(rotation)) * glm::scale(mat4(1.0f), m_halfSize);
		return;
	}


	if (m_collider->GetColliderShape() == EColliderShape::Box)
	{
		BoxCollider* box = static_cast<BoxCollider*>(m_collider);
		if (box->GetHit())
		{
			SetFractureHit();
		}
	}

	if (m_transform != nullptr)
	{
		m_transform->SetPosition(getPosition().toVec3());
		rot = RotationMatrixToEulerAngles(getTransform()).toVec3();
		m_transform->SetEulerAngles(rot);

		mat4 m(1.0f);
		glm::quat rotation = glm::quat(rot);
		if(PiGameObject->m_Player==m_owner)
			m_modelMatrix = glm::translate(m, getPosition().toVec3()) * (glm::toMat4(rotation)) * glm::scale(mat4(1.0f),m_halfSize);
		else
			m_modelMatrix = glm::translate(m, getPosition().toVec3()) * (glm::toMat4(rotation)) * glm::scale(mat4(1.0f),m_halfSize);

	}

	if (PiEngine->IsDebugDrawOn())
	{
		m_pDebugRenderer->SetModelMatrix(m_modelMatrix);
		if (m_pDebugRenderer->IsRenderable())
		{
			PiRenderer->GetRenderableContainer()->PushRenderable(m_pDebugRenderer);
		}
		//m_pDebugRenderer.SetRenderable(true);
	}
}

void RigidBodyComponent::Serialize(const json& j)
{
	m_transform = m_owner->GetComponent<Transform>("Transform");
	vec3 posVec3;
	if (m_transform != nullptr)
	{
		posVec3 = m_transform->GetPosition();
		position = Vector3D(posVec3);
		m_rotation = m_transform->GetRotation();
		m_halfSize = m_transform->GetScaleVec()*1.f;
	}
	//Transform TC = GetOwner().GetComponent("Transform");
	/*
	m_position = Parent()->GetComponent<TransformComponent>()->LocalPosition();
	m_halfSize = Parent()->GetComponent<TransformComponent>()->GetScaleVector()*0.5;
	m_rotation = Parent()->GetComponent<TransformComponent>()->GetRotVector();
	*/
	// is box 
	if (ValueExists(j, "Dynamic"))
	{
		BodyStatic = false;
		m_type = ERigiBodyType::Dynamic;
	}

	if (ValueExists(j, "CollisionLayer"))
		CollisionLayer = ParseInt(j, "CollisionLayer");

	if (ValueExists(j, "activeGravity")) {
		ActiveGravity = ParseBool(j, "activeGravity");
	}

	if (ValueExists(j, "Box"))
	{
		vec3 velocity_ = vec3(0, 0, 0),
			acceleration_ = vec3(0, 0, 0), halfSize_ = m_halfSize;
		float mass_ = 1., LinearDamping_ = 0.95f, AngularDamping_ = 0.8f;
		// Quaternion rotation_ = Quaternion(m_rotation);

		//cur_type = RigidBodyType::BoxRigidBody;
		//position_.x = ParseFloat(j["position"], "x");
		//position_.y = ParseFloat(j["position"], "y");
		//position_.z = ParseFloat(j["position"], "z");


		if (ValueExists(j, "mass")) {
			mass_ = ParseFloat(j, "mass");
		}

		if (ValueExists(j, "velocity")) {
			velocity_.x = ParseFloat(j["velocity"], "x");
			velocity_.y = ParseFloat(j["velocity"], "y");
			velocity_.z = ParseFloat(j["velocity"], "z");
		}

		/*if (ValueExists(j, "rotation")) {
		  rotation_.x = ParseFloat(j["rotation"], "x");
		  rotation_.y = ParseFloat(j["rotation"], "y");
		  rotation_.z = ParseFloat(j["rotation"], "z");
		}*/

		if (ValueExists(j, "acceleration")) {
			acceleration_.x = ParseFloat(j["acceleration"], "x");
			acceleration_.y = ParseFloat(j["acceleration"], "y");
			acceleration_.z = ParseFloat(j["acceleration"], "z");
		}

		//if (ValueExists(j, "scale")) {
		//  halfSize_.x = ValueExists(j["scale"], "x") ? j["scale"]["x"] : halfSize_.x;
		//  halfSize_.y = ValueExists(j["scale"], "y") ? j["scale"]["y"] : halfSize_.y;
		//  halfSize_.z = ValueExists(j["scale"], "z") ? j["scale"]["z"] : halfSize_.z;
		//}

		if (ValueExists(j, "linearDamping")) {
			LinearDamping_ = ParseFloat(j, "linearDamping");
		}

		if (ValueExists(j, "angularDamping")) {
			AngularDamping_ = ParseFloat(j, "angularDamping");
		}

		/*if (ValueExists(j, "activeGravity")) {
			ActiveGravity = ParseBool(j, "activeGravity");
		}*/

    if (ValueExists(j, "colliderSize")) {
      AutoColliderSize = false;
      halfSize_.x = ParseFloat(j["colliderSize"], "x");
      halfSize_.y = ParseFloat(j["colliderSize"], "y");
      halfSize_.z = ParseFloat(j["colliderSize"], "z");

	  m_halfSize = halfSize_;
    }

		m_collider = PiMemory->GetColliderFromMemory(EColliderShape::Box);
		m_collider->SetRBOwner(this);
		BoxCollider* box = static_cast<BoxCollider*>(m_collider);
		box->SetRBOwnerState(mass_, posVec3, velocity_, m_rotation, acceleration_, halfSize_, LinearDamping_, AngularDamping_, ActiveGravity, BodyStatic, CollisionLayer);

		//todo push to pool
		//PiPhysics->AddRigidBodyToPhyManager(this);
		//std::cout << "lasun\n";
	}

	else if (ValueExists(j, "Sphere"))
	{
		vec3 position_ = posVec3, velocity_ = vec3(0, 0, 0),
			acceleration_ = vec3(0, 0, 0);
		float mass_ = 1., LinearDamping_ = 0.95f, AngularDamping_ = 0.8f, radius_ = m_halfSize.x;

		//cur_type = RigidBodyType::SphereRigidBody;

		if (ValueExists(j, "position")) {
			position_.x = ParseFloat(j["position"], "x");
			position_.y = ParseFloat(j["position"], "y");
			position_.z = ParseFloat(j["position"], "z");
		}

		if (ValueExists(j, "mass")) {
			mass_ = ParseFloat(j, "mass");
		}

		if (ValueExists(j, "velocity")) {
			velocity_.x = ParseFloat(j["velocity"], "x");
			velocity_.y = ParseFloat(j["velocity"], "y");
			velocity_.z = ParseFloat(j["velocity"], "z");
		}

		if (ValueExists(j, "acceleration")) {
			acceleration_.x = ParseFloat(j["acceleration"], "x");
			acceleration_.y = ParseFloat(j["acceleration"], "y");
			acceleration_.z = ParseFloat(j["acceleration"], "z");
		}

		/*if (ValueExists(j, "radius")) {
		  radius_ = ParseFloat(j, "radius");
		}*/

		if (ValueExists(j, "linearDamping")) {
			LinearDamping_ = ParseFloat(j, "linearDamping");
		}

		if (ValueExists(j, "angularDamping")) {
			AngularDamping_ = ParseFloat(j, "angularDamping");
		}

		

    if (ValueExists(j, "colliderSize")) {
      AutoColliderSize = false;
      radius_ = ParseFloat(j, "colliderSize");
    }

		m_collider = PiMemory->GetColliderFromMemory(EColliderShape::Sphere);
		m_collider->SetRBOwner(this);
		SphereCollider* sphere = static_cast<SphereCollider*>(m_collider);
		sphere->SetRBOwnerState(mass_, radius_, position_, velocity_, acceleration_, LinearDamping_, AngularDamping_, ActiveGravity, BodyStatic, CollisionLayer);

		//todo push to pool
		//PiPhysics->AddRigidBodyToPhyManager(this);
	}

	else if (ValueExists(j, "Plane"))
	{
		vec3 direction_ = vec3(0, 1, 0);
		float offset_ = 0.;

		//cur_type = RigidBodyType::PlaneRigidBody;

		if (ValueExists(j, "direction")) {
			direction_.x = ParseFloat(j["direction"], "x");
			direction_.y = ParseFloat(j["direction"], "y");
			direction_.z = ParseFloat(j["direction"], "z");
		}

		if (ValueExists(j, "offset")) {
			offset_ = ParseFloat(j, "offset");
		}

		/*direction_ = vec3(0.f, 0.f, -1.f, 0.f);
		auto aa = Parent()->GetComponent<TransformComponent>()->GetRotVector();
		auto RotationMatrix = Parent()->GetComponent<TransformComponent>()->GetRotationMatrix();
		direction_ = RotationMatrix * direction_;*/
		//offset_ = m_position.y;


		m_collider = PiMemory->GetColliderFromMemory(EColliderShape::Plane);
		m_collider->SetRBOwner(this);
		PlaneCollider* plane = static_cast<PlaneCollider*>(m_collider);
		plane->SetRBOwnerState(direction_, offset_);

		//todo push to pool
		//INFECT_PHYSICS.m_PlanePool.push_back(mp_newPlane);
		//PiPhysics->AddRigidBodyToPhyManager(this);

	}
	if (m_owner->GetGameObjectName() != "Player")
	{
		PiPhysics->AddRigidBodyToPhyManager(this);
	}
}
void RigidBodyComponent::HandleEvent(Event* a_pEvent)
{

}

void RigidBodyComponent::Deactivate(void)
{
	CollisionLayer = -1;
	m_type = ERigiBodyType::Static;
	BodyStatic = true;
	ActiveGravity = false;

	setMass(0.0001f);
	setVelocity(0.0f, 0.0f, 0.0f);
	setAcceleration(0.0f, 0.0f, 0.0f);
	setRotation(0.0f, 0.0f, 0.0f);

	// Set the data common to all particle types
	setPosition(0.0f, 0.0f, 0.0f);

	m_transform = nullptr;
	
	PiPhysics->RemoveRigidBodyComponent(this);
	PiMemory->SetColliderBackInMemory(m_collider);

	m_collider = nullptr;

	PiMemory->SetRendrableBackInMemory(m_pDebugRenderer, "DebugRenderer");

	m_pDebugRenderer = nullptr;
}

void RigidBodyComponent::SetFractureHit()
{

	m_owner->GetComponent<FractureComponent>("FractureComponent")->m_Hit = true;
	//mp_newbox->m_hit = false;
	if (m_collider->GetColliderShape() == EColliderShape::Box)
	{
		BoxCollider* m_box = static_cast<BoxCollider*>(m_collider);
		m_box->SetHit(false);
	}
}


void RigidBodyComponent::OverrideParams(const json& j,float offset)
{
	//m_position = Parent()->GetComponent<TransformComponent>()->WorldPosition();
	//m_halfSize = Parent()->GetComponent<TransformComponent>()->GetScaleVector()*0.5;
	//m_rotation = Parent()->GetComponent<TransformComponent>()->GetRotVector();
	// is box 
	//Transform *transform = m_owner->GetComponent<Transform>("Transform");
	vec3 posVec3;
	if (m_transform != nullptr)
	{
		posVec3 = m_transform->GetPosition();
		position = Vector3D(posVec3);
		m_rotation = m_transform->GetRotation();
		if(AutoColliderSize)
		m_halfSize = m_transform->GetScaleVec()*1.f;
	}

	if (ValueExists(j, "CollisionLayer"))
		CollisionLayer = ParseInt(j, "CollisionLayer");


	if (m_collider->GetColliderShape() == EColliderShape::Box)
	{
		BoxCollider* box = static_cast<BoxCollider*>(m_collider);
		Vector3D position_ = getPosition(), velocity_ = getVelocity(),
			acceleration_ = getAcceleration(),
			halfSize_ = m_halfSize;
		float mass_ = getMass(), LinearDamping_ = getLinearDamping(),
			AngularDamping_ = getAngularDamping();

		if (ValueExists(j, "mass")) {
			mass_ = ParseFloat(j, "mass");
		}

		if (ValueExists(j, "velocity")) {
			velocity_.x = ParseFloat(j["velocity"], "x");
			velocity_.y = ParseFloat(j["velocity"], "y");
			velocity_.z = ParseFloat(j["velocity"], "z");
		}

		/*if (ValueExists(j, "rotation")) {
		  rotation_.x = ParseFloat(j["rotation"], "x");
		  rotation_.y = ParseFloat(j["rotation"], "y");
		  rotation_.z = ParseFloat(j["rotation"], "z");
		}*/

		if (ValueExists(j, "acceleration")) {
			acceleration_.x = ParseFloat(j["acceleration"], "x");
			acceleration_.y = ParseFloat(j["acceleration"], "y");
			acceleration_.z = ParseFloat(j["acceleration"], "z");
		}

		//if (ValueExists(j, "scale")) {
		//  halfSize_.x = ValueExists(j["scale"], "x") ? j["scale"]["x"] : halfSize_.x;
		//  halfSize_.y = ValueExists(j["scale"], "y") ? j["scale"]["y"] : halfSize_.y;
		//  halfSize_.z = ValueExists(j["scale"], "z") ? j["scale"]["z"] : halfSize_.z;
		//}

		if (ValueExists(j, "linearDamping")) {
			LinearDamping_ = ParseFloat(j, "linearDamping");
		}

		if (ValueExists(j, "angularDamping")) {
			AngularDamping_ = ParseFloat(j, "angularDamping");
		}

		if (ValueExists(j, "activeGravity")) {
			ActiveGravity = ParseBool(j, "activeGravity");
		}

    if (ValueExists(j, "colliderSize")) {
      AutoColliderSize = false;
      halfSize_.x = ParseFloat(j["colliderSize"], "x");
      halfSize_.y = ParseFloat(j["colliderSize"], "y");
      halfSize_.z = ParseFloat(j["colliderSize"], "z");

	  m_halfSize = halfSize_.toVec3();
    }
    else
      AutoColliderSize = true;


		box->SetRBOwnerState(mass_, position_.toVec3(), velocity_.toVec3(),	m_transform->GetRotation(),
			acceleration_.toVec3(), halfSize_.toVec3(), LinearDamping_, AngularDamping_, ActiveGravity, BodyStatic, CollisionLayer);
		//INFECT_PHYSICS.m_BoxPool.push_back(newbox);
	}

	else if (m_collider->GetColliderShape() == EColliderShape::Sphere)
	{
		SphereCollider* sphere = static_cast<SphereCollider*>(m_collider);

		Vector3D position_ = getPosition(), velocity_ = getVelocity(),
			acceleration_ = getAcceleration();
		float mass_ = getMass(), LinearDamping_ = getLinearDamping(),
			AngularDamping_ = getAngularDamping(),
			radius_ = m_halfSize.x;

		//cur_type = RigidBodyType::SphereRigidBody;


		/*if (ValueExists(j, "position")) {
			position_.x = ParseFloat(j["position"], "x");
			position_.y = ParseFloat(j["position"], "y");
			position_.z = ParseFloat(j["position"], "z");
		}*/

		if (ValueExists(j, "mass")) {
			mass_ = ParseFloat(j, "mass");
		}

		if (ValueExists(j, "velocity")) {
			velocity_.x = ParseFloat(j["velocity"], "x");
			velocity_.y = ParseFloat(j["velocity"], "y");
			velocity_.z = ParseFloat(j["velocity"], "z");
		}

		if (ValueExists(j, "acceleration")) {
			acceleration_.x = ParseFloat(j["acceleration"], "x");
			acceleration_.y = ParseFloat(j["acceleration"], "y");
			acceleration_.z = ParseFloat(j["acceleration"], "z");
		}

		/*if (ValueExists(j, "radius")) {
		  radius_ = ParseFloat(j, "radius");
		}*/

		if (ValueExists(j, "linearDamping")) {
			LinearDamping_ = ParseFloat(j, "linearDamping");
		}

		if (ValueExists(j, "angularDamping")) {
			AngularDamping_ = ParseFloat(j, "angularDamping");
		}

		if (ValueExists(j, "linearDamping")) {
			LinearDamping_ = ParseFloat(j, "linearDamping");
		}

		if (ValueExists(j, "angularDamping")) {
			AngularDamping_ = ParseFloat(j, "angularDamping");
		}

		if (ValueExists(j, "activeGravity")) {
			ActiveGravity = ParseBool(j, "activeGravity");
		}

    if (ValueExists(j, "colliderSize")) {
      AutoColliderSize = false;
      radius_ = ParseFloat(j, "colliderSize");
    }
    else
      AutoColliderSize = true;

		sphere->SetRBOwnerState(mass_, radius_, position_.toVec3(), velocity_.toVec3(), acceleration_.toVec3(),
			LinearDamping_, AngularDamping_, ActiveGravity, BodyStatic, CollisionLayer);
	}

	else if (m_collider->GetColliderShape() == EColliderShape::Plane)
	{
		PlaneCollider* plane = static_cast<PlaneCollider*>(m_collider);

		Vector3D direction_ = plane->direction;
		float offset_ = plane->offset;

		//cur_type = RigidBodyType::PlaneRigidBody;

		if (ValueExists(j, "direction")) {
			direction_.x = ParseFloat(j["direction"], "x");
			direction_.y = ParseFloat(j["direction"], "y");
			direction_.z = ParseFloat(j["direction"], "z");
		}

		if (ValueExists(j, "offset")) {
			offset_ = ParseFloat(j, "offset");
		}

		plane->SetRBOwnerState(direction_.toVec3(), offset_);
	}
}

void RigidBodyComponent::AddToRotation(float a_x, float a_y, float a_z)
{
	rotation.x += a_x;
	rotation.y += a_y;
	rotation.z += a_z;
}


/**
 * Internal function to do an intertia tensor transform by a quaternion.
 * Note that the implementation of this function was created by an
 * automated code-generator and optimizer.
 */
static inline void GobalTensortrans(Matrix3x3 &input,
                                           const Quaternion &orientation,
                                           const Matrix3x3 &body,
                                           const Matrix4x4 &rotation)
{
    real t4 = 
        rotation.m_matrix[0][0]*body.m_matrix[0][0]+
        rotation.m_matrix[0][1]*body.m_matrix[1][0]+
        rotation.m_matrix[0][2]*body.m_matrix[2][0];
    real t9 = 
        rotation.m_matrix[0][0]*body.m_matrix[0][1]+
        rotation.m_matrix[0][1]*body.m_matrix[1][1]+
        rotation.m_matrix[0][2]*body.m_matrix[2][1];
    real t14 = 
        rotation.m_matrix[0][0]*body.m_matrix[0][2]+
        rotation.m_matrix[0][1]*body.m_matrix[1][2]+
        rotation.m_matrix[0][2]*body.m_matrix[2][2];
    real t28 = 
        rotation.m_matrix[1][0]*body.m_matrix[0][0]+
        rotation.m_matrix[1][1]*body.m_matrix[1][0]+
        rotation.m_matrix[1][2]*body.m_matrix[2][0];
    real t33 = 
        rotation.m_matrix[1][0]*body.m_matrix[0][1]+
        rotation.m_matrix[1][1]*body.m_matrix[1][1]+
        rotation.m_matrix[1][2]*body.m_matrix[2][1];
    real t38 = 
        rotation.m_matrix[1][0]*body.m_matrix[0][2]+
        rotation.m_matrix[1][1]*body.m_matrix[1][2]+
        rotation.m_matrix[1][2]*body.m_matrix[2][2];
    real t52 =
        rotation.m_matrix[2][0]*body.m_matrix[0][0]+
        rotation.m_matrix[2][1]*body.m_matrix[1][0]+
        rotation.m_matrix[2][2]*body.m_matrix[2][0];
    real t57 =
        rotation.m_matrix[2][0]*body.m_matrix[0][1]+
        rotation.m_matrix[2][1]*body.m_matrix[1][1]+
        rotation.m_matrix[2][2]*body.m_matrix[2][1];
    real t62 = 
        rotation.m_matrix[2][0]*body.m_matrix[0][2]+
        rotation.m_matrix[2][1]*body.m_matrix[1][2]+
        rotation.m_matrix[2][2]*body.m_matrix[2][2];

    input.m_matrix[0][0] = t4*rotation.m_matrix[0][0]+
        t9*rotation.m_matrix[0][1]+
        t14*rotation.m_matrix[0][2];
    input.m_matrix[0][1] = t4*rotation.m_matrix[1][0]+
        t9*rotation.m_matrix[1][1]+
        t14*rotation.m_matrix[1][2];
    input.m_matrix[0][2] = t4*rotation.m_matrix[2][0]+
        t9*rotation.m_matrix[2][1]+
        t14*rotation.m_matrix[2][2];
    input.m_matrix[1][0] = t28*rotation.m_matrix[0][0]+
        t33*rotation.m_matrix[0][1]+
        t38*rotation.m_matrix[0][2];
    input.m_matrix[1][1] = t28*rotation.m_matrix[1][0]+
        t33*rotation.m_matrix[1][1]+
        t38*rotation.m_matrix[1][2];
    input.m_matrix[1][2] = t28*rotation.m_matrix[2][0]+
        t33*rotation.m_matrix[2][1]+
        t38*rotation.m_matrix[2][2];
    input.m_matrix[2][0] = t52*rotation.m_matrix[0][0]+
        t57*rotation.m_matrix[0][1]+
        t62*rotation.m_matrix[0][2];
    input.m_matrix[2][1] = t52*rotation.m_matrix[1][0]+
        t57*rotation.m_matrix[1][1]+
        t62*rotation.m_matrix[1][2];
    input.m_matrix[2][2] = t52*rotation.m_matrix[2][0]+
        t57*rotation.m_matrix[2][1]+
        t62*rotation.m_matrix[2][2];
}

/**
 * Inline function that creates a transform matrix from a
 * position and orientation.
 */
static inline void GobalGetTransMatrix(Matrix4x4 &transform, const Vector3D &pos, const Quaternion &ori)
{
    transform.m_matrix[0][0] = 1-2*ori.j*ori.j-  2*ori.k*ori.k;
    transform.m_matrix[0][1] = 2*ori.i*ori.j -  2*ori.r*ori.k;
    transform.m_matrix[0][2] = 2*ori.i*ori.k + 2*ori.r*ori.j;
    transform.m_matrix[0][3] = pos.x;

    transform.m_matrix[1][0] = 2*ori.i*ori.j +  2*ori.r*ori.k;
    transform.m_matrix[1][1] = 1-2*ori.i*ori.i-  2*ori.k*ori.k;
    transform.m_matrix[1][2] = 2*ori.j*ori.k -  2*ori.r*ori.i;
    transform.m_matrix[1][3] = pos.y;

    transform.m_matrix[2][0] = 2*ori.i*ori.k -2*ori.r*ori.j;
    transform.m_matrix[2][1] = 2*ori.j*ori.k +2*ori.r*ori.i;
    transform.m_matrix[2][2] = 1-2*ori.i*ori.i- 2*ori.j*ori.j;
    transform.m_matrix[2][3] = pos.z;
}


void RigidBodyComponent::CalculateOriginate()
{
    orientation.normalise();

    // Calculate the transform matrix for the body.
    GobalGetTransMatrix(transformMatrix, position, orientation);

    // Calculate the inertiaTensor in world space.
    GobalTensortrans(WorldTensor_inverse,
        orientation,
        inverseInsideTensor,
        transformMatrix);

}

void RigidBodyComponent::calculateIntegrate(real duration)
{
    if (!isAwake) 
      return;

    // Calculate linear acceleration 
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration.addScaledVector(forceAccum, m_inverseMass);

    // Calculate angular acceleration 
    Vector3D AccAngle = WorldTensor_inverse.transform(torqueAccum);

    // Update linear velocity
    m_velocity.addScaledVector(lastFrameAcceleration, duration);

    // Update angular velocity 
    rotation.addScaledVector(AccAngle, duration);

    // Impose drag.
    m_velocity *= pow(linearDamping, duration);
    rotation *= pow(angularDamping, duration);

    // Update linear position.
    position.addScaledVector(m_velocity, duration);

    // Update angular position.
    orientation.addScaledVector(rotation, duration);

    // position and orientation
    CalculateOriginate();

    clearAcc();

    if (canSleep) {

        real detla = real_pow(0.5f, duration);
        motion = detla*motion + (1-detla)*(m_velocity.scalarProduct(m_velocity) + rotation.scalarProduct(rotation));

        if (motion < EpsilonValueForSleep) 
          setSleep(false);
        else if (motion > 10 * EpsilonValueForSleep)
          motion = 10 * EpsilonValueForSleep;
    }
}
real RigidBodyComponent::getMass() const
{
  if (m_inverseMass == 0) {
    return REAL_MAX;
  }
  else {
    return ((real)1.0) / m_inverseMass;
  }
}

void RigidBodyComponent::setInverseMass(const real mass)
{
  RigidBodyComponent::m_inverseMass = mass;
}

void RigidBodyComponent::setMass(const real mass)
{
    assert(mass != 0);
    RigidBodyComponent::m_inverseMass = ((real)1.0)/mass;
}


real RigidBodyComponent::getInversedMass() const
{
    return m_inverseMass;
}

bool RigidBodyComponent::hasFiniteMass() const
{
    return m_inverseMass >= 0.0f;
}

void RigidBodyComponent::setInertiaTensor(const Matrix3x3 &input)
{
    inverseInsideTensor.setInverse(input);

}

void RigidBodyComponent::getInertiaTensor(Matrix3x3 *input) const
{
    input->setInverse(inverseInsideTensor);
}

Matrix3x3 RigidBodyComponent::getInertiaTensor() const
{
  Matrix3x3 temp = Matrix3x3(0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f);;
    getInertiaTensor(&temp);
    return temp;
}


Matrix3x3 RigidBodyComponent::getInertiaTensorWorld() const
{
  Matrix3x3 temp = Matrix3x3(0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f);;
    getInertiaTensorWorld(&temp);
    return temp;
}
Matrix3x3 RigidBodyComponent::getInverseTensor() const
{
  return inverseInsideTensor;
}

void RigidBodyComponent::getInversedTensor(Matrix3x3 *Tensor) const
{
  *Tensor = WorldTensor_inverse;
}

void RigidBodyComponent::getInertiaTensorWorld(Matrix3x3 *input) const
{
  input->setInverse(WorldTensor_inverse);
}

void RigidBodyComponent::setInverseTensor(const Matrix3x3 &Tensor)
{

    RigidBodyComponent::inverseInsideTensor = Tensor;
}




Matrix3x3 RigidBodyComponent::getInversedTensor() const
{
    return WorldTensor_inverse;
}


void RigidBodyComponent::setLinearDamping(const float linearDamping)
{
    RigidBodyComponent::linearDamping = linearDamping;
}

real RigidBodyComponent::getLinearDamping() const
{
    return linearDamping;
}

void RigidBodyComponent::getInverseTensor(Matrix3x3 *Tensor) const
{
  *Tensor = RigidBodyComponent::inverseInsideTensor;
}


void RigidBodyComponent::setAngularDamping(const real angularDamping)
{
    RigidBodyComponent::angularDamping = angularDamping;
}

real RigidBodyComponent::getAngularDamping() const
{
    return angularDamping;
}
void RigidBodyComponent::setDamping
(const real linearDamping_,
  const real angularDamping_)
{
  RigidBodyComponent::linearDamping = linearDamping_;
  RigidBodyComponent::angularDamping = angularDamping_;
}

void RigidBodyComponent::setPosition(const Vector3D &position)
{
    RigidBodyComponent::position = position;
}



void RigidBodyComponent::getPosition(Vector3D *position) const
{
    *position = RigidBodyComponent::position;
}

void RigidBodyComponent::getOrientation(Matrix3x3 *matrix) const
{
  //matrix->test = 0;
  getOrientation(matrix->m_matrix);
}

Vector3D RigidBodyComponent::getPosition() const
{
    return position;
}

void RigidBodyComponent::setOrientation(const Quaternion &orientation)
{
    RigidBodyComponent::orientation = orientation;
    RigidBodyComponent::orientation.normalise();
}

void RigidBodyComponent::setOrientation(const real r, const real i,
                   const real j, const real k)
{
    orientation.r = r;
    orientation.i = i;
    orientation.j = j;
    orientation.k = k;
    orientation.normalise();
}

void RigidBodyComponent::getOrientation(Quaternion *orientation) const
{
    *orientation = RigidBodyComponent::orientation;
}

Quaternion RigidBodyComponent::getOrientation() const
{
    return orientation;
}



void RigidBodyComponent::getOrientation(real matrix[3][3]) const
{
    matrix[0][0] = transformMatrix.m_matrix[0][0];
    matrix[0][1] = transformMatrix.m_matrix[0][1];
    matrix[0][2] = transformMatrix.m_matrix[0][2];

    matrix[1][0] = transformMatrix.m_matrix[1][0];
    matrix[1][1] = transformMatrix.m_matrix[1][1];
    matrix[1][2] = transformMatrix.m_matrix[1][2];

    matrix[2][0] = transformMatrix.m_matrix[2][0];
    matrix[2][1] = transformMatrix.m_matrix[2][1];
    matrix[2][2] = transformMatrix.m_matrix[2][2];
}

void RigidBodyComponent::setPosition(const real x, const real y, const real z)
{
  position.x = x;
  position.y = y;
  position.z = z;
}

void RigidBodyComponent::getTransform(Matrix4x4 *transform) const
{
   // transform->test = 0;
    memcpy(transform, &transformMatrix.m_matrix, sizeof(Matrix4x4));
}



void RigidBodyComponent::getGLTransform(float matrix[16]) const
{
    matrix[0] = (float)transformMatrix.m_matrix[0][0];
    matrix[1] = (float)transformMatrix.m_matrix[1][0];
    matrix[2] = (float)transformMatrix.m_matrix[2][0];
    matrix[3] = 0;

    matrix[4] = (float)transformMatrix.m_matrix[0][1];
    matrix[5] = (float)transformMatrix.m_matrix[1][1];
    matrix[6] = (float)transformMatrix.m_matrix[2][1];
    matrix[7] = 0;

    matrix[8] = (float)transformMatrix.m_matrix[0][2];
    matrix[9] = (float)transformMatrix.m_matrix[1][2];
    matrix[10] = (float)transformMatrix.m_matrix[2][2];
    matrix[11] = 0;

    matrix[12] = (float)transformMatrix.m_matrix[0][3];
    matrix[13] = (float)transformMatrix.m_matrix[1][3];
    matrix[14] = (float)transformMatrix.m_matrix[2][3];
    matrix[15] = 1;
}

Matrix4x4 RigidBodyComponent::getTransform() const
{
    return transformMatrix;
}
void RigidBodyComponent::getTransform(real matrix[16]) const
{
  memcpy(matrix, transformMatrix.m_matrix, sizeof(real) * 12);
  matrix[12] = matrix[13] = matrix[14] = 0;
  matrix[15] = 1;
}

Vector3D RigidBodyComponent::getPositionInWorld(const Vector3D &point) const
{
  return transformMatrix.transform(point);
}


Vector3D RigidBodyComponent::gePositionInLocal(const Vector3D &point) const
{
    return transformMatrix.transformInverse(point);
}


Vector3D RigidBodyComponent::GetDirecInLocal(const Vector3D &direction) const
{
    return transformMatrix.transformInverseDirection(direction);
}

Vector3D RigidBodyComponent::getDirectionInWorldSpace(const Vector3D &direction) const
{
    return transformMatrix.transformDirection(direction);
}


void RigidBodyComponent::setVelocity(const Vector3D &velocity)
{
    RigidBodyComponent::m_velocity = velocity;
}

void RigidBodyComponent::setVelocity(const real x, const real y, const real z)
{
    m_velocity.x = x;
    m_velocity.y = y;
    m_velocity.z = z;
}

void RigidBodyComponent::getVelocity(Vector3D *velocity) const
{
    *velocity = RigidBodyComponent::m_velocity;
}

Vector3D RigidBodyComponent::getVelocity() const
{
    return m_velocity;
}

void RigidBodyComponent::addVelocity(const Vector3D &deltaVelocity)
{
    m_velocity += deltaVelocity;
}


void RigidBodyComponent::addVelocity( vec3 &deltaVelocity)
{
	
	m_velocity += Vector3D(deltaVelocity);
}


void RigidBodyComponent::setRotation(const Vector3D &rotation)
{
    RigidBodyComponent::rotation = rotation;
}

void RigidBodyComponent::setRotation(const real x, const real y, const real z)
{
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
}

void RigidBodyComponent::getRotation(Vector3D *rotation) const
{
    *rotation = RigidBodyComponent::rotation;
}

Vector3D RigidBodyComponent::getRotation() const
{
    return rotation;
}

void RigidBodyComponent::addRotation(const Vector3D &deltaRotation)
{
    rotation += deltaRotation;
}



void RigidBodyComponent::setSleep(const bool sleep)
{
    if (sleep) {
        isAwake= true;

        // Add a bit of motion to avoid it falling asleep immediately.
        motion = EpsilonValueForSleep*2.0f;
    } else {
        isAwake = false;
        m_velocity.Zero();
        rotation.Zero();
    }
}

inline bool RigidBodyComponent::getCanSleep() const
{
  return canSleep;
}

void RigidBodyComponent::setCanSleep(const bool canSleep)
{
    RigidBodyComponent::canSleep = canSleep;

    if (!canSleep && !isAwake) setSleep();
}


void RigidBodyComponent::AccelerationFromPreviousFrame(Vector3D *acceleration) const
{
    *acceleration = lastFrameAcceleration;
}

Vector3D RigidBodyComponent::AccelerationFromPreviousFrame() const
{
    return lastFrameAcceleration;
}

void RigidBodyComponent::clearAcc()
{
    forceAccum.Zero();
    torqueAccum.Zero();
}

void RigidBodyComponent::addForce(const Vector3D &force)
{
    forceAccum += force;
    isAwake = true;
}

void RigidBodyComponent::addForceOnthePoint(const Vector3D &force,
                                    const Vector3D &point)
{
    Vector3D temp = getPositionInWorld(point);
    addForceAtPoint(force, temp);

}

void RigidBodyComponent::addForceAtPoint(const Vector3D &force,
                                const Vector3D &point)
{
    Vector3D temp = point;
    temp -= position;

    forceAccum += force;
    torqueAccum += temp % force;

    isAwake = true;
}

void RigidBodyComponent::addTorque(const Vector3D &torque)
{
    torqueAccum += torque;
    isAwake = true;
}

void RigidBodyComponent::setAcceleration(const Vector3D &acceleration)
{
    RigidBodyComponent::acceleration = acceleration;
}

void RigidBodyComponent::setAcceleration(const real x, const real y, const real z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

void RigidBodyComponent::getAcceleration(Vector3D *acceleration) const
{
    *acceleration = RigidBodyComponent::acceleration;
}

Vector3D RigidBodyComponent::getAcceleration() const
{
    return acceleration;
}


void PrintMatrix4x4(const Matrix4x4 & mat)
{
  cout << std::setprecision(2);
  cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout << " " << mat.m_matrix[0][0] << " " << mat.m_matrix[0][1] << " " << mat.m_matrix[0][2] << " " << mat.m_matrix[0][3] << std::endl
    << " " << mat.m_matrix[1][0] << " " << mat.m_matrix[1][1] << " " << mat.m_matrix[1][2] << " " << mat.m_matrix[1][3] << std::endl
    << " " << mat.m_matrix[2][0] << " " << mat.m_matrix[2][1] << " " << mat.m_matrix[2][2] << " " << mat.m_matrix[2][3] << std::endl
    << " " << mat.m_matrix[3][0] << " " << mat.m_matrix[3][1] << " " << mat.m_matrix[3][2] << " " << mat.m_matrix[3][3] << std::endl;
  std::cout <<"----------------------------------------" << std::endl;
}
Vector3D RotationMatrixToEulerAngles(Matrix4x4 R)
{
  float twmp = sqrt(R.m_matrix[0][0] * R.m_matrix[0][0] + R.m_matrix[1][0] * R.m_matrix[1][0]);

  bool singular = twmp < 1e-6; // If

  float x, y, z;
  if (!singular)
  {
    x = atan2(R.m_matrix[2][1], R.m_matrix[2][2]);
    y = atan2(-R.m_matrix[2][0], twmp);
    z = atan2(R.m_matrix[1][0], R.m_matrix[0][0]);
  }
  else
  {
    x = atan2(-R.m_matrix[1][2], R.m_matrix[1][1]);
    y = atan2(-R.m_matrix[2][0], twmp);
    z = 0;
  }
  return Vector3D(x, y, z);
}


/*

refernce: wiki
*/
glm::quat RotationMatrixToQuat(const Matrix4x4 &R)
{
  float m11 = R.m_matrix[0][0], m12 = R.m_matrix[0][1], m13 = R.m_matrix[0][2];
  float m21 = R.m_matrix[1][0], m22 = R.m_matrix[1][1], m23 = R.m_matrix[1][2];
  float m31 = R.m_matrix[2][0], m32 = R.m_matrix[2][1], m33 = R.m_matrix[2][2];

  // Output quaternion
  float w, x, y, z;
  // Determine which of w,x,y, or z has the largest absolute value
  float fourWSquaredMinus1 = m11 + m22 + m33;
  float fourXSquaredMinus1 = m11 - m22 - m33;
  float fourYSquaredMinus1 = m22 - m11 - m33;
  float fourZSquaredMinus1 = m33 - m11 - m22;

  int index = 0;
  float SquaredMinus1 = fourWSquaredMinus1;

  if (fourXSquaredMinus1 > SquaredMinus1) {
    SquaredMinus1 = fourXSquaredMinus1;
    index = 1;
  }
  if (fourYSquaredMinus1 > SquaredMinus1) {
    SquaredMinus1 = fourYSquaredMinus1;
    index = 2;
  }
  if (fourZSquaredMinus1 > SquaredMinus1) {
    SquaredMinus1 = fourZSquaredMinus1;
    index = 3;
  }
  // Per form square root and division
  float biggestVal = sqrt(SquaredMinus1 + 1.0f) * 0.5f;
  float mult = 0.25f / biggestVal;

  // Apply table to compute quaternion values
  switch (index) {
  case 0:
    w = biggestVal;
    x = (m23 - m32) * mult;
    y = (m31 - m13) * mult;
    z = (m12 - m21) * mult;
    break;
  case 1:
    x = biggestVal;
    w = (m23 - m32) * mult;
    y = (m12 + m21) * mult;
    z = (m31 + m13) * mult;
    break;
  case 2:
    y = biggestVal;
    w = (m31 - m13) * mult;
    x = (m12 + m21) * mult;
    z = (m23 + m32) * mult;
    break;
  case 3:
    z = biggestVal;
    w = (m12 - m21) * mult;
    x = (m31 + m13) * mult;
    y = (m23 + m32) * mult;
    break;

    float quatX = x;
    float quatY = y;
    float quatZ = z;
    float quatW = w;

    //model = vec4(quatX, quatY, quatZ, quatW);
  }
  return glm::quat(w, x, y, z);
}