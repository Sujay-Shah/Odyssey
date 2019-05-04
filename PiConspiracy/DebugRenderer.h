//
//  DebugRenderer.h
//  PiEngine
//
//  Created by PiConspiracy on 11/01/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "Renderable.h"
#include "Renderer.h"

class ShaderManager;
class DebugRenderer:public Renderable
{
public:
	DebugRenderer();
	~DebugRenderer();
	bool						Initialize(const GLchar* a_strShaderName);
	void						CreateCube();
	void						CreateSphere();

	void						DrawCube(mat4 a_ModelMatrix);
	void						DrawSphere(mat4 a_ModelMatrix);

	inline ShaderManager*		GetDebugShader();

	
	virtual void                Draw(Material* a_pMaterial);

	inline void					setTypeDraw(int a_value);

private:

	uint32					m_uCubeVAO;
	uint32					m_uCubeIBO;
	uint32					m_uSphereVAO;
	uint32					m_uSphereIBO;

	size_t					m_uSphereIndexCount;

	ShaderManager*			m_pShader;
	int						m_typeDraw;
};

inline ShaderManager* DebugRenderer::GetDebugShader()
{
	return m_pShader;
}

inline void DebugRenderer::setTypeDraw(int a_value)
{
	m_typeDraw = a_value;
}
