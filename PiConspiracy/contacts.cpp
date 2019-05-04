//
//  contacts.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#include "PiEnginePreCompiled.h"
#include "contacts.h"
#include "GameObjectManager.h"

void Contact::setBodyData(RigidBodyComponent* first, RigidBodyComponent *sec, real friction, real restitution)
{
    Contact::restitution = restitution;
    Contact::friction = friction;
    Contact::body[0] = first;
    Contact::body[1] = sec;

}

void Contact::CompareBodyState()
{
    if (!body[1]) 
      return;

    bool Secbodyawake = body[1]->IsBodyAwake();
    bool firstbodyawake = body[0]->IsBodyAwake();
    
    if (firstbodyawake ^ Secbodyawake) {
        if (firstbodyawake) body[1]->setSleep();
        else body[0]->setSleep();
    }
}



inline
void Contact::calculate2SegmentContact()
{
    Vector3D segment[2];
    //  campare  x z
    if (real_abs(ConNormalVector.y) <= real_abs(ConNormalVector.x))
    {
      auto z_square = ConNormalVector.z * ConNormalVector.z;
      auto x_square = ConNormalVector.x * ConNormalVector.x;
        real tempv = (real)1.0f/ (real)real_sqrt(z_square +
          x_square);


        segment[0].x = ConNormalVector.z*tempv;
        segment[0].y = 0;
        segment[0].z = -ConNormalVector.x*tempv;


        segment[1].x = ConNormalVector.y*segment[0].x;
        segment[1].y = ConNormalVector.z*segment[0].x - ConNormalVector.x*segment[0].z;
        segment[1].z = -ConNormalVector.y*segment[0].x;
    }
    else
    {
      //  campare  y z
      auto z_square = ConNormalVector.z*ConNormalVector.z;
      auto y_square = ConNormalVector.y*ConNormalVector.y;

        real tempv = (real)1.0/ (real)real_sqrt(z_square + y_square);

        segment[0].x = 0;
        segment[0].y = -ConNormalVector.z*tempv;
        segment[0].z = ConNormalVector.y*tempv;

        segment[1].x = ConNormalVector.y*segment[0].z -
            ConNormalVector.z*segment[0].y;
        segment[1].y = -ConNormalVector.x*segment[0].z;
        segment[1].z = ConNormalVector.x*segment[0].y;
    }

    // Make a matrix from the three vectors.
    ContactSysterm.Setting( ConNormalVector,segment[0], segment[1]);
}

Vector3D Contact::calculateVelocityInLocalWorld(unsigned bodyFlag, real duration)
{
  /*
  collision point's velocity
  change coordinates
  Calculate velocity
  planar velocities
  */
	RigidBodyComponent *currentRigidBody = body[bodyFlag];

  Vector3D contactPointVelocity, result, acceleration;
     contactPointVelocity = currentRigidBody->getRotation() % RelativeCollisionP[bodyFlag];
    contactPointVelocity += currentRigidBody->getVelocity();

     result = ContactSysterm.transformTranspose(contactPointVelocity);

     acceleration = currentRigidBody->AccelerationFromPreviousFrame() * duration;

    acceleration = ContactSysterm.transformTranspose(acceleration);

    acceleration.x = 0;

    result += acceleration;

    return result;
}


void Contact::TheVelocityOFDelta(float duration)
{
    float TheAcceleration = 0;

    if (body[0]->IsBodyAwake())
    {
	     TheAcceleration+= body[0]->AccelerationFromPreviousFrame() * duration * ConNormalVector;
    }

    if (body[1] && body[1]->IsBodyAwake())
    {
        TheAcceleration -=
            body[1]->AccelerationFromPreviousFrame() * duration * ConNormalVector;
    }

    float temp = restitution;
    if (real_abs(VelocityAtContactPoint.x) < 0.25f)
    {
        temp = (float)0.0f;
    }

    VelocityOfDelta = 0.f-VelocityAtContactPoint.x -temp * (VelocityAtContactPoint.x - TheAcceleration);
}


void Contact::calculateInternals(float duration)
{

  if (!body[0])
  {
    ConNormalVector *= -1;

    RigidBodyComponent *temp = body[0];
    body[0] = body[1];
    body[1] = temp;
  };
    assert(body[0]);


    calculate2SegmentContact();

   
    RelativeCollisionP[0] = theCollisionPoint - body[0]->getPosition();
    if (body[1]) {
        RelativeCollisionP[1] = theCollisionPoint - body[1]->getPosition();
    }


    VelocityAtContactPoint = calculateVelocityInLocalWorld(0, duration);
    if (body[1]) {
        VelocityAtContactPoint -= calculateVelocityInLocalWorld(1, duration);
    }

    TheVelocityOFDelta(duration);
}

void Contact::ModifyVelocityNow(Vector3D velocity_[2],
                                  Vector3D rotation_[2])
{

    Matrix3x3 Inertia[2];
    body[0]->getInversedTensor(&Inertia[0]);
    if (body[1])
        body[1]->getInversedTensor(&Inertia[1]);

    Vector3D CollisionImpulse, CurIM, temp;

    if (friction == (real)0.0)
    {

        CollisionImpulse = calculateImpulse(Inertia);
    }
    else
    {

        CollisionImpulse = ImpulseForceHasFriction(Inertia);
    }


    CurIM = ContactSysterm.transform(CollisionImpulse);

    temp = RelativeCollisionP[0] % CurIM;
    rotation_[0] = Inertia[0].transform(temp);
    velocity_[0].Zero();
    velocity_[0].addScaledVector(CurIM, body[0]->getInversedMass());


    body[0]->addVelocity(velocity_[0]);
    body[0]->addRotation(rotation_[0]);

    if (body[1])
    {
        Vector3D temp_ = CurIM % RelativeCollisionP[1];
        rotation_[1] = Inertia[1].transform(temp_);
        velocity_[1].Zero();
        velocity_[1].addScaledVector(CurIM, -body[1]->getInversedMass());

        body[1]->addVelocity(velocity_[1]);
        body[1]->addRotation(rotation_[1]);
    }
}

inline
Vector3D Contact::calculateImpulse(Matrix3x3 * Tensor_)
{
    Vector3D Collision_impulse, Velocity_delta;

    Velocity_delta = RelativeCollisionP[0] % ConNormalVector;
    Velocity_delta = Tensor_[0].transform(Velocity_delta);
    Velocity_delta = Velocity_delta % RelativeCollisionP[0];


    float CurrentVelocity = Velocity_delta * ConNormalVector;

    CurrentVelocity += body[0]->getInversedMass();


    if (body[1])
    {

        Vector3D temp = RelativeCollisionP[1] % ConNormalVector;
        temp = Tensor_[1].transform(temp);
        temp = temp % RelativeCollisionP[1];


        CurrentVelocity += temp * ConNormalVector;


        CurrentVelocity += body[1]->getInversedMass();
    }

    Collision_impulse.x = VelocityOfDelta / CurrentVelocity;
    Collision_impulse.y = 0;
    Collision_impulse.z = 0;
    return Collision_impulse;
}

/*
  / The equivalent of the cross product in the matrix is multiplication
//Through the skewed symmetric matrix - we build the transformation matrix
//Between linear and angular quantities.
// Build a matrix to convert the contact pulse into a speed change
//In world coordinates.
  */
inline
Vector3D Contact::ImpulseForceHasFriction(Matrix3x3 * Tensor_)
{

    Vector3D CurrentCollisionImpulse;
    real MyMass = body[0]->getInversedMass();

    Matrix3x3 ItoT = Matrix3x3(0.f, 0.f, 0.f,
      0.f, 0.f, 0.f,
      0.f, 0.f, 0.f);
    ItoT.Symmetric(RelativeCollisionP[0]);

    Matrix3x3 velocityMatrix = ItoT;
    velocityMatrix *= Tensor_[0];
    velocityMatrix *= ItoT;
    velocityMatrix *= -1;

    if (body[1])
    {
        ItoT.Symmetric(RelativeCollisionP[1]);

        Matrix3x3 deltaVelWorld2 = ItoT;
        deltaVelWorld2 *= Tensor_[1];
        deltaVelWorld2 *= ItoT;
        deltaVelWorld2 *= -1;

        velocityMatrix += deltaVelWorld2;

        MyMass += body[1]->getInversedMass();
    }
    Matrix3x3 cur_impulse, Velocity_;
    Velocity_ = ContactSysterm.transpose();
    Velocity_ *= velocityMatrix;
    Velocity_ *= ContactSysterm;

    Velocity_.m_matrix[0][0] += MyMass;
    Velocity_.m_matrix[1][1] += MyMass;
    Velocity_.m_matrix[2][2] += MyMass;


    cur_impulse = Velocity_.inverse();

    Vector3D temp(VelocityOfDelta, 0.f-VelocityAtContactPoint.y, 0.f -VelocityAtContactPoint.z);

    CurrentCollisionImpulse = cur_impulse.transform(temp);

    real float_impulseContact = sqrt(
        CurrentCollisionImpulse.y*CurrentCollisionImpulse.y +
        CurrentCollisionImpulse.z*CurrentCollisionImpulse.z
        );
    if (float_impulseContact > CurrentCollisionImpulse.x * friction)
    {
      CurrentCollisionImpulse.z /= float_impulseContact;
      CurrentCollisionImpulse.y /= float_impulseContact;
        CurrentCollisionImpulse.x = Velocity_.m_matrix[0][0] +Velocity_.m_matrix[0][1]*friction*CurrentCollisionImpulse.y +Velocity_.m_matrix[0][2]*friction*CurrentCollisionImpulse.z;
        CurrentCollisionImpulse.x = VelocityOfDelta / CurrentCollisionImpulse.x;
        CurrentCollisionImpulse.y *= friction * CurrentCollisionImpulse.x;
        CurrentCollisionImpulse.z *= friction * CurrentCollisionImpulse.x;
    }
    return CurrentCollisionImpulse;
}

void Contact::ModifyPosition(Vector3D linear_[2],
                                  Vector3D angular_[2],
  float n)
{
  float totalN = 0;
  float totalNArray[2];
  float NewLinear[2];
  float degreeInertia[2];
  float NewAngular[2];
  const float is_first = 1.f;
  const float is_sec = -is_first;

    for (unsigned i = 0; i < 2; i++) if (body[i])
    {
      Matrix3x3 temp = Matrix3x3(0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f);
        body[i]->getInversedTensor(&temp);

    
        Vector3D InertiaLocal =   RelativeCollisionP[i] % ConNormalVector;
        InertiaLocal =   temp.transform(InertiaLocal);
        InertiaLocal = InertiaLocal % RelativeCollisionP[i];
        degreeInertia[i] =   InertiaLocal * ConNormalVector;

     
        totalNArray[i] = body[i]->getInversedMass();


        totalN += totalNArray[i] + degreeInertia[i];
    }

    /*
    // Avoid angle projection is too large 
//but the inertia tensor is small. Limit the angular movement./ Use a small angle approximation of the angle sine (ie
//The amplitude is sine (angular)* projection.magnitude
//But we approximate the sine (angular) to angular).
    */
    for (unsigned k = 0; k < 2; k++) if (body[k])
    {

      float flag = (float)(k == 0) ? is_first : is_sec;
        NewAngular[k] =  flag * n * (degreeInertia[k] / totalN);
        NewLinear[k] = flag * n * (totalNArray[k] / totalN);

        Vector3D projToAxis = RelativeCollisionP[k];
        projToAxis.addScaledVector(ConNormalVector, 0.f -RelativeCollisionP[k].scalarProduct(ConNormalVector)
            );

        float maxprojection = 0.2f * projToAxis.Length();

        if (NewAngular[k] < -maxprojection)
        {
          float preLinear = NewAngular[k] + NewLinear[k];
            NewAngular[k] = -maxprojection;
            NewLinear[k] = preLinear - NewAngular[k];
        }
        else if (NewAngular[k] > maxprojection)
        {
          float preMoving = NewAngular[k] + NewLinear[k];
            NewAngular[k] = maxprojection;
            NewLinear[k] = preMoving - NewAngular[k];
        }

        if (NewAngular[k] == 0)
        {
            angular_[k].Zero();
        }
        else
        {
           
            Vector3D DirectionAng = RelativeCollisionP[k].vectorProduct(ConNormalVector);

            Matrix3x3 InertiaTenLocal = Matrix3x3(0.f, 0.f, 0.f,
              0.f, 0.f, 0.f,
              0.f, 0.f, 0.f);;
            body[k]->getInversedTensor(&InertiaTenLocal);

          
            angular_[k] = InertiaTenLocal.transform(DirectionAng) *(NewAngular[k] / degreeInertia[k]);
        }

        linear_[k] = ConNormalVector * NewLinear[k];

        // Apply the linear movement
        Vector3D theFinalPosition;
        body[k]->getPosition(&theFinalPosition);
        theFinalPosition.addScaledVector(ConNormalVector, NewLinear[k]);
        body[k]->setPosition(theFinalPosition);

        // change for orientation
        Quaternion orientation;
        body[k]->getOrientation(&orientation);
        orientation.addScaledVector(angular_[k], ((real)1.0));
        body[k]->setOrientation(orientation);

        if (!body[k]->IsBodyAwake())
          body[k]->CalculateOriginate();
    }
}





SolveContactSysterm::SolveContactSysterm(collision_flag I, real velocity, real position)
{
    ContactResolverhelper1(I, I);
    ContactResolverhelper2(velocity, position);
}

SolveContactSysterm::SolveContactSysterm(collision_flag velocity,
  collision_flag position,real velocityE,  real positionE)
{
    ContactResolverhelper1(velocity);
    ContactResolverhelper2(velocityE, positionE);
}

void SolveContactSysterm::ContactResolverhelper1(collision_flag I)
{
    ContactResolverhelper1(I, I);
}

void SolveContactSysterm::ContactResolverhelper1(collision_flag velocity,
  collision_flag position)
{
    SolveContactSysterm::Loopingvel = velocity;
    SolveContactSysterm::Loopingpos = position;
}

void SolveContactSysterm::ContactResolverhelper2(real ApproximationVelocity,
                                 real ApproximationPosition)
{
    SolveContactSysterm::ApproximationVelocity = ApproximationVelocity;
    SolveContactSysterm::ApproximationPosition = ApproximationPosition;
}

void SolveContactSysterm::resolveContacts(Contact *contacts,collision_flag count, real duration)
{
    // we have a job
    if (count == 0) return;
    if (!isValid()) return;

    // Prepare the contacts for processing
    prepareContacts(contacts, count, duration);

    // Resolve the interpenetration problems with the contacts.
    GetAdjustPos(contacts, count, duration);

    // Resolve the velocity problems with the contacts.
    GetAdjustVel(contacts, count, duration);
}

void SolveContactSysterm::prepareContacts(Contact* contacts, collision_flag numContacts, real duration)
{
    Contact* lastOne = contacts + numContacts;
	CollisionEvent collisionEvent;

    for (Contact* contact=contacts; contact < lastOne; contact++)
    {
		if (contact->body[0]->GetOwner() == nullptr || contact->body[1]->GetOwner() == nullptr)
		{
			continue;
		}
		collisionEvent.m_gameObject = contact->body[1]->GetOwner();
		contact->body[0]->GetOwner()->HandleEvents(&collisionEvent);
		collisionEvent.m_gameObject = contact->body[0]->GetOwner();
		contact->body[1]->GetOwner()->HandleEvents(&collisionEvent);
		
		EGameObjectType body1 = contact->body[1]->GetOwner()->GetGameObjectType();
		EGameObjectType body0 = contact->body[0]->GetOwner()->GetGameObjectType();
		if (body0 == EGameObjectType::Player || body1 == EGameObjectType::Player)
		{
			if (body1 == EGameObjectType::Trigger || body1 == EGameObjectType::PickUp)
			{
				PiGameObject->DeActivateGameObject(contact->body[1]->GetOwner());
				continue;
			}

			else if (body0 == EGameObjectType::Trigger || body0 == EGameObjectType::PickUp)
			{
				PiGameObject->DeActivateGameObject(contact->body[0]->GetOwner());
				continue;
			}
		}
		if(body0 == EGameObjectType::Trigger || body1 == EGameObjectType::Trigger)
		{
			continue;
		}

		if (contact->body[0]->BodyStatic)
		{
			contact->body[0] = NULL;
		}

		else if (contact->body[1]->BodyStatic)
		{
			contact->body[1] = NULL;
		}
		
		//physics calculations
		contact->calculateInternals(duration);
    }
}
/*
 Find the contact point for the largest possible speed change.
Match the awake state of the contact
The resolution on the contact is the most important.
Check each body in the contact
Check the matching of each body in the new body
Resolved the contact
*/
void SolveContactSysterm::GetAdjustVel(Contact *contact_,unsigned count,real duration)
{
    Vector3D Modifyvelocity[2], Modifyrotation[2];
    Vector3D temp_velocity;

    currentVelLoop = 0;
    while (currentVelLoop < Loopingvel)
    {
      
        real maxnumber = ApproximationVelocity;
        unsigned flag = count;
        for (unsigned i = 0; i < count; i++)
        {
            if (contact_[i].VelocityOfDelta > maxnumber)
            {
                maxnumber = contact_[i].VelocityOfDelta;
                flag = i;
            }
        }
        if (flag == count) break;

        
        contact_[flag].CompareBodyState();

       
        contact_[flag].ModifyVelocityNow(Modifyvelocity, Modifyrotation);

        for (collision_flag j = 0; j < count; j++)
        {
            
            for (collision_flag k = 0; k < 2; k++) if (contact_[j].body[k])
            {
               
                for (collision_flag t = 0; t < 2; t++)
                {
                    if (contact_[j].body[k] == contact_[flag].body[t])
                    {
                        temp_velocity = Modifyvelocity[t] +  Modifyrotation[t].vectorProduct( contact_[j].RelativeCollisionP[k]);
                        contact_[j].VelocityAtContactPoint += contact_[j].ContactSysterm.transformTranspose(temp_velocity) * (real)(k ? -1 : 1);
                        contact_[j].TheVelocityOFDelta(duration);
                    }
                }
            }
        }
        currentVelLoop++;
    }
}
// Iteratively solve the interpenetration in order of severity.
// Match the contact awake state
// Check body in the contact
void SolveContactSysterm::GetAdjustPos(Contact *contact_, unsigned count, real duration)
{
    unsigned kk,flag;
    real maxNumber;
    Vector3D ModifyLinear[2], ModifyAngular[2];
    Vector3D OutPosition;

    PrepositionLoop = 0;
    while (PrepositionLoop < Loopingpos)
    {
        maxNumber = ApproximationPosition;
        flag = count;
        for (kk=0; kk<count; kk++)
        {
            if (contact_[kk].penetration > maxNumber)
            {
                maxNumber = contact_[kk].penetration;
                flag = kk;
            }
        }
        if (flag == count) break;

        
        contact_[flag].CompareBodyState();

        contact_[flag].ModifyPosition(  ModifyLinear,   ModifyAngular,  maxNumber);

        for (kk = 0; kk < count; kk++)
        {
            for (unsigned i = 0; i < 2; i++) if (contact_[kk].body[i])
            {
                for (unsigned d = 0; d < 2; d++)
                {
                    if (contact_[kk].body[i] == contact_[flag].body[d])
                    {
                        OutPosition = ModifyLinear[d] + ModifyAngular[d].vectorProduct(contact_[kk].RelativeCollisionP[i]);

                        contact_[kk].penetration += OutPosition.scalarProduct(contact_[kk].ConNormalVector)  * (i?
                          1:-1);
                    }
                }
            }
        }
        PrepositionLoop++;
    }
}
