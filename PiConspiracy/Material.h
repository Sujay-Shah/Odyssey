//
//  Material.h
//  PiEngine
//
//  Created by PiConspiracy on 11/18/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "ResourceManager.h"

//Tutorial Texture Related


class ShaderManager;
class TextureData;
class Material
{
public:
    enum eTextureType : uint32
    {
        eTextureType_Albedo,			// Diffuse/Albedo Map
        eTextureType_Normal,			// Normal Map
		eTextureType_PerlinNoise0,		// Perlin Noise 0
		eTextureType_PerlinNoise1,		// Perlin Noise 1
		eTextureType_PerlinNoise2,		// Perlin Noise 2
		eTextureType_PerlinNoise3,		// Perlin Noise 3
        eTextureType_CoolDownTexture,	//Sujay Ke liye

										
		//Tutorial related Textures

		eTextureType_Left,
		eTextureType_Right,
		eTextureType_Jump,
		eTextureType_Boost,
		eTextureType_ELife,
		eTextureType_Pickups,
      
		eTextureType_Max,

        // TO DO:: Rohit/ Zoheb/ Sujay
		eTextureType_Parallax,			// Parallax Map
        eTextureType_Specular,			// Specular Map
        eTextureType_Height,			// Height Map
        eTextureType_Shininess,
        eTextureType_Reflection,		
        eTextureType_Opacity

		

    };

	/*struct Tutorial_Textures
	{


		Tutorial_Textures() :m_leftTexture(false), m_rightTexture(false), m_BoostTexture(false), m_jumpTexture(false)
		{

		}

		void SetleftTexture(bool a_set)
		{
			m_leftTexture = a_set;
		}

		void SetrightTexture(bool a_set)
		{
			m_rightTexture = a_set;
		}

		void SetBoostTexture(bool a_set)
		{
			m_BoostTexture = a_set;
		}

		void SetjumpTexture(bool a_set)
		{
			m_jumpTexture = a_set;
		}



	};*/


                                    Material();
                                    ~Material();
    
    inline void                     SetShader(string& a_strShaderName);
    
    inline void                     SetColor(Color3& a_vColor);
    inline void                     SetBrightColor(Color3& a_vColor);
    inline void                     SetDiffuseColor(Color3& a_vColor);
    inline void                     SetSpecularColor(Color4& a_vColor);
    inline void                     SetTexture(string& a_strTextureName, eTextureType a_eTextureType);
	inline void						SetInstancedCout(uint32 a_uInstancedCount);

    inline TextureData*             GetTexture(eTextureType a_eTextureType);
    inline Color3                   GetColor();
    inline Color3                   GetDiffuseColor();
    inline Color4                   GetSpecularColor();
	inline Color3					GetBrightColor();
    inline ShaderManager*           GetShader();
	inline uint32					GetInstancedCount();
	/*inline  Tutorial_Textures& getTutorialTextureState()
	{
		return m_tutorialTexture;
	}*/

	//Tutorial Textures

	bool							m_leftTexture;
	bool							m_rightTexture;
	bool							m_BoostTexture;
	bool							m_jumpTexture;
	bool							m_ElifeTexture;
	bool							m_PickupTexture;

	void SetAllFalse()
	{
		m_leftTexture = false;
		m_rightTexture = false;
		m_BoostTexture = false;
		m_jumpTexture = false;
		m_ElifeTexture = false;
		m_PickupTexture = false;
	}

	// Material Properties
	bool							m_bIsReceiveLight;
	bool							m_bIsInstanced;
    bool                            m_bIsReflective;

	//hack
	bool m_isCoolDown,m_customAlpha;
	float m_alpha;
	//Tutorial_Textures				m_tutorialTexture;
private:
	
	uint32							m_uInstanceCount;

	ShaderManager*                  m_pShader;
    Color3                          m_vColor;
    TextureData*                    m_pTextures[eTextureType_Max];

	Color3							m_vDiffuseColor;
	Color4							m_vSpecularColor;
	Color3							m_vBrightColor;

	

};

inline void Material::SetShader(string& a_strShaderName)
{
	m_pShader = PiResource->GetShader(a_strShaderName);
}

inline void Material::SetColor(Color3& a_vColor)
{
    m_vColor = a_vColor;
}

inline void Material::SetBrightColor(Color3 & a_vColor)
{
	m_vBrightColor = a_vColor;
}

inline void Material::SetDiffuseColor(Color3& a_vDiffuseColor)
{
	m_vDiffuseColor = a_vDiffuseColor;
}

inline void Material::SetSpecularColor(Color4& a_vSpecularColor)
{
	m_vSpecularColor = a_vSpecularColor;
}

inline void Material::SetInstancedCout(uint32 a_uInstancedCount)
{
	m_uInstanceCount = a_uInstancedCount;
}

inline Color3 Material::GetColor()
{
    return m_vColor;
}

inline Color3 Material::GetDiffuseColor()
{
	return m_vDiffuseColor;
}

inline Color4 Material::GetSpecularColor()
{
	return m_vSpecularColor;
}

inline Color3 Material::GetBrightColor()
{
	return m_vBrightColor;
}

inline ShaderManager* Material::GetShader()
{
     return m_pShader;
}


inline void Material::SetTexture(string& a_strTextureName, eTextureType a_eTextureType)
{
    m_pTextures[a_eTextureType] = PiResource->GetTexture(a_strTextureName);
}

inline TextureData* Material::GetTexture(eTextureType a_eTextureType)
{
    return m_pTextures[a_eTextureType];
}

inline uint32 Material::GetInstancedCount()
{
	return m_uInstanceCount;
}