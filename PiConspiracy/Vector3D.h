//
//  Vector3D.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

#ifndef VECTOR3_H
#define VECTOR3_H
typedef float real;
class Vector3D
{
public:
	float x, y, z, w;
	
	Vector3D();
    Vector3D(vec3 &other) { x = other.x; y = other.y; z = other.z; };
	Vector3D(const Vector3D& vec, const float& _w);
	Vector3D(const float& _x, const float& _y, const float& _z);
	Vector3D(const float& _x, const float& _y, const float& _z, const float& _w);
	Vector3D(const Vector3D & rhs);
	Vector3D& operator= (const Vector3D& rhs);
	~Vector3D();


  //operator vec3() const { return vec3(x, y, z);}

  vec3 toVec3() { 
    return vec3(round(x * 1000.0f) / 1000.0f,
      y = round(y * 1000.0f) / 1000.0f,
      z = round(z * 1000.0f) / 1000.0f);
    /*return vec3(x,
      y ,
      z);*/
  }


	void Set(float _x, float _y, float _z);
	void Set(float _x, float _y, float _z, float _w);

	operator float*();
	bool operator== (const  Vector3D& other) const;
	bool operator!= (const  Vector3D& other) const;
	Vector3D operator+ (const Vector3D& other) const;
	Vector3D& operator+= (const Vector3D& other);
	Vector3D operator- (const Vector3D& other) const;
	Vector3D& operator-= (const Vector3D& other);
	Vector3D operator* (const float scalar) const;
	Vector3D operator/ (const float divisor) const;
	float operator[](const int idx) const;
	float& operator[](const int idx);


	void Div(const float divisor);
	void Print() const;
	void FillValues(float&, float&, float&);

	float AngleRadians() const;
	float AngleDegrees() const;

	void Zero();
	void Negate();
	float SquareLength() const;
	float Length() const;
	void Normalize();
	bool IsVectorZero(float error = EPSILON) const;

	static float SquareDistance(const Vector3D& vector0, const Vector3D& vector1);
	static float Distance(const Vector3D& vector0, const Vector3D& vector1);
	static Vector3D Normalize(const Vector3D& vector);
	static float Dot(const Vector3D& vector0, const Vector3D& vector1);
	static Vector3D Cross(const Vector3D& vector0, const Vector3D& vector1);
	static Vector3D VectorFromAngleDegrees(float degrees);
	static Vector3D AngleRadians(float radians, Vector3D& axis);
	static Vector3D AngleDegrees(float degrees, Vector3D& axis);


  // physics

  const static Vector3D GRAVITY;
  const static Vector3D HIGH_GRAVITY;
  const static Vector3D UP;
  const static Vector3D RIGHT;
  const static Vector3D OUT_OF_SCREEN;
  const static Vector3D X;
  const static Vector3D Y;
  const static Vector3D Z;

  void operator*=(const real value);

  /**
   * Calculates and returns a component-wise product of this
   * vector with the given vector.
   */
  Vector3D componentProduct(const Vector3D &vector) const;

  void componentProductUpdate(const Vector3D &vector);

  Vector3D vectorProduct(const Vector3D &vector) const;

  void operator %=(const Vector3D &vector);

  /**
   * Calculates and returns the vector product of this vector
   * with the given vector.
   */
  Vector3D operator%(const Vector3D &vector) const;

  real scalarProduct(const Vector3D &vector) const;

  real operator *(const Vector3D &vector) const;

  void addScaledVector(const Vector3D& vector, real scale);

  /** Limits the size of the vector to the given maximum. */
  void trim(real size);

  /** Turns a non-zero vector into a vector of unit length. */
  void normalise();

  /** Returns the normalised version of a vector. */
  Vector3D unit() const;

  bool operator<(const Vector3D& other) const;

  bool operator>(const Vector3D& other) const;

  bool operator<=(const Vector3D& other) const;

  bool operator>=(const Vector3D& other) const;

};

Vector3D operator*(const float scalar, const Vector3D& other);
std::ostream& operator<<(std::ostream& out, const Vector3D& vec);

static const Vector3D XAXIS = Vector3D(1, 0, 0, 0);
static const Vector3D YAXIS = Vector3D(0, 1, 0, 0);
static const Vector3D ZAXIS = Vector3D(0, 0, 1, 0);

void Vector3DTests();

#endif