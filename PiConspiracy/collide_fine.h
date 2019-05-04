
#ifndef COLLISION_FINE_H
#define COLLISION_FINE_H


namespace physics {

    // Forward declarations of primitive friends
    class CrossLine;
    class CoDet;
    class ColClineSphere;
    class ColClineBox;
    class ColClinePlane
    {
    public:

      Vector3D direction;
      real offset;
    };
    /**
     * Represents a primitive to detect collisions against.
     */
    class CollisionBoss
    {
    public:
        friend class CrossLine;
        friend class CoDet;

        /**
         * The rigid body that is represented by this primitive.
         */
        RigidBody * body;

        Matrix4x4 offset = Matrix4x4(1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          0.f, 0.f, 0.f, 0.f);


        void calPhyInner();


        Vector3D getCool(collision_flag flag) const;

       
        const Matrix4x4& getTransform() const;


    protected:
       
      Matrix4x4 transform = Matrix4x4(1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 0.f);
    };

    

    struct CollisionInfo;

    class CoDet
    {
    public:
      static collision_flag ColCheck_BallAPlane(
        const ColClineSphere &sphere,
        const ColClinePlane &plane,
        CollisionInfo *info
      );

      static collision_flag precaBallAPlane(ColClineSphere &one, ColClinePlane &two);
  
      /*
      static collision_flag precaBallAPlane(ColClineSphere &one, ColClinePlane &two)
      {
        Vector3D CLinent = one.getCool(3);
        real normal = one.radius;
        real penetration = (two.direction - one.body->getVelocity()).x;
        real centreDistance = one.offset.Phy_multiply_Vector3(CLinent).x;
        Vector3D currentforce = CLinent * normal;
        // Check if we're within radius
        if ((currentforce+CLinent).x > penetration + normal)
        {
          return 0;
        }
        if (centreDistance < 0)
        {
          one.radius += 1;
        }
        return 1;
      }*/

      static const collision_flag Cubeface1(const physics::ColClineBox & first, const physics::ColClineBox & sec, Vector3D &toCentre, real &pen, collision_flag &compareflag);

      static collision_flag ColCheck_BoxABox(
        const ColClineBox &fisrt,
        const ColClineBox &sec,
        CollisionInfo *info, int static_flag = 0
      );

      static const collision_flag Cubeface2(const physics::ColClineBox & first, const physics::ColClineBox & sec, Vector3D &toCentre, real &pen, collision_flag &compareflag);

      static collision_flag ColCheck_BallAHPlane(
        const ColClineSphere &sphere,
        const ColClinePlane &plane,
        CollisionInfo *info
      );

      static collision_flag ColCheck_BallABall(
        const ColClineSphere &fisrt,
        const ColClineSphere &sec,
        CollisionInfo *info, int static_flag = 0
      );

      CollisionInfo * precaBallABall(const ColClineSphere &fisrt,
        const ColClineSphere &sec);

      static collision_flag ColCheck_BallAHPlane(
        const ColClineBox &box,
        const ColClinePlane &plane,
        CollisionInfo *info
      );

      static collision_flag precaBallAHPlane(
        const ColClineBox &box,
        const ColClinePlane &plane);

      static collision_flag ColCheck_BallABox(
        const ColClineBox &box,
        const ColClineSphere &sphere,
        CollisionInfo *info, int static_flag = 0
      );
      CollisionInfo * precaBoxABox(const ColClineBox &fisrt,
        const ColClineBox &sec);

      static collision_flag ColCheck_BoxAPoint(
        const ColClineBox &box,
        const Vector3D &point,
        CollisionInfo *info
      );
     
      
    };


    class Contact;
    struct CollisionInfo
    {
      void reset(collision_flag maxContacts)
      {
        FlagOfcon = maxContacts;
        currentcount = 0;
        CollisionInfo = CollisionInfoPool;
      }
      bool hasMoreContacts()
      {
        return FlagOfcon > 0;
      }

      Contact *CollisionInfoPool;
      real toleranceForce;
      real frictionForce;
      real restitution;
      int FlagOfcon;
      collision_flag currentcount;
      Contact *CollisionInfo;
      Contact *indvi_pool;
      real ownflag;
      void addContacts(collision_flag count);
    };

    class CrossLine
    {
    public:

        static bool CollisionBeBallAHPlane(const ColClineSphere &sphere,const ColClinePlane &plane);

        static bool CollisionBeBallABall(const ColClineSphere &first,const ColClineSphere &sec);

        static bool CollisionBeBoxABox(const ColClineBox &first,const ColClineBox &sec);

        static bool CollisionBeBoxAPlane(const ColClineBox &box,const ColClinePlane &plane);
    };


    

    

    class ColClineSphere : public CollisionBoss
    {
    public:
      real radius;
    };
    
    class ColClineBox : public CollisionBoss
    {
    public:
      Vector3D halfSize;
    };

} 

#endif 
