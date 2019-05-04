//
//  FrameBuffer.h
//  PiEngine
//
//  Created by PiConspiracy on 11/13/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class FrameBuffer
{
public:
								FrameBuffer();
								~FrameBuffer();

	bool						Initialize(int32 a_uWidth, int32 a_uHeight);
	
	void						BindForWriting();
	void						BindForReading(GLenum a_eTextureUnit);
	void						UnBindFBO();

	inline	uint32				getFBO();
	inline	uint32				GetDepthMap();

private:
	uint32						m_uFbo;
	uint32						m_urbo;
	
	// Shadow Map
	uint32						m_uDepthMap;   
};

inline uint32 FrameBuffer::GetDepthMap()
{
	return m_uDepthMap;
}
