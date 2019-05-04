//
//  ImportMesh.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ImportMesh.h"
#include "Material.h"
#include "TextureData.h"
#include "ShaderManager.h"
#include "Renderer.h"
#include "FrameBuffer.h"


ImportMesh::ImportMesh()
{
}

ImportMesh::ImportMesh(vector<Vertex> a_vertices, vector<uint32> a_indices, vector<Texture> a_textures)
{
	m_vertices = a_vertices;
	m_indices = a_indices;
	m_textures = a_textures;

	//m_instanceCount = 1;
	//
	////float dt = 0.0f;
	//m_InstancetranslationVector.resize(m_instanceCount * m_instanceCount);
	//
	//for (int k = 0; k < m_instanceCount; ++k)
	//{
	//	int rand_test = rand() % 30;
	//	m_InstancetranslationVector[k].x = k + 5.0f*k;
	//	m_InstancetranslationVector[k].y = 0;
	//	m_InstancetranslationVector[k].z = 0;
	//
	//}


	Initialize();
}

ImportMesh::~ImportMesh()
{

}

void Vertex::addBoneData(uint32 a_boneId, float a_weight)
{
	for (uint32 i = 0; i < NUM_BONES_PER_VERTEX; i++)
	{
		if (m_weights[i] == 0.0)
		{
			m_ids[i] = a_boneId;
			m_weights[i] = a_weight;
			return;
		}
	}
}

void ImportMesh::Initialize()
{

	
	
	glGenVertexArrays(1, &m_uVAO);
	glGenBuffers(1, &m_uVBO);
	glGenBuffers(1, &m_uEBO);

	glBindVertexArray(m_uVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32), &m_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));

	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_ids));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_weights));

	//==================================================================
	//Instancing Part
	//==================================================================

	//glGenBuffers(1, &m_instanceVBO);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_instanceCount, &m_InstancetranslationVector[0], GL_STATIC_DRAW);
	//
	//
	//glEnableVertexAttribArray(3);
	//glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//glVertexAttribDivisor(3, 1);

	//==================================================================




	glBindVertexArray(0);
}

void ImportMesh::Draw(Material* a_pMaterial)
{
	TextureData* pTextureAlbedoMap = a_pMaterial->GetTexture(Material::eTextureType_Albedo);
	if (pTextureAlbedoMap)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pTextureAlbedoMap->GetTexData()->textureID);
	}

	//For Normal Map
	TextureData* pTextureNormalMap = a_pMaterial->GetTexture(Material::eTextureType_Normal);
	if (pTextureNormalMap)
	{
		ShaderManager* pShader = a_pMaterial->GetShader();
		pShader->SetUniform1f(pShader->GetShaderID(), "strmaterial.isNormalMapped", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pTextureNormalMap->GetTexData()->textureID);
	}

	//FrameBuffer* pBuffer = PiRenderer->GetFrameBuffer();
	//pBuffer->BindForReading(GL_TEXTURE7);
	//glUniform1i(101, GL_TEXTURE7);

	glBindVertexArray(m_uVAO);
	glDrawElements(GL_TRIANGLES, (int32)m_indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArraysInstanced(GL_LINE_LOOP, 0, 36, m_instanceCount);
	glBindVertexArray(0);

}