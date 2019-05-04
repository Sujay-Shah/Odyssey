//
//  NPC_SideMovementComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class NPC_SideMovementComponent : public Component
{
	COMPONENT_DECLARE(NPC_SideMovementComponent);
public:

	~NPC_SideMovementComponent(void);

	NPC_SideMovementComponent(float a_range, float a_speed);

	void Initialize() override final;
	void Update(float a_dTime) override final;
	void Serialize(const json&) override final;
	void HandleEvent(Event* a_pEvent) override final;

	virtual void Deactivate();

	void SetSpeed(float a_speed);
	void SetRange(float a_range);

	float GetSpeed(void);
	float GetRange(void);
	// Inherited via Component
	virtual void OverrideParams(const json&,float offset) override;
private:

	float m_range;
	float m_speed;
	float ticks = 0.0f;
	bool x, y;
	vec3 m_initialPos;
	Transform* m_p;
	RigidBodyComponent* m_pRigidBody;



	

};

