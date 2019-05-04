//
//  SkyBox.FS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core


layout(location = 0) out vec4 BloomBright;
layout(location = 1) out vec4 FragColor;
//out vec4 FragColor;

in vec3 TexCoords;
in vec4 viewSpace;

//  Fog uniforms
uniform vec3 fogColor;
uniform int  FogEnable;
uniform float fogStartDistance;
uniform float fogEndDistance;
uniform float fogDensity;
uniform int fogDepth;
uniform int fogSelector;

// Fog Calculations
float fogDistance = 0;
float fogFactor = 0;

uniform sampler2D UsedTexture;
uniform samplerCube textureskybox;

float CalculateFog(float startDistance, float endDistance, float Density, int a_fogDepth, int a_fogSelector)
{
	if(a_fogDepth == 1)
    {
        fogDistance = abs(viewSpace.z); // Plane Based Fog
    }
    else if(a_fogDepth == 2)
    {
        fogDistance = length(viewSpace); // Range Based Fog
    }

	if(a_fogSelector == 1) // linear fog
    {
        fogFactor = (endDistance - fogDistance)/(endDistance - startDistance);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
    }
    else if(a_fogSelector == 2) // exponential fog
    {
        fogFactor = 1.0/exp(fogDistance * Density);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
    }
    else if(a_fogSelector == 3)  // exponential fog^2
    {
        fogFactor = 1.0/exp((fogDistance * Density)*(fogDistance * Density));
        fogFactor = clamp(fogFactor, 0.0, 1.0);
    }

	return fogFactor;
}

void main()
{
	float final_fogFactor = CalculateFog(fogStartDistance, fogEndDistance, fogDensity, fogDepth, fogSelector);
    
	vec4 result_SkyboxTexture = texture(textureskybox, TexCoords);

	vec4 final_color = mix(vec4(fogColor,1.0), result_SkyboxTexture, final_fogFactor);

	FragColor = final_color;
}