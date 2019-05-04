/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include "PiEnginePreCompiled.h"

Matrix4x4::Matrix4x4() {
	m_matrix[0][0] = 0;
	m_matrix[0][1] = 0;
	m_matrix[0][2] = 0;
	m_matrix[0][3] = 0;

	m_matrix[1][0] = 0;
	m_matrix[1][1] = 0;
	m_matrix[1][2] = 0;
	m_matrix[1][3] = 0;

	m_matrix[2][0] = 0;
	m_matrix[2][1] = 0;
	m_matrix[2][2] = 0;
	m_matrix[2][3] = 0;

	m_matrix[3][0] = 0;
	m_matrix[3][1] = 0;
	m_matrix[3][2] = 0;
	m_matrix[3][3] = 0;
}

Matrix4x4::Matrix4x4(
	float x1, float y1, float z1, float w1, 
	float x2, float y2, float z2, float w2, 
	float x3, float y3, float z3, float w3, 
	float x4, float y4, float z4, float w4)
{
	m_matrix[0][0] = x1;
	m_matrix[0][1] = y1;
	m_matrix[0][2] = z1;
	m_matrix[0][3] = w1;

	m_matrix[1][0] = x2;
	m_matrix[1][1] = y2;
	m_matrix[1][2] = z2;
	m_matrix[1][3] = w2;

	m_matrix[2][0] = x3;
	m_matrix[2][1] = y3;
	m_matrix[2][2] = z3;
	m_matrix[2][3] = w3;

	m_matrix[3][0] = x4;
	m_matrix[3][1] = y4;
	m_matrix[3][2] = z4;
	m_matrix[3][3] = w4;
}

mat4 Matrix4x4::toMat4() const
{
  mat4 result;
 
  result[0][0] = m_matrix[0][0];
  result[0][1] = m_matrix[0][1];
  result[0][2] = m_matrix[0][2];
  result[0][3] = m_matrix[0][3];

  result[1][0] = m_matrix[1][0];
  result[1][1] = m_matrix[1][1];
  result[1][2] = m_matrix[1][2];
  result[1][3] = m_matrix[1][3];

  result[2][0] = m_matrix[2][0];
  result[2][1] = m_matrix[2][1];
  result[2][2] = m_matrix[2][2];
  result[2][3] = m_matrix[2][3];

  result[3][0] = m_matrix[3][0];
  result[3][1] = m_matrix[3][1];
  result[3][2] = m_matrix[3][2];
  result[3][3] = m_matrix[3][3];
  return result;
}

//Matrix4x4::Matrix4x4(const D3DXMATRIX & D3DMatrix)
//{
//	m_matrix[0][0] = D3DMatrix._11;
//	m_matrix[0][1] = D3DMatrix._12;
//	m_matrix[0][2] = D3DMatrix._13;
//	m_matrix[0][3] = D3DMatrix._14;
//
//	m_matrix[1][0] = D3DMatrix._21;
//	m_matrix[1][1] = D3DMatrix._22;
//	m_matrix[1][2] = D3DMatrix._23;
//	m_matrix[1][3] = D3DMatrix._24;
//
//	m_matrix[2][0] = D3DMatrix._31;
//	m_matrix[2][1] = D3DMatrix._32;
//	m_matrix[2][2] = D3DMatrix._33;
//	m_matrix[2][3] = D3DMatrix._34;
//
//	m_matrix[3][0] = D3DMatrix._41;
//	m_matrix[3][1] = D3DMatrix._42;
//	m_matrix[3][2] = D3DMatrix._43;
//	m_matrix[3][3] = D3DMatrix._44;
//}

Matrix4x4::Matrix4x4(const Matrix4x4 &other)
{
	m_matrix[0][0] = other.m_matrix[0][0];
	m_matrix[0][1] = other.m_matrix[0][1];
	m_matrix[0][2] = other.m_matrix[0][2];
	m_matrix[0][3] = other.m_matrix[0][3];

	m_matrix[1][0] = other.m_matrix[1][0];
	m_matrix[1][1] = other.m_matrix[1][1];
	m_matrix[1][2] = other.m_matrix[1][2];
	m_matrix[1][3] = other.m_matrix[1][3];

	m_matrix[2][0] = other.m_matrix[2][0];
	m_matrix[2][1] = other.m_matrix[2][1];
	m_matrix[2][2] = other.m_matrix[2][2];
	m_matrix[2][3] = other.m_matrix[2][3];

	m_matrix[3][0] = other.m_matrix[3][0];
	m_matrix[3][1] = other.m_matrix[3][1];
	m_matrix[3][2] = other.m_matrix[3][2];
	m_matrix[3][3] = other.m_matrix[3][3];
}


Matrix4x4& Matrix4x4::operator=(const Matrix4x4& other)
{
	m_matrix[0][0] = other.m_matrix[0][0];
	m_matrix[0][1] = other.m_matrix[0][1];
	m_matrix[0][2] = other.m_matrix[0][2];
	m_matrix[0][3] = other.m_matrix[0][3];

	m_matrix[1][0] = other.m_matrix[1][0];
	m_matrix[1][1] = other.m_matrix[1][1];
	m_matrix[1][2] = other.m_matrix[1][2];
	m_matrix[1][3] = other.m_matrix[1][3];

	m_matrix[2][0] = other.m_matrix[2][0];
	m_matrix[2][1] = other.m_matrix[2][1];
	m_matrix[2][2] = other.m_matrix[2][2];
	m_matrix[2][3] = other.m_matrix[2][3];

	m_matrix[3][0] = other.m_matrix[3][0];
	m_matrix[3][1] = other.m_matrix[3][1];
	m_matrix[3][2] = other.m_matrix[3][2];
	m_matrix[3][3] = other.m_matrix[3][3];
	return *this;
}

Matrix4x4::~Matrix4x4() {}





void Matrix4x4::Print() const
{
	std::cout << "[ " << m_matrix[0][0] << ", " << m_matrix[0][1] << ", " << m_matrix[0][2] << ", " << m_matrix[0][3] << "]" << std::endl;
	std::cout << "[ " << m_matrix[1][0] << ", " << m_matrix[1][1] << ", " << m_matrix[1][2] << ", " << m_matrix[1][3] << "]" << std::endl;
	std::cout << "[ " << m_matrix[2][0] << ", " << m_matrix[2][1] << ", " << m_matrix[2][2] << ", " << m_matrix[2][3] << "]" << std::endl;
	std::cout << "[ " << m_matrix[3][0] << ", " << m_matrix[3][1] << ", " << m_matrix[3][2] << ", " << m_matrix[3][3] << "]" << std::endl;
}


#pragma region Static Methods





//physics
Matrix4x4 Matrix4x4::Phy_multiply_Matrix(const Matrix4x4 &o) const
{
  Matrix4x4 result = Matrix4x4(1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 0.f);
  result.m_matrix[0][0] = (o.m_matrix[0][0] * m_matrix[0][0]) + (o.m_matrix[1][0] * m_matrix[0][1]) + (o.m_matrix[2][0] * m_matrix[0][2]);
  result.m_matrix[1][0] = (o.m_matrix[0][0] * m_matrix[1][0]) + (o.m_matrix[1][0] * m_matrix[1][1]) + (o.m_matrix[2][0] * m_matrix[1][2]);
  result.m_matrix[2][0] = (o.m_matrix[0][0] * m_matrix[2][0]) + (o.m_matrix[1][0] * m_matrix[2][1]) + (o.m_matrix[2][0] * m_matrix[2][2]);

  result.m_matrix[0][1] = (o.m_matrix[0][1] * m_matrix[0][0]) + (o.m_matrix[1][1] * m_matrix[0][1]) + (o.m_matrix[2][1] * m_matrix[0][2]);
  result.m_matrix[1][1] = (o.m_matrix[0][1] * m_matrix[1][0]) + (o.m_matrix[1][1] * m_matrix[1][1]) + (o.m_matrix[2][1] * m_matrix[1][2]);
  result.m_matrix[2][1] = (o.m_matrix[0][1] * m_matrix[2][0]) + (o.m_matrix[1][1] * m_matrix[2][1]) + (o.m_matrix[2][1] * m_matrix[2][2]);

  result.m_matrix[0][2] = (o.m_matrix[0][2] * m_matrix[0][0]) + (o.m_matrix[1][2] * m_matrix[0][1]) + (o.m_matrix[2][2] * m_matrix[0][2]);
  result.m_matrix[1][2] = (o.m_matrix[0][2] * m_matrix[1][0]) + (o.m_matrix[1][2] * m_matrix[1][1]) + (o.m_matrix[2][2] * m_matrix[1][2]);
  result.m_matrix[2][2] = (o.m_matrix[0][2] * m_matrix[2][0]) + (o.m_matrix[1][2] * m_matrix[2][1]) + (o.m_matrix[2][2] * m_matrix[2][2]);

  result.m_matrix[0][3] = (o.m_matrix[0][3] * m_matrix[0][0]) + (o.m_matrix[1][3] * m_matrix[0][1]) + (o.m_matrix[2][3] * m_matrix[0][2]) + m_matrix[0][3];
  result.m_matrix[1][3] = (o.m_matrix[0][3] * m_matrix[1][0]) + (o.m_matrix[1][3] * m_matrix[1][1]) + (o.m_matrix[2][3] * m_matrix[1][2]) + m_matrix[1][3];
  result.m_matrix[2][3] = (o.m_matrix[0][3] * m_matrix[2][0]) + (o.m_matrix[1][3] * m_matrix[2][1]) + (o.m_matrix[2][3] * m_matrix[2][2]) + m_matrix[2][3];

  return result;
}
Vector3D Matrix4x4::Phy_multiply_Vector3(const Vector3D &vector) const
{
  return Vector3D(
    vector.x * m_matrix[0][0] +
    vector.y * m_matrix[0][1] +
    vector.z * m_matrix[0][2] + m_matrix[0][3],

    vector.x * m_matrix[1][0] +
    vector.y * m_matrix[1][1] +
    vector.z * m_matrix[1][2] + m_matrix[1][3],

    vector.x * m_matrix[2][0] +
    vector.y * m_matrix[2][1] +
    vector.z * m_matrix[2][2] + m_matrix[2][3]
  );
}

void Matrix4x4::physicsConstructor()
{
  m_matrix[0][1] = m_matrix[0][2] = m_matrix[0][3] = m_matrix[1][0] = m_matrix[1][2] =
  m_matrix[1][3] = m_matrix[2][0] = m_matrix[2][1] = m_matrix[2][3] = 0;
  m_matrix[0][0] = m_matrix[1][1] = m_matrix[2][2] = 1;
}

Vector3D Matrix4x4::transform(const Vector3D &vector) const
{
  return (*this) * vector;
}

real Matrix4x4::getDeterminant() const
{
  return -m_matrix[2][0] * m_matrix[1][1] * m_matrix[0][2] +
    m_matrix[1][0] * m_matrix[2][1] * m_matrix[0][2] +
    m_matrix[2][0] * m_matrix[0][1] * m_matrix[1][2] -
    m_matrix[0][0] * m_matrix[2][1] * m_matrix[1][2] -
    m_matrix[1][0] * m_matrix[0][1] * m_matrix[2][2] +
    m_matrix[0][0] * m_matrix[1][1] * m_matrix[2][2];
}

void Matrix4x4::setDiagonal(real a, real b, real c)
{
  m_matrix[0][0] = a;
  m_matrix[1][1] = b;
  m_matrix[2][2] = c;
}

void Matrix4x4::setInverse(const Matrix4x4 &m)
{
  // Make sure the determinant is non-zero.
  real det = getDeterminant();
  if (det == 0) return;
  det = ((real)1.0) / det;

  m_matrix[0][0] = (-m.m_matrix[2][1] * m.m_matrix[1][2] + m.m_matrix[1][1] * m.m_matrix[2][2])*det;
  m_matrix[1][0] = (m.m_matrix[2][0] * m.m_matrix[1][2] - m.m_matrix[1][0] * m.m_matrix[2][2])*det;
  m_matrix[2][0] = (-m.m_matrix[2][0] * m.m_matrix[1][1] + m.m_matrix[1][0] * m.m_matrix[2][1])*det;

  m_matrix[0][1] = (m.m_matrix[2][1] * m.m_matrix[0][2] - m.m_matrix[0][1] * m.m_matrix[2][2])*det;
  m_matrix[1][1] = (-m.m_matrix[2][0] * m.m_matrix[0][2] + m.m_matrix[0][0] * m.m_matrix[2][2])*det;
  m_matrix[2][1] = (m.m_matrix[2][0] * m.m_matrix[0][1] - m.m_matrix[0][0] * m.m_matrix[2][1])*det;

  m_matrix[0][2] = (-m.m_matrix[1][1] * m.m_matrix[0][2] + m.m_matrix[0][1] * m.m_matrix[1][2])*det;
  m_matrix[1][2] = (+m.m_matrix[1][0] * m.m_matrix[0][2] - m.m_matrix[0][0] * m.m_matrix[1][2])*det;
  m_matrix[2][2] = (-m.m_matrix[1][0] * m.m_matrix[0][1] + m.m_matrix[0][0] * m.m_matrix[1][1])*det;

  m_matrix[0][3] = (m.m_matrix[2][1] * m.m_matrix[1][2] * m.m_matrix[0][3]
    - m.m_matrix[1][1] * m.m_matrix[2][2] * m.m_matrix[0][3]
    - m.m_matrix[2][1] * m.m_matrix[0][2] * m.m_matrix[1][3]
    + m.m_matrix[0][1] * m.m_matrix[2][2] * m.m_matrix[1][3]
    + m.m_matrix[1][1] * m.m_matrix[0][2] * m.m_matrix[2][3]
    - m.m_matrix[0][1] * m.m_matrix[1][2] * m.m_matrix[2][3])*det;
  m_matrix[1][3] = (-m.m_matrix[2][0] * m.m_matrix[1][2] * m.m_matrix[0][3]
    + m.m_matrix[1][0] * m.m_matrix[2][2] * m.m_matrix[0][3]
    + m.m_matrix[2][0] * m.m_matrix[0][2] * m.m_matrix[1][3]
    - m.m_matrix[0][0] * m.m_matrix[2][2] * m.m_matrix[1][3]
    - m.m_matrix[1][0] * m.m_matrix[0][2] * m.m_matrix[2][3]
    + m.m_matrix[0][0] * m.m_matrix[1][2] * m.m_matrix[2][3])*det;
  m_matrix[2][3] = (m.m_matrix[2][0] * m.m_matrix[1][1] * m.m_matrix[0][3]
    - m.m_matrix[1][0] * m.m_matrix[2][1] * m.m_matrix[0][3]
    - m.m_matrix[2][0] * m.m_matrix[0][1] * m.m_matrix[1][3]
    + m.m_matrix[0][0] * m.m_matrix[2][1] * m.m_matrix[1][3]
    + m.m_matrix[1][0] * m.m_matrix[0][1] * m.m_matrix[2][3]
    - m.m_matrix[0][0] * m.m_matrix[1][1] * m.m_matrix[2][3])*det;
}

/*float* Matrix4x4::physicsMatrix()
{
  return m_matrix;
}*/

Matrix4x4 Matrix4x4::inverse() const
{
  Matrix4x4 result = Matrix4x4(1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 0.f);;
  result.setInverse(*this);
  return result;
}

void Matrix4x4::invert()
{
  setInverse(*this);
}

Vector3D Matrix4x4::transformDirection(const Vector3D &vector) const
{
  return Vector3D(
    vector.x * m_matrix[0][0] +
    vector.y * m_matrix[0][1] +
    vector.z * m_matrix[0][2],

    vector.x * m_matrix[1][0] +
    vector.y * m_matrix[1][1] +
    vector.z * m_matrix[1][2],

    vector.x * m_matrix[2][0] +
    vector.y * m_matrix[2][1] +
    vector.z * m_matrix[2][2]
  );
}


Vector3D Matrix4x4::transformInverseDirection(const Vector3D &vector) const
{
  return Vector3D(
    vector.x * m_matrix[0][0] +
    vector.y * m_matrix[1][0] +
    vector.z * m_matrix[2][0],

    vector.x * m_matrix[0][1] +
    vector.y * m_matrix[1][1] +
    vector.z * m_matrix[2][1],

    vector.x * m_matrix[0][2] +
    vector.y * m_matrix[1][2] +
    vector.z * m_matrix[2][2]
  );
}


Vector3D Matrix4x4::transformInverse(const Vector3D &vector) const
{
  Vector3D tmp = vector;
  tmp.x -= m_matrix[0][3];
  tmp.y -= m_matrix[1][3];
  tmp.z -= m_matrix[2][3];
  return Vector3D(
    tmp.x * m_matrix[0][0] +
    tmp.y * m_matrix[1][0] +
    tmp.z * m_matrix[2][0],

    tmp.x * m_matrix[0][1] +
    tmp.y * m_matrix[1][1] +
    tmp.z * m_matrix[2][1],

    tmp.x * m_matrix[0][2] +
    tmp.y * m_matrix[1][2] +
    tmp.z * m_matrix[2][2]
  );
}


Vector3D Matrix4x4::getAxisVector(int i) const
{
  return Vector3D(m_matrix[0][i], m_matrix[1][i], m_matrix[2][i]);
}


void Matrix4x4::setOrientationAndPos(const Quaternion &q, const Vector3D &pos)
{
  m_matrix[0][0] = 1 - (2 * q.j*q.j + 2 * q.k*q.k);
  m_matrix[0][1] = 2 * q.i*q.j + 2 * q.k*q.r;
  m_matrix[0][2] = 2 * q.i*q.k - 2 * q.j*q.r;
  m_matrix[0][3] = pos.x;

  m_matrix[1][0] = 2 * q.i*q.j - 2 * q.k*q.r;
  m_matrix[1][1] = 1 - (2 * q.i*q.i + 2 * q.k*q.k);
  m_matrix[1][2] = 2 * q.j*q.k + 2 * q.i*q.r;
  m_matrix[1][3] = pos.y;

  m_matrix[2][0] = 2 * q.i*q.k + 2 * q.j*q.r;
  m_matrix[2][1] = 2 * q.j*q.k - 2 * q.i*q.r;
  m_matrix[2][2] = 1 - (2 * q.i*q.i + 2 * q.j*q.j);
  m_matrix[2][3] = pos.z;
}


void Matrix4x4::fillGLArray(float array[16]) const
{
  array[0] = (float)m_matrix[0][0];
  array[1] = (float)m_matrix[1][0];
  array[2] = (float)m_matrix[2][0];
  array[3] = (float)0;

  array[4] = (float)m_matrix[0][1];
  array[5] = (float)m_matrix[1][1];
  array[6] = (float)m_matrix[2][1];
  array[7] = (float)0;

  array[8] = (float)m_matrix[0][2];
  array[9] = (float)m_matrix[1][2];
  array[10] = (float)m_matrix[2][2];
  array[11] = (float)0;

  array[12] = (float)m_matrix[0][3];
  array[13] = (float)m_matrix[1][3];
  array[14] = (float)m_matrix[2][3];
  array[15] = (float)1;
}

#pragma endregion

#pragma region Operation Overrides
Matrix4x4::operator float*()
{
	return &(m_matrix[0][0]);
}

bool Matrix4x4::operator==(const Matrix4x4& other) const
{
	double d = 0.0f;

	d += fabs(m_matrix[0][0] - other.m_matrix[0][0]);
	d += fabs(m_matrix[0][1] - other.m_matrix[0][1]);
	d += fabs(m_matrix[0][2] - other.m_matrix[0][2]);
	d += fabs(m_matrix[0][3] - other.m_matrix[0][3]);

	d += fabs(m_matrix[1][0] - other.m_matrix[1][0]);
	d += fabs(m_matrix[1][1] - other.m_matrix[1][1]);
	d += fabs(m_matrix[1][2] - other.m_matrix[1][2]);
	d += fabs(m_matrix[1][3] - other.m_matrix[1][3]);

	d += fabs(m_matrix[2][0] - other.m_matrix[2][0]);
	d += fabs(m_matrix[2][1] - other.m_matrix[2][1]);
	d += fabs(m_matrix[2][2] - other.m_matrix[2][2]);
	d += fabs(m_matrix[2][3] - other.m_matrix[2][3]);

	d += fabs(m_matrix[3][0] - other.m_matrix[3][0]);
	d += fabs(m_matrix[3][1] - other.m_matrix[3][1]);
	d += fabs(m_matrix[3][2] - other.m_matrix[3][2]);
	d += fabs(m_matrix[3][3] - other.m_matrix[3][3]);
	return d < EPSILON;
}

bool Matrix4x4::operator!=(const Matrix4x4& other) const
{
	double d = 0.0f;

	d += fabs(m_matrix[0][0] - other.m_matrix[0][0]);
	d += fabs(m_matrix[0][1] - other.m_matrix[0][1]);
	d += fabs(m_matrix[0][2] - other.m_matrix[0][2]);
	d += fabs(m_matrix[0][3] - other.m_matrix[0][3]);

	d += fabs(m_matrix[1][0] - other.m_matrix[1][0]);
	d += fabs(m_matrix[1][1] - other.m_matrix[1][1]);
	d += fabs(m_matrix[1][2] - other.m_matrix[1][2]);
	d += fabs(m_matrix[1][3] - other.m_matrix[1][3]);

	d += fabs(m_matrix[2][0] - other.m_matrix[2][0]);
	d += fabs(m_matrix[2][1] - other.m_matrix[2][1]);
	d += fabs(m_matrix[2][2] - other.m_matrix[2][2]);
	d += fabs(m_matrix[2][3] - other.m_matrix[2][3]);

	d += fabs(m_matrix[3][0] - other.m_matrix[3][0]);
	d += fabs(m_matrix[3][1] - other.m_matrix[3][1]);
	d += fabs(m_matrix[3][2] - other.m_matrix[3][2]);
	d += fabs(m_matrix[3][3] - other.m_matrix[3][3]);
	return d >= EPSILON;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const
{
	return Matrix4x4(
		m_matrix[0][0] + other.m_matrix[0][0], m_matrix[0][1] + other.m_matrix[0][1], m_matrix[0][2] + other.m_matrix[0][2], m_matrix[0][3] + other.m_matrix[0][3],
		m_matrix[1][0] + other.m_matrix[1][0], m_matrix[1][1] + other.m_matrix[1][1], m_matrix[1][2] + other.m_matrix[1][2], m_matrix[1][3] + other.m_matrix[1][3],
		m_matrix[2][0] + other.m_matrix[2][0], m_matrix[2][1] + other.m_matrix[2][1], m_matrix[2][2] + other.m_matrix[2][2], m_matrix[2][3] + other.m_matrix[2][3],
		m_matrix[3][0] + other.m_matrix[3][0], m_matrix[3][1] + other.m_matrix[3][1], m_matrix[3][2] + other.m_matrix[3][2], m_matrix[3][3] + other.m_matrix[3][3]
	);
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const
{
	return Matrix4x4(
		m_matrix[0][0] - other.m_matrix[0][0], m_matrix[0][1] - other.m_matrix[0][1], m_matrix[0][2] - other.m_matrix[0][2], m_matrix[0][3] - other.m_matrix[0][3],
		m_matrix[1][0] - other.m_matrix[1][0], m_matrix[1][1] - other.m_matrix[1][1], m_matrix[1][2] - other.m_matrix[1][2], m_matrix[1][3] - other.m_matrix[1][3],
		m_matrix[2][0] - other.m_matrix[2][0], m_matrix[2][1] - other.m_matrix[2][1], m_matrix[2][2] - other.m_matrix[2][2], m_matrix[2][3] - other.m_matrix[2][3],
		m_matrix[3][0] - other.m_matrix[3][0], m_matrix[3][1] - other.m_matrix[3][1], m_matrix[3][2] - other.m_matrix[3][2], m_matrix[3][3] - other.m_matrix[3][3]
	);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
	return Matrix4x4(
		(m_matrix[0][0] * other.m_matrix[0][0]) + (m_matrix[0][1] * other.m_matrix[1][0]) + (m_matrix[0][2] * other.m_matrix[2][0]) + (m_matrix[0][3] * other.m_matrix[3][0]),
		(m_matrix[0][0] * other.m_matrix[0][1]) + (m_matrix[0][1] * other.m_matrix[1][1]) + (m_matrix[0][2] * other.m_matrix[2][1]) + (m_matrix[0][3] * other.m_matrix[3][1]),
		(m_matrix[0][0] * other.m_matrix[0][2]) + (m_matrix[0][1] * other.m_matrix[1][2]) + (m_matrix[0][2] * other.m_matrix[2][2]) + (m_matrix[0][3] * other.m_matrix[3][2]),
		(m_matrix[0][0] * other.m_matrix[0][3]) + (m_matrix[0][1] * other.m_matrix[1][3]) + (m_matrix[0][2] * other.m_matrix[2][3]) + (m_matrix[0][3] * other.m_matrix[3][3]),

		(m_matrix[1][0] * other.m_matrix[0][0]) + (m_matrix[1][1] * other.m_matrix[1][0]) + (m_matrix[1][2] * other.m_matrix[2][0]) + (m_matrix[1][3] * other.m_matrix[3][0]),
		(m_matrix[1][0] * other.m_matrix[0][1]) + (m_matrix[1][1] * other.m_matrix[1][1]) + (m_matrix[1][2] * other.m_matrix[2][1]) + (m_matrix[1][3] * other.m_matrix[3][1]),
		(m_matrix[1][0] * other.m_matrix[0][2]) + (m_matrix[1][1] * other.m_matrix[1][2]) + (m_matrix[1][2] * other.m_matrix[2][2]) + (m_matrix[1][3] * other.m_matrix[3][2]),
		(m_matrix[1][0] * other.m_matrix[0][3]) + (m_matrix[1][1] * other.m_matrix[1][3]) + (m_matrix[1][2] * other.m_matrix[2][3]) + (m_matrix[1][3] * other.m_matrix[3][3]),

		(m_matrix[2][0] * other.m_matrix[0][0]) + (m_matrix[2][1] * other.m_matrix[1][0]) + (m_matrix[2][2] * other.m_matrix[2][0]) + (m_matrix[2][3] * other.m_matrix[3][0]),
		(m_matrix[2][0] * other.m_matrix[0][1]) + (m_matrix[2][1] * other.m_matrix[1][1]) + (m_matrix[2][2] * other.m_matrix[2][1]) + (m_matrix[2][3] * other.m_matrix[3][1]),
		(m_matrix[2][0] * other.m_matrix[0][2]) + (m_matrix[2][1] * other.m_matrix[1][2]) + (m_matrix[2][2] * other.m_matrix[2][2]) + (m_matrix[2][3] * other.m_matrix[3][2]),
		(m_matrix[2][0] * other.m_matrix[0][3]) + (m_matrix[2][1] * other.m_matrix[1][3]) + (m_matrix[2][2] * other.m_matrix[2][3]) + (m_matrix[2][3] * other.m_matrix[3][3]),

		(m_matrix[3][0] * other.m_matrix[0][0]) + (m_matrix[3][1] * other.m_matrix[1][0]) + (m_matrix[3][2] * other.m_matrix[2][0]) + (m_matrix[3][3] * other.m_matrix[3][0]),
		(m_matrix[3][0] * other.m_matrix[0][1]) + (m_matrix[3][1] * other.m_matrix[1][1]) + (m_matrix[3][2] * other.m_matrix[2][1]) + (m_matrix[3][3] * other.m_matrix[3][1]),
		(m_matrix[3][0] * other.m_matrix[0][2]) + (m_matrix[3][1] * other.m_matrix[1][2]) + (m_matrix[3][2] * other.m_matrix[2][2]) + (m_matrix[3][3] * other.m_matrix[3][2]),
		(m_matrix[3][0] * other.m_matrix[0][3]) + (m_matrix[3][1] * other.m_matrix[1][3]) + (m_matrix[3][2] * other.m_matrix[2][3]) + (m_matrix[3][3] * other.m_matrix[3][3])
	);
}

Matrix4x4 Matrix4x4::operator*(const float scalar) const
{
	return Matrix4x4(
		m_matrix[0][0] * scalar, m_matrix[0][1] * scalar, m_matrix[0][2] * scalar, m_matrix[0][3] * scalar,
		m_matrix[1][0] * scalar, m_matrix[1][1] * scalar, m_matrix[1][2] * scalar, m_matrix[1][3] * scalar,
		m_matrix[2][0] * scalar, m_matrix[2][1] * scalar, m_matrix[2][2] * scalar, m_matrix[2][3] * scalar,
		m_matrix[3][0] * scalar, m_matrix[3][1] * scalar, m_matrix[3][2] * scalar, m_matrix[3][3] * scalar
	);
}

Matrix4x4 Matrix4x4::operator/(const float divisor) const
{
	return Matrix4x4(
		m_matrix[0][0] / divisor, m_matrix[0][1] / divisor, m_matrix[0][2] / divisor, m_matrix[0][3] / divisor,
		m_matrix[1][0] / divisor, m_matrix[1][1] / divisor, m_matrix[1][2] / divisor, m_matrix[1][3] / divisor,
		m_matrix[2][0] / divisor, m_matrix[2][1] / divisor, m_matrix[2][2] / divisor, m_matrix[2][3] / divisor,
		m_matrix[3][0] / divisor, m_matrix[3][1] / divisor, m_matrix[3][2] / divisor, m_matrix[3][3] / divisor
	);
}


Vector3D Matrix4x4::operator*(const Vector3D& other) const
{
	return Vector3D(
		m_matrix[0][0] * other.x + m_matrix[0][1] * other.y + m_matrix[0][2] * other.z + m_matrix[0][3] * other.w,
		m_matrix[1][0] * other.x + m_matrix[1][1] * other.y + m_matrix[1][2] * other.z + m_matrix[1][3] * other.w,
		m_matrix[2][0] * other.x + m_matrix[2][1] * other.y + m_matrix[2][2] * other.z + m_matrix[2][3] * other.w,
		m_matrix[3][0] * other.x + m_matrix[3][1] * other.y + m_matrix[3][2] * other.z + m_matrix[3][3] * other.w
	);
}
#pragma endregion

