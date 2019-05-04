//
//  LightComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "LightComponent.h"
#include "LightManager.h"
#include "Renderer.h"


LightComponent::LightComponent()
	: Component(STRINGIFY(LightComponent))
{
}


LightComponent::~LightComponent()
{
}

void LightComponent::Initialize()
{
	
}

void LightComponent::Update(float a_dTime)
{
}

void LightComponent::Serialize(const json &read_value)
{
	//Read from Json
	LightProperties light_properties;
	LightManager* lightmanagerObj = PiRenderer->GetLightManager();

	light_properties.enumLightType = ParseInt(read_value, "Light_ID");

	if (light_properties.enumLightType == lightmanagerObj->LightType::LIGHT_DIRECTIONAL_)
	{

		light_properties.DirectionalColor.x = ParseFloat(read_value["Color"], "x");
		light_properties.DirectionalColor.y = ParseFloat(read_value["Color"], "y");
		light_properties.DirectionalColor.z = ParseFloat(read_value["Color"], "z");
		
		m_lightColor = light_properties.DirectionalColor;

		light_properties.DirectionalDir.x = ParseFloat(read_value["Direction"], "x");
		light_properties.DirectionalDir.y = ParseFloat(read_value["Direction"], "y");
		light_properties.DirectionalDir.z = ParseFloat(read_value["Direction"], "z");
		light_properties.DirectionalDir.w = ParseFloat(read_value["Direction"], "w");
		
		m_lightDirection = light_properties.DirectionalDir;
		

	}

	if (light_properties.enumLightType == lightmanagerObj->LightType::LIGHT_POINT_)
	{
		light_properties.PointColor.x = ParseFloat(read_value["Color"], "x");
		light_properties.PointColor.y = ParseFloat(read_value["Color"], "y");
		light_properties.PointColor.z = ParseFloat(read_value["Color"], "z");

		light_properties.PointPosition.x = ParseFloat(read_value["Position"], "x");
		light_properties.PointPosition.y = ParseFloat(read_value["Position"], "y");
		light_properties.PointPosition.z = ParseFloat(read_value["Position"], "z");
		light_properties.PointPosition.w = ParseFloat(read_value["Position"], "w");

		light_properties.pointRadius = ParseFloat(read_value, "Radius");
	}

	if (light_properties.enumLightType == lightmanagerObj->LightType::LIGHT_SPOT_)
	{
		light_properties.SpotColor.x = ParseFloat(read_value["Color"], "x");
		light_properties.SpotColor.y = ParseFloat(read_value["Color"], "y");
		light_properties.SpotColor.z = ParseFloat(read_value["Color"], "z");

		light_properties.SpotPosition.x = ParseFloat(read_value["Position"], "x");
		light_properties.SpotPosition.y = ParseFloat(read_value["Position"], "y");
		light_properties.SpotPosition.z = ParseFloat(read_value["Position"], "z");
		light_properties.SpotPosition.w = ParseFloat(read_value["Position"], "1");

		light_properties.SpotConeRatio = ParseFloat(read_value, "ConeRatio");

		light_properties.SpotIntensity = ParseFloat(read_value, "Intensity");
	}

	//call fill the container here

	lightmanagerObj->FilltheContainer(light_properties);
	return;

}

void LightComponent::HandleEvent(Event * a_pEvent)
{
}

void LightComponent::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
}

void LightComponent::OverrideParams(const json&,float)
{
}
