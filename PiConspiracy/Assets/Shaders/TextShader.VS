//
//  TextShader.VS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform mat4 projection_Matrix;

void main()
{
	gl_Position = projection_Matrix * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;

}