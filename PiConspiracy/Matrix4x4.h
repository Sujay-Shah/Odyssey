//
//  Matrix4x4.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

//#pragma once

#ifndef MATRIX4X4_H
#define MATRIX4X4_H

class Matrix4x4
{
public:
	float m_matrix[4][4];

public:
	Matrix4x4();
	Matrix4x4(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4);
	//Matrix4x4(const D3DXMATRIX &D3DMatrix);
	Matrix4x4(const Matrix4x4 &other); // Copy Ctor

	Matrix4x4& operator= (const Matrix4x4 &other); // Assignment Operator
	~Matrix4x4();
  mat4 toMat4() const;
	void Print() const;

  //physics

  void physicsConstructor();
  void setDiagonal(real a, real b, real c);
  Vector3D transform(const Vector3D &vector) const;
  void setInverse(const Matrix4x4 &m);
  Matrix4x4 inverse() const;
  void invert();
  Vector3D transformDirection(const Vector3D &vector) const;
  Vector3D transformInverseDirection(const Vector3D &vector) const;
  Vector3D transformInverse(const Vector3D &vector) const;
  Vector3D getAxisVector(int i) const;
  void setOrientationAndPos(const Quaternion &q, const Vector3D &pos);
  void fillGLArray(float array[16]) const;
  real getDeterminant() const;
  Matrix4x4 Phy_multiply_Matrix(const Matrix4x4 &o) const;
  Vector3D Phy_multiply_Vector3(const Vector3D &vector) const;
  //float* physicsMatrix();

	operator float*();
	bool operator== (const  Matrix4x4& other) const;
	bool operator!= (const  Matrix4x4& other) const;
	Matrix4x4 operator+ (const Matrix4x4& other) const;
	Matrix4x4 operator- (const Matrix4x4& other) const;
	Matrix4x4 operator* (const Matrix4x4& other) const;
	Matrix4x4 operator* (const float scalar) const;
	Matrix4x4 operator/ (const float divisor) const;
	
	Vector3D operator* (const Vector3D& other) const;
};



#endif