//
//  Light.FS
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#version 430 core

layout (location = 3) uniform sampler2D depthMap;

layout(location = 0) out vec4 BloomBright;
layout(location = 1) out vec4 FragColor;

#define PI 3.1415926535

//out vec4 			FragColor;

in 	vec2 			TexCoord;
in 	vec3 			objectColor;
in 	vec3 			Normals;
in 	vec3 			Fragpos;
in 	vec4 			vert_position;
in 	vec4 			viewSpace;
				

uniform sampler2D 	AlbedoTexture;
uniform sampler2D 	NormalMapTexture;
uniform sampler2D 	PerlinNoise0;
uniform sampler2D 	PerlinNoise1;
uniform sampler2D 	PerlinNoise2;
uniform sampler2D 	PerlinNoise3;


uniform vec3 		cameraPos;
uniform vec3 		lightPos;
	
uniform int 		LightMode;
uniform vec4 		LightColor;
	
	
uniform float 		DeltaTime;
	
//  Fog uniforms	
uniform vec3 		fogColor;
uniform int  		FogEnable;
uniform float		fogStartDistance;
uniform float		fogEndDistance;
uniform float		fogDensity;
uniform int 		fogDepth;
uniform int 		fogSelector;


uniform float 		Lightintensity;

// Fog Calculations
		float 		fogDistance = 0;
		float 		fogFactor 	= 0;

struct Material
{
		vec3 		objectCol;
		vec3 		diffuse;		//kd
		vec3 		specular;		//ks
		vec3 		LightCol;		
		float		shininess;
		float		isNormalMapped;
};

uniform Material strmaterial;

struct DirectionalLight
{
		vec3 		lightColor;
		vec4 		LightDirection;
		int	 		lightMode;
	
};
uniform DirectionalLight strdirLight;

uniform int 		near_plane;
uniform int 		far_plane;

		vec3 		ambientColor;

struct PointLight
{
		float 		constant  ;
		float 		linear    ;
		float 		quadratic ;
		int	 		lightMode ;	
};
uniform PointLight pLight;


struct SpotLight
{
		float 		radius;
		float 		spotIntensity;

};
uniform SpotLight spLight;


// Shadow Light Space Vector
in 		vec4 		FragLightSpace;

		vec3		threshold = vec3(0.2126,0.7152,0.0722);


float LinearizeDepth(float depth)
{
	float z = depth * 2.0 -1.0;
	return (2.0 * 1 * 200) / (200 + 1 - z * (200 - 1));
}

float ShadowCalculation()
{

	vec3 projCoords = FragLightSpace.xyz / FragLightSpace.w;

	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(depthMap, projCoords.xy).r;

	float currentDepth = projCoords.z;
	closestDepth = vec4(vec3(LinearizeDepth(closestDepth)/ 200), 1.0).r;

	//closestDepth =  1.0 - (1.0 - closestDepth) * 100.0;
	vec3 normal = normalize(Normals);
	vec3 lightDir = normalize(- lightPos);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	float shadow = 0.0;
	vec2 texelSize = 1.0/textureSize(depthMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias >pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	if(projCoords.z > 1.0)
	{
		shadow = 0.0;
	}

	return shadow;
}

vec3 CalculateDirectionalLight(Material strmaterial, vec3 newNormals,int LightMode)
{

	if(LightMode == 1)		//Phong Model
	{
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		float Lightintensity = 0.011;
	
		//Normalize Object Color
		//vec3 objectColor = normalize(strmaterial.objectCol);
		
		//================================================================
		//TO DO: If texture selected uncomment the line below
		//       which will be serialized later
		//================================================================
		strmaterial.objectCol *= texture(AlbedoTexture,TexCoord).xyz;
	
		//Ambient Color
		ambientColor = vec3(1.0f,1.0f,1.0f);
		float ambientStrength = 0.8;
		ambient = ambientStrength * ambientColor;
		ambient = ambient * strmaterial.objectCol;
	
		//Diffuse 
		vec3 lightdir = normalize(lightPos - Fragpos);
		//lightdir = -lightdir;
		vec3 norm = normalize(newNormals);
		norm = abs(norm);
		float diff = max(dot(norm,lightdir),0.0);
		diffuse = diff * strmaterial.objectCol ;//* Lightintensity
	
		//Specular
		float specularStrength = 10.0f;
		float specularIntensity = 2;
		vec3 viewdir = normalize( cameraPos - Fragpos);
		vec3 reflectdir = reflect(-lightdir,norm);
		float spec = pow(max(dot(viewdir,reflectdir),0.0),specularIntensity);
	 	specular = specularStrength * spec * strmaterial.objectCol * Lightintensity;
		
		return ambient + diffuse + specular * Lightintensity;
	
	 // Calculate Shadow
		//float shadow  = ShadowCalculation();
		//return (  ambient +  (1- shadow) * (diffuse + specular ));
	}
	else if(LightMode == 2)			//BRDF Model
	{
		
		strmaterial.objectCol *= texture(AlbedoTexture,TexCoord).xyz;
		
		vec3 	kd 		= strmaterial.objectCol;//vec3(0.5);//strmaterial.diffuse;
		vec3 	ks 		= vec3(0.5,0.5,0.5);//strmaterial.specular;
		float   alpha 	= 200;
		
		//vec3 lightdir 	= vec3(0.0,100.0,-400.0);
		vec3 L 			= normalize(lightPos - Fragpos);			//(lightPos - Fragpos);
		vec3 V 			= normalize(cameraPos - Fragpos);
		vec3 N 			= normalize(newNormals);
	
		
		vec3 	H 		= 	normalize(L + V);
		float 	LN 		=	max(dot(L,N),0.0); 
		float 	HN 		=	max(dot(H,N),0.0); 
		float 	LH 		=	max(dot(L, H),0.0); 
		
		//strmaterial.objectCol *= texture().xyz;//texture(AlbedoTexture,TexCoord + 50 + 50).xyz;
		
		vec3 	I  		= vec3(1.0f,1.0f,1.0f);	//Light Color
		vec3 	Ia 		= vec3(0.2f);	//Ambient Color
		
		vec3 FresnelTerm = ks + (vec3(1.0f) - ks) * pow((1.0f - LH), 5);//pow((1-dot(L,H)),5);
		
		float Gterm 	= 1 / pow(LH,2);
			
		float DTerm 	= ((alpha + 2) / (2 * PI)) * pow(HN,alpha);
		
		
		
		vec3  BRDF  	= kd/PI + ((FresnelTerm * Gterm * DTerm)/4);// DTerm *  Gterm * FresnelTerm) / 4);
		
		vec3 final_BRDF = (Ia * kd )+ I * LN * BRDF;
		
		return final_BRDF * Lightintensity;
		
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
    float k0 = texture(PerlinNoise0, TexCoord).x;
    float k1 = texture(PerlinNoise1, TexCoord).y;
    float k2 = texture(PerlinNoise2, TexCoord).y;
    float k3 = texture(PerlinNoise3, TexCoord).x;

    float turbulence;
    turbulence = (k0 + k1/2.0 + k2/4.0 + k3/8.0);

    float fDensity = Density * turbulence;

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
        fogFactor = 1.0/exp(fogDistance * fDensity);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
    }
    else if(a_fogSelector == 3)  // exponential fog^2
    {
        fogFactor = 1.0/exp((fogDistance * fDensity)*(fogDistance * fDensity));
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
	vec3 newNormals;
  
  if(strmaterial.isNormalMapped == 1)	//If the Object has normal texture on it
  {
	  
	mat3 TBN = CalculateTBN(Fragpos,TexCoord);
	
	newNormals = texture(NormalMapTexture,TexCoord).xyz * 2.0 - 1.0;
	
	newNormals = normalize(TBN * newNormals);
		
    result_Dir = CalculateDirectionalLight(strmaterial,newNormals,LightMode);
 
  }
  else									//If it doesn't contain any
  {
	result_Dir = CalculateDirectionalLight(strmaterial,Normals,LightMode);
  }
	//TO DO: Uncomment this if you want to calculate the point light.
    //vec3 result_Point = CalculatePointLight(pLight);
    
    float final_fogFactor = CalculateFog(fogStartDistance, fogEndDistance, fogDensity, fogDepth, fogSelector);
    
	vec3 final_color = mix(fogColor, result_Dir, final_fogFactor);
	
	float Brightness = dot(strmaterial.LightCol,threshold);
	
	//=====================================================
	//Testing
	//=====================================================
	
	if(Brightness > 1.0f)
	{
		BloomBright = vec4(strmaterial.LightCol,1.0);//objectCol  LightCol
	}
	else
	{
		BloomBright = vec4(0.0,0.0,0.0,1.0);
	}
	
	FragColor = vec4(final_color,1.0f);
	
	
   //=====================================================
   //=====================================================
	
    /*if(FogEnable == 1)
    {
		//Writing values for the Bloom Text
		//float depthVal = texture(depthMap, TexCoord).r;
		
		//if(Brightness > 1)
		//{
		//	BloomBright = vec4(strmaterial.LightCol,1.0);//objectCol  LightCol
		//}
		//else
		//{
		//	BloomBright = vec4(0.0,0.0,0.0,1.0);
		//}
		
		FragColor = vec4(final_color,1.0f);
		
		//vec4(vec3(LinearizeDepth(depthVal)/1000.0),1.0); //vec4(final_color,1.0f);
   
   }
    else
    {
		//if(Brightness > 1)
		//{
		//	BloomBright = vec4(strmaterial.LightCol,1.0);
		//}
		//else
		//{
		//	BloomBright = vec4(0.0,0.0,0.0,1.0);
		//}
		
		FragColor = vec4(result_Dir,1.0f);
    }
	*/

}
