
//
//  Created by PiConspiracy on 3/6/2019
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) in vec3 position;

layout(location = 1) in vec3 normals;

layout(location = 2) in vec2 aTexCoord;

vec3 scale_factor = vec3(1.0, 1.0, 1.0);



uniform mat4 model_Matrix;

uniform mat4 view_Matrix;

uniform mat4 projection_Matrix;

uniform mat4 MVP;

//out vec4 viewSpace;

out vec4 FragPos;
out vec2 TexCoord;

//out vec4 outColor;
//out vec3 anormals;
//uniform mat4 orthographic_Matrix;
void main()
{
	

	TexCoord = vec2(aTexCoord.s ,1.0 -aTexCoord.t );

	FragPos = model_Matrix * vec4(position , 1.0f);

	gl_Position = projection_Matrix * view_Matrix * model_Matrix * vec4(position,1.0f);//projection_Matrix * view_Matrix *model_Matrix *
	//gl_Position = orthographic_Matrix * model_Matrix * vec4(position,1.0f);//projection_Matrix * view_Matrix *model_Matrix *
}