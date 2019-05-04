//
//  Rotations.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class Rotations : public Component
{
	COMPONENT_DECLARE(Rotations);
public:

	~Rotations(void);

	void Initialize() override final;
	void Update(float a_dTime) override final;
	void Serialize(const json&) override final;
	void HandleEvent(Event* a_pEvent) override final;

	virtual void Deactivate();

	// Inherited via Component
	virtual void OverrideParams(const json&,float offset) override;
private:

	uint32 m_axis; // x = 1, y = 2, z = 3
	float m_rotRate;

	vec3 m_rotation;

	Transform* m_pTrans;
};

