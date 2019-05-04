//
//  NPC_RotateComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class NPC_RotateComponent : public Component
{
	COMPONENT_DECLARE(NPC_RotateComponent);
public:

	~NPC_RotateComponent(void);

	NPC_RotateComponent(float a_speed);

	void Initialize() override final;
	void Update(float a_dTime) override final;
	void Serialize(const json&) override final;
	void HandleEvent(Event* a_pEvent) override final;

	virtual void Deactivate();

	void SetSpeed(float a_speed);


	float GetSpeed(void);
	// Inherited via Component
	virtual void OverrideParams(const json&, float offset) override;
private:


	float m_speed;
	Transform* m_pTrans;



	

};

