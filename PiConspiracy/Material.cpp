//
//  Material.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/18/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Material.h"
#include "ShaderManager.h"
#include "TextureData.h"

Material::Material():
m_pShader(nullptr),
m_vColor(0.5f, 0.5f, 0.5f),
m_vDiffuseColor(0.5f, 0.5f, 0.5f),
m_vBrightColor(0.0f,0.0f,0.0f),
m_vSpecularColor(0.5f, 0.5f, 0.5f, 160.0f),
m_bIsInstanced(false),
m_bIsReceiveLight(true),
m_bIsReflective(false),
m_isCoolDown(false),
m_uInstanceCount(0)
{
    ZERO_MEM(m_pTextures);
}

Material::~Material()
{

}