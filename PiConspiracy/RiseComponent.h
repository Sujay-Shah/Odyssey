//
//  RiseComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class RiseComponent : public Component
{
	COMPONENT_DECLARE(RiseComponent);
public:

	~RiseComponent(void);

	RiseComponent(float a_speed);

	void Initialize() override final;
	void Update(float a_dTime) override final;
	void Serialize(const json&) override final;
	void HandleEvent(Event* a_pEvent) override final;

	virtual void Deactivate();

	void SetSpeed(float a_speed);
	float GetSpeed(void);
	// Inherited via Component
	virtual void OverrideParams(const json&, float offset) override;

	bool isRise;
private:

	Transform* m_transform;
	RigidBodyComponent* m_RigidBody;
	float m_speed;
	vec3 m_pos;
	vec3 scaleValue;
	bool shake;



	

};

