//
//  AnimatedMode.FS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 330 core

layout(location = 0) out vec4 BloomBright;
layout(location = 1) out vec4 FragColor;

//out vec4 FragColor;

in vec3 objectColor;
in vec3 Normals;
in vec3 Fragpos;
in vec4 vert_position;
in vec4 viewSpace;

in vec2 TexCoord;

uniform sampler2D AlbedoTexture;
uniform sampler2D NormalMapTexture;


uniform vec3 cameraPos;
uniform vec3 lightPos;


uniform float DeltaTime;

//  Fog uniforms
uniform vec3 fogColor;
uniform int  FogEnable;
uniform float fogStartDistance;
uniform float fogEndDistance;
uniform float fogDensity;
uniform int fogDepth;
uniform int fogSelector;

// Fog Calculations
float fogDistance = 0;
float fogFactor = 0;

struct Material
{
	vec3 objectCol;
	vec3 diffuse;		//kd
	vec3 specular;		//ks
	float shininess;
	float isNormalMapped;
};
uniform Material strmaterial;

struct DirectionalLight
{
	vec3 lightColor;
	vec4 LightDirection;
	int	 lightMode;
	
};
uniform DirectionalLight strdirLight;

struct PointLight
{
	float 	constant  ;
	float 	linear    ;
	float 	quadratic ;
	int	 	lightMode ;	
};
uniform PointLight pLight;


struct SpotLight
{
    float radius;
    float spotIntensity;

};
uniform SpotLight spLight;

vec3 ambientColor;

int lightMode = 0;
vec3 CalculateDirectionalLight(Material strmaterial, vec3 newNormals)
{
    
	if(lightMode == 0)		//Phong Model
	{
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	
		//Normalize Object Color
		//vec3 objectColor = normalize(strmaterial.objectCol);
		
//================================================================
//TO DO: If texture selected uncomment the line below
//       which will be serialized later
//================================================================
		strmaterial.objectCol = texture(AlbedoTexture,TexCoord).xyz;
	
		//Ambient Color
		ambientColor = vec3(1.0f,1.0f,1.0f);
		float ambientStrength = 0.5;
		ambient = ambientStrength * ambientColor * strmaterial.objectCol;
	
		//Diffuse 
		vec3 lightdir = normalize( lightPos -  Fragpos);
		vec3 norm = normalize(newNormals);
		float diff = max(dot(norm,lightdir),0.0);
		diffuse = diff * strmaterial.objectCol;
	
		//Specular
		float specularStrength = 1.0f;
		float specularIntensity = 256;
		vec3 viewdir = normalize( cameraPos - Fragpos);
		vec3 reflectdir = reflect(-lightdir,norm);
		float spec = pow(max(dot(viewdir,reflectdir),0.0),specularIntensity);
		specular = specularStrength * spec * strmaterial.objectCol;
	
		return (  ambient + diffuse + specular );
	}
	else if(lightMode == 1)			//BRDF Model
	{
		vec3 	kd 		= strmaterial.diffuse;
		vec3 	ks 		= strmaterial.specular;
		float   alpha 	= strmaterial.shininess;
		
		vec3 L = normalize(lightPos - Fragpos);
		vec3 V = normalize(cameraPos - Fragpos);
		vec3 N = normalize(Normals);
		
		vec3 	H 		= 	normalize(L + V);
		float 	LN 		=	max(dot(L,N),0.0); 
		float 	HN 		=	max(dot(H,N),0.0); 
		
		vec3 I  = vec3(1.0f,1.0f,1.0f);	//Light Color
		vec3 Ia = vec3(0.2f,0.2f,0.2f);	//Ambient Color
		
		vec3 FresnelTerm = ks + (vec3(1.0,1.0,1.0) - ks) * pow((1-dot(L,H)),5);
		
		float Gterm = dot(L,N) * (dot(V,N) / pow(dot(L,H),2));
		
		float DTerm = (alpha + 2)/ (2*3.14) * pow(dot(N,H),alpha);
		
		
		
		vec3 BRDF  = kd/3.14 + ( FresnelTerm * Gterm * DTerm) / 4;
		
		vec3 final_BRDF = Ia * kd + I * BRDF * (LN);
		
		return final_BRDF * strmaterial.objectCol;
		
	}
	
}


vec3 CalculatePointLight(PointLight pLight)
{
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	//vec3 objectColor = normalize(objectCol);
	
    //Ambient Color
    ambientColor = vec3(1.0f,1.0f,1.0f);
    float ambientStrength = 0.5;
    ambient = ambientStrength * ambientColor * objectColor;

    //Diffuse 
	vec3 lightdir = normalize(lightPos - Fragpos);
	vec3 norm = normalize(Normals);
	float diff = max(dot(norm,lightdir),0.0);
	diffuse = diff * objectColor;

    //Specular
	float specularStrength = 0.5f;
	float specularIntensity = 256;
	vec3 viewdir = normalize(cameraPos - Fragpos);
	vec3 reflectdir = reflect(-lightdir,norm);
	float spec = pow(max(dot(viewdir,reflectdir),0.0),specularIntensity);
	specular = specularStrength * spec * objectColor;


	float distance = length(lightPos - Fragpos);
	float attenuation = 1.0f / (pLight.constant + pLight.linear * distance + (pLight.quadratic * distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

    return (specular + ambient + diffuse);
}

float CalculateFog(float startDistance, float endDistance, float Density, int a_fogDepth, int a_fogSelector)
{
	if(a_fogDepth == 1)
    {
        fogDistance = abs(viewSpace.z); // Plane Based Fog
    }
    else if(a_fogDepth == 2)
    {
        fogDistance = length(viewSpace); // Range Based Fog
    }

	if(a_fogSelector == 1) // linear fog
    {
        fogFactor = (endDistance - fogDistance)/(endDistance - startDistance);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
    }
    else if(a_fogSelector == 2) // exponential fog
    {
        fogFactor = 1.0/exp(fogDistance * Density);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
    }
    else if(a_fogSelector == 3)  // exponential fog^2
    {
        fogFactor = 1.0/exp((fogDistance * Density)*(fogDistance * Density));
        fogFactor = clamp(fogFactor, 0.0, 1.0);
    }

	return fogFactor;
}



mat3 CalculateTBN(vec3 Fragpos,vec2 TexCoord)
{
	vec3 Qx = dFdx(Fragpos);
	vec3 Qy = dFdy(Fragpos);
	
	vec2 Tx = dFdx(TexCoord);
	vec2 Ty = dFdy(TexCoord);
	
	
	vec3 N = normalize(Normals);
	vec3 T = normalize(Qx*Ty.t - Qy*Tx.t);
	vec3 B = normalize(cross(N,T));
	
	mat3 TBN = transpose(mat3(T,B,N));
	
	return TBN;
}

void main()
{
	vec3 result_Dir;
  
  if(strmaterial.isNormalMapped == 1)	//If the Object has normal texture on it
  {
	  
	mat3 TBN = CalculateTBN(Fragpos,TexCoord);
	
	vec3 newNormals = texture(NormalMapTexture,TexCoord).xyz * 2.0 - 1.0;
	
	newNormals = normalize(TBN * newNormals);
		
    result_Dir = CalculateDirectionalLight(strmaterial,newNormals);
 
  }
  else									//If it doesn't contain any
  {
	result_Dir = CalculateDirectionalLight(strmaterial,Normals);
  }

    vec3 result_Point = CalculatePointLight(pLight);
    
    float final_fogFactor = CalculateFog(fogStartDistance, fogEndDistance, fogDensity, fogDepth, fogSelector);
    
	vec3 final_color = mix(fogColor, result_Dir, final_fogFactor);

	
    //if(FogEnable == 1)
    //{
     //   gl_FragColor = vec4(final_color,1.0f);
    //}
    //else
    //{
        FragColor = vec4(result_Dir,1.0f);
    //}
	//gl_FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}
