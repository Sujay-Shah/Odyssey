//
//  Cube.VS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) in vec3 position;

layout(location = 1) in vec3 normals;

layout(location = 2) in vec2 aTexCoord;


uniform mat4 model_Matrix;

uniform mat4 view_Matrix;

uniform mat4 orthographic_Matrix;

uniform mat4 projection_Matrix;

out vec4 FragPos;
out vec2 TexCoord;

void main()
{
	

	TexCoord = vec2(aTexCoord.s ,1.0 -aTexCoord.t );

	FragPos = model_Matrix * vec4(position , 1.0f);

	gl_Position = orthographic_Matrix  * model_Matrix * vec4(position,1.0f);

}