/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include "PiEnginePreCompiled.h"

Matrix3x3::Matrix3x3() {}

Matrix3x3::Matrix3x3(mat3 &other)
{
  m_matrix[0][0] = other[0][0];
  m_matrix[0][1] = other[0][1];
  m_matrix[0][2] = other[0][2];

  m_matrix[1][0] = other[1][0];
  m_matrix[1][1] = other[1][1];
  m_matrix[1][2] = other[1][2];

  m_matrix[2][0] = other[2][0];
  m_matrix[2][1] = other[2][1];
  m_matrix[2][2] = other[2][2];
}

Matrix3x3::Matrix3x3(
	float x1, float y1, float z1, 
	float x2, float y2, float z2, 
	float x3, float y3, float z3)
{
	m_matrix[0][0] = x1;
	m_matrix[0][1] = y1;
	m_matrix[0][2] = z1;

	m_matrix[1][0] = x2;
	m_matrix[1][1] = y2;
	m_matrix[1][2] = z2;

	m_matrix[2][0] = x3;
	m_matrix[2][1] = y3;
	m_matrix[2][2] = z3;
}


Matrix3x3::Matrix3x3(const Matrix3x3 &other)
{
	m_matrix[0][0] = other.m_matrix[0][0];
	m_matrix[0][1] = other.m_matrix[0][1];
	m_matrix[0][2] = other.m_matrix[0][2];

	m_matrix[1][0] = other.m_matrix[1][0];
	m_matrix[1][1] = other.m_matrix[1][1];
	m_matrix[1][2] = other.m_matrix[1][2];

	m_matrix[2][0] = other.m_matrix[2][0];
	m_matrix[2][1] = other.m_matrix[2][1];
	m_matrix[2][2] = other.m_matrix[2][2];
}

Matrix3x3::Matrix3x3(const Vector3D &compOne, const Vector3D &compTwo,
  const Vector3D &compThree)
{
  Setting(compOne, compTwo, compThree);
}

void Matrix3x3::setDiagonal(real a, real b, real c)
{
  setInertiaTensorCoeffs(a, b, c);
}

void Matrix3x3::setInertiaTensorCoeffs(real ix, real iy, real iz,
  real ixy, real ixz, real iyz)
{
  m_matrix[0][0] = ix;
  m_matrix[0][1] = m_matrix[1][0] = -ixy;
  m_matrix[0][2] = m_matrix[2][0] = -ixz;
  m_matrix[1][1] = iy;
  m_matrix[1][2] = m_matrix[2][1] = -iyz;
  m_matrix[2][2] = iz;
}

void Matrix3x3::setBlockInertiaTensor(const Vector3D &halfSizes, real mass)
{
  Vector3D squares = halfSizes.componentProduct(halfSizes);
  setInertiaTensorCoeffs(0.3f*mass*(squares.y + squares.z),
    0.3f*mass*(squares.x + squares.z),
    0.3f*mass*(squares.x + squares.y));
}

void Matrix3x3::Symmetric(const Vector3D vector)
{
  m_matrix[0][0] = m_matrix[1][1] = m_matrix[2][2] = 0;
  m_matrix[0][1] = -vector.z;
  m_matrix[0][2] = vector.y;
  m_matrix[1][0] = vector.z;
  m_matrix[1][2] = -vector.x;
  m_matrix[2][0] = -vector.y;
  m_matrix[2][1] = vector.x;
}

void Matrix3x3::Setting(const Vector3D &compOne, const Vector3D &compTwo,
  const Vector3D &compThree)
{
  m_matrix[0][0] = compOne.x;
  m_matrix[0][1] = compTwo.x;
  m_matrix[0][2] = compThree.x;
  m_matrix[1][0] = compOne.y;
  m_matrix[1][1] = compTwo.y;
  m_matrix[1][2] = compThree.y;
  m_matrix[2][0] = compOne.z;
  m_matrix[2][1] = compTwo.z;
  m_matrix[2][2] = compThree.z;
}

Vector3D Matrix3x3::transform(const Vector3D &vector)
{
  return (*this) * vector;
}

Vector3D Matrix3x3::transformTranspose(const Vector3D &vector) const
{
  return Vector3D(
    vector.x * m_matrix[0][0] + vector.y * m_matrix[1][0] + vector.z * m_matrix[2][0],
    vector.x * m_matrix[0][1] + vector.y * m_matrix[1][1] + vector.z * m_matrix[2][1],
    vector.x * m_matrix[0][2] + vector.y * m_matrix[1][2] + vector.z * m_matrix[2][2]
  );
}

Vector3D Matrix3x3::getRowVector(int i) const
{
  return Vector3D(m_matrix[i][0], m_matrix[i][1], m_matrix[i][2]);
}

Vector3D Matrix3x3::getAxisVector(int i) const
{
  return Vector3D(m_matrix[0][i], m_matrix[1][i], m_matrix[2][i]);
}

void Matrix3x3::setInverse(const Matrix3x3 &m)
{
  real t4 = m.m_matrix[0][0] * m.m_matrix[1][1];
  real t6 = m.m_matrix[0][0] * m.m_matrix[1][2];
  real t8 = m.m_matrix[0][1] * m.m_matrix[1][0];
  real t10 = m.m_matrix[0][2] * m.m_matrix[1][0];
  real t12 = m.m_matrix[0][1] * m.m_matrix[2][0];
  real t14 = m.m_matrix[0][2] * m.m_matrix[2][0];

  // Calculate the determinant
  real t16 = (t4*m.m_matrix[2][2] - t6 * m.m_matrix[2][1] - t8 * m.m_matrix[2][2] +
    t10 * m.m_matrix[2][1] + t12 * m.m_matrix[1][2] - t14 * m.m_matrix[1][1]);

  // Make sure the determinant is non-zero.
  if (t16 == (real)0.0f) return;
  real t17 = 1 / t16;

  m_matrix[0][0] = (m.m_matrix[1][1] * m.m_matrix[2][2] - m.m_matrix[1][2] * m.m_matrix[2][1])*t17;
  m_matrix[0][1] = -(m.m_matrix[0][1] * m.m_matrix[2][2] - m.m_matrix[0][2] * m.m_matrix[2][1])*t17;
  m_matrix[0][2] = (m.m_matrix[0][1] * m.m_matrix[1][2] - m.m_matrix[0][2] * m.m_matrix[1][1])*t17;
  m_matrix[1][0] = -(m.m_matrix[1][0] * m.m_matrix[2][2] - m.m_matrix[1][2] * m.m_matrix[2][0])*t17;
  m_matrix[1][1] = (m.m_matrix[0][0] * m.m_matrix[2][2] - t14)*t17;
  m_matrix[1][2] = -(t6 - t10)*t17;
  m_matrix[2][0] = (m.m_matrix[1][0] * m.m_matrix[2][1] - m.m_matrix[1][1] * m.m_matrix[2][0])*t17;
  m_matrix[2][1] = -(m.m_matrix[0][0] * m.m_matrix[2][1] - t12)*t17;
  m_matrix[2][2] = (t4 - t8)*t17;
}

Matrix3x3 Matrix3x3::inverse() const
{
  Matrix3x3 result = Matrix3x3(0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f);;
  result.setInverse(*this);
  return result;
}

void Matrix3x3::invert()
{
  setInverse(*this);
}

Matrix3x3 Matrix3x3::linearInterpolate(const Matrix3x3& a, const Matrix3x3& b, real prop)
{
  Matrix3x3 result = Matrix3x3(0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f);
  for (unsigned i = 0; i < 3; i++) {
    for (unsigned j = 0; j < 3; j++)
    {
      result.m_matrix[i][j] = a.m_matrix[i][j] * (1 - prop) + b.m_matrix[i][j] * prop;
    }
  }
  return result;
}

void Matrix3x3::setTranspose(const Matrix3x3 &m)
{
  m_matrix[0][0] = m.m_matrix[0][0];
  m_matrix[0][1] = m.m_matrix[1][0];
  m_matrix[0][2] = m.m_matrix[2][0];
  m_matrix[1][0] = m.m_matrix[0][1];
  m_matrix[1][1] = m.m_matrix[1][1];
  m_matrix[1][2] = m.m_matrix[2][1];
  m_matrix[2][0] = m.m_matrix[0][2];
  m_matrix[2][1] = m.m_matrix[1][2];
  m_matrix[2][2] = m.m_matrix[2][2];
}

Matrix3x3 Matrix3x3::transpose() const
{
  Matrix3x3 result = Matrix3x3(0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f);;
  result.setTranspose(*this);
  return result;
}

Matrix3x3& Matrix3x3::operator=(const Matrix3x3& other)
{
	m_matrix[0][0] = other.m_matrix[0][0];
	m_matrix[0][1] = other.m_matrix[0][1];
	m_matrix[0][2] = other.m_matrix[0][2];

	m_matrix[1][0] = other.m_matrix[1][0];
	m_matrix[1][1] = other.m_matrix[1][1];
	m_matrix[1][2] = other.m_matrix[1][2];

	m_matrix[2][0] = other.m_matrix[2][0];
	m_matrix[2][1] = other.m_matrix[2][1];
	m_matrix[2][2] = other.m_matrix[2][2];
	return *this;
}

Matrix3x3::~Matrix3x3(){}



void Matrix3x3::Print() const
{
	std::cout << "[ " << m_matrix[0][0] << ", " << m_matrix[0][1] << ", " << m_matrix[0][2] << "]" << std::endl;
	std::cout << "[ " << m_matrix[1][0] << ", " << m_matrix[1][1] << ", " << m_matrix[1][2] << "]" << std::endl;
	std::cout << "[ " << m_matrix[2][0] << ", " << m_matrix[2][1] << ", " << m_matrix[2][2] << "]" << std::endl;
}

#pragma region Static Methods

#pragma endregion

#pragma region Operation Overrides
bool Matrix3x3::operator==(const Matrix3x3& other)
{
	double d = 0.0f;

	d += fabs(m_matrix[0][0] - other.m_matrix[0][0]);
	d += fabs(m_matrix[0][1] - other.m_matrix[0][1]);
	d += fabs(m_matrix[0][2] - other.m_matrix[0][2]);

	d += fabs(m_matrix[1][0] - other.m_matrix[1][0]);
	d += fabs(m_matrix[1][1] - other.m_matrix[1][1]);
	d += fabs(m_matrix[1][2] - other.m_matrix[1][2]);

	d += fabs(m_matrix[2][0] - other.m_matrix[2][0]);
	d += fabs(m_matrix[2][1] - other.m_matrix[2][1]);
	d += fabs(m_matrix[2][2] - other.m_matrix[2][2]);
	return d < EPSILON;
}

bool Matrix3x3::operator!=(const Matrix3x3& other)
{
	double d = 0.0f;

	d += fabs(m_matrix[0][0] - other.m_matrix[0][0]);
	d += fabs(m_matrix[0][1] - other.m_matrix[0][1]);
	d += fabs(m_matrix[0][2] - other.m_matrix[0][2]);

	d += fabs(m_matrix[1][0] - other.m_matrix[1][0]);
	d += fabs(m_matrix[1][1] - other.m_matrix[1][1]);
	d += fabs(m_matrix[1][2] - other.m_matrix[1][2]);

	d += fabs(m_matrix[2][0] - other.m_matrix[2][0]);
	d += fabs(m_matrix[2][1] - other.m_matrix[2][1]);
	d += fabs(m_matrix[2][2] - other.m_matrix[2][2]);
	return d >= EPSILON;
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3& other)
{
	return Matrix3x3(
		m_matrix[0][0] + other.m_matrix[0][0], m_matrix[0][1] + other.m_matrix[0][1], m_matrix[0][2] + other.m_matrix[0][2],
		m_matrix[1][0] + other.m_matrix[1][0], m_matrix[1][1] + other.m_matrix[1][1], m_matrix[1][2] + other.m_matrix[1][2],
		m_matrix[2][0] + other.m_matrix[2][0], m_matrix[2][1] + other.m_matrix[2][1], m_matrix[2][2] + other.m_matrix[2][2]
	);
}

Matrix3x3 Matrix3x3::operator-(const Matrix3x3& other)
{
	return Matrix3x3(
		m_matrix[0][0] - other.m_matrix[0][0], m_matrix[0][1] - other.m_matrix[0][1], m_matrix[0][2] - other.m_matrix[0][2],
		m_matrix[1][0] - other.m_matrix[1][0], m_matrix[1][1] - other.m_matrix[1][1], m_matrix[1][2] - other.m_matrix[1][2],
		m_matrix[2][0] - other.m_matrix[2][0], m_matrix[2][1] - other.m_matrix[2][1], m_matrix[2][2] - other.m_matrix[2][2]
	);
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& other)
{
	return Matrix3x3(
		(m_matrix[0][0] * other.m_matrix[0][0]) + (m_matrix[0][1] * other.m_matrix[1][0]) + (m_matrix[0][2] * other.m_matrix[2][0]),
		(m_matrix[0][0] * other.m_matrix[0][1]) + (m_matrix[0][1] * other.m_matrix[1][1]) + (m_matrix[0][2] * other.m_matrix[2][1]),
		(m_matrix[0][0] * other.m_matrix[0][2]) + (m_matrix[0][1] * other.m_matrix[1][2]) + (m_matrix[0][2] * other.m_matrix[2][2]),

		(m_matrix[1][0] * other.m_matrix[0][0]) + (m_matrix[1][1] * other.m_matrix[1][0]) + (m_matrix[1][2] * other.m_matrix[2][0]),
		(m_matrix[1][0] * other.m_matrix[0][1]) + (m_matrix[1][1] * other.m_matrix[1][1]) + (m_matrix[1][2] * other.m_matrix[2][1]),
		(m_matrix[1][0] * other.m_matrix[0][2]) + (m_matrix[1][1] * other.m_matrix[1][2]) + (m_matrix[1][2] * other.m_matrix[2][2]),

		(m_matrix[2][0] * other.m_matrix[0][0]) + (m_matrix[2][1] * other.m_matrix[1][0]) + (m_matrix[2][2] * other.m_matrix[2][0]),
		(m_matrix[2][0] * other.m_matrix[0][1]) + (m_matrix[2][1] * other.m_matrix[1][1]) + (m_matrix[2][2] * other.m_matrix[2][1]),
		(m_matrix[2][0] * other.m_matrix[0][2]) + (m_matrix[2][1] * other.m_matrix[1][2]) + (m_matrix[2][2] * other.m_matrix[2][2])
	);
}

Matrix3x3 Matrix3x3::operator*(const float scalar)
{
	return Matrix3x3(
		m_matrix[0][0] * scalar, m_matrix[0][1] * scalar, m_matrix[0][2] * scalar,
		m_matrix[1][0] * scalar, m_matrix[1][1] * scalar, m_matrix[1][2] * scalar,
		m_matrix[2][0] * scalar, m_matrix[2][1] * scalar, m_matrix[2][2] * scalar
	);
}

Matrix3x3 Matrix3x3::operator/(const float divisor)
{
	return Matrix3x3(
		m_matrix[0][0] / divisor, m_matrix[0][1] / divisor, m_matrix[0][2] / divisor,
		m_matrix[1][0] / divisor, m_matrix[1][1] / divisor, m_matrix[1][2] / divisor,
		m_matrix[2][0] / divisor, m_matrix[2][1] / divisor, m_matrix[2][2] / divisor
	);
}



Vector3D Matrix3x3::operator*(const Vector3D& other)
{
	return Vector3D(
		m_matrix[0][0] * other.x + m_matrix[0][1] * other.y + m_matrix[0][2] * other.z,
		m_matrix[1][0] * other.x + m_matrix[1][1] * other.y + m_matrix[1][2] * other.z,
		m_matrix[2][0] * other.x + m_matrix[2][1] * other.y + m_matrix[2][2] * other.z
	);
}

void Matrix3x3::operator*=(const Matrix3x3 &o)
{
  real t1;
  real t2;
  real t3;

  t1 = m_matrix[0][0] * o.m_matrix[0][0] + m_matrix[0][1] * o.m_matrix[1][0] + m_matrix[0][2] * o.m_matrix[2][0];
  t2 = m_matrix[0][0] * o.m_matrix[0][1] + m_matrix[0][1] * o.m_matrix[1][1] + m_matrix[0][2] * o.m_matrix[2][1];
  t3 = m_matrix[0][0] * o.m_matrix[0][2] + m_matrix[0][1] * o.m_matrix[1][2] + m_matrix[0][2] * o.m_matrix[2][2];
  m_matrix[0][0] = t1;
  m_matrix[0][1] = t2;
  m_matrix[0][2] = t3;

  t1 = m_matrix[1][0] * o.m_matrix[0][0] + m_matrix[1][1] * o.m_matrix[1][0] + m_matrix[1][2] * o.m_matrix[2][0];
  t2 = m_matrix[1][0] * o.m_matrix[0][1] + m_matrix[1][1] * o.m_matrix[1][1] + m_matrix[1][2] * o.m_matrix[2][1];
  t3 = m_matrix[1][0] * o.m_matrix[0][2] + m_matrix[1][1] * o.m_matrix[1][2] + m_matrix[1][2] * o.m_matrix[2][2];
  m_matrix[1][0] = t1;
  m_matrix[1][1] = t2;
  m_matrix[1][2] = t3;

  t1 = m_matrix[2][0] * o.m_matrix[0][0] + m_matrix[2][1] * o.m_matrix[1][0] + m_matrix[2][2] * o.m_matrix[2][0];
  t2 = m_matrix[2][0] * o.m_matrix[0][1] + m_matrix[2][1] * o.m_matrix[1][1] + m_matrix[2][2] * o.m_matrix[2][1];
  t3 = m_matrix[2][0] * o.m_matrix[0][2] + m_matrix[2][1] * o.m_matrix[1][2] + m_matrix[2][2] * o.m_matrix[2][2];
  m_matrix[2][0] = t1;
  m_matrix[2][1] = t2;
  m_matrix[2][2] = t3;
}


void Matrix3x3::operator*=(const real scalar)
{
  m_matrix[0][0] *= scalar; m_matrix[0][1] *= scalar; m_matrix[0][2] *= scalar;
  m_matrix[1][0] *= scalar; m_matrix[1][1] *= scalar; m_matrix[1][2] *= scalar;
  m_matrix[2][0] *= scalar; m_matrix[2][1] *= scalar; m_matrix[2][2] *= scalar;
}


void Matrix3x3::operator+=(const Matrix3x3 &o)
{
  m_matrix[0][0] += o.m_matrix[0][0]; m_matrix[0][1] += o.m_matrix[0][1]; m_matrix[0][2] += o.m_matrix[0][2];
  m_matrix[1][0] += o.m_matrix[1][0]; m_matrix[1][1] += o.m_matrix[1][1]; m_matrix[1][2] += o.m_matrix[1][2];
  m_matrix[2][0] += o.m_matrix[2][0]; m_matrix[2][1] += o.m_matrix[2][1]; m_matrix[2][2] += o.m_matrix[2][2];
}

void Matrix3x3::setOrientation(const Quaternion &q)
{
  m_matrix[0][0] = 1 - (2 * q.j*q.j + 2 * q.k*q.k);
  m_matrix[0][1] = 2 * q.i*q.j + 2 * q.k*q.r;
  m_matrix[0][2] = 2 * q.i*q.k - 2 * q.j*q.r;
  m_matrix[1][0] = 2 * q.i*q.j - 2 * q.k*q.r;
  m_matrix[1][1] = 1 - (2 * q.i*q.i + 2 * q.k*q.k);
  m_matrix[1][2] = 2 * q.j*q.k + 2 * q.i*q.r;
  m_matrix[2][0] = 2 * q.i*q.k + 2 * q.j*q.r;
  m_matrix[2][1] = 2 * q.j*q.k - 2 * q.i*q.r;
  m_matrix[2][2] = 1 - (2 * q.i*q.i + 2 * q.j*q.j);
}
#pragma endregion