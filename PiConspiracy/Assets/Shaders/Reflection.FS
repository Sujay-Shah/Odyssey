//
//  Reflection.FS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

in vec3 Normal;
in vec3 FragPos;

//out vec4 FragColor;

layout(location = 0) out vec4 BloomBright;
layout(location = 1) out vec4 FragColor;


uniform vec3 cameraPos;

uniform samplerCube textureskybox;

void main()
{
	float ratio = 1.0/2.42;
	vec3 I = normalize(FragPos - cameraPos);
	vec3 N = normalize(Normal);

	//If Refraction
	//vec3 Refract = refract(I,N,ratio);
	//FragColor = vec4(texture(textureskybox,Refract).rgb,1.0f);

	//If reflection
	vec3 Reflection = reflect(I,N);
	FragColor = vec4(texture(textureskybox,Reflection).rgb,1.0f);
}