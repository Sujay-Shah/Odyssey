#include "PiEnginePreCompiled.h"
#include "collide_fine.h"
using namespace physics;

void CollisionBoss::calPhyInner()
{
    transform = body->getTransform().Phy_multiply_Matrix(offset);
}

inline Vector3D physics::CollisionBoss::getCool(collision_flag index) const
{
  return transform.getAxisVector(index);
}

inline const Matrix4x4 & physics::CollisionBoss::getTransform() const
{
  return transform;
}

bool CrossLine::CollisionBeBallAHPlane(
    const ColClineSphere &sphere,
    const ColClinePlane &plane)
{
    // Find the distance from the origin
    real ballDistance =
        plane.direction *
        sphere.getCool(3) -
        sphere.radius;

    // Check for the intersection
    return ballDistance <= plane.offset;
}

bool CrossLine::CollisionBeBallABall(
    const ColClineSphere &first,
    const ColClineSphere &sec)
{
    // Find the vector between the objects
    Vector3D midline = first.getCool(3) - sec.getCool(3);

    // See if it is large enough.
    return midline.SquareLength() <
        (first.radius+sec.radius)*(first.radius+sec.radius);
}

static inline real transformToAxis(
    const ColClineBox &box,
    const Vector3D &cool
    )
{
    return
      box.halfSize.y * real_abs(cool * box.getCool(1)) +
      box.halfSize.x * real_abs(cool * box.getCool(0)) +
      box.halfSize.z * real_abs(cool * box.getCool(2));
}

/**
 * This function checks if the sec boxes overlap
 * along the given axis. 
 */
static inline bool overlapOnAxis( const ColClineBox &first,
    const ColClineBox &sec, const Vector3D &axis,
    const Vector3D &toCentre
    )
{

    return (real_abs(toCentre * axis) < transformToAxis(first, axis) + transformToAxis(sec, axis));
}

// This preprocessor definition is only used as a convenience
// in the boxAndBox intersection  method.
#define FACECHECKING(axis) overlapOnAxis(first, sec, (axis), toCentre)

bool CrossLine::CollisionBeBoxABox(
    const ColClineBox &first,
    const ColClineBox &sec
    )
{
    // Find the vector between the sec centres
    Vector3D toCentre = sec.getCool(3) - first.getCool(3);

    return (
        
        FACECHECKING(first.getCool(0)) &&
        FACECHECKING(first.getCool(1)) &&
        FACECHECKING(first.getCool(2)) &&

        
        FACECHECKING(sec.getCool(0)) &&
        FACECHECKING(sec.getCool(1)) &&
        FACECHECKING(sec.getCool(2)) &&

        
        FACECHECKING(first.getCool(0) % sec.getCool(0)) &&
        FACECHECKING(first.getCool(0) % sec.getCool(1)) &&
        FACECHECKING(first.getCool(0) % sec.getCool(2)) &&
        FACECHECKING(first.getCool(1) % sec.getCool(0)) &&
        FACECHECKING(first.getCool(1) % sec.getCool(1)) &&
        FACECHECKING(first.getCool(1) % sec.getCool(2)) &&
        FACECHECKING(first.getCool(2) % sec.getCool(0)) &&
        FACECHECKING(first.getCool(2) % sec.getCool(1)) &&
        FACECHECKING(first.getCool(2) % sec.getCool(2))
    );
}
#undef TEST_OVERLAP

bool CrossLine::CollisionBeBoxAPlane(
    const ColClineBox &box,
    const ColClinePlane &plane
    )
{
    real projectedRadius = transformToAxis(box, plane.direction);

    // how far the box is from the origin
    real boxDistance =
        plane.direction *
        box.getCool(3) -
        projectedRadius;

    // Check for the intersection
    return boxDistance <= plane.offset;
}

collision_flag CoDet::ColCheck_BallAPlane(
    const ColClineSphere &sphere,
    const ColClinePlane &plane,
    CollisionInfo *data
    )
{

    if (data->FlagOfcon <= 0) return 0;


    Vector3D spherelocation = sphere.getCool(3);


    real dis = plane.direction * spherelocation - plane.offset;


    if (dis*dis > sphere.radius*sphere.radius)
    {
        return 0;
    }

 
    Vector3D plandir = plane.direction;
    real temp = -dis;
    if (dis < 0)
    {
        plandir *= -1;
        temp = -temp;
    }
    temp += sphere.radius;

 
    Contact* colInfo = data->CollisionInfo;
    
    colInfo->contactPoint = spherelocation - plane.direction * dis;
    colInfo->contactNormal = plandir;
    colInfo->penetration = temp;
    colInfo->setBodyData(sphere.body, NULL, data->frictionForce, data->restitution);

    data->addContacts(1);
    return 1;
}

inline collision_flag physics::CoDet::precaBallAPlane(ColClineSphere & first, ColClinePlane & sec)
{
  Vector3D CLinent = first.getCool(3);
  real normal = first.radius;
  real temp = (sec.direction - first.body->getVelocity()).x;
  real centreDistance = first.offset.Phy_multiply_Vector3(CLinent).x;
  Vector3D currentforce = CLinent * normal;

  if ((currentforce + CLinent).x > temp + normal)
  {
    return 0;
  }
  if (centreDistance < 0)
  {
    first.radius += 1;
  }
  return 1;
}

collision_flag CoDet::ColCheck_BallAHPlane(
    const ColClineSphere &sphere,
    const ColClinePlane &plane,
    CollisionInfo *data
    )
{
 
    if (data->FlagOfcon <= 0) return 0;

    Vector3D spherelocation = sphere.getCool(3);

   
    real ballDistance =
        plane.direction * spherelocation -
        sphere.radius - plane.offset;

    if (ballDistance >= 0) return 0;


    Contact* colInfo = data->CollisionInfo;
    
    colInfo->penetration = -ballDistance;
    colInfo->contactNormal = plane.direction;
    colInfo->contactPoint =
        spherelocation - plane.direction * (ballDistance + sphere.radius);
    colInfo->setBodyData(sphere.body, NULL, data->frictionForce, data->restitution);

    data->addContacts(1);
    return 1;
}

collision_flag CoDet::ColCheck_BallABall(
    const ColClineSphere &first,
    const ColClineSphere &sec,
    CollisionInfo *data, int static_flag
    )
{
    //have contacts
    if (data->FlagOfcon <= 0) return 0;

    Vector3D positionfirst = first.getCool(3);
    Vector3D positionsec = sec.getCool(3);

    // Find the vector between the objects
    Vector3D midline = positionfirst - positionsec;
    real size = midline.Length();

    // See if it is large enough.
    if (size <= 0.0f || size >= first.radius+sec.radius)
    {
        return 0;
    }

    Vector3D normal = midline * (((real)1.0)/size);

    Contact* colInfo = data->CollisionInfo;
    colInfo->contactNormal = normal;
    colInfo->contactPoint = positionfirst + midline * (real)0.5;
    colInfo->penetration = (first.radius+sec.radius - size);


    if(static_flag == 0)
    colInfo->setBodyData(first.body, sec.body,
        data->frictionForce, data->restitution);
    else if (static_flag == 1)
      colInfo->setBodyData(NULL, sec.body,
        data->frictionForce, data->restitution);
    else if (static_flag == 2)
      colInfo->setBodyData(first.body, NULL,
        data->frictionForce, data->restitution);

    data->addContacts(1);
    return 1;
}




static inline real penetrationOnAxis(
    const ColClineBox &first,
    const ColClineBox &sec,
    const Vector3D &axis,
    const Vector3D &toCentre
    )
{


    // overlap
    return transformToAxis(first, axis) + transformToAxis(sec, axis) - real_abs(toCentre * axis);
}


static inline bool parallelaxes( const ColClineBox &first,const ColClineBox &sec,
    Vector3D axis,const Vector3D& toCentre,collision_flag index, real& smallestPenetration,
    collision_flag &smallestCase
    )
{
    if (axis.SquareLength() < 0.0001) return true;
    axis.normalise();

    real temp = penetrationOnAxis(first, sec, axis, toCentre);

    if (temp < 0) return false;
    if (temp < smallestPenetration)
    {
      smallestCase = index;
        smallestPenetration = temp;
        
    }
    return true;
}
// i'm here
void BoxFacing(const ColClineBox &first,
    const ColClineBox &sec, const Vector3D &toCentre,CollisionInfo *data,
    collision_flag best, real pen
    )
{
    Contact* contact = data->CollisionInfo;

    Vector3D normal = first.getCool(best);
    if (first.getCool(best) * toCentre > 0)
    {
        normal = normal * -1.0f;
    }

    Vector3D vertex = sec.halfSize;
    if (sec.getCool(0) * normal < 0) vertex.x = -vertex.x;
    if (sec.getCool(1) * normal < 0) vertex.y = -vertex.y;
    if (sec.getCool(2) * normal < 0) vertex.z = -vertex.z;

    contact->contactNormal = normal;
    contact->penetration = pen;
    contact->contactPoint = sec.getTransform().Phy_multiply_Vector3(vertex);
    contact->setBodyData(first.body, sec.body,
        data->frictionForce, data->restitution);
}

static inline Vector3D contactPoint(
    const Vector3D &pfirst,
    const Vector3D &dfirst,
    real firstSize,
    const Vector3D &psec,
    const Vector3D &dsec,
    real secSize,

 
    bool usefirst)
{
    Vector3D toSt, cfirst, csec;
    real dpStafirst, dpStasec, dpfirstsec, smfirst, smsec;
    real denom, mua, mub;

    smfirst = dfirst.SquareLength();
    smsec = dsec.SquareLength();
    dpfirstsec = dsec * dfirst;

    toSt = pfirst - psec;
    dpStafirst = dfirst * toSt;
    dpStasec = dsec * toSt;

    denom = smfirst * smsec - dpfirstsec * dpfirstsec;

    // Zero denominator indicates parrallel lines
    if (real_abs(denom) < 0.0001f) {
        return usefirst?pfirst:psec;
    }

    mua = (dpfirstsec * dpStasec - smsec * dpStafirst) / denom;
    mub = (smfirst * dpStasec - dpfirstsec * dpStafirst) / denom;

    if (mua > firstSize ||
        mua < -firstSize ||
        mub > secSize ||
        mub < -secSize)
    {
        return usefirst?pfirst:psec;
    }
    else
    {
        cfirst = pfirst + dfirst * mua;
        csec = psec + dsec * mub;

        return cfirst * 0.5 + csec * 0.5;
    }
}

/*#define CHECK_OVERLAP(axis, index) \
    if (!parallelaxes(first, sec, (axis), toCentre, (index), pen, compareflag)) return 0;

const collision_flag physics::CoDet::Cubeface1(const physics::ColClineBox & first, const physics::ColClineBox & sec, Vector3D &toCentre, real &pen, collision_flag &compareflag)
{
  CHECK_OVERLAP(first.getCool(0), 0);
  CHECK_OVERLAP(first.getCool(1), 1);
  CHECK_OVERLAP(first.getCool(2), 2);

  CHECK_OVERLAP(sec.getCool(0), 3);
  CHECK_OVERLAP(sec.getCool(1), 4);
  CHECK_OVERLAP(sec.getCool(2), 5);
  return 0;
}*/
void fillPointFaceBoxBox(
  const ColClineBox &one,
  const ColClineBox &two,
  const Vector3D &toCentre,
  CollisionInfo *data,
  unsigned best,
  real pen, int static_flag
)
{
  Contact* contact = data->CollisionInfo;

  Vector3D normal = one.getCool(best);
  if (one.getCool(best) * toCentre > 0)
  {
    normal = normal * -1.0f;
  }

  Vector3D vertex = two.halfSize;
  if (two.getCool(0) * normal < 0) vertex.x = -vertex.x;
  if (two.getCool(1) * normal < 0) vertex.y = -vertex.y;
  if (two.getCool(2) * normal < 0) vertex.z = -vertex.z;

  contact->contactNormal = normal;
  contact->penetration = pen;
  contact->contactPoint = two.getTransform().Phy_multiply_Vector3(vertex);
  /*contact->setBodyData(one.body, two.body,
    data->frictionForce, data->restitution);*/


  if (static_flag == 0)
    contact->setBodyData(one.body, two.body,
      data->frictionForce, data->restitution);
  else if (static_flag == 1)
    contact->setBodyData(NULL, two.body,
      data->frictionForce, data->restitution);
  else if (static_flag == 2)
    contact->setBodyData(one.body, NULL,
      data->frictionForce, data->restitution);
}
static inline bool tryAxis(
  const ColClineBox &one,
  const ColClineBox &two,
  Vector3D axis,
  const Vector3D& toCentre,
  unsigned index,

  // values may be updated
  real& smallestPenetration,
  unsigned &smallestCase
)
{
  // Make sure we have a normalized axis, and don't check almost parallel axes
  if (axis.SquareLength() < 0.0001) return true;
  axis.normalise();

  real penetration = penetrationOnAxis(one, two, axis, toCentre);

  if (penetration < 0) return false;
  if (penetration < smallestPenetration) {
    smallestPenetration = penetration;
    smallestCase = index;
  }
  return true;
}

#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best)) return 0;
collision_flag CoDet::ColCheck_BoxABox(
    const ColClineBox &one,
    const ColClineBox &two,
    CollisionInfo *data, int static_flag
    )
{
  // Find the vector between the two centres
  Vector3D toCentre = two.getCool(3) - one.getCool(3);

  real pen = REAL_MAX;
  unsigned best = 0xffffff;

  CHECK_OVERLAP(one.getCool(0), 0);
  CHECK_OVERLAP(one.getCool(1), 1);
  CHECK_OVERLAP(one.getCool(2), 2);

  CHECK_OVERLAP(two.getCool(0), 3);
  CHECK_OVERLAP(two.getCool(1), 4);
  CHECK_OVERLAP(two.getCool(2), 5);

  // Store the best axis-major
  unsigned bestSingleAxis = best;

  CHECK_OVERLAP(one.getCool(0) % two.getCool(0), 6);
  CHECK_OVERLAP(one.getCool(0) % two.getCool(1), 7);
  CHECK_OVERLAP(one.getCool(0) % two.getCool(2), 8);
  CHECK_OVERLAP(one.getCool(1) % two.getCool(0), 9);
  CHECK_OVERLAP(one.getCool(1) % two.getCool(1), 10);
  CHECK_OVERLAP(one.getCool(1) % two.getCool(2), 11);
  CHECK_OVERLAP(one.getCool(2) % two.getCool(0), 12);
  CHECK_OVERLAP(one.getCool(2) % two.getCool(1), 13);
  CHECK_OVERLAP(one.getCool(2) % two.getCool(2), 14);

  // Make sure we've got a result.
  assert(best != 0xffffff);

  if (best < 3)
  {
    fillPointFaceBoxBox(one, two, toCentre, data, best, pen, static_flag);
    data->addContacts(1);
    return 1;
  }
  else if (best < 6)
  {
    fillPointFaceBoxBox(two, one, toCentre*-1.0f, data, best - 3, pen , static_flag);
    data->addContacts(1);
    return 1;
  }
  else
  {
    best -= 6;
    unsigned oneAxisIndex = best / 3;
    unsigned twoAxisIndex = best % 3;
    Vector3D oneAxis = one.getCool(oneAxisIndex);
    Vector3D twoAxis = two.getCool(twoAxisIndex);
    Vector3D axis = oneAxis % twoAxis;
    axis.normalise();

    // The axis should point from box one to box two.
    if (axis * toCentre > 0) axis = axis * -1.0f;

    Vector3D ptOnOneEdge = one.halfSize;
    Vector3D ptOnTwoEdge = two.halfSize;
    for (unsigned i = 0; i < 3; i++)
    {
      if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
      else if (one.getCool(i) * axis > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

      if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
      else if (two.getCool(i) * axis < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
    }

    ptOnOneEdge = one.transform.Phy_multiply_Vector3(ptOnOneEdge);
    ptOnTwoEdge = two.transform.Phy_multiply_Vector3(ptOnTwoEdge);

    // So we have a point and a direction for the colliding edges.
    // We need to find out point of closest approach of the two
    // line-segments.
    Vector3D vertex = contactPoint(
      ptOnOneEdge, oneAxis, one.halfSize[oneAxisIndex],
      ptOnTwoEdge, twoAxis, two.halfSize[twoAxisIndex],
      bestSingleAxis > 2
    );

    // We can fill the contact.
    Contact* contact = data->CollisionInfo;
    contact->penetration = pen;
    contact->contactNormal = axis;
    contact->contactPoint = vertex;
    
	//DETECTS COLLISION BETWEEN 2 BODIES
	/*CollisionEvent collisionEvent;
	if ((contact->body[0] != nullptr && contact->body[0]->mp_com != nullptr) && (contact->body[1] != nullptr && contact->body[1]->mp_com != nullptr))
	{
		collisionEvent.m_gameObject[0] = contact->body[0]->mp_com->GetOwner();
		collisionEvent.m_gameObject[1] = contact->body[1]->mp_com->GetOwner();
		contact->body[0]->mp_com->GetOwner()->HandleEvents(&collisionEvent);
		contact->body[1]->mp_com->GetOwner()->HandleEvents(&collisionEvent);
	}*/


    if (static_flag == 0)
      contact->setBodyData(one.body, two.body,
        data->frictionForce, data->restitution);
    else if (static_flag == 1)
      contact->setBodyData(NULL, two.body,
        data->frictionForce, data->restitution);
    else if (static_flag == 2)
      contact->setBodyData(one.body, NULL,
        data->frictionForce, data->restitution);

    data->addContacts(1);
    return 1;
  }
  return 0;

    /*Vector3D toCentre = sec.getCool(3) - first.getCool(3);

    real pen = REAL_MAX;
    collision_flag compareflag = COLLISIONFLAF_MAX;

    return Cubeface1(first, sec, toCentre, pen, compareflag);


    collision_flag bestSingleAxis = compareflag;

    return Cubeface2(first, sec, toCentre, pen, compareflag);


    assert(compareflag != COLLISIONFLAF_MAX);

    if (compareflag < 3)
    {
        BoxFacing(first, sec, toCentre, data, compareflag, pen);
        data->addContacts(1);
        return 1;
    }
    else if (compareflag < 6)
    {
        BoxFacing(sec, first, toCentre*-1.0f, data, compareflag-3, pen);
        data->addContacts(1);
        return 1;
    }
    else
    {
        compareflag -= 6;
        collision_flag secAxisIndex = compareflag % 3;
        collision_flag firstAxisIndex = compareflag / 3;
        Vector3D secAxis = sec.getCool(secAxisIndex);
        Vector3D firstAxis = first.getCool(firstAxisIndex);
        Vector3D FirstHS = first.halfSize;
        Vector3D cool = firstAxis % secAxis;
        Vector3D SecHS = sec.halfSize;
        cool.normalise();


        if (cool * toCentre > 0) cool = cool * -1.0f;

       
        for (collision_flag i = 0; i < 3; i++)
        {
            if (i == firstAxisIndex) FirstHS[i] = 0;
            else if (first.getCool(i) * cool > 0) FirstHS[i] = -FirstHS[i];

            if (i == secAxisIndex) SecHS[i] = 0;
            else if (sec.getCool(i) * cool < 0) SecHS[i] = -SecHS[i];
        }

        FirstHS = first.transform.Phy_multiply_Vector3(FirstHS);
        SecHS = sec.transform.Phy_multiply_Vector3(SecHS);

       
        Vector3D vertex = contactPoint(
            FirstHS, firstAxis, first.halfSize[firstAxisIndex],
            SecHS, secAxis, sec.halfSize[secAxisIndex],
            bestSingleAxis > 2
            );

     
        Contact* colInfo = data->CollisionInfo;

        colInfo->penetration = pen;
        colInfo->contactNormal = cool;
        colInfo->contactPoint = vertex;
        if(static_flag == 0)
        colInfo->setBodyData(first.body, sec.body,
            data->frictionForce, data->restitution);
        else if (static_flag == 1)
          colInfo->setBodyData(NULL, sec.body,
            data->frictionForce, data->restitution);
        else if (static_flag == 2)
          colInfo->setBodyData(first.body, NULL,
            data->frictionForce, data->restitution);


        data->addContacts(1);
        return 1;
    }
    return 0;*/
}
/*const collision_flag physics::CoDet::Cubeface2(const physics::ColClineBox & first, const physics::ColClineBox & sec, Vector3D &toCentre, real &pen, collision_flag &compareflag)
{
  CHECK_OVERLAP(first.getCool(0) % sec.getCool(0), 6);
  CHECK_OVERLAP(first.getCool(0) % sec.getCool(1), 7);
  CHECK_OVERLAP(first.getCool(0) % sec.getCool(2), 8);
  CHECK_OVERLAP(first.getCool(1) % sec.getCool(0), 9);
  CHECK_OVERLAP(first.getCool(1) % sec.getCool(1), 10);
  CHECK_OVERLAP(first.getCool(1) % sec.getCool(2), 11);
  CHECK_OVERLAP(first.getCool(2) % sec.getCool(0), 12);
  CHECK_OVERLAP(first.getCool(2) % sec.getCool(1), 13);
  CHECK_OVERLAP(first.getCool(2) % sec.getCool(2), 14);
  return 0;
}*/
inline CollisionInfo * physics::CoDet::precaBoxABox(const ColClineBox & fisrt, const ColClineBox & sec)
{
  CollisionInfo *flag = new CollisionInfo;
  flag->FlagOfcon = 0;
  float best = 999999999.f;
  Vector3D toCentrex, toCentrey, toCentrez;
  int j = 1;
  for (int i = 1; i <= 3; i++)
  {
    if (j > 3) j = 1;
    toCentrex = sec.getCool(i) - fisrt.getCool(j++);
    if (toCentrex.Length() < best)
    {
      best = toCentrex.Length();
    }

    if (j > 3) j = 1;
    toCentrey = sec.getCool(i) - fisrt.getCool(j++);
    if (toCentrey.Length() < best)
    {
      best = toCentrey.Length();
    }
   
    if (j > 3) j = 1;
    toCentrez = sec.getCool(i) - fisrt.getCool(j++);
    if (toCentrez.Length() < best)
    {
      best = toCentrez.Length();
    }
    
  }

  flag->FlagOfcon = (int)best;
  return flag;
}
#undef CHECK_OVERLAP




collision_flag CoDet::ColCheck_BoxAPoint(
    const ColClineBox &box,
    const Vector3D &point,
    CollisionInfo *data
    )
{
    
    Vector3D relPt = box.transform.transformInverse(point);

    Vector3D normal;

  
    real min_depth = box.halfSize.x - real_abs(relPt.x);
    if (min_depth < 0) return 0;
    normal = box.getCool(0) * (real)((relPt.x < 0)?-1:1);

    real depth = box.halfSize.y - real_abs(relPt.y);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getCool(1) * (real)((relPt.y < 0) ? -1 : 1);
    }

    depth = box.halfSize.z - real_abs(relPt.z);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getCool(2) * (real)((relPt.z < 0) ? -1 : 1);
    }

    
    Contact* contact = data->CollisionInfo;
    contact->contactNormal = normal;
    contact->contactPoint = point;
    contact->penetration = min_depth;

   
    contact->setBodyData(box.body, NULL,
        data->frictionForce, data->restitution);

    data->addContacts(1);
    return 1;
}

collision_flag CoDet::ColCheck_BallABox(
    const ColClineBox &box,
    const ColClineSphere &sphere,
    CollisionInfo *data, int static_flag
    )
{
   
    Vector3D centre = sphere.getCool(3);
    Vector3D relCentre = box.transform.transformInverse(centre);

 
    if (real_abs(relCentre.x) - sphere.radius > box.halfSize.x ||
        real_abs(relCentre.y) - sphere.radius > box.halfSize.y ||
        real_abs(relCentre.z) - sphere.radius > box.halfSize.z)
    {
        return 0;
    }

    Vector3D closestPt(0,0,0);
    real compareDistance;

   
    compareDistance = relCentre.x;
    if (compareDistance > box.halfSize.x) compareDistance = box.halfSize.x;
    if (compareDistance < -box.halfSize.x) compareDistance = -box.halfSize.x;
    closestPt.x = compareDistance;

    compareDistance = relCentre.y;
    if (compareDistance > box.halfSize.y) compareDistance = box.halfSize.y;
    if (compareDistance < -box.halfSize.y) compareDistance = -box.halfSize.y;
    closestPt.y = compareDistance;

    compareDistance = relCentre.z;
    if (compareDistance > box.halfSize.z) compareDistance = box.halfSize.z;
    if (compareDistance < -box.halfSize.z) compareDistance = -box.halfSize.z;
    closestPt.z = compareDistance;

    compareDistance = (closestPt - relCentre).SquareLength();
    if (compareDistance > sphere.radius * sphere.radius) return 0;

    Vector3D closestPsecrld = box.transform.transform(closestPt);

    Contact* colInfo = data->CollisionInfo;
    colInfo->contactNormal = (closestPsecrld - centre);
    colInfo->contactNormal.normalise();
    colInfo->contactPoint = closestPsecrld;
    colInfo->penetration = sphere.radius - real_sqrt(compareDistance);


    if(static_flag == 0)
      colInfo->setBodyData(box.body, sphere.body,
        data->frictionForce, data->restitution);
    else if (static_flag == 1)
      colInfo->setBodyData(NULL, sphere.body,
        data->frictionForce, data->restitution);
    else if (static_flag == 2)
      colInfo->setBodyData(box.body, NULL,
        data->frictionForce, data->restitution);


    data->addContacts(1);
    return 1;
}

collision_flag CoDet::ColCheck_BallAHPlane(
    const ColClineBox &box,
    const ColClinePlane &plane,
    CollisionInfo *data
    )
{

    if (data->FlagOfcon <= 0) return 0;


    if (!CrossLine::CollisionBeBoxAPlane(box, plane))
    {
        return 0;
    }

    Contact* Coinfo = data->CollisionInfo;
    collision_flag count = 0;
    for (collision_flag i = 0; i < 8; i++) {


        Vector3D cubeV(cubeface[i][0], cubeface[i][1], cubeface[i][2]);
        cubeV.componentProductUpdate(box.halfSize);
        cubeV = box.transform.transform(cubeV);


        real vertexDistance = cubeV * plane.direction;


        if (vertexDistance <= plane.offset)
        {
            Coinfo->contactPoint = plane.direction;
            Coinfo->contactPoint *= (vertexDistance-plane.offset);
            Coinfo->contactPoint += cubeV;
            Coinfo->contactNormal = plane.direction;
            Coinfo->penetration = plane.offset - vertexDistance;


            Coinfo->setBodyData(box.body, NULL,
                data->frictionForce, data->restitution);


            Coinfo++;
            count++;
            if (count == (collision_flag)data->FlagOfcon) return count;
        }
    }

    data->addContacts(count);
    return count;
}

inline collision_flag physics::CoDet::precaBallAHPlane(const ColClineBox & box, const ColClinePlane & plane)
{
  Contact* contact = new Contact;
  collision_flag result = 0;
  for (collision_flag i = 0; i < 4; i++)
  {
    Matrix4x4 temp = box.getTransform();
    Vector3D axis = temp.getAxisVector(i);
    Vector3D temp2 = plane.direction;
    contact->contactNormal = temp2 - axis;
  }
  contact->body[0]->calculateDerivedData();
  result = (collision_flag)contact->restitution;
  return result;
}

inline void physics::CollisionInfo::addContacts(collision_flag count)
{
  FlagOfcon -= count;
  currentcount += count;

  CollisionInfo += count;
}

inline CollisionInfo * physics::CoDet::precaBallABall(const ColClineSphere & fisrt, const ColClineSphere & sec)
{
  CollisionInfo *flag = new CollisionInfo;
  flag->FlagOfcon = 0;
  if (fisrt.getCool(1).Length() > sec.getCool(2).Length())
  {
    flag->addContacts(1);
    flag->FlagOfcon = (2);
  }
  if (fisrt.getCool(2).Length() > sec.getCool(3).Length())
  {
    flag->addContacts(2);
    flag->FlagOfcon = (3);
  }
  if (fisrt.getCool(3).Length() > sec.getCool(1).Length())
  {
    flag->addContacts(3);
    flag->FlagOfcon = (1);
  }
  return flag;

}
