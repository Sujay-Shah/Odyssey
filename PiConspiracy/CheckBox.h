//
//  CheckBox.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class CheckBox :
	public Component
{
	COMPONENT_DECLARE(CheckBox);
public:

	~CheckBox();

	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json&) override;
	virtual void HandleEvent(Event* a_pEvent) override;
	virtual void Deactivate(void) override;
	virtual void OverrideParams(const json&, float) override;

	bool m_isVisible;
	bool isFullScreen;
private:
	vec3 m_checkBoxPos;
	EInputType inputType;
	bool m_Ischecked;
	Transform* m_trans;
	std::string m_windowText;
	std::string m_OnClickFunc;
	std::string m_CBname;
	//ECheckboxType m_checkboxType;
	bool pressed;

	bool isMusicMute;
	bool isSFXMute;
	int code;
};

