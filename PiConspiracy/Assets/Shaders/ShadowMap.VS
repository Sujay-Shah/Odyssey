//
//  ShadowMap.VS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 LightSpaceMatrix;
uniform mat4 model_Matrix;

void main()
{
	gl_Position =  LightSpaceMatrix * model_Matrix * vec4(position,1.0);
}