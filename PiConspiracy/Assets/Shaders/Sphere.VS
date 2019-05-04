//
//  Sphere.VS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texture;

uniform mat4 MVP;

out vec4 outColor;

void main()
{
	outColor = vec4(0.0f,0.0f,1.0f,1.0);
	gl_Position =  MVP * vec4(position,1.0);

}