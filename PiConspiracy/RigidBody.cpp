#include "PiEnginePreCompiled.h"

using namespace physics;
//using namespace physics;

/**
 * Inzernal function that checks the validity of an inverse inertia tensor.
 * Perform a validity check in an assert.
 */
static inline void _checkInverseInertiaTensor(const Matrix3x3 &iitWorld)
{
}

/**
 * Internal function to do an intertia tensor transform by a quaternion.
 * Note that the implementation of this function was created by an
 * automated code-generator and optimizer.
 */
static inline void _transformInertiaTensor(Matrix3x3 &iitWorld,
                                           const Quaternion &q,
                                           const Matrix3x3 &iitBody,
                                           const Matrix4x4 &rotmat)
{
    real t4 = rotmat.m_matrix[0][0]*iitBody.m_matrix[0][0]+
        rotmat.m_matrix[0][1]*iitBody.m_matrix[1][0]+
        rotmat.m_matrix[0][2]*iitBody.m_matrix[2][0];
    real t9 = rotmat.m_matrix[0][0]*iitBody.m_matrix[0][1]+
        rotmat.m_matrix[0][1]*iitBody.m_matrix[1][1]+
        rotmat.m_matrix[0][2]*iitBody.m_matrix[2][1];
    real t14 = rotmat.m_matrix[0][0]*iitBody.m_matrix[0][2]+
        rotmat.m_matrix[0][1]*iitBody.m_matrix[1][2]+
        rotmat.m_matrix[0][2]*iitBody.m_matrix[2][2];
    real t28 = rotmat.m_matrix[1][0]*iitBody.m_matrix[0][0]+
        rotmat.m_matrix[1][1]*iitBody.m_matrix[1][0]+
        rotmat.m_matrix[1][2]*iitBody.m_matrix[2][0];
    real t33 = rotmat.m_matrix[1][0]*iitBody.m_matrix[0][1]+
        rotmat.m_matrix[1][1]*iitBody.m_matrix[1][1]+
        rotmat.m_matrix[1][2]*iitBody.m_matrix[2][1];
    real t38 = rotmat.m_matrix[1][0]*iitBody.m_matrix[0][2]+
        rotmat.m_matrix[1][1]*iitBody.m_matrix[1][2]+
        rotmat.m_matrix[1][2]*iitBody.m_matrix[2][2];
    real t52 = rotmat.m_matrix[2][0]*iitBody.m_matrix[0][0]+
        rotmat.m_matrix[2][1]*iitBody.m_matrix[1][0]+
        rotmat.m_matrix[2][2]*iitBody.m_matrix[2][0];
    real t57 = rotmat.m_matrix[2][0]*iitBody.m_matrix[0][1]+
        rotmat.m_matrix[2][1]*iitBody.m_matrix[1][1]+
        rotmat.m_matrix[2][2]*iitBody.m_matrix[2][1];
    real t62 = rotmat.m_matrix[2][0]*iitBody.m_matrix[0][2]+
        rotmat.m_matrix[2][1]*iitBody.m_matrix[1][2]+
        rotmat.m_matrix[2][2]*iitBody.m_matrix[2][2];

    iitWorld.m_matrix[0][0] = t4*rotmat.m_matrix[0][0]+
        t9*rotmat.m_matrix[0][1]+
        t14*rotmat.m_matrix[0][2];
    iitWorld.m_matrix[0][1] = t4*rotmat.m_matrix[1][0]+
        t9*rotmat.m_matrix[1][1]+
        t14*rotmat.m_matrix[1][2];
    iitWorld.m_matrix[0][2] = t4*rotmat.m_matrix[2][0]+
        t9*rotmat.m_matrix[2][1]+
        t14*rotmat.m_matrix[2][2];
    iitWorld.m_matrix[1][0] = t28*rotmat.m_matrix[0][0]+
        t33*rotmat.m_matrix[0][1]+
        t38*rotmat.m_matrix[0][2];
    iitWorld.m_matrix[1][1] = t28*rotmat.m_matrix[1][0]+
        t33*rotmat.m_matrix[1][1]+
        t38*rotmat.m_matrix[1][2];
    iitWorld.m_matrix[1][2] = t28*rotmat.m_matrix[2][0]+
        t33*rotmat.m_matrix[2][1]+
        t38*rotmat.m_matrix[2][2];
    iitWorld.m_matrix[2][0] = t52*rotmat.m_matrix[0][0]+
        t57*rotmat.m_matrix[0][1]+
        t62*rotmat.m_matrix[0][2];
    iitWorld.m_matrix[2][1] = t52*rotmat.m_matrix[1][0]+
        t57*rotmat.m_matrix[1][1]+
        t62*rotmat.m_matrix[1][2];
    iitWorld.m_matrix[2][2] = t52*rotmat.m_matrix[2][0]+
        t57*rotmat.m_matrix[2][1]+
        t62*rotmat.m_matrix[2][2];
}

/**
 * Inline function that creates a transform matrix from a
 * position and orientation.
 */
static inline void _calculateTransformMatrix(Matrix4x4 &transformMatrix,
                                             const Vector3D &position,
                                             const Quaternion &orientation)
{
    transformMatrix.m_matrix[0][0] = 1-2*orientation.j*orientation.j-
        2*orientation.k*orientation.k;
    transformMatrix.m_matrix[0][1] = 2*orientation.i*orientation.j -
        2*orientation.r*orientation.k;
    transformMatrix.m_matrix[0][2] = 2*orientation.i*orientation.k +
        2*orientation.r*orientation.j;
    transformMatrix.m_matrix[0][3] = position.x;

    transformMatrix.m_matrix[1][0] = 2*orientation.i*orientation.j +
        2*orientation.r*orientation.k;
    transformMatrix.m_matrix[1][1] = 1-2*orientation.i*orientation.i-
        2*orientation.k*orientation.k;
    transformMatrix.m_matrix[1][2] = 2*orientation.j*orientation.k -
        2*orientation.r*orientation.i;
    transformMatrix.m_matrix[1][3] = position.y;

    transformMatrix.m_matrix[2][0] = 2*orientation.i*orientation.k -
        2*orientation.r*orientation.j;
    transformMatrix.m_matrix[2][1] = 2*orientation.j*orientation.k +
        2*orientation.r*orientation.i;
    transformMatrix.m_matrix[2][2] = 1-2*orientation.i*orientation.i-
        2*orientation.j*orientation.j;
    transformMatrix.m_matrix[2][3] = position.z;
}


void RigidBody::calculateDerivedData()
{
    orientation.normalise();

    // Calculate the transform matrix for the body.
    _calculateTransformMatrix(transformMatrix, position, orientation);

    // Calculate the inertiaTensor in world space.
    _transformInertiaTensor(inverseInertiaTensorWorld,
        orientation,
        inverseInertiaTensor,
        transformMatrix);

}

void RigidBody::integrate(real duration)
{
    if (!isAwake) return;

    // Calculate linear acceleration from force inputs.
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration.addScaledVector(forceAccum, inverseMass);

    // Calculate angular acceleration from torque inputs.
    Vector3D angularAcceleration =
        inverseInertiaTensorWorld.transform(torqueAccum);

    // Adjust velocities
    // Update linear velocity from both acceleration and impulse.
    velocity.addScaledVector(lastFrameAcceleration, duration);

    // Update angular velocity from both acceleration and impulse.
    rotation.addScaledVector(angularAcceleration, duration);

    // Impose drag.
    velocity *= real_pow(linearDamping, duration);
    rotation *= real_pow(angularDamping, duration);

    // Adjust positions
    // Update linear position.
    position.addScaledVector(velocity, duration);

    // Update angular position.
    orientation.addScaledVector(rotation, duration);

    // Normalise the orientation, and update the matrices with the new
    // position and orientation
    calculateDerivedData();

    // Clear accumulators.
    clearAccumulators();

    // Update the kinetic energy store, and possibly put the body to
    // sleep.
    if (canSleep) {
        real currentMotion = velocity.scalarProduct(velocity) +
            rotation.scalarProduct(rotation);

        real bias = real_pow(0.5f, duration);
        motion = bias*motion + (1-bias)*currentMotion;

        if (motion < sleepEpsilon) setAwake(false);
        else if (motion > 10 * sleepEpsilon) motion = 10 * sleepEpsilon;
    }
}

void RigidBody::setMass(const real mass)
{
    assert(mass != 0);
    RigidBody::inverseMass = ((real)1.0)/mass;
}

real RigidBody::getMass() const
{
    if (inverseMass == 0) {
        return REAL_MAX;
    } else {
        return ((real)1.0)/inverseMass;
    }
}

void RigidBody::setInverseMass(const real inverseMass)
{
    RigidBody::inverseMass = inverseMass;
}

real RigidBody::getInverseMass() const
{
    return inverseMass;
}

bool RigidBody::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
}

void RigidBody::setInertiaTensor(const Matrix3x3 &inertiaTensor)
{
    inverseInertiaTensor.setInverse(inertiaTensor);
    _checkInverseInertiaTensor(inverseInertiaTensor);
}

void RigidBody::getInertiaTensor(Matrix3x3 *inertiaTensor) const
{
    inertiaTensor->setInverse(inverseInertiaTensor);
}

Matrix3x3 RigidBody::getInertiaTensor() const
{
  Matrix3x3 it = Matrix3x3(0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f);;
    getInertiaTensor(&it);
    return it;
}

void RigidBody::getInertiaTensorWorld(Matrix3x3 *inertiaTensor) const
{
    inertiaTensor->setInverse(inverseInertiaTensorWorld);
}

Matrix3x3 RigidBody::getInertiaTensorWorld() const
{
  Matrix3x3 it = Matrix3x3(0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f);;
    getInertiaTensorWorld(&it);
    return it;
}

void RigidBody::setInverseInertiaTensor(const Matrix3x3 &inverseInertiaTensor)
{
    _checkInverseInertiaTensor(inverseInertiaTensor);
    RigidBody::inverseInertiaTensor = inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensor(Matrix3x3 *inverseInertiaTensor) const
{
    *inverseInertiaTensor = RigidBody::inverseInertiaTensor;
}

Matrix3x3 RigidBody::getInverseInertiaTensor() const
{
    return inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensorWorld(Matrix3x3 *inverseInertiaTensor) const
{
    *inverseInertiaTensor = inverseInertiaTensorWorld;
}

Matrix3x3 RigidBody::getInverseInertiaTensorWorld() const
{
    return inverseInertiaTensorWorld;
}

void RigidBody::setDamping(const real linearDamping,
               const real angularDamping)
{
    RigidBody::linearDamping = linearDamping;
    RigidBody::angularDamping = angularDamping;
}

void RigidBody::setLinearDamping(const real linearDamping)
{
    RigidBody::linearDamping = linearDamping;
}

real RigidBody::getLinearDamping() const
{
    return linearDamping;
}

void RigidBody::setAngularDamping(const real angularDamping)
{
    RigidBody::angularDamping = angularDamping;
}

real RigidBody::getAngularDamping() const
{
    return angularDamping;
}

void RigidBody::setPosition(const Vector3D &position)
{
    RigidBody::position = position;
}

void RigidBody::setPosition(const real x, const real y, const real z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void RigidBody::getPosition(Vector3D *position) const
{
    *position = RigidBody::position;
}

Vector3D RigidBody::getPosition() const
{
    return position;
}

void RigidBody::setOrientation(const Quaternion &orientation)
{
    RigidBody::orientation = orientation;
    RigidBody::orientation.normalise();
}

void RigidBody::setOrientation(const real r, const real i,
                   const real j, const real k)
{
    orientation.r = r;
    orientation.i = i;
    orientation.j = j;
    orientation.k = k;
    orientation.normalise();
}

void RigidBody::getOrientation(Quaternion *orientation) const
{
    *orientation = RigidBody::orientation;
}

Quaternion RigidBody::getOrientation() const
{
    return orientation;
}

void RigidBody::getOrientation(Matrix3x3 *matrix) const
{
  //matrix->test = 0;
    getOrientation(matrix->m_matrix);
}

void RigidBody::getOrientation(real matrix[3][3]) const
{
    matrix[0][0] = transformMatrix.m_matrix[0][0];
    matrix[0][1] = transformMatrix.m_matrix[0][1];
    matrix[0][2] = transformMatrix.m_matrix[0][2];

    matrix[1][0] = transformMatrix.m_matrix[1][0];
    matrix[1][1] = transformMatrix.m_matrix[1][1];
    matrix[1][2] = transformMatrix.m_matrix[1][2];

    matrix[2][0] = transformMatrix.m_matrix[2][0];
    matrix[2][1] = transformMatrix.m_matrix[2][1];
    matrix[2][2] = transformMatrix.m_matrix[2][2];
}

void RigidBody::getTransform(Matrix4x4 *transform) const
{
   // transform->test = 0;
    memcpy(transform, &transformMatrix.m_matrix, sizeof(Matrix4x4));
}


void RigidBody::getTransform(real matrix[16]) const
{
    memcpy(matrix, transformMatrix.m_matrix, sizeof(real)*12);
    matrix[12] = matrix[13] = matrix[14] = 0;
    matrix[15] = 1;
}

void RigidBody::getGLTransform(float matrix[16]) const
{
    matrix[0] = (float)transformMatrix.m_matrix[0][0];
    matrix[1] = (float)transformMatrix.m_matrix[1][0];
    matrix[2] = (float)transformMatrix.m_matrix[2][0];
    matrix[3] = 0;

    matrix[4] = (float)transformMatrix.m_matrix[0][1];
    matrix[5] = (float)transformMatrix.m_matrix[1][1];
    matrix[6] = (float)transformMatrix.m_matrix[2][1];
    matrix[7] = 0;

    matrix[8] = (float)transformMatrix.m_matrix[0][2];
    matrix[9] = (float)transformMatrix.m_matrix[1][2];
    matrix[10] = (float)transformMatrix.m_matrix[2][2];
    matrix[11] = 0;

    matrix[12] = (float)transformMatrix.m_matrix[0][3];
    matrix[13] = (float)transformMatrix.m_matrix[1][3];
    matrix[14] = (float)transformMatrix.m_matrix[2][3];
    matrix[15] = 1;
}

Matrix4x4 RigidBody::getTransform() const
{
    return transformMatrix;
}


Vector3D RigidBody::getPointInLocalSpace(const Vector3D &point) const
{
    return transformMatrix.transformInverse(point);
}

Vector3D RigidBody::getPointInWorldSpace(const Vector3D &point) const
{
    return transformMatrix.transform(point);
}

Vector3D RigidBody::getDirectionInLocalSpace(const Vector3D &direction) const
{
    return transformMatrix.transformInverseDirection(direction);
}

Vector3D RigidBody::getDirectionInWorldSpace(const Vector3D &direction) const
{
    return transformMatrix.transformDirection(direction);
}


void RigidBody::setVelocity(const Vector3D &velocity)
{
    RigidBody::velocity = velocity;
}

void RigidBody::setVelocity(const real x, const real y, const real z)
{
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void RigidBody::getVelocity(Vector3D *velocity) const
{
    *velocity = RigidBody::velocity;
}

Vector3D RigidBody::getVelocity() const
{
    return velocity;
}

void RigidBody::addVelocity(const Vector3D &deltaVelocity)
{
    velocity += deltaVelocity;
}


void RigidBody::addVelocity( vec3 &deltaVelocity)
{
	
	velocity += Vector3D(deltaVelocity);
}


void RigidBody::setRotation(const Vector3D &rotation)
{
    RigidBody::rotation = rotation;
}

void RigidBody::setRotation(const real x, const real y, const real z)
{
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
}

void RigidBody::getRotation(Vector3D *rotation) const
{
    *rotation = RigidBody::rotation;
}

Vector3D RigidBody::getRotation() const
{
    return rotation;
}

void RigidBody::addRotation(const Vector3D &deltaRotation)
{
    rotation += deltaRotation;
}



void RigidBody::setAwake(const bool awake)
{
    if (awake) {
        isAwake= true;

        // Add a bit of motion to avoid it falling asleep immediately.
        motion = sleepEpsilon*2.0f;
    } else {
        isAwake = false;
        velocity.Zero();
        rotation.Zero();
    }
}

inline bool physics::RigidBody::getCanSleep() const
{
  return canSleep;
}

void RigidBody::setCanSleep(const bool canSleep)
{
    RigidBody::canSleep = canSleep;

    if (!canSleep && !isAwake) setAwake();
}


void RigidBody::getLastFrameAcceleration(Vector3D *acceleration) const
{
    *acceleration = lastFrameAcceleration;
}

Vector3D RigidBody::getLastFrameAcceleration() const
{
    return lastFrameAcceleration;
}

void RigidBody::clearAccumulators()
{
    forceAccum.Zero();
    torqueAccum.Zero();
}

void RigidBody::addForce(const Vector3D &force)
{
    forceAccum += force;
    isAwake = true;
}

void RigidBody::addForceAtBodyPoint(const Vector3D &force,
                                    const Vector3D &point)
{
    // Convert to coordinates relative to center of mass.
    Vector3D pt = getPointInWorldSpace(point);
    addForceAtPoint(force, pt);

}

void RigidBody::addForceAtPoint(const Vector3D &force,
                                const Vector3D &point)
{
    // Convert to coordinates relative to center of mass.
    Vector3D pt = point;
    pt -= position;

    forceAccum += force;
    torqueAccum += pt % force;

    isAwake = true;
}

void RigidBody::addTorque(const Vector3D &torque)
{
    torqueAccum += torque;
    isAwake = true;
}

void RigidBody::setAcceleration(const Vector3D &acceleration)
{
    RigidBody::acceleration = acceleration;
}

void RigidBody::setAcceleration(const real x, const real y, const real z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

void RigidBody::getAcceleration(Vector3D *acceleration) const
{
    *acceleration = RigidBody::acceleration;
}

Vector3D RigidBody::getAcceleration() const
{
    return acceleration;
}


void PrintMatrix4x4(const Matrix4x4 & mat)
{
  cout << std::setprecision(2);
  cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout << " " << mat.m_matrix[0][0] << " " << mat.m_matrix[0][1] << " " << mat.m_matrix[0][2] << " " << mat.m_matrix[0][3] << std::endl
    << " " << mat.m_matrix[1][0] << " " << mat.m_matrix[1][1] << " " << mat.m_matrix[1][2] << " " << mat.m_matrix[1][3] << std::endl
    << " " << mat.m_matrix[2][0] << " " << mat.m_matrix[2][1] << " " << mat.m_matrix[2][2] << " " << mat.m_matrix[2][3] << std::endl
    << " " << mat.m_matrix[3][0] << " " << mat.m_matrix[3][1] << " " << mat.m_matrix[3][2] << " " << mat.m_matrix[3][3] << std::endl;
  std::cout <<"----------------------------------------" << std::endl;
}
Vector3D RotationMatrixToEulerAngles(Matrix4x4 R)
{
  float sy = sqrt(R.m_matrix[0][0] * R.m_matrix[0][0] + R.m_matrix[1][0] * R.m_matrix[1][0]);

  bool singular = sy < 1e-6; // If

  float x, y, z;
  if (!singular)
  {
    x = atan2(R.m_matrix[2][1], R.m_matrix[2][2]);
    y = atan2(-R.m_matrix[2][0], sy);
    z = atan2(R.m_matrix[1][0], R.m_matrix[0][0]);
  }
  else
  {
    x = atan2(-R.m_matrix[1][2], R.m_matrix[1][1]);
    y = atan2(-R.m_matrix[2][0], sy);
    z = 0;
  }
  return Vector3D(x, y, z);
}

glm::quat RotationMatrixToQuat(const Matrix4x4 &R)
{
  // Output quaternion
  /*float w, x, y, z;
  // Determine which of w,x,y, or z has the largest absolute value
  float fourWSquaredMinus1 = R.m_matrix[1][1] + R.m_matrix[2][2] + R.m_matrix[3][3];
  float fourXSquaredMinus1 = R.m_matrix[1][1] - R.m_matrix[2][2] - R.m_matrix[3][3];
  float fourYSquaredMinus1 = R.m_matrix[2][2] - R.m_matrix[1][1] - R.m_matrix[3][3];
  float fourZSquaredMinus1 = R.m_matrix[3][3] - R.m_matrix[1][1] - R.m_matrix[2][2];

  int biggestIndex = 0;
  float fourBiggestSquaredMinus1 = fourWSquaredMinus1;

  if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
    fourBiggestSquaredMinus1 = fourXSquaredMinus1;
    biggestIndex = 1;
  }
  if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
    fourBiggestSquaredMinus1 = fourYSquaredMinus1;
    biggestIndex = 2;
  }
  if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
    fourBiggestSquaredMinus1 = fourZSquaredMinus1;
    biggestIndex = 3;
  }
  // Per form square root and division
  float biggestVal = sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
  float mult = 0.25f / biggestVal;

  // Apply table to compute quaternion values
  switch (biggestIndex) {
  case 0:
    w = biggestVal;
    x = (R.m_matrix[2][3] - R.m_matrix[3][2]) * mult;
    y = (R.m_matrix[3][1] - R.m_matrix[1][3]) * mult;
    z = (R.m_matrix[1][2] - R.m_matrix[2][1]) * mult;
    break;
  case 1:
    x = biggestVal;
    w = (R.m_matrix[2][3] - R.m_matrix[3][2]) * mult;
    y = (R.m_matrix[1][2] + R.m_matrix[2][1]) * mult;
    z = (R.m_matrix[3][1] + R.m_matrix[1][3]) * mult;
    break;
  case 2:
    y = biggestVal;
    w = (R.m_matrix[3][1] - R.m_matrix[1][3]) * mult;
    x = (R.m_matrix[1][2] + R.m_matrix[2][1]) * mult;
    z = (R.m_matrix[2][3] + R.m_matrix[3][2]) * mult;
    break;
  case 3:
    z = biggestVal;
    w = (R.m_matrix[1][2] - R.m_matrix[2][1]) * mult;
    x = (R.m_matrix[3][1] + R.m_matrix[1][3]) * mult;
    y = (R.m_matrix[2][3] + R.m_matrix[3][2]) * mult;
    break;

    float quatX = x;
    float quatY = y;
    float quatZ = z;
    float quatW = w;
    //return glm::quat(quatX, quatY, quatZ, quatW);
    //float model = vec4(quatX, quatY, quatZ, quatW);
  }*/

  float m11 = R.m_matrix[0][0], m12 = R.m_matrix[0][1], m13 = R.m_matrix[0][2];
  float m21 = R.m_matrix[1][0], m22 = R.m_matrix[1][1], m23 = R.m_matrix[1][2];
  float m31 = R.m_matrix[2][0], m32 = R.m_matrix[2][1], m33 = R.m_matrix[2][2];

  /*float m11 = 0.96, m12 = -0.27, m13 = 0;
  float m21 = 0.27, m22 = 0.96, m23 = 0;
  float m31 = 0, m32 = 0, m33 = 1;*/

  // Output quaternion
  float w, x, y, z;
  // Determine which of w,x,y, or z has the largest absolute value
  float fourWSquaredMinus1 = m11 + m22 + m33;
  float fourXSquaredMinus1 = m11 - m22 - m33;
  float fourYSquaredMinus1 = m22 - m11 - m33;
  float fourZSquaredMinus1 = m33 - m11 - m22;

  int biggestIndex = 0;
  float fourBiggestSquaredMinus1 = fourWSquaredMinus1;

  if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
    fourBiggestSquaredMinus1 = fourXSquaredMinus1;
    biggestIndex = 1;
  }
  if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
    fourBiggestSquaredMinus1 = fourYSquaredMinus1;
    biggestIndex = 2;
  }
  if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
    fourBiggestSquaredMinus1 = fourZSquaredMinus1;
    biggestIndex = 3;
  }
  // Per form square root and division
  float biggestVal = sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
  float mult = 0.25f / biggestVal;

  // Apply table to compute quaternion values
  switch (biggestIndex) {
  case 0:
    w = biggestVal;
    x = (m23 - m32) * mult;
    y = (m31 - m13) * mult;
    z = (m12 - m21) * mult;
    break;
  case 1:
    x = biggestVal;
    w = (m23 - m32) * mult;
    y = (m12 + m21) * mult;
    z = (m31 + m13) * mult;
    break;
  case 2:
    y = biggestVal;
    w = (m31 - m13) * mult;
    x = (m12 + m21) * mult;
    z = (m23 + m32) * mult;
    break;
  case 3:
    z = biggestVal;
    w = (m12 - m21) * mult;
    x = (m31 + m13) * mult;
    y = (m23 + m32) * mult;
    break;

    float quatX = x;
    float quatY = y;
    float quatZ = z;
    float quatW = w;

    //model = vec4(quatX, quatY, quatZ, quatW);
  }
  return glm::quat(w, x, y, z);
}



/*glm::quat EulerAnglesToQuaternion(Vector3D angle)
{
  glm::quat result;


  float yaw = angle.z;
  float roll = angle.x;
  float pitch = angle.y;


  float cy = cos(yaw * 0.5f);
  float sy = sin(yaw  * 0.5f);
  float cr = cos(roll   * 0.5f);
  float sr = sin(roll   * 0.5f);
  float cp = cos(pitch  * 0.5f);
  float sp = sin(pitch  * 0.5f);


  result.w = cy * cr * cp + sy * sr * sp;
  result.x = cy * sr * cp - sy * cr * sp;
  result.y = cy * cr * sp + sy * sr * cp;
  result.z = sy * cr * cp - cy * sr * sp;
  return result;

}

glm::quat EulerAnglesToQuaternion(vec3 angle)
{
  return EulerAnglesToQuaternion(Vector3D(angle));

}*/