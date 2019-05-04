//
//  BloomFinal.VS
//  PiEngine
//
//  Created by PiConspiracy on 2/9/2019
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 430 core

layout(location = 0) in vec3 apos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 view_Matrix;
uniform mat4 projection_Matrix;
uniform mat4 model_Matrix;

void main()
{

	vec4 Fragpos		= vec4(apos  , 1.0f);//projection_Matrix * view_Matrix *//* vec3(2,-2,0)

	TexCoord			= aTexCoord;
    gl_Position 		= Fragpos;
}