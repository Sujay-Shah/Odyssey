//
//  FractureComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "PhysicsManager.h"
#include "GameObjectManager.h"
#include "ObjectFactory.h"
#include "MemoryManager.h"

FractureComponent::FractureComponent() : Component(STRINGIFY(FractureComponent))
{

}

void FractureComponent::Update(float a_dTime)
{
  if (m_Hit)
  {
	  if (m_CanBeBroken_local)
	  {
		  for (size_t i = 0; i < 8; ++i)
		  {
			  blocks[i] = static_cast<RigidBodyComponent*>(PiMemory->GetComponentFromMemory("RigidBodyComponent"));
			  blocks[i]->m_collider = PiMemory->GetColliderFromMemory(EColliderShape::Box);
			  blocks[i]->m_collider->SetRBOwner(blocks[i]);
		  }
		  _DivideBlock(PiPhysics->GetFractureContact());

	  }
	 m_Hit = false;
  }
}

void FractureComponent::Initialize()
{

}

void FractureComponent::Serialize(const json& j)
{
	mp_Target = GetOwner()->GetComponent<RigidBodyComponent>("RigidBodyComponent");
	if (mp_Target != nullptr)
	{
		if (mp_Target->m_collider->GetColliderShape() == EColliderShape::Box)
		{
			BoxCollider *box = static_cast<BoxCollider*>(mp_Target->m_collider);
		
			if (ValueExists(j, "CanBeBroken"))
			{
				box->SetCanBeBroken(true);
				m_CanBeBroken_local = true;
			}
		
			else if (ValueExists(j, "Breaker"))
			{
				box->SetBreaker(true);
				m_Breaker_local = true;
			}
		}
	
		if (mp_Target->m_collider->GetColliderShape() == EColliderShape::Sphere)
		{
			if (ValueExists(j, "Breaker"))
			{
				SphereCollider *sphere = static_cast<SphereCollider*>(GetOwner()->GetComponent<RigidBodyComponent>("RigidBodyComponent")->m_collider);
				sphere->SetBreaker(true);
			}
		}
	}
}
void FractureComponent::HandleEvent(Event* a_pEvent)
{
	if (a_pEvent->GetEventType() == EEventType::Collision)
	{
		CollisionEvent* cevent = static_cast<CollisionEvent*>(a_pEvent);
		FractureComponent *temp = cevent->m_gameObject->GetComponent<FractureComponent>("FractureComponent");
		if (temp && temp->m_Breaker_local)
		{
			m_Hit = true;
		}
	}
}

void FractureComponent::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
}

void FractureComponent::_DivideBlock(const Contact& contact)
{
  RigidBodyComponent CommonSetting;
  Vector3D contactInfo = contact.ConNormalVector;
  Vector3D localhalfsize = mp_Target->m_halfSize;
  RigidBodyComponent *smallerCube = contact.body[0];
  if (smallerCube != mp_Target)
  {
    contactInfo.Negate();
    smallerCube = contact.body[1];
  }


  Vector3D CollisionPoint = smallerCube->gePositionInLocal(contact.theCollisionPoint);
  contactInfo = smallerCube->GetDirecInLocal(contactInfo);


  CollisionPoint = CollisionPoint - contactInfo * (CollisionPoint * contactInfo);

  SettingSCubes(CommonSetting, smallerCube);

  real invDensity = static_cast<BoxCollider*>(mp_Target->m_collider)->halfSize.Length() * cube_count * smallerCube->getInversedMass();

  // Remove the old block
  //PiPhysics->m_BoxPool.remove(mp_Target);
  //delete mp_Target;
  //TODO : Delete deActivate gos on fly
  /*PiGameObject->
  GetOwner()->DeActivate();*/
  PiGameObject->DeActivateGameObject(m_owner);

  /*PHYMANAGER.m_BoxPool.remove(mp_Target);
  mp_Target->body->setPosition(Vector3D(100, 100, 110));*/
  // Now split the block into eight.
  for (uint32 i = 0; i < cube_count; i++)
  {
    // Find the minimum and maximum extents of the new block
    // in old-block coordinates
    Vector3D EdgeMin, EdgeMax;
    if ((i & 1) == 0) {
      EdgeMin.x = -localhalfsize.x;
      EdgeMax.x = CollisionPoint.x;
    }
    else {
      EdgeMin.x = CollisionPoint.x;
      EdgeMax.x = localhalfsize.x;
    }

    if ((i & 2) == 0) {
      EdgeMin.y = -localhalfsize.y;
      EdgeMax.y = CollisionPoint.y;
    }
    else {
      EdgeMin.y = CollisionPoint.y;
      EdgeMax.y = localhalfsize.y;
    }

    if ((i & 4) == 0) {
      EdgeMin.z = -localhalfsize.z;
      EdgeMax.z = CollisionPoint.z;
    }
    else {
      EdgeMin.z = CollisionPoint.z;
      EdgeMax.z = localhalfsize.z;
    }

    // Get the origin and half size of the block, in old-body
    // local coordinates.
    Vector3D halfSize = (EdgeMax - EdgeMin) * 0.5f;
    Vector3D newPos = halfSize + EdgeMin;

    // Convert the origin to world coordinates.
    newPos = CommonSetting.getPositionInWorld(newPos);

    // Work out the direction to the impact.
    Vector3D direction = newPos - contact.theCollisionPoint;
    direction.normalise();

    // Set the body's properties (we assume the block has a body
    // already that we're going to overwrite).
	blocks[i]->setPosition(newPos);
    blocks[i]->setVelocity(CommonSetting.getVelocity() + direction * 100.0f);
    blocks[i]->setOrientation(CommonSetting.getOrientation());
    blocks[i]->setRotation(CommonSetting.getRotation());
    blocks[i]->setLinearDamping(CommonSetting.getLinearDamping());
    blocks[i]->setAngularDamping(CommonSetting.getAngularDamping());
    blocks[i]->setSleep(true);
    blocks[i]->setAcceleration(Vector3D::GRAVITY);
    blocks[i]->clearAcc();
    blocks[i]->CalculateOriginate();
    blocks[i]->m_collider->offset = Matrix4x4(1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 0.f);;
	static_cast<BoxCollider*>(blocks[i]->m_collider)->halfSize = halfSize;

    // Finally calculate the mass and inertia tensor of the new block
	static_cast<BoxCollider*>(blocks[i]->m_collider)->CalculateMassProperties(invDensity);

	//TODO: determine which list should they be put in
	GameObject* pGo = PiGameObject->GetObjectFactoryInstance()->CreateGameObject("physicsTestCube2", false);
	pGo->GetComponent<Transform>("Transform")->SetPosition(newPos.toVec3());
	pGo->GetComponent<Transform>("Transform")->SetScale((halfSize).toVec3());  // now Scale size = half size
	pGo->AddComponent(blocks[i]);
    
	//add to box pool, which pool?
	PiPhysics->AddRigidBodyToPhyManager(blocks[i]);
  }

  //GameObjectManager* pGom = PiGameObject;


}

void FractureComponent::SettingSCubes(RigidBodyComponent &CommonSetting, RigidBodyComponent * smallerCube)
{
  CommonSetting.setPosition(smallerCube->getPosition());
  CommonSetting.setVelocity(smallerCube->getVelocity());
  CommonSetting.setRotation(smallerCube->getRotation());
  CommonSetting.setOrientation(smallerCube->getOrientation());
  CommonSetting.setAngularDamping(smallerCube->getAngularDamping());
  CommonSetting.setInverseTensor(smallerCube->getInverseTensor());
  CommonSetting.setLinearDamping(smallerCube->getLinearDamping());
  CommonSetting.CalculateOriginate();
}



void FractureComponent::OverrideParams(const json &j,float offset)
{
  if (ValueExists(j, "Box"))
  {
	  mp_Target = GetOwner()->GetComponent<RigidBodyComponent>("RigidBodyComponent");
	  if (mp_Target->m_collider->GetColliderShape() == EColliderShape::Box)
	  {
		  BoxCollider *box = static_cast<BoxCollider*>(mp_Target->m_collider);

		  if (ValueExists(j, "CanBeBroken"))
		  {
			  box->SetCanBeBroken(true);
		  }

		  else if (ValueExists(j, "Breaker"))
			  box->SetBreaker(true);
	  }
  }

  if (ValueExists(j, "Sphere"))
  {
	  if (ValueExists(j, "Breaker"))
	  {
		  SphereCollider *sphere = static_cast<SphereCollider*>(GetOwner()->GetComponent<RigidBodyComponent>("RigidBodyComponent")->m_collider);
		  sphere->SetBreaker(true);
	  }
  }

}

