//
//  Button.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class Button : public Component
{
	COMPONENT_DECLARE(Button)
public:

	~Button();

	// Inherited via Component
	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;

	virtual void OverrideParams(const json &,float offset) override;

	string m_buttonName;
	bool m_isButtonVisible;
	bool m_isPressed;
private:
	string _buttonFunction;
	Transform* m_trans;
	vec3 m_ButtonPos;
};

