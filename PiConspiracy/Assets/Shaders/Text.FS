//
//  Text.FS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D text;
uniform vec3 text_color;

void main()
{    
    vec4 sampled    =   vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    fragcolor       =   vec4(0.714,0.979,0.636, 1.0) * sampled;
} 