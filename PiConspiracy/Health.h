//
//  Health.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class Health : public Component
{
	COMPONENT_DECLARE(Health);
public:
	~Health();

	// Inherited via Component
	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;
	virtual void OverrideParams(const json &,float) override;

	float inline GetHealth()				{ return m_currentHealth; }
	void inline SetHealth(float a_health)	{ m_currentHealth = a_health; }
	void inline SetHealthToMax()			{ m_currentHealth = m_maxHealth; }

private: 
	float m_currentHealth;
	float m_maxHealth;
};

