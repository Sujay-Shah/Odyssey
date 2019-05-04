//
//  LightComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class LightComponent : public Component
{
	COMPONENT_DECLARE(LightComponent);
public:
	//LightComponent();
	~LightComponent(void);

	void Initialize() override final;
	void Update(float a_dTime) override final;
	void Serialize(const json&) override final;
	void HandleEvent(Event* a_pEvent) override final;


	// Inherited via Component
	virtual void Deactivate(void) override;


	// Inherited via Component
	virtual void OverrideParams(const json&,float) override;

	inline int getLightID()
	{
		return m_lightID;
	}

	inline vec3 getLightColor()
	{
		return m_lightColor;
	}

	inline vec4 getLightDirection()
	{
		return m_lightDirection;
	}

	inline void setLightColor(vec3 setColor)
	{
		m_lightColor = setColor;
	}

	inline void setLightDirection(vec4 setDirection)
	{
		m_lightDirection = setDirection;
	}


private:
	int		m_lightID;
	vec3	m_lightColor;
	vec4	m_lightDirection;


};

