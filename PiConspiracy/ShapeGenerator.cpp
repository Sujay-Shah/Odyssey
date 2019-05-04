//
//  ShapeGenerator.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/26/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ShaderManager.h"
#include <random>
#include "FrameRateController.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "TextureData.h"
#include "ImguiManager.h"
#include "RandomNumberGenerator.h"
#include "FrameBuffer.h"

#include "GameObjectManager.h"
#include "LightComponent.h"
#include "LightManager.h"
#include "TextureData.h"
#include "Engine.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"


//=====================================================================
//Input Layout Locations Order
//=====================================================================
//0->Position
//1->Normals
//2->TexCoords
//3->Instancing Offset
//4->Color?
//=====================================================================

ShapeGenerator::ShapeGenerator():
m_shaderID(nullptr)
{
	//================================================================
	//Testing
	//Instance Data
	//================================================================
	
	#pragma region Instancing Data
	
	/*m_instanceCount = 30;

	//float dt = 0.0f;
	m_InstancetranslationVector.resize(m_instanceCount * m_instanceCount);
	
	for (int k = 0; k < m_instanceCount; ++k)
	{
		int rand_test = rand() % 30;
		m_InstancetranslationVector[k].x = k + 2.0f*k;
		m_InstancetranslationVector[k].y = 0;
		m_InstancetranslationVector[k].z = 0;

	}*/
	#pragma endregion

}

ShapeGenerator::~ShapeGenerator()
{

}

bool ShapeGenerator::InitializePrimitive(uint32 a_uType, Material* a_pMaterial)
{
    m_shaderID = a_pMaterial->GetShader();
	
    m_shaderID->Use();
	uint32 uShaderID = m_shaderID->GetShaderID();

	m_shaderID->SetInt(uShaderID, "AlbedoTexture", 0);
	m_shaderID->SetInt(uShaderID, "NormalMapTexture", 1);
	m_shaderID->SetInt(uShaderID, "textureskybox", 0);
	m_shaderID->SetInt(uShaderID, "PerlinNoise0", 3);
	m_shaderID->SetInt(uShaderID, "PerlinNoise1", 4);
	m_shaderID->SetInt(uShaderID, "PerlinNoise2", 5);
	m_shaderID->SetInt(uShaderID, "PerlinNoise3", 6);
	m_shaderID->SetInt(uShaderID, "depthMap", 7);

	m_shaderID->SetInt(uShaderID, "CoolDownTexture", 8);
	m_shaderID->SetInt(uShaderID, "TextureMap", 9);//For the Tutorial texture
	
	switch (a_uType)
	{
		case Mesh::eType_Triangle:
								Triangle_Generator();
								break;
		case Mesh::eType_Quad:
								Quad_Generator();
								break;
		case Mesh::eType_Cube:
								Cube_Generator(false, 0);
								break;
		case Mesh::eType_Sphere:
								Sphere_Generator();
								break;
		default:
				break;
	}

    return true;
}

void ShapeGenerator::Triangle_Generator()
{
	float fTriangleVertices[] = 
    {
		 2.0f,	 2.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 3.0f,  1.0f, 0.0f
	};

	uint32 uIndices[] =
	{
		0,1,2
	};
    glGenVertexArrays(1, &m_PrimitiveMeshes[eShapeType_Triangle].m_uVAO);
    glGenBuffers(1, &m_PrimitiveMeshes[eShapeType_Triangle].m_uVBO);
    glGenBuffers(1, &m_PrimitiveMeshes[eShapeType_Triangle].m_uIBO);

    //Bind Buffers and arrays
    glBindVertexArray(m_PrimitiveMeshes[eShapeType_Triangle].m_uVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_PrimitiveMeshes[eShapeType_Triangle].m_uVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fTriangleVertices), fTriangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PrimitiveMeshes[eShapeType_Triangle].m_uIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uIndices), uIndices, GL_STATIC_DRAW);

	//Send the data as Input layout i.e Locations

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	//unbind the vbo and ibo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ShapeGenerator::Quad_Generator()
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
    glGenVertexArrays(1, &m_PrimitiveMeshes[eShapeType_Quad].m_uVAO);

    glGenBuffers(1, &m_PrimitiveMeshes[eShapeType_Quad].m_uVBO);
    glGenBuffers(1, &m_PrimitiveMeshes[eShapeType_Quad].m_uIBO);
	
    //Bind Buffers and arrays
    glBindVertexArray(m_PrimitiveMeshes[eShapeType_Quad].m_uVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_PrimitiveMeshes[eShapeType_Quad].m_uVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fQuadVertices), fQuadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PrimitiveMeshes[eShapeType_Quad].m_uIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uQuadIndices), uQuadIndices, GL_STATIC_DRAW);

	//Send the data as Input layout i.e Locations

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,(void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind the vbo and ibo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}


void ShapeGenerator::Cube_Generator(bool a_bIsInstancingOn, uint32 a_uInstanceCount)
{
	float width_ = 1.0f;
	float height_ = 1.0f;
	float depth_ = 1.0f;

	float fCubeVertices[] = 
    {
		//Position				        //Normals				//TexCoords
		-width_, -height_, -depth_  ,	 0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
		 width_, -height_, -depth_  ,	 0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
		 width_,  height_, -depth_  ,	 0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
		 width_,  height_, -depth_  ,	 0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
		-width_,  height_, -depth_  ,	 0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
		-width_, -height_, -depth_  ,	 0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
						     
		-width_, -height_,  depth_  ,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
		 width_, -height_,  depth_  ,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
		 width_,  height_,  depth_  ,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		 width_,  height_,  depth_  ,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		-width_,  height_,  depth_  ,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
		-width_, -height_,  depth_  ,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
						     
		-width_,  height_,  depth_  ,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-width_,  height_, -depth_  ,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-width_, -height_, -depth_  ,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-width_, -height_, -depth_  ,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-width_, -height_,  depth_  ,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-width_,  height_,  depth_  ,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
						     
		 width_,  height_,  depth_  ,  	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		 width_,  height_, -depth_  ,  	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		 width_, -height_, -depth_  ,  	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		 width_, -height_, -depth_  ,  	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		 width_, -height_,  depth_  ,  	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		 width_,  height_,  depth_  ,  	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
						     
		-width_, -height_, -depth_  ,	 0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,
		 width_, -height_, -depth_  ,	 0.0f,  -1.0f,  0.0f,	1.0f, 1.0f,
		 width_, -height_,  depth_  ,	 0.0f,  -1.0f,  0.0f,	1.0f, 0.0f,
		 width_, -height_,  depth_  ,	 0.0f,  -1.0f,  0.0f,	1.0f, 0.0f,
		-width_, -height_,  depth_  ,	 0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,
		-width_, -height_, -depth_  ,	 0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,

		-width_,  height_, -depth_  ,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
		 width_,  height_, -depth_  ,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		 width_,  height_,  depth_  ,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		 width_,  height_,  depth_  ,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		-width_,  height_,  depth_  ,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		-width_,  height_, -depth_  ,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f
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

	//Generate Buffers
	glGenVertexArrays(1, &m_PrimitiveMeshes[eShapeType_Cube].m_uVAO);
	glGenBuffers(1, &m_PrimitiveMeshes[eShapeType_Cube].m_uVBO);
	glGenBuffers(1, &m_PrimitiveMeshes[eShapeType_Cube].m_uIBO);

	//Bind Buffers and arrays
	glBindVertexArray(m_PrimitiveMeshes[eShapeType_Cube].m_uVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_PrimitiveMeshes[eShapeType_Cube].m_uVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fCubeVertices), fCubeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PrimitiveMeshes[eShapeType_Cube].m_uIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uIndices), uIndices, GL_STATIC_DRAW);

	//Send the data as Input layout i.e Locations

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);  // was 6 
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//=============================================
	//		INSTANCING
	//=============================================
	if (a_bIsInstancingOn)
	{
		
		glGenBuffers(1, &m_instanceVBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_instanceCount, &m_InstancetranslationVector[0], GL_STATIC_DRAW);


		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glVertexAttribDivisor(3, 1);

	}
	//=============================================

	//unbind the vbo and ibo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


//===============================================
//Credit: JOEY DE VRIES 
//www.learnopengl.com
//===============================================
//===============================================

void ShapeGenerator::Sphere_Generator()
{

	glGenVertexArrays(1, &m_PrimitiveMeshes[eShapeType_Sphere].m_uVAO);

	//unsigned int vbo, ebo;
	glGenBuffers(1, &m_PrimitiveMeshes[eShapeType_Sphere].m_uVBO);
	glGenBuffers(1, &m_PrimitiveMeshes[eShapeType_Sphere].m_uIBO);

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
	m_indexCount = indices.size();

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

	glBindVertexArray(m_PrimitiveMeshes[eShapeType_Sphere].m_uVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_PrimitiveMeshes[eShapeType_Sphere].m_uVBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PrimitiveMeshes[eShapeType_Sphere].m_uIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	GLsizei stride = (3 + 2 + 3) * sizeof(float);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(1);


}

float counter = 0.0f;
uint32 uCount = 0;

void ShapeGenerator::Draw(Material* a_pMaterial, 
						  MeshPrimitive a_Primitive, 
						  mat4 a_modelMatrix, uint32 a_uCode, bool a_bIsFreeCamOn)
{
	//==============================
	//Uniform Variables Update
	//==============================
		//PiRenderer->SetDebugCamMode(a_bIsFreeCamOn);
		Camera* pCamera = PiRenderer->GetCamera();

		m_shaderID = a_pMaterial->GetShader();
		m_shaderID->Use();

		glm::mat4 lsm = PiRenderer->m_LightProjectionMatrix * PiRenderer->m_LightView;
		m_shaderID->SetUniformMatrix4fv(m_shaderID->GetShaderID(), "LightSpaceMatrix", lsm);
		Renderer::FogParameters fogParameters = PiRenderer->GetFogParameters();
		
		//if (a_pMaterial != PiRenderer->GetShadowMaterial())
		//{
		//	FrameBuffer* pBuffer = PiRenderer->GetFrameBuffer();
		//	pBuffer->BindForReading(GL_TEXTURE7);
		//}
	#pragma region MVP-matrix
		//Model Matrix

		glm::mat4 Model_matrix;
 		Model_matrix = a_modelMatrix;//translate_mat * rotate_mat * scale_mat;

		m_shaderID->SetUniformMatrix4fv(m_shaderID->GetShaderID(), "model_Matrix", Model_matrix);

		/************************debug from LPOV*/
		m_shaderID->SetUniformMatrix4fv(m_shaderID->GetShaderID(), "view_Matrix", pCamera->GetViewMatrix());

		//Projection Matrix
		mat4 Projection_matrix = pCamera->GetProjectionMatrix();
		m_shaderID->SetUniformMatrix4fv(m_shaderID->GetShaderID(), "projection_Matrix", pCamera->GetProjectionMatrix());
		/************************/

		m_shaderID->SetUniformMatrix4fv(m_shaderID->GetShaderID(), "orthographic_Matrix", pCamera->GetOrthoProjectionMatrix());

		//set HUD cooldown texture
		m_shaderID->SetInt(m_shaderID->GetShaderID(), "isCoolDown", a_pMaterial->m_isCoolDown);

		//set custom alpha if any
		/*if (a_pMaterial->m_customAlpha)
		{
			m_shaderID->SetInt(m_shaderID->GetShaderID(), "customAlpha", a_pMaterial->m_customAlpha);
			m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "alpha", a_pMaterial->m_alpha);
		}*/
		
#pragma endregion


	#pragma region TimeUniform

	float fDeltaTime = PiFPS->GetDeltaTime();
	m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "DeltaTime", fDeltaTime);

	#pragma endregion


	//=================================================
	//For Light Uniforms
	//=================================================

	#pragma region Light-Uniforms
	


	if (a_pMaterial->m_bIsReceiveLight)
	{

		int Lmode;
#ifdef _DEBUG
		{
			Lmode = PiImgui->GetLightModel();
		}
#else 
		{
			Lmode = PiRenderer->GetFogParameters().m_uLightMode;//2;
		}
#endif
		
		m_shaderID->SetInt(m_shaderID->GetShaderID(), "LightMode", Lmode);

		float Lintensity =  PiImgui->GetLightIntensity();//fogParameters.m_fLightIntensity;//
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "Lightintensity", Lintensity);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "near_plane", PiRenderer->near_plane);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "far_plane", PiRenderer->far_plane);

		vec3 cameraPos = pCamera->GetPosition();
		m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);

		//vec3 pos_player = PiGameObject->m_Player->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition();
		
		LightManager* lightmgr_obj = PiRenderer->GetLightManager();
		vec3 lightcolor		= lightmgr_obj->getDirectionalLightList()[0].DirectionalColor;
		vec3 lightPosition	= lightmgr_obj->getDirectionalLightList()[0].DirectionalDir;
		//lightPosition.z += pos_player.z;


		m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "lightColor", lightcolor.x, lightcolor.y, lightcolor.z);
		m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "lightPos"	, lightPosition.x, lightPosition.y, lightPosition.z);

		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "pLight.constant", 0.10f);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "pLight.linear", 0.09f);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "pLight.quadratic", 0.032f);

		
	}
	#pragma endregion
	
	
	#pragma region Object-Properties

	
	vec3 color_obj = a_pMaterial->GetColor();
	vec3 light_color_obj = a_pMaterial->GetBrightColor();

	m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "strmaterial.objectCol", color_obj.x, color_obj.y, color_obj.z);
	m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "strmaterial.LightCol", light_color_obj.x, light_color_obj.y, light_color_obj.z);

	
	//If Mesh has reflective property then send skybox Texture
	//a_isMeshReflective = 1;
	if(a_pMaterial->m_bIsReflective)
	{
		vec3 cameraPos = pCamera->GetPosition();
		m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);


	}

#pragma endregion

	#pragma region Material-Properties

	vec3 diffuse_color_obj = a_pMaterial->GetDiffuseColor();
	vec4 specular_color_obj = a_pMaterial->GetSpecularColor();

	m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "strmaterial.diffuse", diffuse_color_obj.x, diffuse_color_obj.y, diffuse_color_obj.z);
	m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "strmaterial.specular", specular_color_obj.x, specular_color_obj.y, specular_color_obj.z);
	m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "strmaterial.shininess", specular_color_obj.w);
	m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "strmaterial.isNormalMapped", 0);

	#pragma endregion


	#pragma region Fog-Properties

#ifdef _DEBUG
	{

	glm::vec3 fog_color = PiImgui->getFogColor();
	ImguiManager::FogParameters IM_fogParameters = PiImgui->GetFogParameters();

		m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "fogColor", fog_color.x, fog_color.y, fog_color.z);
	//m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "fogColor", IM_fogParameters..x, IM_fogParameters.m_vColor.y, IM_fogParameters.m_vColor.z);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "fogStartDistance", IM_fogParameters.m_fFogStartDistance);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "fogEndDistance", IM_fogParameters.m_fFogEndDistance);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "fogDensity", IM_fogParameters.m_fFogDensity);//m_fFogDensity
		m_shaderID->SetInt(m_shaderID->GetShaderID(), "fogDepth", IM_fogParameters.m_uDepthType);//m_uDepthType
		m_shaderID->SetInt(m_shaderID->GetShaderID(), "fogSelector", IM_fogParameters.m_uFogSelector);
		m_shaderID->SetInt(m_shaderID->GetShaderID(), "fogSelector", IM_fogParameters.m_uFogSelector);

		if (fogParameters.m_bfogEnabled == true)
		{
			m_shaderID->SetInt(m_shaderID->GetShaderID(), "FogEnable", 1);
		}
		else
		{
			m_shaderID->SetInt(m_shaderID->GetShaderID(), "FogEnable", 0);
		}
	}
#else 
	{
		//glm::vec3 fog_color = PiImgui->getFogColor();

	//ImguiManager::FogParameters fogParameters = PiImgui->GetFogParameters();


	//m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "fogColor", fog_color.x, fog_color.y, fog_color.z);
		m_shaderID->SetUniform3f(m_shaderID->GetShaderID(), "fogColor", fogParameters.m_vColor.x, fogParameters.m_vColor.y, fogParameters.m_vColor.z);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "fogStartDistance", fogParameters.m_fFogStartDistance);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "fogEndDistance", fogParameters.m_fFogEndDistance);
		m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "fogDensity", fogParameters.m_fFogDensity);//m_fFogDensity
		m_shaderID->SetInt(m_shaderID->GetShaderID(), "fogDepth", fogParameters.m_uFogDepth);//m_uDepthType
		m_shaderID->SetInt(m_shaderID->GetShaderID(), "fogSelector", fogParameters.m_uFogSelector);
		m_shaderID->SetInt(m_shaderID->GetShaderID(), "fogSelector", fogParameters.m_uFogSelector);

		if (fogParameters.m_bfogEnabled == true)
		{
			m_shaderID->SetInt(m_shaderID->GetShaderID(), "FogEnable", 1);
		}
		else
		{
			m_shaderID->SetInt(m_shaderID->GetShaderID(), "FogEnable", 0);
		}
	}
	
#endif
	#pragma endregion



	#pragma region Texture-region
    TextureData* pTextureAlbedoMap		=	 a_pMaterial->GetTexture(Material::eTextureType_Albedo);
    TextureData* pTextureNormalMap		=	 a_pMaterial->GetTexture(Material::eTextureType_Normal);
	TextureData* pTexturePerlinNoise0	=	 a_pMaterial->GetTexture(Material::eTextureType_PerlinNoise0);
	TextureData* pTexturePerlinNoise1	=	 a_pMaterial->GetTexture(Material::eTextureType_PerlinNoise1);
	TextureData* pTexturePerlinNoise2	=	 a_pMaterial->GetTexture(Material::eTextureType_PerlinNoise2);
	TextureData* pTexturePerlinNoise3	=	 a_pMaterial->GetTexture(Material::eTextureType_PerlinNoise3);
	TextureData* pCoolDownTexture		=	 a_pMaterial->GetTexture(Material::eTextureType_CoolDownTexture);
	TextureData* pleftTexture			=	 a_pMaterial->GetTexture(Material::eTextureType_Left);
	TextureData* prightTexture			=	 a_pMaterial->GetTexture(Material::eTextureType_Right);
	TextureData* pjumpTexture			=	 a_pMaterial->GetTexture(Material::eTextureType_Jump);
	TextureData* pboostTexture			=	 a_pMaterial->GetTexture(Material::eTextureType_Boost);
	TextureData* pelifeTexture			=	 a_pMaterial->GetTexture(Material::eTextureType_ELife);
	TextureData* ppickupTexture			=	 a_pMaterial->GetTexture(Material::eTextureType_Pickups);
	TextureData* pTextureDatasky		=	 PiResource->GetSkyBoxTextureId("skybox");
#pragma endregion

	//=================================================
	// Update and bind m_vao
	//=================================================

	//Triangle
	#pragma region Triangle-Bind

	if (a_uCode == 0)
	{
		glBindVertexArray(a_Primitive.m_uVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_Primitive.m_uIBO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}

	#pragma endregion

	//quad
	#pragma region Quad-Bind
	if (a_uCode == 1)
	{
		//stbi_set_flip_vertically_on_load(true);
      	
		//For Albedo Texture
		if (pTextureAlbedoMap)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pTextureAlbedoMap->GetTexData()->textureID);
		}

		//stbi_set_flip_vertically_on_load(false);

		//For Normal Map
		if (pTextureNormalMap)
		{
			m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "strmaterial.isNormalMapped", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, pTextureNormalMap->GetTexData()->textureID);
		}

		// Perline Noise 0
		if (pTexturePerlinNoise0)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, pTexturePerlinNoise0->GetTexData()->textureID);
		}

		// Perline Noise 1
		if (pTexturePerlinNoise1)
		{
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, pTexturePerlinNoise1->GetTexData()->textureID);
		}

		// Perline Noise 2
		if (pTexturePerlinNoise2)
		{
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, pTexturePerlinNoise2->GetTexData()->textureID);
		}

		// Perline Noise 3
		if (pTexturePerlinNoise3)
		{
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_2D, pTexturePerlinNoise3->GetTexData()->textureID);
		}

		if (pCoolDownTexture)
		{
			glActiveTexture(GL_TEXTURE8);
			glBindTexture(GL_TEXTURE_2D, pCoolDownTexture->GetTexData()->textureID);
		}

		
		//bool state_cond = a_pMaterial->getTutorialTextureState().m_leftTexture;
		//
		//LOG_DEBUG(state_cond);

		//Left
		if (a_pMaterial->m_leftTexture)
		{
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_2D, pleftTexture->GetTexData()->textureID);
		}

		//Right
		if (a_pMaterial->m_rightTexture)
		{
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_2D, prightTexture->GetTexData()->textureID);
		}

		//Jump
		if (a_pMaterial->m_jumpTexture)
		{
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_2D, pjumpTexture->GetTexData()->textureID);
		}

		//Boost
		if (a_pMaterial->m_BoostTexture)
		{
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_2D, pboostTexture->GetTexData()->textureID);
		}

		//ELife
		if (a_pMaterial->m_ElifeTexture)
		{
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_2D, pelifeTexture->GetTexData()->textureID);
		}

		//PickUp
		if (a_pMaterial->m_PickupTexture)
		{
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_2D, ppickupTexture->GetTexData()->textureID);
		}

		glBindVertexArray(a_Primitive.m_uVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_Primitive.m_uIBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
				

	}
	#pragma endregion

	//Cube
	#pragma region Cube-Bind

	if (a_uCode == 2 )
	{
		//if (a_pMaterial->m_bIsReflective == 1)
		//{
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, pTextureDatasky->GetTexData()->textureID);
		//
		//	glBindVertexArray(a_Primitive.m_uVAO);
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_Primitive.m_uIBO);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//else
		{
			//For Albedo Texture
			if (pTextureAlbedoMap)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pTextureAlbedoMap->GetTexData()->textureID);
			}

			//For Normal Map
			if (pTextureNormalMap)
			{
				m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "strmaterial.isNormalMapped", 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, pTextureNormalMap->GetTexData()->textureID);
			}

			glBindVertexArray(a_Primitive.m_uVAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_Primitive.m_uIBO);

			if (a_pMaterial->m_bIsInstanced == true)
			{
				glDrawArraysInstanced(GL_TRIANGLES, 0, 36, m_instanceCount);
			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

		}
	}
		
	#pragma endregion


	//Sphere
	#pragma region Sphere-Bind
	
	if (a_uCode == 3)
	{
		//if (a_pMaterial->m_bIsReflective == 1)
		//{
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, pTextureAlbedoMap->GetTexData()->textureID);
		//
		//}
		//else 
		{
			//For Albedo Texture
			if (pTextureAlbedoMap)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pTextureAlbedoMap->GetTexData()->textureID);
			}

			//For Normal Map
			if (pTextureNormalMap)
			{
				m_shaderID->SetUniform1f(m_shaderID->GetShaderID(), "strmaterial.isNormalMapped", 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, pTextureNormalMap->GetTexData()->textureID);
			}

		}
		
		glBindVertexArray(a_Primitive.m_uVAO);
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)m_indexCount, GL_UNSIGNED_INT, (void*)0);
	}
	#pragma endregion
}

void ShapeGenerator::Destroy()
{
	//Clears the current shader ID
	m_shaderID->ClearShader();

	//Delete the Shader Pointer which we made
	SAFE_DELETE(m_shaderID);
}

