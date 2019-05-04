//
//  BaseCollider.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#include "PiEnginePreCompiled.h"
#include "BaseCollider.h"

void BaseCollider::CalPhyInner()
{
	m_transform = m_rbOwner->getTransform().Phy_multiply_Matrix(offset);
}

Vector3D BaseCollider::GetCool(collision_flag flag) const
{
	return m_transform.getAxisVector(flag);
}

const Matrix4x4 & BaseCollider::GetTransform() const
{
	return m_transform;
}
