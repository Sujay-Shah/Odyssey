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

uniform sampler2D AlbedoTexture;

void main()
{

	//float final_fogFactor = CalculateFog(fogStartDistance, fogEndDistance, fogDensity, fogDepth, fogSelector);


    vec4 result;// = texture(AlbedoTexture, TexCoord).rgba;
    
   
        result = texture(AlbedoTexture, TexCoord);
    
    //vec4 final_color = mix(vec4(fogColor,1.0), result, final_fogFactor);
        
    if(result.a < 0.5)
    {
        discard;
    }

    FragColor = result;
    BloomBright = result; //vec4(1.0f,1.0f,1.0f,1.0f);//result;//vec4(0.5f,0.5f,0.5f,1.0f);
}