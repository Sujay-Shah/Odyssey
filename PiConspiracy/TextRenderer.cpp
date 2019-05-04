//
//  TextRendering.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "TextRenderer.h"

#include "ShaderManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "Renderer.h"
#include "RenderableContainer.h"
#include <ft2build.h>
#include "Material.h"

#include FT_FREETYPE_H
TextRenderer::TextRenderer():
m_pTextShader(nullptr)
{

}


TextRenderer::~TextRenderer()
{
	glDeleteBuffers(1, &m_uVBOText);
	glDeleteVertexArrays(1, &m_uVAO_text);
}

bool TextRenderer::Initialize(const GLchar* a_strShaderName)
{
	m_pTextShader = PiResource->GetShader(a_strShaderName);
	m_pTextShader->Use();

	glGenVertexArrays(1, &m_uVAO_text);
	glGenBuffers(1, &m_uVBOText);

	glBindVertexArray(m_uVAO_text);

	glBindBuffer(GL_ARRAY_BUFFER, m_uVBOText);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}

void TextRenderer::LoadText(std::string a_font, GLuint a_fontSize)
{
	m_Characters.clear();

	FT_Library ft_lib;
	if (FT_Init_FreeType(&ft_lib))
		LOG_DEBUG("Free type initialize error");

	FT_Face ft_face;
	if (FT_New_Face(ft_lib, a_font.c_str(), 0, &ft_face))
		LOG_DEBUG("Font not Loaded");

	FT_Set_Pixel_Sizes(ft_face, 0, a_fontSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(ft_face, c, FT_LOAD_RENDER))
		{
			LOG_ERROR_ARG("Error loading Font Character: {}", (char)c);
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			GL_RED,
			ft_face->glyph->bitmap.width, 
			ft_face->glyph->bitmap.rows, 
			0, 
			GL_RED, 
			GL_UNSIGNED_BYTE, 
			ft_face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		Character character = { texture,
			glm::ivec2(ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows),
			glm::ivec2(ft_face->glyph->bitmap_left, ft_face->glyph->bitmap_top),
			ft_face->glyph->advance.x
		};

		m_Characters.insert(std::pair<GLchar, Character>(c, character));

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(ft_face);
	FT_Done_FreeType(ft_lib);
}

void TextRenderer::Update()
{

}

void TextRenderer::Draw(Material* a_pMaterial)
{
	m_pTextShader->Use();
	
	Camera* pCamera = PiRenderer->GetMainCamera();
	Projection_matrix = pCamera->GetOrthoProjectionMatrix();
	m_pTextShader->SetUniformMatrix4fv(m_pTextShader->GetShaderID(), "projection_Matrix", (Projection_matrix));
	m_pTextShader->SetUniform3f(m_pTextShader->GetShaderID(), "text_color", m_vColor.x, m_vColor.y, m_vColor.z);
	m_pTextShader->SetInt(m_pTextShader->GetShaderID(),"text", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_uVAO_text);

	float offsetX = m_vPosition.x;
	float offsetY = m_vPosition.y;

	string::const_iterator c;
	for (c = m_strText.begin(); c != m_strText.end(); c++)
	{
		Character ch = m_Characters[*c];

		float Xpos = offsetX + ch.m_bearing.x * m_fScale;
		float Ypos = offsetY - (ch.m_size.y - ch.m_bearing.y) * m_fScale;

		float w = ch.m_size.x * m_fScale;
		float h = ch.m_size.y * m_fScale;

		float vertices[6][4] =
		{
			//x			//y				//tex coords
			{Xpos,		Ypos + h,		0.0, 0.0},
			{Xpos,		Ypos,			0.0, 1.0},
			{Xpos + w,	Ypos,			1.0, 1.0},

			{Xpos,		Ypos + h,		0.0, 0.0},
			{Xpos + w,	Ypos,			1.0, 1.0},
			{Xpos + w,	Ypos + h,		1.0, 0.0},
		};

		glBindTexture(GL_TEXTURE_2D, ch.m_textureId);

		glBindBuffer(GL_ARRAY_BUFFER, m_uVBOText);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		offsetX += (ch.m_advance >> 6) * m_fScale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

