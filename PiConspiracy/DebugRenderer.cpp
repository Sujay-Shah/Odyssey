//
//  DebugRenderer.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/01/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "DebugRenderer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Material.h"

DebugRenderer::DebugRenderer():
m_uCubeVAO(0),
m_uCubeIBO(0),
m_uSphereVAO(0),
m_uSphereIBO(0),
m_uSphereIndexCount(0),
m_pShader(nullptr),
m_typeDraw(0)
{

}

DebugRenderer::~DebugRenderer()
{

}

bool DebugRenderer::Initialize(const GLchar* a_strShaderName)
{
	m_pShader = PiResource->GetShader(a_strShaderName);
	m_pShader->Use();

	CreateCube();
	CreateSphere();

	return true;
}

void DebugRenderer::CreateCube()
{
	float width_ = 1.0f;
	float height_ = 1.0f;
	float depth_ = 1.0f;

	float fCubeVertices[] =
	{
		//Position				      
		-width_, -height_, -depth_  ,
		 width_, -height_, -depth_  ,
		 width_,  height_, -depth_  ,
		 width_,  height_, -depth_  ,
		-width_,  height_, -depth_  ,
		-width_, -height_, -depth_  ,

		-width_, -height_,  depth_  ,
		 width_, -height_,  depth_  ,
		 width_,  height_,  depth_  ,
		 width_,  height_,  depth_  ,
		-width_,  height_,  depth_  ,
		-width_, -height_,  depth_  ,

		-width_,  height_,  depth_  ,
		-width_,  height_, -depth_  ,
		-width_, -height_, -depth_  ,
		-width_, -height_, -depth_  ,
		-width_, -height_,  depth_  ,
		-width_,  height_,  depth_  ,

		 width_,  height_,  depth_  , 
		 width_,  height_, -depth_  , 
		 width_, -height_, -depth_  , 
		 width_, -height_, -depth_  , 
		 width_, -height_,  depth_  , 
		 width_,  height_,  depth_  , 

		-width_, -height_, -depth_  ,
		 width_, -height_, -depth_  ,
		 width_, -height_,  depth_  ,
		 width_, -height_,  depth_  ,
		-width_, -height_,  depth_  ,
		-width_, -height_, -depth_  ,

		-width_,  height_, -depth_  ,
		 width_,  height_, -depth_  ,
		 width_,  height_,  depth_  ,
		 width_,  height_,  depth_  ,
		-width_,  height_,  depth_  ,
		-width_,  height_, -depth_  
	};


	uint32 uIndices[] =
	{
		//front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
	};

	uint32 VBO;
	//Generate Buffers
	glGenVertexArrays(1, &m_uCubeVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &m_uCubeIBO);

	//Bind Buffers and arrays
	glBindVertexArray(m_uCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fCubeVertices), fCubeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uCubeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uIndices), uIndices, GL_STATIC_DRAW);

	//Send the data as Input layout i.e Locations

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);  // was 6 
	glEnableVertexAttribArray(0);

	//unbind the vbo and ibo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void DebugRenderer::CreateSphere()
{
	glGenVertexArrays(1, &m_uSphereVAO);

	uint32 uVBO;
	glGenBuffers(1, &uVBO);
	glGenBuffers(1, &m_uSphereIBO);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 60;
	const unsigned int Y_SEGMENTS = 60;
	//const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	
	m_uSphereIndexCount = indices.size();

	std::vector<float> data;
	for (size_t i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
	}

	glBindVertexArray(m_uSphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uSphereIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	GLsizei stride = (3 + 2 + 3) * sizeof(float);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
}

void DebugRenderer::DrawCube(mat4 a_ModelMatrix)
{
	//bool bIsDebugCameraOn = PiImgui->IsDebugCameraOn();
	Camera* pCamera = PiRenderer->GetCamera();

	glm::mat4 Model_matrix;
	Model_matrix = a_ModelMatrix;

	//View Matrix
	mat4 View_matrix = pCamera->GetViewMatrix();

	//Projection Matrix
	mat4 Projection_matrix = pCamera->GetProjectionMatrix();

	glm::mat4 MVP = Projection_matrix * View_matrix * Model_matrix;
	m_pShader->SetUniformMatrix4fv(m_pShader->GetShaderID(), "MVP", MVP);

	glBindVertexArray(m_uCubeVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uCubeIBO);
	glLineWidth(2.5f);
	glDrawArrays(GL_LINE_LOOP, 0, 36);

	glBindVertexArray(0);
}

void DebugRenderer::DrawSphere(mat4 a_ModelMatrix)
{
	//bool bIsDebugCameraOn = PiImgui->IsDebugCameraOn();
	Camera* pCamera = PiRenderer->GetCamera();

	glm::mat4 Model_matrix;
	Model_matrix = a_ModelMatrix;

	//View Matrix
	mat4 View_matrix = pCamera->GetViewMatrix();

	//Projection Matrix
	mat4 Projection_matrix = pCamera->GetProjectionMatrix();

	glm::mat4 MVP = Projection_matrix * View_matrix * Model_matrix;
	m_pShader->SetUniformMatrix4fv(m_pShader->GetShaderID(), "MVP", MVP);

	glBindVertexArray(m_uSphereVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uSphereIBO);
	glLineWidth(4.0f);
	glDrawElements(GL_LINE_LOOP, (GLsizei)m_uSphereIndexCount, GL_UNSIGNED_INT, (void*)0);
}


void DebugRenderer::Draw(Material* a_pMaterial)
{
	m_pShader->Use();
	//Cube
	if (m_typeDraw == 0)
	{
		DrawCube(m_ModelMatrix);
	}
	//Sphere
	if (m_typeDraw == 1)
	{
		DrawSphere(m_ModelMatrix);
	}

}