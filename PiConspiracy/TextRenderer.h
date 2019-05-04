//
//  TextRendering.h
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "Renderable.h"

struct Character
{
	GLuint		m_textureId;
	glm::ivec2	m_size;
	glm::ivec2	m_bearing;
	GLuint		m_advance;
};

class TextRenderer : public Renderable
{
public:
								TextRenderer();
								~TextRenderer();

	virtual void				Draw(Material* a_pMaterial);
	bool						Initialize(const GLchar* a_strShaderName);
	void						LoadText(std::string a_font, GLuint a_fontSize);
	void						Update();

	inline void					SetText(string a_text);
	inline void					SetColor(Color3& a_vColor);
	inline void					SetScale(float a_fScale);
	inline void					SetPosition(vec2& a_vPosition);

	inline string				GetText();
	inline ShaderManager*		GetShader();


private:

	std::map<GLchar, Character> m_Characters;
	uint32						m_uVAO_text;
	uint32						m_uVBOText;
	
	vec2						m_vPosition;
	string						m_strText;
	Color3						m_vColor;
	float						m_fScale;

	ShaderManager*				m_pTextShader;

	mat4						Projection_matrix;
};

inline void TextRenderer::SetText(string a_strText)
{
	m_strText = a_strText;
}

inline void TextRenderer::SetColor(Color3& a_vColor)
{
	m_vColor = a_vColor;
}

inline void TextRenderer::SetScale(float a_fScale)
{
	m_fScale = a_fScale;
}

inline ShaderManager* TextRenderer::GetShader()
{
	return m_pTextShader;
}

inline void TextRenderer::SetPosition(vec2& a_vPosition)
{
	m_vPosition = a_vPosition;
}

inline string TextRenderer::GetText()
{
	return m_strText;
}