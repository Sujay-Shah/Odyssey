//
//  Matrix3x3.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
//#pragma once

#ifndef MATRIX3X3_H
#define MATRIX3X3_H

class Quaternion;
class Matrix3x3
{
public:
	float m_matrix[3][3];

public:
	Matrix3x3();
  Matrix3x3(mat3 &other);
	Matrix3x3(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
  Matrix3x3(const Vector3D &compOne, const Vector3D &compTwo, const Vector3D &compThree);
	Matrix3x3(const Matrix3x3 &other); // Copy Ctor
	Matrix3x3& operator= (const Matrix3x3 &other); // Assignment Operator
	~Matrix3x3();


	void Print() const;

  //physics
  void setDiagonal(real a, real b, real c);
  //Sets the value of the matrix from inertia tensor values.
  void setInertiaTensorCoeffs(real ix, real iy, real iz, real ixy = 0, real ixz = 0, real iyz = 0);
  /**
    * Sets the value of the matrix as an inertia tensor of
    * a rectangular block aligned with the body's coordinate
    * system with the given axis half-sizes and mass.
    */
  void setBlockInertiaTensor(const Vector3D &halfSizes, real mass);
  /**
    * Sets the matrix to be a skew symmetric matrix based on
    * the given vector. The skew symmetric matrix is the equivalent
    * of the vector product. So if a,b are vectors. a x b = A_s b
    * where A_s is the skew symmetric form of a.
    */
  void Symmetric(const Vector3D vector);
  /**
    * Sets the matrix values from the given three vector components.
    * These are arranged as the three columns of the vector.
    */
  void Setting(const Vector3D &compOne, const Vector3D &compTwo, const Vector3D &compThree);
  Vector3D transform(const Vector3D &vector);
  Vector3D transformTranspose(const Vector3D &vector) const;
  Vector3D getRowVector(int i) const;
  Vector3D getAxisVector(int i) const;
  void setInverse(const Matrix3x3 &m);
  Matrix3x3 inverse() const;
  void invert();
  void setTranspose(const Matrix3x3 &m);
  Matrix3x3 transpose() const;
  void setOrientation(const Quaternion &q);
  static Matrix3x3 linearInterpolate(const Matrix3x3& a, const Matrix3x3& b, real prop);
  

	bool operator== (const  Matrix3x3& other);
	bool operator!= (const  Matrix3x3& other);
	Matrix3x3 operator+ (const Matrix3x3& other);
	Matrix3x3 operator- (const Matrix3x3& other);
	Matrix3x3 operator* (const Matrix3x3& other);
	Matrix3x3 operator* (const float scalar);
	Matrix3x3 operator/ (const float divisor);

	Vector3D operator* (const Vector3D& other);
  void operator*=(const Matrix3x3 &o);
  void operator*=(const real scalar);
  void operator+=(const Matrix3x3 &o);

};



#endif