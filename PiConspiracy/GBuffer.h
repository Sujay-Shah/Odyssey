//
//  GBuffer.h
//  PiEngine
//
//  Created by PiConspiracy on 12/01/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class GBuffer
{
public:

	enum eGBufferType
	{
		eGBufferType_Position,
		eGBufferType_Diffuse,
		eGBufferType_Normal,
		eGBufferType_Texture,

		eGubfferType_Max,

		// If needed
		eGBufferType_Specular
	};
					GBuffer();
					~GBuffer();
	bool			Initialize(uint32 a_uWindowWidth, uint32 a_uWindowHeight);

	void			BindForWriting();
	void			BindForReading();
	void			SetReadBuffer(eGBufferType eTextureType);
private:
	uint32			m_uFBO;
	uint32			m_uTextures[eGubfferType_Max];
	uint32			m_uDepthTexture;
};

