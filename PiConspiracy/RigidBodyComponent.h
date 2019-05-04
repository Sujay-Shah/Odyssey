//
//  RigidBodyComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//



#include "DebugRenderer.h"
#pragma once
struct RigiBodyType
{
	enum Enum
	{
		Dynamic,
		Static,

		NUM
	};
};
TYPEDEF_ENUM(RigiBodyType);

class RigidBodyComponent : public Component
{
	COMPONENT_DECLARE(RigidBodyComponent);
public:
	~RigidBodyComponent();
  void Initialize() override final;
  void Update(float a_dTime) override final; // the update function
  void Serialize(const json&) override final;
  void HandleEvent(Event* a_pEvent) override final;
  void Deactivate(void) override final;
  void OverrideParams(const json&,float offset) override final;

  void AddToRotation(float a_x, float a_y, float a_z);

  inline mat4 GetModelingMatrix();
  inline DebugRenderer* GetDebugRenderer();

  int CollisionLayer = -1;
  vec3 m_halfSize;
  glm::quat m_rotation;
  mat4 m_modelMatrix;
  // for gravity 
  bool ActiveGravity = true;
  // for collision
  bool BodyStatic = false;

  bool AutoColliderSize = true;

  ERigiBodyType m_type;
  BaseCollider* m_collider;
  Transform *m_transform;
  DebugRenderer *m_pDebugRenderer;
protected:
    /**
     * Holds the inverse of the mass of the rigid body.
       setting mass: use setMass()
     */
    real m_inverseMass;

    /**
     * Holds the inverse of the body's inertia tensor. The
     * inertia tensor provided must not be degenerate
     */
    Matrix3x3 inverseInsideTensor = Matrix3x3(0.f, 0.f, 0.f,
      0.f, 0.f, 0.f,
      0.f, 0.f, 0.f);

    /**
     * Holds the amount of damping applied to linear
     * motion.
     */
    real linearDamping;

    /**
     * Holds the amount of damping applied to angular
     * motion.
     */
    real angularDamping;

    Vector3D position;
    Quaternion orientation;
    Vector3D m_velocity;
    Vector3D rotation;

  
    Matrix3x3 WorldTensor_inverse = Matrix3x3(0.f, 0.f, 0.f,
      0.f, 0.f, 0.f,
      0.f, 0.f, 0.f);

    /**
     * Holds the amount of motion of the body. This is a recency
     * weighted mean that can be used to put a body to sleap.
     */
    real motion;

    bool isAwake;

    bool canSleep;

    /**
     * Holds a transform matrix for converting body space into
     * world space and vice versa. This can be achieved by calling
     * the getPointIn*Space functions.
     *
     * use for getPointInLocalSpace
     * use for getPointInWorldSpace
     * use for getTransform
     */
    Matrix4x4 transformMatrix = Matrix4x4(1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 0.f);;

    /**
     * These data members store the current force, torque and
     * acceleration of the rigid body.
     */

    Vector3D forceAccum;
    Vector3D torqueAccum;
    Vector3D acceleration;

    /**
     * Holds the linear acceleration of the rigid body, for the
     * previous frame.
     */
    Vector3D lastFrameAcceleration;

    /*@}*/

public:

    /**
     * Calculates internal data from state data. 
     */
    void CalculateOriginate();

    /**
     * Integrates the rigid body forward in time by the given amount.
     */
    void calculateIntegrate(real duration);

    void setMass(const real mass);
    real getMass() const;

    void setInverseMass(const real inverseMass);
    real getInversedMass() const;

    /**
     * Returns true if the mass of the body is not-infinite.
     */
    bool hasFiniteMass() const;

    /*
     * Sets the intertia tensor for the rigid body.
     */
    void setInertiaTensor(const Matrix3x3 &inertiaTensor);

    void getInertiaTensor(Matrix3x3 *inertiaTensor) const;

    Matrix3x3 getInertiaTensor() const;

    /**
     * Copies the current inertia tensor of the rigid body into
     * the given matrix.
     */
    void getInertiaTensorWorld(Matrix3x3 *inertiaTensor) const;
    Matrix3x3 getInertiaTensorWorld() const;

    //Inverse Inertia Tensor
    void setInverseTensor(const Matrix3x3 &inverseInertiaTensor);
    void getInverseTensor(Matrix3x3 *inverseInertiaTensor) const;
    Matrix3x3 getInverseTensor() const;

    /**
     * Copies the current inverse inertia tensor of the rigid body
     * into the given matrix.
     */
    void getInversedTensor(Matrix3x3 *inverseInertiaTensor) const;

    /**
     * Gets a copy of the current inverse inertia tensor 
     */
    Matrix3x3 getInversedTensor() const;

    /* Damping
     * Sets both linear and angular damping in one function call.
     */
    void setDamping(const real linearDamping, const real angularDamping);
    void setLinearDamping(const real linearDamping);
    real getLinearDamping() const;
    void setAngularDamping(const real angularDamping);
    real getAngularDamping() const;

    // Position
    void setPosition(const Vector3D &position);
    void setPosition(const real x, const real y, const real z);
    void getPosition(Vector3D *position) const;
    Vector3D getPosition() const;

    /**
     * Sets the orientation of the rigid body.
     */
    void setOrientation(const Quaternion &orientation);
    void setOrientation(glm::quat &other)
    {
      setOrientation(Quaternion(other));
    }
    void setOrientation(const real r, const real i, const real j, const real k);
    void getOrientation(Quaternion *orientation) const;
    Quaternion getOrientation() const;
    void getOrientation(Matrix3x3 *matrix) const;
    void getOrientation(real matrix[3][3]) const;

    /**
     * Fills the given matrix with a transformation representing
     * the rigid body's position and orientation.
     */
    void getTransform(Matrix4x4 *transform) const;
    void getTransform(real matrix[16]) const;
    void getGLTransform(float matrix[16]) const;

    /**
     * Gets a transformation representing the rigid body's
     * position and orientation.
     */
    Matrix4x4 getTransform() const;

    /**
     * Converts the given point from world space into the body's
     * local space.
     
     */
    Vector3D gePositionInLocal(const Vector3D &point) const;

    /**
     * Converts the given point from world space into the body's
     * local space.
     */
    Vector3D getPositionInWorld(const Vector3D &point) const;

    /**
     * Converts the given direction from world space into the
     * body's local space.
     *
     */
    Vector3D GetDirecInLocal(const Vector3D &direction) const;

    /**
     * Converts the given direction from world space into the
     * body's local space.
     */
    Vector3D getDirectionInWorldSpace(const Vector3D &direction) const;

    /**
     * Sets the velocity of the rigid body.
     */
    void setVelocity(const Vector3D &velocity);

    void setVelocity(const real x, const real y, const real z);

    void getVelocity(Vector3D *velocity) const;

    Vector3D getVelocity() const;

    void addVelocity(const Vector3D &deltaVelocity);

	void addVelocity( vec3 &deltaVelocity);

    /* set rotation force,  NOT rotate object by angle!!!!!  
     rotate object by angle -> setOrientation(.....)
    */
    void setRotation(const Vector3D &rotation);

    /*set rotation force, NOT rotate object by angle!!!!!
      rotate object by angle->setOrientation(.....)
      */
    void setRotation(const real x, const real y, const real z);

    /* don't use these fuctions unless you know what they are exactly
       get/add rotation force
    */
    void getRotation(Vector3D *rotation) const;
    Vector3D getRotation() const;
    void addRotation(const Vector3D &deltaRotation);

    bool IsBodyAwake()
    {             
      return isAwake;
    }

    void setSleep(const bool awake=true);

    bool getCanSleep() const;

    void setCanSleep(const bool canSleep=true);

    void AccelerationFromPreviousFrame(Vector3D *linearAcceleration) const;

    Vector3D AccelerationFromPreviousFrame() const;

    void clearAcc();

    void addForce(const Vector3D &force);


    void addForceAtPoint(const Vector3D &force, const Vector3D &point);


    void addForceOnthePoint(const Vector3D &force, const Vector3D &point);


    void addTorque(const Vector3D &torque);


    void setAcceleration(const Vector3D &acceleration);


    void setAcceleration(const real x, const real y, const real z);

    void getAcceleration(Vector3D *acceleration) const;

    Vector3D getAcceleration() const;

private:
	void SetFractureHit();
};

void PrintMatrix4x4(const Matrix4x4 & mat);
Vector3D RotationMatrixToEulerAngles(Matrix4x4 R);
glm::quat RotationMatrixToQuat(const Matrix4x4 &R);

inline mat4 RigidBodyComponent::GetModelingMatrix()
{
	return m_modelMatrix;
}

inline DebugRenderer* RigidBodyComponent::GetDebugRenderer()
{
	return m_pDebugRenderer;
}