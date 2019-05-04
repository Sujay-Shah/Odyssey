//
//  SkyBox.h
//  PiEngine
//
//  Created by PiConspiracy on 11/05/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#pragma once

#include "Renderable.h"

class ShaderManager;
class TextureData;
class Transform;
class SkyBox : public Renderable
{
public:
									SkyBox();
    virtual							~SkyBox();

	bool							InitializeSkyBox(const GLchar* ShaderPath);
	void							LoadSkyBox();
	void							Update();
	virtual void					Draw(Material* a_pMaterial) override;

	inline ShaderManager*			GetShaderId();
private:
	
	ShaderManager*					m_shaderID;
	TextureData*					m_texture;
	uint32							m_vao;
	uint32							m_vbo;
	uint32							m_ibo;

	std::vector<std::string>		m_CubeFaces;
};

inline ShaderManager* SkyBox::GetShaderId()
{
	return m_shaderID;
}

