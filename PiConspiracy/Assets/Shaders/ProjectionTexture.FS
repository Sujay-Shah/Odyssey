
//
//  Created by PiConspiracy on 3/6/2019
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) out vec4 BloomBright;
layout(location = 1) out vec4 FragColor;

in vec4 FragPos;

in vec2 TexCoord;

uniform bool isCoolDown,customAlpha;
uniform sampler2D AlbedoTexture;

uniform sampler2D CoolDownTexture;

uniform float alpha;
void main()
{

    vec4 result;
    
    if(isCoolDown)
    {
        result = texture(CoolDownTexture, TexCoord).rgba;
    }
    else
    {
        result = texture(AlbedoTexture, TexCoord).rgba;
    }
    
    
    /*if(result.a < 0.5)
    {
        discard;
    }*/
    /*
    if(customAlpha)
    {
        result.a = alpha;
    }*/
    FragColor = result;
}