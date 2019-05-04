
//
//  FrameBuffer.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/13/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "FrameBuffer.h"


FrameBuffer::FrameBuffer() :
m_uFbo(0),
m_uDepthMap(0)
{

}

FrameBuffer::~FrameBuffer()
{
	if (m_uFbo != 0)
	{
		glDeleteFramebuffers(1, &m_uFbo);
	}
}
bool FrameBuffer::Initialize(int32 a_uWidth, int32 a_uHeight)
{
	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &m_uFbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, m_uFbo);

	// create depth texture
	glGenTextures(1, &m_uDepthMap);
	glBindTexture(GL_TEXTURE_2D, m_uDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, a_uWidth, a_uHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_uFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLenum eStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (eStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_DEBUG_ARG("FBO Error, status: {}", eStatus);
		return false;
	}

	LOG_DEBUG("FBO Initialized Successfully");
	
	return true;
}

void FrameBuffer::BindForWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uFbo);
}

void FrameBuffer::UnBindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindForReading(GLenum a_eTextureUnit)
{
	glActiveTexture(a_eTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_uDepthMap);
}

inline uint32 FrameBuffer::getFBO()
{
	return m_uFbo;
}
