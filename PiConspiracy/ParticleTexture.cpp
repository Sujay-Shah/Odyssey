//
//  ParticleTexture.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ParticleTexture.h"


ParticleTexture::ParticleTexture()
{
}


ParticleTexture::~ParticleTexture()
{
}

int ParticleTexture::GetTextureID(void) const
{
	return m_textureID;
}

int ParticleTexture::GetNumRows(void) const
{
	return m_numRows;
}

void ParticleTexture::SetTextureID(int id)
{
	m_textureID = id;
}
