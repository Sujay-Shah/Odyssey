#version 430 core 

layout(location = 0) out vec4 BloomBright;
layout(location = 1) out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D particleTexture;

void main(void)
{
	vec4 Result = texture(particleTexture,TexCoords).rgba;

	if(Result.a < 0.5)
    {
        discard;
    }

	FragColor = Result; 
}

