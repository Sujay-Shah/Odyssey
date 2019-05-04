/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include "PiEnginePreCompiled.h"

Vector3D::Vector3D() : x(0.f), y(0.f), z(0.f), w(1.f) { }



Vector3D::Vector3D(const Vector3D & vec, const float & _w) : x(vec.x), y(vec.y), z(vec.z), w(_w) { }

Vector3D::Vector3D(const float& _x, const float& _y, const float& _z) : x(_x), y(_y), z(_z), w(1.f) { }

Vector3D::Vector3D(const float& _x, const float& _y, const float& _z, const float& _w) : x(_x), y(_y), z(_z), w(_w) { }

Vector3D::Vector3D(const Vector3D & rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) { }



Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

Vector3D::~Vector3D() { }

void Vector3D::Set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void Vector3D::Set(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

#pragma region Operator Overrides
Vector3D::operator float*()
{
	return &(x);
}

bool Vector3D::operator==(const Vector3D& other) const
{
	double d = 0.0f;

	d += fabs(x - other.x);
	d += fabs(y - other.y);
	d += fabs(z - other.z);
	d += fabs(w - other.w);

	return d < EPSILON;
}

bool Vector3D::operator!=(const Vector3D& other) const
{
	double d = 0.0f;

	d += fabs(x - other.x);
	d += fabs(y - other.y);
	d += fabs(z - other.z);
	d += fabs(w - other.w);

	return d >= EPSILON;
}

Vector3D Vector3D::operator+(const Vector3D& other) const
{
	return Vector3D(
		x + other.x,
		y + other.y,
		z + other.z
	);
}

Vector3D & Vector3D::operator+=(const Vector3D & other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
	return Vector3D(
		x - other.x,
		y - other.y,
		z - other.z
	);
}

Vector3D & Vector3D::operator-=(const Vector3D & other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3D Vector3D::operator*(const float scalar) const
{
	return Vector3D(
		x * scalar,
		y * scalar,
		z * scalar
	);
}

Vector3D Vector3D::operator/(const float divisor) const
{
	return Vector3D(
		x / divisor,
		y / divisor,
		z / divisor
	);
}

float Vector3D::operator[](const int idx) const
{
	return *(&x + idx);
}

float& Vector3D::operator[](const int idx)
{
	return *(&x + idx);
}
#pragma endregion



void Vector3D::Div(const float divisor)
{
	x /= divisor;
	y /= divisor;
	z /= divisor;
}

void Vector3D::Print() const
{
	std::cout << "<X: " << x << ", Y: " << y << ", Z: " << z << ", W: " << w << ">" << std::endl;
}

void Vector3D::FillValues(float & _x, float & _y, float & _z)
{
	_x = x;
	_y = y;
	_z = z;
}

void Vector3D::Zero()
{
	x = y = z = 0.0f;
}

bool Vector3D::IsVectorZero(float error) const {
	return (fabsf(x) < error && fabsf(y) < error && fabsf(z) < error);
}

void Vector3D::Negate()
{
	x = -x;
	y = -y;
	z = -z;
}

float Vector3D::SquareLength() const
{
	return x * x + y * y + z * z;
}

float Vector3D::Length() const
{
	return sqrtf(SquareLength());
}

void Vector3D::Normalize()
{
	if (x == 0 && y == 0 && z == 0)
		return;
	Div(Length());
}

float Vector3D::AngleRadians() const {
	return atan2f(y, x);
}

float Vector3D::AngleDegrees() const {
	return AngleRadians() * RAD_TO_DEG;
}

#pragma region Static Methods
float Vector3D::SquareDistance(const Vector3D& vector0, const Vector3D& vector1)
{
	float x = vector1.x - vector0.x;
	float y = vector1.y - vector0.y;
	float z = vector1.z - vector0.z;
	return x * x + y * y + z * z;
}

float Vector3D::Distance(const Vector3D& vector0, const Vector3D& vector1)
{
	return sqrtf(SquareDistance(vector0, vector1));
}

Vector3D Vector3D::Normalize(const Vector3D& vector)
{
	float length = vector.Length();
	if (length == 0.0f)
		return Vector3D();
	return vector / length;
}

float Vector3D::Dot(const Vector3D& vector0, const Vector3D& vector1)
{
	return vector0.x * vector1.x + vector0.y * vector1.y + vector0.z * vector1.z;
}

Vector3D Vector3D::Cross(const Vector3D& vector0, const Vector3D& vector1)
{
	return Vector3D(
		vector0.y * vector1.z - vector0.z * vector1.y,
		vector0.z * vector1.x - vector0.x * vector1.z,
		vector0.x * vector1.y - vector0.y * vector1.x,
		0.f
	);
}

Vector3D Vector3D::AngleRadians(float radians, Vector3D& axis)
{
	axis.Print();
	std::cout << "Radians: " << radians << std::endl;
	return Vector3D();
}

Vector3D Vector3D::AngleDegrees(float degrees, Vector3D& axis)
{
	return AngleRadians(degrees * DEG_TO_RAD, axis);
}

Vector3D Vector3D::VectorFromAngleDegrees(float degrees)
{
	float rad = degrees*DEG_TO_RAD;
	return Vector3D(cosf(rad), sinf(rad), 0);
}

void Vector3D::operator*=(const real value)
{
  x *= value;
  y *= value;
  z *= value;
}

Vector3D Vector3D::componentProduct(const Vector3D &vector) const
{
  return Vector3D(x * vector.x, y * vector.y, z * vector.z);
}

void Vector3D::componentProductUpdate(const Vector3D &vector)
{
  x *= vector.x;
  y *= vector.y;
  z *= vector.z;
}

Vector3D Vector3D::vectorProduct(const Vector3D &vector) const
{
  return Vector3D(y*vector.z - z * vector.y,
    z*vector.x - x * vector.z,
    x*vector.y - y * vector.x);
}

void Vector3D::operator %=(const Vector3D &vector)
{
  *this = vectorProduct(vector);
}

/**
 * Calculates and returns the vector product of this vector
 * with the given vector.
 */
Vector3D Vector3D::operator%(const Vector3D &vector) const
{
  return Vector3D(y*vector.z - z * vector.y,
    z*vector.x - x * vector.z,
    x*vector.y - y * vector.x);
}


real Vector3D::scalarProduct(const Vector3D &vector) const
{
  return x * vector.x + y * vector.y + z * vector.z;
}


real Vector3D::operator *(const Vector3D &vector) const
{
  return x * vector.x + y * vector.y + z * vector.z;
}


void Vector3D::addScaledVector(const Vector3D& vector, real scale)
{
  x += vector.x * scale;
  y += vector.y * scale;
  z += vector.z * scale;
}

/** Limits the size of the vector to the given maximum. */
void Vector3D::trim(real size)
{
  if (SquareLength() > size*size)
  {
    normalise();
    x *= size;
    y *= size;
    z *= size;
  }
}

/** Turns a non-zero vector into a vector of unit length. */
void Vector3D::normalise()
{
  real l = Length();
  if (l > 0)
  {
    (*this) *= ((real)1) / l;
  }
}

/** Returns the normalised version of a vector. */
Vector3D Vector3D::unit() const
{
  Vector3D result = *this;
  result.normalise();
  return result;
}


bool Vector3D::operator<(const Vector3D& other) const
{
  return x < other.x && y < other.y && z < other.z;
}

bool Vector3D::operator>(const Vector3D& other) const
{
  return x > other.x && y > other.y && z > other.z;
}


bool Vector3D::operator<=(const Vector3D& other) const
{
  return x <= other.x && y <= other.y && z <= other.z;
}


bool Vector3D::operator>=(const Vector3D& other) const
{
  return x >= other.x && y >= other.y && z >= other.z;
}

#pragma endregion

Vector3D operator*(const float scalar, const Vector3D& other)
{
	return Vector3D(
		other.x * scalar,
		other.y * scalar,
		other.z * scalar,
		other.w * scalar
	);
}

std::ostream & operator<<(std::ostream & out, const Vector3D & vec)
{
	out << "X: " << vec.x << ", Y: " << vec.y << ", Z: " << vec.z;
	return out;
}


