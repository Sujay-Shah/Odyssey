//
//  Blur.FS
//  PiEngine
//
//  Created by PiConspiracy on 2/9/2019
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 430 core

uniform bool isHorizontal;
uniform sampler2D image;

in 	vec2 TexCoord;
out vec4 FragColor;

uniform float weight[5] = float[] (0.2270270270,0.1945945946,0.1216216216,0.0540540541,0.016262162162);//(0.06136,0.24477,0.38774,0.24477,0.06136);

void main()
{
	vec2 tex_offset = 1.0 / textureSize(image , 0); // This gives each individual texture pixel
	vec3 final_result = texture(image,TexCoord).rgb * weight[0];	//Sample the image
	if(isHorizontal)
	{
		for(int i = 1;i<5;++i)
		{
			final_result += texture(image, TexCoord + vec2(tex_offset.x*i,0.0)).rgb * weight[i];
			final_result += texture(image, TexCoord - vec2(tex_offset.x*i,0.0)).rgb * weight[i];
		}

	}
	else {
		for(int i = 1;i<5;++i)
		{
			final_result += texture(image, TexCoord + vec2(0.0,tex_offset.y*i)).rgb * weight[i];
			final_result += texture(image, TexCoord - vec2(0.0,tex_offset.y*i)).rgb * weight[i];
		}
	}
	
	
	FragColor = vec4(final_result,1.0);
	
}