//
//  ParticleTexture.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class ParticleTexture
{
public:
	ParticleTexture();
	~ParticleTexture();

	int GetTextureID(void) const;
	int GetNumRows(void) const;

	void SetTextureID(int id);
private:
	int m_textureID;
	int m_numRows;

};

