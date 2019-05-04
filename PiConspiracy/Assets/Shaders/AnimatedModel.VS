//
//  AnimatedMode.VS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 anormals;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 offsetpos;
layout(location = 4) in ivec4 bone_ids;
layout(location = 5) in vec4 weights;

uniform mat4 projection_Matrix;
uniform mat4 view_Matrix;
uniform mat4 model_Matrix;
uniform vec3 objectCol;

uniform mat4 MVP;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

out vec4 viewSpace;
out vec4 vert_position;

vec3 scale_factor = vec3(1.0,1.0, 1.0);

out vec3 objectColor;
out vec3 Normals;
out vec3 Fragpos;

out vec2 TexCoord;

out vec4 bonetemppos;

//TO DO: GET OBJ COLOR FROM THE CPU

void main()
{
	mat4 bone_transform = bones[bone_ids[0]] * weights[0];
		bone_transform += bones[bone_ids[1]] * weights[1];
		bone_transform += bones[bone_ids[2]] * weights[2];
		bone_transform += bones[bone_ids[3]] * weights[3];

	vec4 boned_position = bone_transform * vec4(apos, 1.0f);
	
	bonetemppos = boned_position;

	vec3 final_scale_factor = scale_factor * apos;
    viewSpace = (view_Matrix * model_Matrix)* vec4(final_scale_factor + offsetpos, 1.0f);
    vert_position = projection_Matrix * viewSpace;

	//gl_Position = projection_Matrix *  view_Matrix * model_Matrix * vec4(apos + offsetpos , 1.0f);
	
	Fragpos		= vec3(model_Matrix * boned_position);
	Normals		= vec3(transpose(inverse(model_Matrix)) * (bone_transform * vec4(anormals, 0.0f)));
	objectColor = (objectCol);
	
	
	TexCoord	= aTexCoord;
    gl_Position =  MVP * boned_position;
}