#ifndef RigidBody_H
#define RigidBody_H

#include "physics_math.h"
class RigidBodyComponent;
namespace physics {

    /**
     * A rigid body is the basic simulation object in the physics
     * core.
     */
   /*
    RigidBody.halfSize = Vector3(4,4,4);
    RigidBody.body->setPosition(0, 7, 0);
    RigidBody.body->setOrientation(1,0,0,0);
    RigidBody.body->setVelocity(0,0,0);
    RigidBody.body->setRotation(0,0,0);
    RigidBody.body->setMass(100.0f);
    Matrix3 it;
    it.setBlockInertiaTensor(RigidBody.halfSize, 100.0f);
    RigidBody.body->setInertiaTensor(it);
    RigidBody.body->setDamping(0.9f, 0.9f);
    RigidBody.body->calculateDerivedData();
    RigidBody.calculateInternals();

    RigidBody.body->setAcceleration(Vector3::GRAVITY);
    RigidBody.body->setAwake(true);
    RigidBody.body->setCanSleep(true);
   */
  
    class RigidBody
    {
    public:

      enum BodyType
      {
        dynamic = 0,
        statics = 1,
        //kinematic = 2
      }bodytype;

      int CollisionLayer = -1;

      RigidBodyComponent *mp_com = NULL;
      
    protected:
        /**
         * Holds the inverse of the mass of the rigid body.
           setting mass: use setMass()
         */
        real inverseMass;

        /**
         * Holds the inverse of the body's inertia tensor. The
         * inertia tensor provided must not be degenerate
         */
        Matrix3x3 inverseInertiaTensor = Matrix3x3(0.f, 0.f, 0.f,
          0.f, 0.f, 0.f,
          0.f, 0.f, 0.f);;;

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

        Vector3D velocity;

        Vector3D rotation;

        /**
         * Holds the inverse inertia tensor of the body in world
         * space. The inverse inertia tensor member is specified in
         * the body's local space.
         *
         * use for inverseInertiaTensor
         */
        Matrix3x3 inverseInertiaTensorWorld = Matrix3x3(0.f, 0.f, 0.f,
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
         * Calculates internal data from state data. This should be called
         * after the body's state is altered directly (it is called
         * automatically during integration). If you change the body's state
         * and then intend to integrate before querying any data (such as
         * the transform matrix), then you can ommit this step.
         */
        void calculateDerivedData();

        /**
         * Integrates the rigid body forward in time by the given amount.
         * This function uses a Newton-Euler integration method, which is a
         * linear approximation to the correct integral. For this reason it
         * may be inaccurate in some cases.
         */
        void integrate(real duration);




        void setMass(const real mass);
        real getMass() const;


        void setInverseMass(const real inverseMass);

        real getInverseMass() const;

        /**
         * Returns true if the mass of the body is not-infinite.
         */
        bool hasFiniteMass() const;

        /*
         * Sets the intertia tensor for the rigid body.
         *
         * param must be a full rank matrix and must be
         * invertible.
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
        void setInverseInertiaTensor(const Matrix3x3 &inverseInertiaTensor);
        void getInverseInertiaTensor(Matrix3x3 *inverseInertiaTensor) const;
        Matrix3x3 getInverseInertiaTensor() const;

        /**
         * Copies the current inverse inertia tensor of the rigid body
         * into the given matrix.
         *
         * @param inverseInertiaTensor A pointer to a matrix to hold
         * the current inverse inertia tensor of the rigid body. The
         * inertia tensor is expressed in world space.
         */
        void getInverseInertiaTensorWorld(Matrix3x3 *inverseInertiaTensor) const;

        /**
         * Gets a copy of the current inverse inertia tensor of the
         * rigid body.
         *
         * @return A new matrix containing the current inverse
         * intertia tensor. The inertia tensor is expressed in world
         * space.
         */
        Matrix3x3 getInverseInertiaTensorWorld() const;

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
        void setOrientation(const real r, const real i,
            const real j, const real k);
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
         *
         * @param point The point to covert, given in world space.
         *
         * @return The converted point, in local space.
         */
        Vector3D getPointInLocalSpace(const Vector3D &point) const;

        /**
         * Converts the given point from world space into the body's
         * local space.
         *
         * @param point The point to covert, given in local space.
         *
         * @return The converted point, in world space.
         */
        Vector3D getPointInWorldSpace(const Vector3D &point) const;

        /**
         * Converts the given direction from world space into the
         * body's local space.
         *
         * @note When a direction is converted between frames of
         * reference, there is no translation required.
         *
         * @param direction The direction to covert, given in world
         * space.
         *
         * @return The converted direction, in local space.
         */
        Vector3D getDirectionInLocalSpace(const Vector3D &direction) const;

        /**
         * Converts the given direction from world space into the
         * body's local space.
         *
         * @note When a direction is converted between frames of
         * reference, there is no translation required.
         *
         * @param direction The direction to covert, given in local
         * space.
         *
         * @return The converted direction, in world space.
         */
        Vector3D getDirectionInWorldSpace(const Vector3D &direction) const;

        /**
         * Sets the velocity of the rigid body.
         *
         * @param velocity The new velocity of the rigid body. The
         * velocity is given in world space.
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


        bool getAwake()
        {             
          return isAwake;
        }

        void setAwake(const bool awake=true);

        bool getCanSleep() const;

        /**
         * Sets whether the body is ever allowed to go to sleep. Bodies
         * under the player's control, or for which the set of
         * transient forces applied each frame are not predictable,
         * should be kept awake.
         *
         * @param canSleep Whether the body can now be put to sleep.
         */
        void setCanSleep(const bool canSleep=true);

        /*@}*/


        /**
         * @name Retrieval Functions for Dynamic Quantities
         *
         * These functions provide access to the acceleration
         * properties of the body. The acceleration is generated by
         * the simulation from the forces and torques applied to the
         * rigid body. Acceleration cannot be directly influenced, it
         * is set during integration, and represent the acceleration
         * experienced by the body of the previous simulation step.
         */
        /*@{*/

        /**
         * Fills the given vector with the current accumulated value
         * for linear acceleration. The acceleration accumulators
         * are set during the integration step. They can be read to
         * determine the rigid body's acceleration over the last
         * integration step. The linear acceleration is given in world
         * space.
         *
         * @param linearAcceleration A pointer to a vector to receive
         * the linear acceleration data.
         */
        void getLastFrameAcceleration(Vector3D *linearAcceleration) const;


        Vector3D getLastFrameAcceleration() const;


        void clearAccumulators();

        void addForce(const Vector3D &force);


        void addForceAtPoint(const Vector3D &force, const Vector3D &point);


        void addForceAtBodyPoint(const Vector3D &force, const Vector3D &point);


        void addTorque(const Vector3D &torque);


        void setAcceleration(const Vector3D &acceleration);


        void setAcceleration(const real x, const real y, const real z);

        void getAcceleration(Vector3D *acceleration) const;

        Vector3D getAcceleration() const;


    };

} 


void PrintMatrix4x4(const Matrix4x4 & mat);
Vector3D RotationMatrixToEulerAngles(Matrix4x4 R);
glm::quat RotationMatrixToQuat(const Matrix4x4 &R);




/*glm::quat EulerAnglesToQuaternion(Vector3D angle);

glm::quat EulerAnglesToQuaternion(vec3 angle);*/
#endif 
