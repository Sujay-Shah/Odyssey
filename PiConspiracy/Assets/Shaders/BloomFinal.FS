//
//  BloomFinal.FS
//  PiEngine
//
//  Created by PiConspiracy on 2/9/2019
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 430 core

uniform sampler2D 	FragColor;
uniform sampler2D 	BloomBright;

uniform float 		exposure;

in 		vec2 		TexCoord;

out 	vec4 		FinalOutput;


void main()
{
	float gamma_correction_value = 2.2f;
	
	vec3 sampleBloomBright = texture(BloomBright,TexCoord).rgb; 
	vec3 sampleFragColor = texture(FragColor,TexCoord).rgb; 
	
	
	//Add Both the texture's using Additive Blending
	sampleFragColor += sampleBloomBright;
	
	//Since we have HDR color we tone it to [0,1]
	//using Tone Mapping
	vec3 result = vec3(1.0) - exp(-sampleFragColor*exposure);
	
	
	//We also perform Gamma Correction
	
	result = pow(result, vec3(1/gamma_correction_value));
	
	FinalOutput = vec4(result,1.0);
	
}