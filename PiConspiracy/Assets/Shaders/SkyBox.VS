//
//  SkyBox.VS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) in vec3 position;


out vec3 TexCoords;
out vec4 viewSpace;
out vec4 vert_position;

uniform mat4 Model_Matrix ;
uniform mat4 View_matrix;
uniform mat4 Projection_matrix ;

vec3 scale_factor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
	vec3 final_scale_factor = scale_factor * position;
    viewSpace = View_matrix * Model_Matrix * vec4(final_scale_factor, 1.0f);
    vert_position = Projection_matrix * viewSpace;

	TexCoords		=  position;
	gl_Position		=  vert_position; //Projection_matrix  * View_matrix * Model_Matrix * vec4(position,1.0);

}