//
//  Quaternion.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#ifndef Quaternion_H
#define Quaternion_H

class Quaternion
{
public:
  union {
    struct {
      real r;
      real i;
      real j;
      real k;
    };
    real data[4];
  };
  Quaternion() : r(1), i(0), j(0), k(0) {}
  Quaternion(const glm::quat &other)
  {
    i = other.x;
    j = other.y;
    k = other.z;
    r = other.w;
  }
  Quaternion(const real r, const real i, const real j, const real k)
    : r(r), i(i), j(j), k(k) {}

  glm::quat toQuat()
  {
    glm::quat result;
    result.x = i;
    result.y = j;
    result.z = k;
    result.w = r;
    return result;
  }

  void normalise();


  void operator *=(const Quaternion &multiplier);


  void addScaledVector(const Vector3D& vector, real scale);

  void rotateByVector(const Vector3D& vector);

  Quaternion DegreeToQuaternion(Vector3D angle);

  Vector3D QuaternionToDegree(Quaternion q);


};
#endif