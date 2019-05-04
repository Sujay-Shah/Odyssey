//
//  Light.VS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 430 core

layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 anormals;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 offsetpos;

uniform mat4 projection_Matrix;
uniform mat4 view_Matrix;
uniform mat4 model_Matrix;
uniform vec3 objectCol;

out vec4 viewSpace;
out vec4 vert_position;

vec3 scale_factor = vec3(1.0,1.0, 1.0);

out vec3 objectColor;
out vec3 Normals;
out vec3 Fragpos;

out vec2 TexCoord;

//TO DO: GET OBJ COLOR FROM THE CPU

uniform mat4 LightSpaceMatrix;

out vec4 FragLightSpace;

void main()
{
	vec3 final_scale_factor = scale_factor * apos;
    viewSpace = view_Matrix * model_Matrix * vec4(apos, 1.0f);
    vert_position = projection_Matrix * viewSpace;

	//gl_Position = projection_Matrix *  view_Matrix * model_Matrix * vec4(apos + offsetpos , 1.0f);
	
	Fragpos		= vec3(model_Matrix * vec4(apos , 1.0f));
	Normals		= mat3(transpose(inverse(model_Matrix))) * anormals;
	objectColor = (objectCol);
	
	FragLightSpace = LightSpaceMatrix * model_Matrix * vec4(apos, 1.0f);
	
	TexCoord	= aTexCoord;
    gl_Position = vert_position;
}