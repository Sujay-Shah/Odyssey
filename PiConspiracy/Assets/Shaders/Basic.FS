//
//  Basic.FS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) out vec4 BloomBright;
layout(location = 1) out vec4 FragColor;

in  vec4 outColor;
//out vec4 FragColor;

void main()
{
	FragColor = outColor;
}