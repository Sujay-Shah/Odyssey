//
//  AIComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class AIComponent :
	public Component
{
	COMPONENT_DECLARE(AIComponent);
public:
	~AIComponent();

	// Inherited via Component
	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;
	
	virtual void OverrideParams(const json &, float) override;
	virtual void ImguiUpdate(float a_dt);
private:
	bool _isDrop;
	float m_offset;
	float m_playerZ;
	float m_AiZ;
	float m_zSpeed, m_xSpeed, m_ySpeed;
	bool m_isFriendly;
	RigidBodyComponent* m_rigid;
	long m_count;
};

