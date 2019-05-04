//
//  GBuffer.cpp
//  PiEngine
//
//  Created by PiConspiracy on 12/01/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "GBuffer.h"


GBuffer::GBuffer():
m_uFBO(0),
m_uDepthTexture(0)
{
	ZERO_MEM(m_uTextures);
}

GBuffer::~GBuffer()
{
	if (m_uFBO != 0)
	{
		glDeleteFramebuffers(1, &m_uFBO);
	}

	if (m_uTextures[0] != 0)
	{
		glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(m_uTextures), m_uTextures);
	}

	if (m_uDepthTexture != 0)
	{
		glDeleteTextures(1, &m_uDepthTexture);
	}
}

bool GBuffer::Initialize(uint32 a_uWindowWidth, uint32 a_uWindowHeight)
{
	glGenFramebuffers(1, &m_uFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFBO);

	// Create GBuffer Textures
	glGenTextures(ARRAY_SIZE_IN_ELEMENTS(m_uTextures), m_uTextures);
	glGenTextures(1, &m_uDepthTexture);

	for (uint32 i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_uTextures); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_uTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, a_uWindowWidth, a_uWindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_uTextures[i], 0);
	}

	// Depth
	glBindBuffer(GL_TEXTURE_2D, m_uDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, a_uWindowWidth, a_uWindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uDepthTexture, 0);
	
	GLenum DrawBuffers[] = 
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3
	};

	glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);

	GLenum eStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (eStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR_ARG("Frame Buffer Error, status: 0x{}", eStatus);
		return false;
	}

	// restore Default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void GBuffer::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::BindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	/*for (uint32 i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_uTextures); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_uTextures[eGBufferType_Position + i]);
	}*/
}

void GBuffer::SetReadBuffer(eGBufferType a_eTextureType)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + a_eTextureType);
}