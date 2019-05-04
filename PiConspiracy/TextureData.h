//
//  TextureData.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

struct TexData
{
	int32 width;
	int32 height;
	int32 channels;
	uint8* data;
	uint8* skyboxData;
	GLuint textureID;
	uint32 m_SkyboxTextureID;
};

class TextureData
{
public:
	                TextureData();
	                ~TextureData();

	TexData*        LoadTexture(const char* a_path);
    TexData*        LoadSkyBoxTexture(std::vector<string> a_CubeFacesPath);
	inline TexData* GetTexData();

private:
	TexData*        tex;
};

inline TexData* TextureData::GetTexData()
{
	return tex;
}

