//
//  Shoot.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class Shoot : public Component
{
	COMPONENT_DECLARE(Shoot);
public:
	~Shoot();

	// Inherited via Component
	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;
	virtual void OverrideParams(const json &,float) override;

private:
	float muzzle_velocity;
	glm::vec2 cursorPos;
	glm::vec3 half;
	float m_forceXZ;
	float m_forceY;
};

