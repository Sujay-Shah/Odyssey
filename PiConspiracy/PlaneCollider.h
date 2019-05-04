//
//  PlaneCollider.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#pragma once
class PlaneCollider : public BaseCollider
{
public:
	PlaneCollider();
	~PlaneCollider();

	void SetRBOwnerState(vec3 direction_, real offset_) { SetRBOwnerState(Vector3D(direction_), offset_); }
	Vector3D direction;
	real offset;
private:
	void SetRBOwnerState(Vector3D direction_, real offset_);
};