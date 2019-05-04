//
//  BaseCollider.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#pragma once
struct ColliderShape
{
	enum Enum
	{
		Box,
		Sphere,
		Plane,

		NUM
	};
};
TYPEDEF_ENUM(ColliderShape);

class BaseCollider
{
public:
	BaseCollider(EColliderShape a_shape)
		: m_shape(a_shape),
		m_rbOwner(nullptr),
		m_activeFlag(false){};
	virtual ~BaseCollider(void) {};

	Matrix4x4 offset = Matrix4x4(1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 0.f);

	void CalPhyInner(void);
	Vector3D GetCool(collision_flag flag) const;
	const Matrix4x4& GetTransform(void) const;

	inline void SetRBOwner(RigidBodyComponent* a_rbComponent) { m_rbOwner = a_rbComponent; }
	inline RigidBodyComponent* GetRBOwner(void) const { return m_rbOwner; }

	inline EColliderShape GetColliderShape(void) const { return m_shape; }

	// for memory manager use
	inline void SetActiveFlag(bool a_activeFlag) { m_activeFlag = a_activeFlag; }
	inline bool GetActiveFlag(void) { return m_activeFlag; }

protected:
	Matrix4x4 m_transform = Matrix4x4(1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 0.f);

	RigidBodyComponent* m_rbOwner;
	EColliderShape m_shape;
	bool m_activeFlag;
};

