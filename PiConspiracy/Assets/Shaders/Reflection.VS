//
//  Reflection.VS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 anormals;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model_Matrix;
uniform mat4 view_Matrix;
uniform mat4 projection_Matrix;


out vec3 Normal;
out vec3 FragPos;


void main()
{

	Normal = mat3(transpose(inverse(model_Matrix))) * anormals;
	FragPos = vec3(model_Matrix * vec4(aPos,1.0f));
	gl_Position = projection_Matrix * view_Matrix * model_Matrix * vec4(aPos,1.0f);

}
