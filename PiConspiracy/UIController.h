//
//  UIController.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "Component.h"
class UIController :
	public Component
{
	COMPONENT_DECLARE(UIController)
	Canvas* m_currentActiveCanvas;
	int m_offset;
	std::pair<float, float> pos;
	bool m_allowSwitch;
public:
	~UIController();

	// Inherited via Component
	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;
	virtual void OverrideParams(const json &, float) override;
};

