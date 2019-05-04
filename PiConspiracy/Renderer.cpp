//
//  Renderer.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Renderer.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "FrameRateController.h"
#include "Camera.h"
#include "Mesh.h"
#include "SkyBox.h"
#include "ShaderManager.h"
#include "InputManager.h"
#include "ImguiManager.h"
#include "FrameBuffer.h"
#include "RenderableContainer.h"
#include "LightManager.h"
#include "DebugRenderer.h"
#include "GameObjectManager.h"
#include "DebugRenderer.h"
#include "Material.h"

bool forwardx = true;
bool forwardy = true;
bool forwardz = true;


Renderer::Renderer():
m_pSkybox(nullptr),
m_pShapeGenerator(nullptr),
//m_pShadowBuffer(nullptr),
m_pRenderableContainer(nullptr),
m_pUIContainer(nullptr),
m_pLightManager(nullptr),
m_pShadowMaterial(nullptr),
m_bIsDebugDrawActive(false),
m_pDebugRenderer(nullptr),
m_DepthMapQuad(nullptr),
m_pBloomFinalProgram(nullptr)
{
	ZERO_MEM(m_pCameras);
}

#define SHADOW_RESOLUTION 512

Renderer::~Renderer()
{
	SAFE_DELETE(m_pDebugRenderer);

    //SAFE_DELETE(m_pShadowBuffer);

    SAFE_DELETE(m_pShapeGenerator);

	SAFE_DELETE(m_pSkybox);
	
	SAFE_DELETE_ARRAY_POINTERS(m_pCameras, eCamera_Max);

	SAFE_DELETE(m_pLightManager);

    SAFE_DELETE(m_pRenderableContainer);

	SAFE_DELETE(m_pUIContainer);//mem leak add
}

bool Renderer::Initialize()
{
	if (!InitializeGLCalls(PiEngine->GetClientWidth(), PiEngine->GetClientHeight()))
	{
        LOG_CRITICAL("OpenGL Initializing Failed!!");
		return false;
	}
    LOG_INFO("OpenGL Initialized Successfully!!");

	m_pCameras[eCamera_Main] = new Camera(eCamera_Main);
	m_pCameras[eCamera_FreeMove] = new Camera(eCamera_FreeMove);
	m_pCameras[eCamera_UI] = new Camera(eCamera_UI);

	//init main cam
	m_pCameras[eCamera_Main]->Initialize();

    m_pSkybox						= new SkyBox;
	m_pShapeGenerator				= new ShapeGenerator;
	m_pLightManager					= new LightManager;
    m_pRenderableContainer			= new RenderableContainer;
	m_pUIContainer					= new RenderableContainer;
	m_pDebugRenderer				= new DebugRenderer;


    PTR_INITIALIZE(m_pRenderableContainer, "Renderer:: m_pRenderQueueContainer");

	if (!m_pDebugRenderer->Initialize("Assets/Shaders/Basic"))
	{
		return false;
	}

	if (!m_pSkybox->InitializeSkyBox("Assets/Shaders/SkyBox"))
	{
		return false;
	}

	CreateQuadForBloomFBO();
	
	//FUNCTION_INITIALIZE(LoadShadowMap, "Renderer::LoadedShadowMap", true);

	//m_DepthMapQuad = PiResource->GetShader("Assets/Shaders/DebugQuad");
	//m_DepthMapQuad->Use();
	//
	//m_DepthMapQuad->SetInt(m_DepthMapQuad->GetShaderID(), "depthMap", 0);

	SetFogParameters();
	InitializeBloom();

	return true;
}

bool Renderer::InitializeGLCalls(uint32 a_uScreenWidth, uint32 a_uScreenHeight)
{
	glViewport(0, 0, a_uScreenWidth, a_uScreenHeight);

	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
    return true;
}

void Renderer::SetFogParameters()
{
	m_FogParameters.m_fFogStartDistance = 0.040f;
	m_FogParameters.m_fFogEndDistance = 8000.0f;
	m_FogParameters.m_fFogDensity = 0.00f;
	m_FogParameters.m_uFogDepth = 2;
	m_FogParameters.m_uFogSelector = 1; 
	m_FogParameters.m_bfogEnabled = true;

	m_FogParameters.m_vColor = Color3(0.2, 0.29, 1.0); //Color3(0.6, 0.29, 0.5);// //(0.0,0.0,0.0);// (0.736f, 0.986f, 1.0f);
	m_FogParameters.m_uLightMode = 2;
	m_FogParameters.m_fLightIntensity = 1.0f;
}

void Renderer::Clear(vec3& a_vColor)
{
	glClearColor(a_vColor.x, a_vColor.y, a_vColor.z, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Renderer::LoadShadowMap()
{
	//m_pShadowBuffer = new FrameBuffer;
	//if (!m_pShadowBuffer->Initialize(SHADOW_RESOLUTION, SHADOW_RESOLUTION))
	//{
	//	return false;
	//}

	string strShadowMaterial = "ShadowMaterial.json";

	m_pShadowMaterial = PiResource->GetMaterial(strShadowMaterial);
    return true;
}

bool Renderer::InitializeBloom()
{
	//Initialize Bloom FBO
	
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	glGenTextures(2, bloombuffer);
	for (unsigned int i = 0; i < 2; ++i)
	{
	
		glBindTexture(GL_TEXTURE_2D, bloombuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, PiEngine->GetClientWidth(), PiEngine->GetClientHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_2D, bloombuffer[i], 0);

		
	}

	glGenRenderbuffers(1, &rbodepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbodepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, PiEngine->GetClientWidth(), PiEngine->GetClientHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbodepth);

	uint32 attachments[2] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	GLenum eStatus2 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (eStatus2 != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Bloom FBO Error, status: " << eStatus2 << std::endl;
		return false;
	}

	//UnBind FBO

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//==============================================================

	glGenFramebuffers(2, m_PingPongBlurFBO);
	glGenTextures(2, m_blurtex);

	for (unsigned int i = 0; i < 2; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongBlurFBO[i]);

		glBindTexture(GL_TEXTURE_2D, m_blurtex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, PiEngine->GetClientWidth(), PiEngine->GetClientHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurtex[i], 0);

		GLenum eStatus2 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (eStatus2 != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Ping Pong FBO Error, status: " << eStatus2 << std::endl;
			return false;
		}
	}

	//==============================================================
	//Initialize Shaders

	m_pBlurProgram			= PiResource->GetShader("Assets/Shaders/Blur");
	m_pBloomFinalProgram	= PiResource->GetShader("Assets/Shaders/BloomFinal");


	m_pBloomFinalProgram->Use();
	m_pBloomFinalProgram->SetInt(m_pBloomFinalProgram->GetShaderID(), "BloomBright", 0);
	m_pBloomFinalProgram->SetInt(m_pBloomFinalProgram->GetShaderID(), "FragColor", 1);

	return true;
}

void Renderer::CreateQuadForBloomFBO()
{
	m_uQuadVAO = 0;
	uint32 uQuadVBO;
	if (m_uQuadVAO == 0)
	{
		float fVertices[] =
		{
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &m_uQuadVAO);
		glGenBuffers(1, &uQuadVBO);
		glBindVertexArray(m_uQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, uQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(fVertices), &fVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
}

void Renderer::RenderQuad(ShaderManager* a_pshader)
{
	Camera* pCamera = PiRenderer->GetMainCamera();
	glm::mat4 projection_matrix = pCamera->GetOrthoProjectionMatrix();
	glm::mat4 view_matrix = pCamera->GetViewMatrix();

	a_pshader->Use();
	a_pshader->SetUniformMatrix4fv(a_pshader->GetShaderID(), "projection_Matrix", projection_matrix);
	a_pshader->SetUniformMatrix4fv(a_pshader->GetShaderID(), "view_Matrix", view_matrix);

	glBindVertexArray(m_uQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Renderer::ShadowPass()
{
	near_plane = 1.0f;
	far_plane = 7.5f;
	vec3 vLightPosition = m_pLightManager->getDirectionalLightList()[0].DirectionalDir;
	if (PiImgui->IsDebugCameraOn()) 
	{
		m_LightProjectionMatrix = GetDebugCamera()->GetProjectionMatrix();

		m_LightView = GetDebugCamera()->GetViewMatrix();
	}
	else 
	{
		//m_LightProjectionMatrix = glm::ortho(-ortho_size, ortho_size, -ortho_size, ortho_size, near_plane, far_plane);
		//m_LightView = glm::lookAt(vLightPosition*distance, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 lightSpaceMatrix = m_LightProjectionMatrix * m_LightView;

		ShaderManager* pShader = m_pShadowMaterial->GetShader();
		pShader->Use();
		pShader->SetUniformMatrix4fv(pShader->GetShaderID(), "LightSpaceMatrix", lightSpaceMatrix);

		m_LightProjectionMatrix = GetDebugCamera()->GetProjectionMatrix();
		m_LightView = GetDebugCamera()->GetViewMatrix();

		m_pLightManager->getDirectionalLightList()[0].DirectionalDir = GetDebugCamera()->GetPosition();
	}

	glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	m_pShadowBuffer->BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);

	RenderSceneWithShadowMap();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Reset View Port
	glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderSceneWithShadowMap() 
{
	//Renderable* pRenderable = m_pRenderableShadowContainer->PopRenderable();
	//while (pRenderable)
	//{
	//	pRenderable->Draw(m_pShadowMaterial);
	//	pRenderable = m_pRenderableShadowContainer->PopRenderable();
	//}
}

void Renderer::RenderMainScene()
{
	Renderable* pRenderable = m_pRenderableContainer->PopRenderable();
	while (pRenderable)
	{
		pRenderable->Draw(nullptr);
		pRenderable = m_pRenderableContainer->PopRenderable();
	}
}

void Renderer::LightingPass()
{
	glViewport(0, 0, PiEngine->GetClientWidth(), PiEngine->GetClientHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderMainScene();

	//m_DepthMapQuad->Use();
	//m_DepthMapQuad->SetUniform1f(m_DepthMapQuad->GetShaderID(), "near_plane", near_plane);
	//m_DepthMapQuad->SetUniform1f(m_DepthMapQuad->GetShaderID(), "far_plane", far_plane);
	//m_pShadowBuffer->BindForReading(GL_TEXTURE0);
	//RenderQuadForFBO();
}

void Renderer::RenderScenePass()
{
	//====================================================
	//Light Calculation
	//====================================================
	glViewport(0, 0, PiEngine->GetClientWidth(), PiEngine->GetClientHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	#pragma region Obj-Rendering
		RenderMainScene();
	#pragma endregion

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::PostProcessingPass()
{
	//This will be divided based on what feature's are required
	//If Bloom is needed bloom will be added here

	
	//====================================================
	//Blur Pass
	//====================================================
	
	m_pBlurProgram->Use();

	int amount_times	= 10;
	bool isHorizontal	= true;
	bool isFirstPass	= true;

	for (int i = 0; i < amount_times; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongBlurFBO[isHorizontal]);
		
		m_pBlurProgram->SetUniform1f(m_pBlurProgram->GetShaderID(), "isHorizontal", isHorizontal);
		m_pBlurProgram->SetInt(m_pBloomFinalProgram->GetShaderID(), "image", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, isFirstPass ? bloombuffer[0] : m_blurtex[!isHorizontal]);
		
		RenderQuad(m_pBlurProgram);

		isHorizontal = !isHorizontal;

		if (isFirstPass == true)
			isFirstPass = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_pBlurProgram->UnUse();

	//====================================================
	//Bloom
	//====================================================
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0, 0.0, 0.0, 1.0);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_pBloomFinalProgram->Use();
	float exposure_value = 0.5f;
	m_pBloomFinalProgram->SetUniform1f(m_pBloomFinalProgram->GetShaderID(), "exposure", exposure_value);

	//Get the texures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,  m_blurtex[!isHorizontal]);// m_blurtex[!isHorizontal]);
	   
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloombuffer[1]);

	//Render Quad
	RenderQuad(m_pBloomFinalProgram);

	//===================================================================
}

void Renderer::CameraandUIPass()
{
#pragma region UI-Rendering
	Renderable* UIRenderable = m_pUIContainer->PopRenderable();
	while (UIRenderable)
	{
		UIRenderable->Draw(nullptr);
		UIRenderable = m_pUIContainer->PopRenderable();
	}
#pragma endregion


#pragma region Camera-Update
	float fDeltaTime = PiFPS->GetDeltaTime();
	for (uint32 i = 0; i < eCamera_Max; ++i)
	{
		m_pCameras[i]->Update(fDeltaTime);
	}
#pragma endregion

}

void Renderer::Draw()
{
	m_pSkybox->GetShaderId()->Use();
	m_pSkybox->Update();


	//FOG CHANGES
	if (forwardx)
		m_FogParameters.m_vColor.x = m_FogParameters.m_vColor.x + 0.001;
	else
		m_FogParameters.m_vColor.x = m_FogParameters.m_vColor.x - 0.001;

	if (forwardy)
		m_FogParameters.m_vColor.y = m_FogParameters.m_vColor.y + 0.001;
	else
		m_FogParameters.m_vColor.y = m_FogParameters.m_vColor.y - 0.001;

	if (forwardz)
		m_FogParameters.m_vColor.z = m_FogParameters.m_vColor.z + 0.001;
	else
		m_FogParameters.m_vColor.z = m_FogParameters.m_vColor.z - 0.001;

	


	if (m_FogParameters.m_vColor.z > 1.0)
		forwardz = false;

	if (m_FogParameters.m_vColor.x > 1.0)
		forwardx = false;

	if (m_FogParameters.m_vColor.y > 1.0)
		forwardy = false;

	if (m_FogParameters.m_vColor.z < 0.0)
		forwardz = true;

	if (m_FogParameters.m_vColor.x < 0.0)
		forwardx = true;

	if (m_FogParameters.m_vColor.y < 0.0)
		forwardy = true;

	//===================================
	//Pre-Pass[Shadows,Reflection,...etc]
	//===================================
	//ShadowPass();
	
	//===================================
	//Scene Render Pre-Pass
	//===================================

	//LightingPass();					// This Pass only works correctly without the memory issue

	RenderScenePass();

	//===================================
	//Post-Processing Pass[Bloom ,.....etc]
	//===================================

	PostProcessingPass();

	CameraandUIPass();

}

bool Renderer::GetDebugCamMode(void) const
{
	return m_isDebugCamOn;
}

void Renderer::SetDebugCamMode(bool a_status)
{
	m_isDebugCamOn = a_status;
}
