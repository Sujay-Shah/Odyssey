//
//  LightManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "LightManager.h"

LightManager::LightManager()
{
	DirectionallightsList.reserve(100);
	PointlightsList.reserve(100);
	SpotlightsList.reserve(100);

}


LightManager::~LightManager()
{
}



void LightManager::FilltheContainer(LightProperties &lightproperties)
{
	if (lightproperties.enumLightType == LIGHT_DIRECTIONAL_)
	{
		Directional_Light dirlight;
		dirlight.DirectionalColor	= lightproperties.DirectionalColor;
		dirlight.DirectionalDir		= lightproperties.DirectionalDir;

		DirectionallightsList.push_back(dirlight);
	}
	
	if (lightproperties.enumLightType == LIGHT_POINT_)
	{
		Point_Light pointlight;
		pointlight.pointColor		= lightproperties.PointColor;
		pointlight.PointPosition	= lightproperties.PointPosition;
		pointlight.pointRadius		= lightproperties.pointRadius;

		PointlightsList.push_back(pointlight);
	}
	
	if (lightproperties.enumLightType == LIGHT_SPOT_)
	{
		Spot_Light spotlight;
		spotlight.SpotColor			= lightproperties.SpotColor;
		spotlight.SpotConeRatio		= lightproperties.SpotConeRatio;
		spotlight.SpotIntensity		= lightproperties.SpotIntensity;
		spotlight.SpotPosition		= lightproperties.SpotPosition;

		SpotlightsList.push_back(spotlight);
	}
}

