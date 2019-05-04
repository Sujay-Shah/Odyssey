//
//  CollisionManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "CollisionManager.h"


inline void CollisionInfo::addContacts(collision_flag count)
{
	FlagOfcon -= count;
	currentcount += count;

	CollisionInfo += count;
}
 
static inline real ProjectTransform(const BoxCollider &box,	const Vector3D &cool)
{
	return
		box.halfSize.y * real_abs(cool * box.GetCool(1)) +
		box.halfSize.x * real_abs(cool * box.GetCool(0)) +
		box.halfSize.z * real_abs(cool * box.GetCool(2));
}

/**
 * This function checks if the sec boxes overlap given axis.
 */
static inline bool overlapOnAxis(const BoxCollider &first, const BoxCollider &sec, const Vector3D &cool, const Vector3D &distance)
{

	return (real_abs(distance * cool) < ProjectTransform(first, cool) + ProjectTransform(sec, cool));
}

#define FACECHECKING(cool) overlapOnAxis(first, sec, (cool), toCentre)

bool CrossLine::CollisionBeBallAHPlane(const SphereCollider & sphere, const PlaneCollider & plane)
{
	// distance from the origin
	real ballDistance =	plane.direction * sphere.GetCool(3) - sphere.GetRadius();

	// intersection
	return ballDistance <= plane.offset;
}

bool CrossLine::CollisionBeBallABall(const SphereCollider & first, const SphereCollider & sec)
{
	// vector between the objects
	Vector3D midline = first.GetCool(3) - sec.GetCool(3);

	// enough large ?.
	return midline.SquareLength() <	(first.GetRadius() + sec.GetRadius())*(first.GetRadius() + sec.GetRadius());
}

bool CrossLine::CollisionBeBoxABox(const BoxCollider & first, const BoxCollider & sec)
{
	// Find the vector between the sec centres
	Vector3D toCentre = sec.GetCool(3) - first.GetCool(3);

	return (

		FACECHECKING(first.GetCool(0)) &&
		FACECHECKING(first.GetCool(1)) &&
		FACECHECKING(first.GetCool(2)) &&


		FACECHECKING(sec.GetCool(0)) &&
		FACECHECKING(sec.GetCool(1)) &&
		FACECHECKING(sec.GetCool(2)) &&


		FACECHECKING(first.GetCool(0) % sec.GetCool(0)) &&
		FACECHECKING(first.GetCool(0) % sec.GetCool(1)) &&
		FACECHECKING(first.GetCool(0) % sec.GetCool(2)) &&
		FACECHECKING(first.GetCool(1) % sec.GetCool(0)) &&
		FACECHECKING(first.GetCool(1) % sec.GetCool(1)) &&
		FACECHECKING(first.GetCool(1) % sec.GetCool(2)) &&
		FACECHECKING(first.GetCool(2) % sec.GetCool(0)) &&
		FACECHECKING(first.GetCool(2) % sec.GetCool(1)) &&
		FACECHECKING(first.GetCool(2) % sec.GetCool(2))
		);
}
#undef FACECHECKING

bool CrossLine::CollisionBeBoxAPlane(const BoxCollider & box, const PlaneCollider & plane)
{
	float temp = ProjectTransform(box, plane.direction);

	// how far the box is from the origin
  float result = plane.direction * box.GetCool(3) - temp;

	// Check for the intersection
	return result <= plane.offset;
}

//-------------------------------------------------------------------------------------------------------------------------------------//

//Checks double sides of the plane
collision_flag ColCheckSphereToPlane(const BaseCollider &a_collider1, const BaseCollider &a_collider2, CollisionInfo * data)
{
	const SphereCollider sphere_ = static_cast<const SphereCollider&>(a_collider1);
	const PlaneCollider plane_ = static_cast<const PlaneCollider&>(a_collider2);
	if (data->FlagOfcon <= 0) return 0;


	Vector3D spherelocation = sphere_.GetCool(3);


	real dis = plane_.direction * spherelocation - plane_.offset;


	if (dis*dis > sphere_.GetRadius()*sphere_.GetRadius())
	{
		return 0;
	}


	Vector3D plandir = plane_.direction;
	real temp = -dis;
	if (dis < 0)
	{
		plandir *= -1;
		temp = -temp;
	}
	temp += sphere_.GetRadius();


	Contact* colInfo = data->CollisionInfo;

	colInfo->theCollisionPoint = spherelocation - plane_.direction * dis;
	colInfo->ConNormalVector = plandir;
	colInfo->penetration = temp;
	colInfo->setBodyData(sphere_.GetRBOwner(), plane_.GetRBOwner(), data->frictionForce, data->restitution);

	data->addContacts(1);
	return 1;
}

static inline real ProjectionOnAxis(const BoxCollider &first, const BoxCollider &sec, const Vector3D &axis, const Vector3D &toCentre)
{
	// overlap
	return ProjectTransform(first, axis) + ProjectTransform(sec, axis) - real_abs(toCentre * axis);
}

static inline Vector3D CCollisionPoint(const Vector3D &pfirst, const Vector3D &dfirst, real firstSize, const Vector3D &psec, const Vector3D &dsec, real secSize, bool usefirst)
{
	Vector3D distance, collision_first, collision_sec;
	real collision_first_dis, collision_sec_dis, twoPointDis, temp1, temp2;
	real demoniator, Mfirst, Msecond;

	temp1 = dfirst.SquareLength();
	temp2 = dsec.SquareLength();
	twoPointDis = dsec * dfirst;

	distance = pfirst - psec;
	collision_first_dis = dfirst * distance;
	collision_sec_dis = dsec * distance;

	demoniator = temp1 * temp2 - twoPointDis * twoPointDis;

	// 0 denominator 
	if (real_abs(demoniator) < 0.0001f) {
		return usefirst ? pfirst : psec;
	}

	Mfirst = (twoPointDis * collision_sec_dis - temp2 * collision_first_dis) / demoniator;
	Msecond = (temp1 * collision_sec_dis - twoPointDis * collision_first_dis) / demoniator;

	if (Mfirst > firstSize ||Mfirst < -firstSize ||Msecond > secSize ||Msecond < -secSize)
	{
		return usefirst ? pfirst : psec;
	}
	else
	{
		collision_first = pfirst + dfirst * Mfirst;
		collision_sec = psec + dsec * Msecond;

		return collision_first * 0.5f + collision_sec * 0.5f;
	}
}


void PointsInTheBox(const BoxCollider &first, const BoxCollider &sec, const Vector3D &distance, CollisionInfo *information, unsigned flag, real axis)
{
	Contact* infor = information->CollisionInfo;

	Vector3D normalize = first.GetCool(flag);
	if (first.GetCool(flag) * distance > 0)
	{
		normalize = normalize * -1.0f;
	}

	Vector3D point_ = sec.halfSize;
  if (sec.GetCool(1) * normalize < 0)
    point_.y = -point_.y;
  if (sec.GetCool(2) * normalize < 0)
    point_.z = -point_.z;
	if (sec.GetCool(0) * normalize < 0) 
    point_.x = -point_.x;
	

	infor->ConNormalVector = normalize;
	infor->penetration = axis;
	infor->theCollisionPoint = sec.GetTransform().Phy_multiply_Vector3(point_);
	/*contact->setBodyData(one.body, two.body,
	  data->frictionForce, data->restitution);*/
	
	infor->setBodyData(first.GetRBOwner(), sec.GetRBOwner(), information->frictionForce, information->restitution);
}

static inline bool helperfunction(const BoxCollider &first, const BoxCollider &sec, Vector3D cool, const Vector3D& distance, unsigned flag,

	real& Penetration,
	unsigned &smallestValue)
{
  float esp = 0.0001f;
	// normalized axis
	if (cool.SquareLength() < esp)
    return true;
	cool.normalise();

	real temp = ProjectionOnAxis(first, sec, cool, distance);

	if (temp < 0) 
    return false;
	if (temp < Penetration) {
		Penetration = temp;
		smallestValue = flag;
	}
	return true;
}

#define IFHASLAP(cool, index) \
    if (!helperfunction(first, two, (cool), distance, (index), Penetration, smallestValue)) return 0;
collision_flag ColCheckBoxToBox(const BaseCollider & a_collider1, const BaseCollider & a_collider2, CollisionInfo * information)
{
	const BoxCollider first = static_cast<const BoxCollider&>(a_collider2);
	const BoxCollider two = static_cast<const BoxCollider&>(a_collider1);

	// Find the vector between the two centres
	Vector3D distance = two.GetCool(3) - first.GetCool(3);

	real Penetration = REAL_MAX;
	unsigned smallestValue = COLLISIONFLAF_MAX;

	IFHASLAP(first.GetCool(0), 0);
	IFHASLAP(first.GetCool(1), 1);
	IFHASLAP(first.GetCool(2), 2);

	IFHASLAP(two.GetCool(0), 3);
	IFHASLAP(two.GetCool(1), 4);
	IFHASLAP(two.GetCool(2), 5);

	//  best axis
	unsigned BestMate = smallestValue;

	IFHASLAP(first.GetCool(0) % two.GetCool(0), 6);
	IFHASLAP(first.GetCool(0) % two.GetCool(1), 7);
	IFHASLAP(first.GetCool(0) % two.GetCool(2), 8);
	IFHASLAP(first.GetCool(1) % two.GetCool(0), 9);
	IFHASLAP(first.GetCool(1) % two.GetCool(1), 10);
	IFHASLAP(first.GetCool(1) % two.GetCool(2), 11);
	IFHASLAP(first.GetCool(2) % two.GetCool(0), 12);
	IFHASLAP(first.GetCool(2) % two.GetCool(1), 13);
	IFHASLAP(first.GetCool(2) % two.GetCool(2), 14);

	// error
	assert(smallestValue != COLLISIONFLAF_MAX);

	if (smallestValue < 3)
	{
		PointsInTheBox(first, two, distance, information, smallestValue, Penetration);
		information->addContacts(1);
		return true;
	}
	else if (smallestValue < 6)
	{
		PointsInTheBox(two, first, distance*-1.0f, information, smallestValue - 3, Penetration);
		information->addContacts(1);
		return true;
	}
	else
	{
		smallestValue -= 6;
		unsigned FirstCoolFlag = smallestValue / 3;
		unsigned SecCoolFlag = smallestValue % 3;
		Vector3D FirstCool = first.GetCool(FirstCoolFlag);
		Vector3D SecCool = two.GetCool(SecCoolFlag);
		Vector3D cool_ = FirstCool % SecCool;
		cool_.normalise();

		// point from box one to other.
		if (cool_ * distance > 0)
      cool_ = cool_ * -1.0f;
    Vector3D firstEdge, SecEdge;
	  firstEdge = first.halfSize;
	  SecEdge = two.halfSize;
		for (unsigned k = 0; k < 3; k++)
		{
			if (k == FirstCoolFlag) 
        firstEdge[k] = 0;
			else if (first.GetCool(k) * cool_ > 0)
        firstEdge[k] = -firstEdge[k];

			if (k == SecCoolFlag)
        SecEdge[k] = 0;
			else if (two.GetCool(k) * cool_ < 0) 
        SecEdge[k] = -SecEdge[k];
		}

		firstEdge = first.GetTransform().Phy_multiply_Vector3(firstEdge);
		SecEdge = two.GetTransform().Phy_multiply_Vector3(SecEdge);

		//  point and a direction for the colliding edges.
		// point of closest approach of the two line-segments.
		Vector3D finalV = CCollisionPoint(
			firstEdge, FirstCool, first.halfSize[FirstCoolFlag],SecEdge, SecCool, two.halfSize[SecCoolFlag],BestMate > 2);

		// We can fill the contact.
		Contact* info = information->CollisionInfo;
    info->ConNormalVector = cool_;
		info->penetration = Penetration;
		info->theCollisionPoint = finalV;

		info->setBodyData(first.GetRBOwner(), two.GetRBOwner(), information->frictionForce, information->restitution);

		information->addContacts(1);
		return true;
	}
	return 0;
}

collision_flag ColCheckBallToHalfPlane(const BaseCollider &a_collider1, const BaseCollider &a_collider2, CollisionInfo * data)
{
	const SphereCollider sphere_ = static_cast<const SphereCollider&>(a_collider1);
	const PlaneCollider plane_ = static_cast<const PlaneCollider&>(a_collider2);

	if (data->FlagOfcon <= 0)
    return 0;

	Vector3D temp = sphere_.GetCool(3);


	real theDis =plane_.direction * temp -	sphere_.GetRadius() - plane_.offset;

	if (theDis >= 0) return 0;


	Contact* colInfo = data->CollisionInfo;

	colInfo->penetration = -theDis;
	colInfo->ConNormalVector = plane_.direction;
	colInfo->theCollisionPoint = temp - plane_.direction * (theDis + sphere_.GetRadius());
	colInfo->setBodyData(sphere_.GetRBOwner(), plane_.GetRBOwner(), data->frictionForce, data->restitution);

	data->addContacts(1);
	return 1;
}

collision_flag ColCheckHalfPlaneToBall(const BaseCollider &a_collider1, const BaseCollider &a_collider2, CollisionInfo * data)
{
	return ColCheckBallToHalfPlane(a_collider2, a_collider1, data);
}
collision_flag ColCheckBallToBall(const BaseCollider & a_first, const BaseCollider & a_second, CollisionInfo * data)
{
	const SphereCollider first = static_cast<const SphereCollider&>(a_first);
	const SphereCollider sec = static_cast<const SphereCollider&>(a_second);

	//have contacts
	if (data->FlagOfcon <= 0) return 0;

	Vector3D positionfirst = first.GetCool(3);
	Vector3D positionsec = sec.GetCool(3);

	// Find the vector between the objects
	Vector3D PointMid = positionfirst - positionsec;
	real TheLength = PointMid.Length();

	// See if it is large enough.
	if (TheLength <= 0.0f || TheLength >= first.GetRadius() + sec.GetRadius())
	{
		return 0;
	}

	Vector3D theNormalValue = PointMid * (((float)1.0f) / TheLength);

	Contact* colInfo = data->CollisionInfo;
	colInfo->ConNormalVector = theNormalValue;
	colInfo->theCollisionPoint = positionfirst + PointMid * (float)0.5f;
	colInfo->penetration = (first.GetRadius() + sec.GetRadius() - TheLength);

	colInfo->setBodyData(first.GetRBOwner(), sec.GetRBOwner(), data->frictionForce, data->restitution);

	data->addContacts(1);
	return 1;
}

//TODO: double check this
collision_flag ColCheckBoxToHalfPlane(const BaseCollider & a_collider1, const BaseCollider & a_collider2, CollisionInfo * data)
{
	const BoxCollider box = static_cast<const BoxCollider&>(a_collider1);
	const PlaneCollider plane = static_cast<const PlaneCollider&>(a_collider2);

	if (data->FlagOfcon <= 0) 
    return 0;


	if (!CrossLine::CollisionBeBoxAPlane(box, plane))
	{
		return 0;
	}

	Contact* Coinfo = data->CollisionInfo;
	collision_flag count = 0;
	for (collision_flag i = 0; i < 8; i++) {


		Vector3D cubeV(cubeface[i][0], cubeface[i][1], cubeface[i][2]);
		cubeV.componentProductUpdate(box.halfSize);
		cubeV = box.GetTransform().transform(cubeV);


		real theDisBetweenObjs = cubeV * plane.direction;


		if (theDisBetweenObjs <= plane.offset)
		{
			Coinfo->theCollisionPoint = plane.direction;
			Coinfo->theCollisionPoint *= (theDisBetweenObjs - plane.offset);
			Coinfo->theCollisionPoint += cubeV;
			Coinfo->ConNormalVector = plane.direction;
			Coinfo->penetration = plane.offset - theDisBetweenObjs;


			Coinfo->setBodyData(box.GetRBOwner(), plane.GetRBOwner(), data->frictionForce, data->restitution);


			Coinfo++;
			count++;
			if (count == (collision_flag)data->FlagOfcon) return count;
		}
	}

	data->addContacts(count);
	return count;
}

collision_flag ColCheckHalfPlaneToBox(const BaseCollider & a_collider1, const BaseCollider & a_collider2, CollisionInfo * data)
{
	return ColCheckBoxToHalfPlane(a_collider2, a_collider1, data);
}

collision_flag ColCheckBallToBox(const BaseCollider & a_collider1, const BaseCollider & a_collider2, CollisionInfo * data)
{
	const SphereCollider sphere = static_cast<const SphereCollider&>(a_collider1);
	const BoxCollider box = static_cast<const BoxCollider&>(a_collider2);

	Vector3D theCool = sphere.GetCool(3);
	Vector3D relCentre = box.GetTransform().transformInverse(theCool);


	if (real_abs(relCentre.x) - sphere.GetRadius() > box.halfSize.x ||
		real_abs(relCentre.y) - sphere.GetRadius() > box.halfSize.y ||
		real_abs(relCentre.z) - sphere.GetRadius() > box.halfSize.z)
	{
		return 0;
	}

	Vector3D closestPt(0, 0, 0);
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
	if (compareDistance > sphere.GetRadius() * sphere.GetRadius()) return 0;

	Vector3D closestPsecrld = box.GetTransform().transform(closestPt);

	Contact* colInfo = data->CollisionInfo;
	colInfo->ConNormalVector = (closestPsecrld - theCool);
	colInfo->ConNormalVector.normalise();
	colInfo->theCollisionPoint = closestPsecrld;
	colInfo->penetration = sphere.GetRadius() - real_sqrt(compareDistance);

	colInfo->setBodyData(box.GetRBOwner(), sphere.GetRBOwner(), data->frictionForce, data->restitution);

	data->addContacts(1);
	return 1;
}

collision_flag ColCheckBoxToBall(const BaseCollider & a_collider1, const BaseCollider & a_collider2, CollisionInfo * data)
{
	return ColCheckBallToBox(a_collider2, a_collider1, data);
}

collision_flag CollisionManager::ColCheckBoxToPoint(const BoxCollider & box, const Vector3D & point, CollisionInfo * data)
{
	Vector3D relPt = box.GetTransform().transformInverse(point);

	Vector3D normal;


	real min_depth = box.halfSize.x - real_abs(relPt.x);
	if (min_depth < 0) return 0;
	normal = box.GetCool(0) * (real)((relPt.x < 0) ? -1 : 1);

	real depth = box.halfSize.y - real_abs(relPt.y);
	if (depth < 0) return 0;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = box.GetCool(1) * (real)((relPt.y < 0) ? -1 : 1);
	}

	depth = box.halfSize.z - real_abs(relPt.z);
	if (depth < 0) return 0;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = box.GetCool(2) * (real)((relPt.z < 0) ? -1 : 1);
	}


	Contact* infoCon = data->CollisionInfo;
	infoCon->ConNormalVector = normal;
	infoCon->theCollisionPoint = point;
	infoCon->penetration = min_depth;


	infoCon->setBodyData(box.GetRBOwner(), NULL,
		data->frictionForce, data->restitution);

	data->addContacts(1);
	return 1;
}


CollisionManager::CollisionManager()
{
	CollisionCheckFunctions[EColliderShape::Box][EColliderShape::Box] = ColCheckBoxToBox;
	CollisionCheckFunctions[EColliderShape::Box][EColliderShape::Sphere] = ColCheckBoxToBall;
	CollisionCheckFunctions[EColliderShape::Box][EColliderShape::Plane] = ColCheckBoxToHalfPlane;
	
	CollisionCheckFunctions[EColliderShape::Sphere][EColliderShape::Box] = ColCheckBallToBox;
	CollisionCheckFunctions[EColliderShape::Sphere][EColliderShape::Sphere] = ColCheckBallToBall;
	CollisionCheckFunctions[EColliderShape::Sphere][EColliderShape::Plane] = ColCheckBallToHalfPlane;

	CollisionCheckFunctions[EColliderShape::Plane][EColliderShape::Box] = ColCheckHalfPlaneToBox;
	CollisionCheckFunctions[EColliderShape::Plane][EColliderShape::Sphere] = ColCheckHalfPlaneToBall;
}

CollisionManager::~CollisionManager()
{}

collision_flag CollisionManager::CheckCollisionGenerateContact(const BaseCollider &a_collider1, const BaseCollider &a_collider2, CollisionInfo *info)
{
	return CollisionCheckFunctions[a_collider1.GetColliderShape()][a_collider2.GetColliderShape()](a_collider1, a_collider2, info);
}