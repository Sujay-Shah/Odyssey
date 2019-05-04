#version 330 core 

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normals;

//out vec4 FragColor;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec4 gAlbedoandSpec;
layout(location = 2) out vec3 gNormal;

///uniform sampler2D AlbedoTexture;
//uniform sampler2D specularmap;

void main()
{
    gNormal = vec3(1,0,0);///normalize(Normals);

    gPosition = vec3(1,1,0);//FragPos;

    gAlbedoandSpec.xyz =vec3(1,0,1);// texture(AlbedoTexture,TexCoords).rgb;
    
    gAlbedoandSpec.z = 1.0;//texture(AlbedoTexture,TexCoords).r;   

    //FragColor = vec4(gPosition,1.0f); 
    
}