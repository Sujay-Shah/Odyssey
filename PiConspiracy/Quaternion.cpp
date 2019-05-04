//
//  Quaternion.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"

void Quaternion::normalise()
{
  real d = r * r + i * i + j * j + k * k;

  // Check for zero length quaternion, and use the no-rotation
  // quaternion in that case.
  if (d < real_epsilon) {
    r = 1;
    return;
  }

  d = ((real)1.0) / real_sqrt(d);
  r *= d;
  i *= d;
  j *= d;
  k *= d;
}


void Quaternion::operator *=(const Quaternion &multiplier)
{
  Quaternion q = *this;
  r = q.r*multiplier.r - q.i*multiplier.i -
    q.j*multiplier.j - q.k*multiplier.k;
  i = q.r*multiplier.i + q.i*multiplier.r +
    q.j*multiplier.k - q.k*multiplier.j;
  j = q.r*multiplier.j + q.j*multiplier.r +
    q.k*multiplier.i - q.i*multiplier.k;
  k = q.r*multiplier.k + q.k*multiplier.r +
    q.i*multiplier.j - q.j*multiplier.i;
}


void Quaternion::addScaledVector(const Vector3D& vector, real scale)
{
  Quaternion q(0,
    vector.x * scale,
    vector.y * scale,
    vector.z * scale);
  q *= *this;
  r += q.r * ((real)0.5);
  i += q.i * ((real)0.5);
  j += q.j * ((real)0.5);
  k += q.k * ((real)0.5);
}

void Quaternion::rotateByVector(const Vector3D& vector)
{
  Quaternion q(0, vector.x, vector.y, vector.z);
  (*this) *= q;
}

Quaternion Quaternion::DegreeToQuaternion(Vector3D angle)
{
  float x = sin(angle.y / 2.f * DEG_TO_RAD) * sin(angle.z / 2.f * DEG_TO_RAD) * cos(angle.x / 2.f * DEG_TO_RAD) + cos(angle.y / 2.f * DEG_TO_RAD)* cos(angle.z / 2.f * DEG_TO_RAD)* sin(angle.x / 2.f * DEG_TO_RAD);
  float y = sin(angle.y / 2.f * DEG_TO_RAD)* cos(angle.z / 2.f * DEG_TO_RAD)* cos(angle.x / 2.f * DEG_TO_RAD) + cos(angle.y / 2.f * DEG_TO_RAD)* sin(angle.z / 2.f * DEG_TO_RAD)* sin(angle.x / 2.f * DEG_TO_RAD);
  float z = cos(angle.y / 2.f * DEG_TO_RAD)* sin(angle.z / 2.f * DEG_TO_RAD)* cos(angle.x / 2.f * DEG_TO_RAD) - sin(angle.y / 2.f * DEG_TO_RAD)* cos(angle.z / 2.f * DEG_TO_RAD)* sin(angle.x / 2.f * DEG_TO_RAD);
  float w = cos(angle.y / 2.f * DEG_TO_RAD)* cos(angle.z / 2.f * DEG_TO_RAD)* cos(angle.x / 2.f * DEG_TO_RAD) - sin(angle.y / 2.f * DEG_TO_RAD)* sin(angle.z / 2.f * DEG_TO_RAD)* sin(angle.x / 2.f * DEG_TO_RAD);

  return Quaternion(w, x, y, z); //r(1), i(0), j(0), k(0) 
}

Vector3D Quaternion::QuaternionToDegree(Quaternion q)
{
  Vector3D result;
  float ysqr = q.j * q.j;
  float t0 = -2.0f * (ysqr + q.k * q.k) + 1.0f;
  float t1 = +2.0f * (q.i * q.j - q.r * q.k);
  float t2 = -2.0f * (q.i * q.k + q.r * q.j);
  float t3 = +2.0f * (q.j * q.k - q.r * q.i);
  float t4 = -2.0f * (q.i * q.i + ysqr) + 1.0f;

  t2 = t2 > 1.0f ? 1.0f : t2;
  t2 = t2 < -1.0f ? -1.0f : t2;

  result.y = std::asin(t2) * RAD_TO_DEG;
  result.x = std::atan2(t3, t4) * RAD_TO_DEG;
  result.z = std::atan2(t1, t0) * RAD_TO_DEG;
  return result;
}