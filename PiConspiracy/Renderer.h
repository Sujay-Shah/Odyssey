//
//  Renderer.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class Camera;
class SkyBox;
class ShaderManager;
class ShapeGenerator;
class RenderableContainer;
class LightManager;
class FrameBuffer;
class DebugRenderer;
class Material;
class Renderer : Singleton<Renderer>
{
public:

	struct FogParameters
	{
		float	m_fFogStartDistance;
		float	m_fFogEndDistance;
		uint32	m_uFogSelector;
		uint32	m_uFogDepth;
		float	m_fFogDensity;
		Color3	m_vColor;
		bool	m_bfogEnabled;

		int		m_uLightMode;
		float	m_fLightIntensity;
	};
	enum eCamera : uint32
	{
		eCamera_Main,
		
		eCamera_FreeMove,

		eCamera_UI,

		eCamera_Max
	};

									Renderer();
									~Renderer();
    
    bool							Initialize();
	bool							InitializeGLCalls(uint32 a_uScreenWidth, uint32 a_uScreenHeight);

	void							Clear(vec3& a_vColor);
    void							Draw();

	bool							GetDebugCamMode(void) const;
	void							SetDebugCamMode(bool a_status);
	inline bool						IsShadowPass() { return m_bShadowPass; }
    inline Camera*					GetMainCamera(/*bool a_bIsFreeCamOn*/);
	inline Camera*					GetCamera(/*bool a_bIsFreeCamOn*/);
	inline Camera*					GetDebugCamera();
	inline Camera*					GetUICamera();
    inline SkyBox*					GetSkyBox();
	inline ShapeGenerator*			GetShapeGenerator();
	inline LightManager*			GetLightManager();
    inline RenderableContainer*		GetRenderableContainer();
	inline RenderableContainer*		GetUIContainer();
	inline DebugRenderer*			GetDebugRenderer();
	inline FrameBuffer*				GetFrameBuffer();
	inline FogParameters			GetFogParameters();
	inline ShaderManager*			GetBloomFinalShader();
	inline ShaderManager*			GetBlurShader();
	inline Material*				GetShadowMaterial();

	Material*						m_pShadowMaterial;

	//Bloom Related Shaders
	ShaderManager*					m_pBloomFinalProgram;
	ShaderManager*					m_pBlurProgram;
	
	uint32							hdrFBO;
	uint32							rbodepth;
	uint32							bloombuffer[2];
	uint32							m_PingPongBlurFBO[2];
	uint32							m_blurtex[2];


	mat4							m_LightProjectionMatrix;
	mat4							m_LightView;
	float							near_plane;
	float							far_plane;

protected: 
	
	void							CreateQuadForBloomFBO();
	void							RenderQuad(ShaderManager* a_pshader);
	void							RenderSceneWithShadowMap();
	void							RenderMainScene();
	void							RenderScenePass();
	void							PostProcessingPass();
	void							CameraandUIPass();

	//Shadows
	bool							LoadShadowMap();
	void							ShadowPass();
	void							LightingPass();

	//Bloom
	bool							InitializeBloom();

	void							SetFogParameters();

private:

	// Buffer for FBO quad
	uint32							m_uQuadVAO;
	uint32							m_uQuadIBO;


	bool							m_isDebugCamOn;
	bool							m_bShadowPass;
	bool							m_bIsDebugDrawActive;

	// Main Camera Right now	 having both free move cam and main camera setting 
    Camera*							m_pCameras[eCamera_Max];

	SkyBox*							m_pSkybox;

	ShapeGenerator*					m_pShapeGenerator;

    FrameBuffer*					m_pShadowBuffer;
    
    RenderableContainer*			m_pRenderableContainer;

    RenderableContainer*			m_pUIContainer;

	LightManager*					m_pLightManager;
	
	ShaderManager*					m_DepthMapQuad;

	DebugRenderer*					m_pDebugRenderer;

	FogParameters					m_FogParameters;
};

inline Camera * Renderer::GetMainCamera()
{
	return m_pCameras[eCamera_Main];
}

inline Camera* Renderer::GetCamera()
{
	if (m_isDebugCamOn)
	{
		return m_pCameras[eCamera_FreeMove];
	}
	else
	{
		return m_pCameras[eCamera_Main];
	}

	//return nullptr;
}
inline Camera * Renderer::GetDebugCamera()
{
	return m_pCameras[eCamera_FreeMove];
}

inline Camera* Renderer::GetUICamera()
{
	return m_pCameras[eCamera_UI];
}

inline SkyBox * Renderer::GetSkyBox()
{
	return m_pSkybox;
}

inline ShapeGenerator* Renderer::GetShapeGenerator()
{
	return m_pShapeGenerator;
}

inline LightManager * Renderer::GetLightManager()
{
	return m_pLightManager;
}

inline RenderableContainer* Renderer::GetRenderableContainer()
{
    return m_pRenderableContainer;
}

inline RenderableContainer* Renderer::GetUIContainer()
{
	return m_pUIContainer;
}

inline DebugRenderer* Renderer::GetDebugRenderer()
{
	return m_pDebugRenderer;
}

inline FrameBuffer* Renderer::GetFrameBuffer()
{
	return m_pShadowBuffer;
}

inline Renderer::FogParameters Renderer::GetFogParameters()
{
	return m_FogParameters;
}

inline ShaderManager * Renderer::GetBloomFinalShader()
{
	return m_pBloomFinalProgram;
}

inline ShaderManager * Renderer::GetBlurShader()
{
	return m_pBlurProgram;
}

inline Material* Renderer::GetShadowMaterial()
{
	return m_pShadowMaterial;
}