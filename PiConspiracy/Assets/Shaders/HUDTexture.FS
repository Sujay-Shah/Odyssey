//
//  Cube.FS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) out vec4 BloomBright;
layout(location = 1) out vec4 FragColor;

in vec4 FragPos;

in vec2 TexCoord;


uniform bool isCoolDown;
uniform sampler2D CoolDownTexture;
uniform sampler2D AlbedoTexture;

void main()
{

    vec4 result;// = texture(AlbedoTexture, TexCoord).rgba;
    
    if(isCoolDown)
    {
        result = texture(CoolDownTexture, TexCoord).rgba;
    }
    else
    {
        result = texture(AlbedoTexture, TexCoord).rgba;
    }

    if(result.a < 0.5)
    {
        discard;
    }

    FragColor = result;
}