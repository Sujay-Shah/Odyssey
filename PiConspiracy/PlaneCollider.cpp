//
//  PlaneCollider.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#include "PiEnginePreCompiled.h"
#include "PlaneCollider.h"

PlaneCollider::PlaneCollider()
	: BaseCollider(EColliderShape::Plane)
{
}

PlaneCollider::~PlaneCollider()
{
}

void PlaneCollider::SetRBOwnerState(Vector3D direction_, real offset_)
{
	direction = direction_;
	offset = offset_;
}
