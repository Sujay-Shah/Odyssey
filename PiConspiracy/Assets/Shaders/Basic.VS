//
//  Basic.VS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 MVP;

out vec4 outColor;

void main()
{
	outColor = vec4(0.0, 1.0, 0.0, 1.0);
	gl_Position =  MVP * vec4(position,1.0);

}