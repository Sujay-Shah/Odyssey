//
//  SkyBox.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/05/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "SkyBox.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "stb_image.h"
#include "Renderer.h"
#include "Camera.h"
#include "RenderableContainer.h"
#include "TextureData.h"
#include "ImguiManager.h"
#include "Material.h"

SkyBox::SkyBox()
{

}


SkyBox::~SkyBox()
{
	//SAFE_DELETE(m_shaderID);//mem leak add
}

bool SkyBox::InitializeSkyBox(const GLchar* ShaderPath)
{
	m_shaderID = PiResource->GetShader(ShaderPath);

	m_shaderID->Use();
	m_shaderID->SetInt(m_shaderID->GetShaderID(),"textureskybox", 0);
	
	LoadSkyBox();

	return true;
}

void SkyBox::LoadSkyBox()
{
	//Loads Texture
	
	//====================================================
	// TO DO Rohit : serialize Later 
	//====================================================

	m_texture = PiResource->GetSkyBoxTextureId("skybox");    
	//Gen and Bind  all the skybox data
	float sizeofskybox = 1.0f;
	float vertices[] = {

		-sizeofskybox,	sizeofskybox, -sizeofskybox,
		-sizeofskybox, -sizeofskybox, -sizeofskybox,
		 sizeofskybox, -sizeofskybox, -sizeofskybox,
		 sizeofskybox, -sizeofskybox, -sizeofskybox,
		 sizeofskybox,	sizeofskybox, -sizeofskybox,
		-sizeofskybox,	sizeofskybox, -sizeofskybox,

		-sizeofskybox,	-sizeofskybox,	sizeofskybox,
		-sizeofskybox,	-sizeofskybox, -sizeofskybox,
		-sizeofskybox,	 sizeofskybox, -sizeofskybox,
		-sizeofskybox,	 sizeofskybox, -sizeofskybox,
		-sizeofskybox,	 sizeofskybox,	sizeofskybox,
		-sizeofskybox,	-sizeofskybox,	sizeofskybox,

		 sizeofskybox,	-sizeofskybox,	-sizeofskybox,
		 sizeofskybox,	-sizeofskybox,   sizeofskybox,
		 sizeofskybox,	 sizeofskybox,   sizeofskybox,
		 sizeofskybox,	 sizeofskybox,   sizeofskybox,
		 sizeofskybox,	 sizeofskybox,	-sizeofskybox,
		 sizeofskybox,	-sizeofskybox,	-sizeofskybox,

		-sizeofskybox,	-sizeofskybox,	 sizeofskybox,
		-sizeofskybox,	 sizeofskybox,   sizeofskybox,
		 sizeofskybox,	 sizeofskybox,   sizeofskybox,
		 sizeofskybox,	 sizeofskybox,   sizeofskybox,
		 sizeofskybox,	-sizeofskybox,	 sizeofskybox,
		-sizeofskybox,	-sizeofskybox,	 sizeofskybox,

		-sizeofskybox,	sizeofskybox,	 -sizeofskybox,
		 sizeofskybox,	sizeofskybox,    -sizeofskybox,
		 sizeofskybox,	sizeofskybox,     sizeofskybox,
		 sizeofskybox,	sizeofskybox,     sizeofskybox,
		-sizeofskybox,	sizeofskybox,	  sizeofskybox,
		-sizeofskybox,	sizeofskybox,	 -sizeofskybox,

		-sizeofskybox,	-sizeofskybox,   -sizeofskybox,
		-sizeofskybox,	-sizeofskybox,    sizeofskybox,
		 sizeofskybox,	-sizeofskybox,   -sizeofskybox,
		 sizeofskybox,	-sizeofskybox,   -sizeofskybox,
		-sizeofskybox,	-sizeofskybox,    sizeofskybox,
		 sizeofskybox,	-sizeofskybox,    sizeofskybox


	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);

}

void SkyBox::Update()
{
	if (m_bIsRenderable)
	{
		PiRenderer->GetRenderableContainer()->PushRenderable(this);
	}
	//m_bIsRenderable = true;
}

void SkyBox::Draw(Material* a_pMaterial)
{
	if (a_pMaterial)
	{
		return;
	}
	//bool bIsFreeCamOn = PiImgui->IsDebugCameraOn();
	Camera* pCamera = PiRenderer->GetCamera();
	float scale_factor = 10000.0f;

	glDepthFunc(GL_LEQUAL);
	m_shaderID->Use();

	Renderer::FogParameters fogParameters = PiRenderer->GetFogParameters();

	#pragma region MVP
	mat4 scale_mat = mat4(1);
	scale_mat = glm::scale(mat4(1),vec3(scale_factor));

	vec3 cubemap_pos = vec3(10, 10, 10) + pCamera->GetPosition();

	glm::mat4 translate_mat = glm::mat4(1);
	translate_mat = glm::translate(mat4(1), cubemap_pos);

	mat4 model_Matrix;
	model_Matrix = translate_mat * scale_mat;
	m_shaderID->SetUniformMatrix4fv(m_shaderID->GetShaderID(), "Model_Matrix", model_Matrix);

	//view matrix
	mat4 View_matrix = pCamera->GetViewMatrix();
	m_shaderID->SetUniformMatrix4fv(m_shaderID->GetShaderID(), "View_matrix", (View_matrix));

	//Projection matrix
	mat4 Projection_matrix = pCamera->GetProjectionMatrix();
	m_shaderID->SetUniformMatrix4fv(m_shaderID->GetShaderID(), "Projection_matrix", (Projection_matrix));
	#pragma endregion

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

	glBindVertexArray(m_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->GetTexData()->m_SkyboxTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_FALSE);

}
