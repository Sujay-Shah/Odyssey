//
//  contacts.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#ifndef CONTACTS_H
#define CONTACTS_H

    /*
    overview of the process of solving for contact forces:.
     */
    class SolveContactSysterm;
	class RigidBodyComponent;
    /**
     * A contact represents two bodies in contact. 
     */
    class Contact
    {
        friend class SolveContactSysterm;

    public:
        RigidBodyComponent* body[2];

        real friction;
        real restitution;

        Vector3D theCollisionPoint;

        Vector3D ConNormalVector;

        real penetration;

        void setBodyData(RigidBodyComponent* one, RigidBodyComponent *two, real friction, real restitution);

    protected:

        /**
         * A transform matrix 
         */
        Matrix3x3 ContactSysterm = Matrix3x3(0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f);;

        Vector3D VelocityAtContactPoint;

        /**
         * Holds the required change in velocity
         */
        real VelocityOfDelta;

        /**
         * Holds the world space position 
         */
        Vector3D RelativeCollisionP[2];

    protected:
        void calculateInternals(real duration);

        /**
         * Updates the awake state 
         */
        void CompareBodyState();

        /**
         * Calculates and sets the internal value 
         */
        void TheVelocityOFDelta(real duration);

        /**
         * Calculates and returns the velocity of the contact
         */
        Vector3D calculateVelocityInLocalWorld(unsigned bodyIndex, real duration);

        void calculate2SegmentContact();

        /**
         * 
         */
        void applyImpulse(const Vector3D &impulse, RigidBodyComponent *body,
                          Vector3D *velocityChange, Vector3D *rotationChange);


        void ModifyVelocityNow(Vector3D velocity_[2],
                                 Vector3D rotation_[2]);

        void ModifyPosition(Vector3D linear_[2],
                                 Vector3D angular_[2],
                                 real n);

        /*
    / The equivalent of the cross product in the matrix is multiplication
  //Through the skewed symmetric matrix - we build the transformation matrix
  //Between linear and angular quantities.
  // Build a matrix to convert the contact pulse into a speed change
  //In world coordinates.
    */
        Vector3D calculateImpulse(Matrix3x3 *Tensor_);

        /*
    / The equivalent of the cross product in the matrix is multiplication
  //Through the skewed symmetric matrix - we build the transformation matrix
  //Between linear and angular quantities.
  // Build a matrix to convert the contact pulse into a speed change
  //In world coordinates.
    */
        Vector3D ImpulseForceHasFriction(Matrix3x3 *Tensor_);
    };

    /**
      The number of position iterations stored in the store
      * Last call to resolve contact.
      Tracks whether the internal configuration is valid.
      * Create a new contact resolution with the given number of iterations.
      * Each resolution is called with an optional epsilon value.
      */
    class SolveContactSysterm
    {
    protected:
        /**
         * Holds the number of iterations to perform when resolving
         * velocity and posiyion.
         */
        unsigned Loopingvel;
        unsigned Loopingpos;

        /**
         * To avoid instability smaller velocities smaller
         */
        real ApproximationVelocity;

        /**
         * To avoid instability smaller Position
         */
        real ApproximationPosition;

    private:
        unsigned currentVelLoop;
        unsigned PrepositionLoop;
    private:

    public:
       
        SolveContactSysterm(unsigned I,real V=(real)0.01, real P=(real)0.01);
        SolveContactSysterm(unsigned Loopingvel, unsigned Loopingpos, real ApproximationVelocity=(real)0.01, real ApproximationPosition=(real)0.01);
        bool isValid()
        {
            return (Loopingvel > 0) &&(Loopingpos > 0) &&(ApproximationPosition >= 0.0f) &&(ApproximationPosition >= 0.0f);
        }
        void ContactResolverhelper1(unsigned V, unsigned P);

        void ContactResolverhelper1(unsigned I);

        void ContactResolverhelper2(real V, real P);

        // just use this
        void resolveContacts(Contact *contactArray, unsigned numContacts, real duration);

    protected:
       
        void prepareContacts(Contact* contacts,
          collision_flag numContacts,
          real duration);

        void GetAdjustVel(Contact *contact_, unsigned count, real duration);

          void GetAdjustPos(Contact *contact_, unsigned count, real duration);
    };

 
    class ContactGenerator
    {
    public:
        
        virtual unsigned addContact(Contact *contactInformation, unsigned falg) const = 0;
    };



#endif 
