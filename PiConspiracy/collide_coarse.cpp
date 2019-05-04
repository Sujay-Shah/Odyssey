#include "PiEnginePreCompiled.h"
using namespace physics;


BoundingTypeSphere::BoundingTypeSphere(const Vector3D &centre, real radius)
{
  BoundingTypeSphere::centre_ = centre;
  BoundingTypeSphere::radius_ = radius;
}
real BoundingTypeSphere::Growth(const BoundingTypeSphere &other) const
{
  BoundingTypeSphere temp(*this, other);
  return temp.radius_*temp.radius_ - radius_ * radius_;
}
bool BoundingTypeSphere::BoundingSphereOverlaps(const BoundingTypeSphere *other) const
{
  return ((centre_ - other->centre_).SquareLength()) < (radius_ + other->radius_)*(radius_ + other->radius_);
}

BoundingTypeSphere::BoundingTypeSphere(const BoundingTypeSphere &First,
  const BoundingTypeSphere &second)
{

  real dis = (second.centre_ - First.centre_).SquareLength();
  real DiffR = second.radius_ - First.radius_;

  if (DiffR*DiffR >= dis)
  {
    compare(First, second);
  }

  else
  {
    dis = real_sqrt(dis);
    radius_ = (dis + First.radius_ + second.radius_) * (0.5f);

    centre_ = First.centre_;
    if (dis > 0)
    {
      centre_ += (second.centre_ - First.centre_) * ((radius_ - First.radius_) / dis);
    }
  }

}
void physics::BoundingTypeSphere::compare(const physics::BoundingTypeSphere & First, const physics::BoundingTypeSphere & second)
{
  if (First.radius_ <= second.radius_)
  {
    centre_ = second.centre_;
    radius_ = second.radius_;
  }
  else
  {
    centre_ = First.centre_;
    radius_ = First.radius_;

  }
}
real BoundingTypeSphere::getSize() const
{
  return 1.33333333f * radius_ * radius_ * radius_ * 3.1415926f;
}


