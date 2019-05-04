//
//  NPC_DropBallComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class NPC_DropBallComponent : public Component
{
	COMPONENT_DECLARE(NPC_DropBallComponent);

public:

	~NPC_DropBallComponent(void);

	NPC_DropBallComponent(float m_rateOfDrop);

	void Initialize(void) override final;
	void Update(float a_dTime) override final;
	void Serialize(const json&) override final;
	void HandleEvent(Event* a_pEvent) override final;

	virtual void Deactivate();

	virtual void OverrideParams(const json&,float offset) override;

	void SetRateOfDrop(float a_rateOfDrop);
	float GetRateOfDrop(void);

private:

	float m_rateOfDrop;
	GameObject* m_ball;
	vec3 pos;
	bool ballCreated = false;
};

