//
//  RotateComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class RotateComponent : public Component
{
	COMPONENT_DECLARE(RotateComponent);
public:
	~RotateComponent(void);

	void Initialize() override final;
	void Update(float a_dTime) override final;
	void Serialize(const json& a_j) override final;
	void HandleEvent(Event* a_pEvent) override final;
	void Deactivate(void) override final;
	void OverrideParams(const json &j, float offset) override final;

private:
	RigidBodyComponent *m_rigidBody;
	float m_x, m_y, m_z;
};