
#ifndef COLLISION_COARSE_H
#define COLLISION_COARSE_H

namespace physics {

  struct PotentialContact
  {

    RigidBody* bodyContact[2];
  };


  struct BoundingTypeSphere
  {
    real radius_;
    Vector3D centre_;


  public:
    BoundingTypeSphere(const BoundingTypeSphere &one, const BoundingTypeSphere &two);
    void compare(const physics::BoundingTypeSphere & First, const physics::BoundingTypeSphere & second);
    BoundingTypeSphere(const Vector3D &centre, real radius);
    real Growth(const BoundingTypeSphere &other) const;

    real getSize() const;

    bool BoundingSphereOverlaps(const BoundingTypeSphere *other) const;



  };




  template<class BoundingVolumeClass>
  class BVHNode
  {
  public:
    RigidBody * Rbody;


    BVHNode * parent;
    BVHNode * children[2];


    BoundingVolumeClass BoundingVolume;



    /**
     * Creates a new node in the hierarchy with the given parameters.
     */
    BVHNode(BVHNode *parent, const BoundingVolumeClass &volume,
      RigidBody* body = NULL)
      : parent(parent), BoundingVolume(volume), Rbody(body)
    {
      children[0] = NULL;
      children[1] = NULL;
    }
    /**
    * Inserts the given rigid body, with the given bounding volume,
    * into the hierarchy. This may involve the creation of
    * further bounding volume nodes.
    */
    void insertToVol(RigidBody* body, const BoundingVolumeClass &volume);
    /**
     * Checks the potential contacts from this node downwards in
     * the hierarchy, writing them to the given array (up to the
     * given limit). Returns the number of potential contacts it
     * found.
     */
    unsigned getPotentlCon(PotentialContact* contacts,
      unsigned limit) const;

    /**
    * Checks if this node is at the bottom of the hierarchy.
    */
    bool IsLeafNode() const
    {
      return (Rbody != NULL);
    }

    ~BVHNode();

  public:


    bool BVHNodeoverlaps(const BVHNode<BoundingVolumeClass> *other) const;
    void recalBVolume(bool recurse = true);

    unsigned PoWith(
      const BVHNode<BoundingVolumeClass> *other, PotentialContact* PotContact, unsigned loag) const;


  };


  template<class BoundingVolumeClass>
  bool BVHNode<BoundingVolumeClass>::BVHNodeoverlaps(
    const BVHNode<BoundingVolumeClass> * other
  ) const
  {
    return BoundingVolume->overlaps(other->BoundingVolume);
  }

  template<class BoundingVolumeClass>
  void BVHNode<BoundingVolumeClass>::insertToVol(
    RigidBody* Body, const BoundingVolumeClass &Volume
  )
  {

    if (IsLeafNode())
    {
      children[0] = new BVHNode<BoundingVolumeClass>(this, BoundingVolume, Rbody);

      children[1] = new BVHNode<BoundingVolumeClass>(this, Volume, Body);

      // And we now loose the body (we're no longer a leaf)
      this->Rbody = NULL;

      // We need to recalculate our bounding volume
      recalBVolume();
    }

    else
    {
      if (children[0]->volume.getGrowth(Volume) <
        children[1]->volume.getGrowth(Volume))
      {
        children[0]->insert(Body, Volume);
      }
      else
      {
        children[1]->insert(Body, Volume);
      }
    }
  }

  template<class BoundingVolumeClass>
  BVHNode<BoundingVolumeClass>::~BVHNode()
  {
    // If we don't have a parent, then we ignore the sibling
    // processing
    if (parent)
    {
      // Find our sibling
      BVHNode<BoundingVolumeClass> *tempS;
      if (parent->children[0] == this)
        tempS = parent->children[1];
      else
        tempS = parent->children[0];

      tempS->children[1] = NULL;
      parent->children[0] = tempS->children[0];
      parent->Rbody = tempS->Rbody;
      parent->children[1] = tempS->children[1];
      parent->BoundingVolume = tempS->BoundingVolume;

      tempS->children[0] = NULL;


      tempS->parent = NULL;
      tempS->Rbody = NULL;

      delete tempS;


      parent->recalBVolume();
    }


    if (children[1]) {
      children[1]->parent = NULL;
      delete children[1];
    }
    if (children[0]) {
      children[0]->parent = NULL;
      delete children[0];
    }

  }
  template<class BoundingVolumeClass>
  unsigned BVHNode<BoundingVolumeClass>::getPotentlCon(
    PotentialContact* Potencon, unsigned flag
  ) const
  {
    if (flag == 0 || IsLeafNode())
      return 0;

    return children[0]->PoWith(
      children[1], Potencon, flag
    );
  }

  template<class BoundingVolumeClass>
  void BVHNode<BoundingVolumeClass>::recalBVolume(
    bool recurse
  )
  {
    if (IsLeafNode()) return;

    // Use the bounding volume combining constructor.
    BoundingVolume = BoundingVolumeClass(
      children[0]->volume,
      children[1]->volume
    );

    // Recurse up the tree
    if (parent) parent->recalBVolume(true);
  }



  template<class BoundingVolumeClass>
  unsigned BVHNode<BoundingVolumeClass>::PoWith(
    const BVHNode<BoundingVolumeClass> *other, PotentialContact* Potencon, unsigned flag
  ) const
  {

    if (!BVHNodeoverlaps(other) || flag == 0)
      return 0;
    if (IsLeafNode() && other->IsLeafNode())
    {
      Potencon->bodyContact[0] = Rbody;
      Potencon->bodyContact[1] = other->Rbody;
      return 1;
    }

    if (other->IsLeafNode() || (!IsLeafNode() && BoundingVolume->getSize() >= other->BoundingVolume->getSize()))
    {

      unsigned tempcout = children[0]->PoWith(other, Potencon, flag);

      if (flag > tempcout) {
        return tempcout + children[1]->PoWith(other, Potencon + tempcout, flag - tempcout);
      }
      else {
        return tempcout;
      }
    }
    else
    {
      // Recurse into the other node
      unsigned tempcout = PoWith(other->children[0], Potencon, flag);

      // Check we have enough slots to do the other side too
      if (flag > tempcout) {
        return tempcout + PoWith(other->children[1], Potencon + tempcout, flag - tempcout);
      }
      else {
        return tempcout;
      }
    }
  }

} // namespace

#endif 