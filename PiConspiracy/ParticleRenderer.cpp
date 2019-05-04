//
//  ParticleRenderer.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ParticleRenderer.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Renderer.h"
#include "Particle.h"
#include "ParticleTexture.h"
#include "ProjectConfig.h"
#include "Material.h"

ParticleRenderer::ParticleRenderer()
{
}


ParticleRenderer::~ParticleRenderer()
{
	m_Particles.clear();
	glDeleteBuffers(1, &m_uVBO);
	glDeleteBuffers(1, &m_uIBO);
	glDeleteVertexArrays(1, &m_uVAO);
}

void ParticleRenderer::SetModelMatrix(const mat4 a_mModelMatrix)
{
	
}

mat4 ParticleRenderer::GetModelMatrix() const
{
	return mat4();
}

void ParticleRenderer::Draw(Material* a_pMaterial)
{
	m_particleShader->Use();
	//loop through render list of particles, and draw them
	//prepare buffers to be drawn
	for (auto particle : m_Particles)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, particle->GetParticleTexture()->GetTextureID());

		if (particle->GetLiveStatus())
		{
			_updateModelViewMatrix(particle->GetPosition(), particle->GetRotation(), particle->GetScale(), PiRenderer->GetCamera()->GetViewMatrix());

			//bind texture and draw
			//draw call
			glBindVertexArray(m_uVAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uIBO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		
	}
}

bool ParticleRenderer::Initialize(const GLchar * a_strShaderName,std::vector<Particle*> &particleList)
{
	string shaderPath = PiResource->GetConfig()->GetShaderDir() + a_strShaderName;
	m_particleShader = PiResource->GetShader(shaderPath);
	m_particleShader->Use();
	m_particleShader->SetInt(m_particleShader->GetShaderID(), "particleTexture", 0);
	
	//init quad to draw particles
	_initQuad();
	m_projectionMatrix = PiRenderer->GetCamera()->GetProjectionMatrix();
	m_Particles = particleList;
	return true;
}

void ParticleRenderer::Update(float a_dt)
{
	
}

void ParticleRenderer::_initQuad()
{
	float fQuadVertices[] =
	{
		// Positions			//Normals			// TexCoord
		 0.5f,   0.5f,	0.0f,	0.0f,0.0f,-1.0f,	1.0f, 1.0f,
		 0.5f,  -0.5f,	0.0f,	0.0f,0.0f,-1.0f,	1.0f, 0.0f,
		-0.5f,  -0.5f,	0.0f,	0.0f,0.0f,-1.0f,	0.0f, 0.0f,
		-0.5f,   0.5f,	0.0f,	0.0f,0.0f,-1.0f,	0.0f, 1.0f
	};

	uint32 uQuadIndices[] =
	{
		0,1,3,
		3,2,1
	};

	//Generate Buffers
	glGenVertexArrays(1, &m_uVAO);

	glGenBuffers(1, &m_uVBO);
	glGenBuffers(1, &m_uIBO);

	//Bind Buffers and arrays
	glBindVertexArray(m_uVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fQuadVertices), fQuadVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uQuadIndices), uQuadIndices, GL_STATIC_DRAW);

	//Send the data as Input layout i.e Locations

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind the vbo and ibo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleRenderer::_updateModelViewMatrix(vec3 pos, float rotation, float scale, const mat4& viewMatrix)
{

	m_particleShader->Use();


	mat4 model(1);
	/*model[0][0] = viewMatrix[0][0];
	model[0][1] = viewMatrix[1][0];
	model[0][2] = viewMatrix[2][0];
	model[1][0] = viewMatrix[0][1];
	model[1][1] = viewMatrix[1][1];
	model[1][2] = viewMatrix[2][1];
	model[2][0] = viewMatrix[0][2];
	model[2][1] = viewMatrix[1][2];
	model[2][2] = viewMatrix[2][2];
*/
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(rotation), vec3(0, 0, 1));
	model = glm::scale(model,vec3(scale));

	mat4 modelView;
	
	if(m_ownerSystem->m_isGlobalSpace)
		modelView = viewMatrix * model;
	else
		modelView = viewMatrix * parentTransformation* model;
	

	//load this modelView in shader
	//set projection matrix
	m_particleShader->SetUniformMatrix4fv(m_particleShader->GetShaderID(),"modelViewMatrix", modelView);
	m_particleShader->SetUniformMatrix4fv(m_particleShader->GetShaderID(),"projectionMatrix", m_projectionMatrix);

}
