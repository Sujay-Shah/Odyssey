//
//  ScaleUI.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class ScaleUI :
	public Component
{
	COMPONENT_DECLARE(ScaleUI);
public:
	~ScaleUI();

	// Inherited via Component
	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;
	virtual void OverrideParams(const json &, float) override;

private:
	Transform* m_trans;
	bool m_isValFull;
	float m_maxHt;
	float m_maxCD;
	float m_cdtimer;
	float m_currHt;
	vec3 m_currentScale;
};

