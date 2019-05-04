//
//  LightManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once


struct LightProperties
{
	// Directional 
	vec4	DirectionalDir;
	vec3	DirectionalColor;

	// Point Light
	vec4	PointPosition;
	vec3	PointColor;
	float	pointRadius;

	// Spot Light
	vec4	SpotPosition;
	float	SpotConeRatio;
	vec3	SpotColor;
	float	SpotIntensity;

	uint32	enumLightType;
	
	LightProperties()
	{
		DirectionalColor	=	vec3(1.0,1.0,1.0);
		PointColor			=	vec3(1.0, 1.0, 1.0);
		SpotColor			=	vec3(1.0, 1.0, 1.0);
	}
};

struct Directional_Light
{
	vec3	DirectionalColor;
	vec3	DirectionalDir;
};

struct Point_Light
{
	vec3 PointPosition;
	vec3 pointColor;
	float pointRadius;
};

struct Spot_Light
{
	vec3	SpotPosition;
	vec3	SpotColor;
	float	SpotConeRatio;
	float	SpotIntensity;
};


class LightManager
{

private:

	std::vector<Directional_Light> DirectionallightsList;
	std::vector<Point_Light> PointlightsList;
	std::vector<Spot_Light> SpotlightsList;
	

public:

	enum LightType
	{
		LIGHT_DIRECTIONAL_,
		LIGHT_POINT_,
		LIGHT_SPOT_
	};

	LightManager();
	~LightManager();

	//void Serialize(const json&);
	void FilltheContainer(LightProperties&);

	inline std::vector<Directional_Light> getDirectionalLightList()
	{
		return DirectionallightsList;
	}


	inline std::vector<Point_Light> getPointLightList()
	{
		return PointlightsList;
	}


	inline std::vector<Spot_Light> getSpotightList()
	{
		return SpotlightsList;
	}



};

