//
//  TextureData.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "TextureData.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


TextureData::TextureData()
{
	tex = new TexData();
}


TextureData::~TextureData()
{
	delete tex;
}

TexData* TextureData::LoadTexture(const char* a_path)
{
	int32 width=0, height=0, channels=0;
	
	// load and generate the texture
	glGenTextures(1, &tex->textureID);
	tex->data = stbi_load(a_path, &width, &height, &channels, 0);
	
	if (tex->data)
	{
		tex->width = width;
		tex->height = height;
		tex->channels = channels;

		GLenum eFormat;
		switch (channels)
		{
			case 1:
				eFormat = GL_RED;
				break;
			case 3:
				eFormat = GL_RGB;
				break;
			case 4:
				eFormat = GL_RGBA;
				break;
			default:
				//eFormat = GL_RGB;
				break;
		}

		glBindTexture(GL_TEXTURE_2D, tex->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, eFormat, width, height, 0, eFormat, GL_UNSIGNED_BYTE, tex->data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		
	
		
		stbi_image_free(tex->data);
		return tex;
	}

	LOG_DEBUG_ARG("Texture failed to load at path: {}", a_path);
	stbi_image_free(tex->data);

	return nullptr;
}

TexData* TextureData::LoadSkyBoxTexture(std::vector<string> a_CubeFacesPath)
{

    glGenTextures(1, &tex->m_SkyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex->m_SkyboxTextureID);

    int32 iWidth = 0, iHeight = 0, iChannels = 0;

    for (uint32 i = 0; i < a_CubeFacesPath.size(); ++i)
    {

        tex->skyboxData = stbi_load(a_CubeFacesPath[i].c_str(), &iWidth, &iHeight, &iChannels, 3);
        if (tex->skyboxData)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->skyboxData);
            stbi_image_free(tex->skyboxData);
        }
        else
        {
			LOG_ERROR_ARG("Error in Loading Skybox Textures: {}", i);
            stbi_image_free(tex->skyboxData);
        }

    }

    // Texture Filters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if (tex->skyboxData)
	{
		return tex;
	}
    return nullptr;
}
