//
//  ImguiManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

#define WINDOW_WIDTH_IMGUI 270
#define WINDOW_HEIGHT_IMGUI 100

	/*ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH_IMGUI+WidthOffSet, WINDOW_HEIGHT_IMGUI+HieghtOffSet));\
	ImGui::SetNextWindowPos(ImVec2(PosxOffset, PiImgui->pos_offset_+PosyOffset));\*/
	#define START_WINDOW(WindowName)\
	ImGui::Begin(WindowName);\
	{\

#define END_WINDOW \
	}\
	ImGui::End();

class ImguiManager : public Singleton<ImguiManager>
{

public:
	ImguiManager():m_fogIsEnable(false),
		m_LightModel(2),
		m_lightIntensity(1.0),
		m_bIsDebugDrawOn(true),
		pos_offset_(0.0f)
	{
	}

	~ImguiManager() { Destroy(); }

	struct FogParameters
	{
		float	m_fFogStartDistance;
		float	m_fFogEndDistance;
		float	m_fFogDensity;
		int32	m_uDepthType;
		int32	m_uFogSelector;
	};

	void Initialize(SDL_Window* window, SDL_GLContext* gl_context);
	void Update(SDL_Window* window, SDL_GLContext * context);
	void Destroy();

	inline bool				IsDebugCameraOn();
	inline glm::vec3		getFogColor();
	inline bool				getIsFogEnable();
	inline FogParameters	GetFogParameters();
	inline int				GetLightModel();
	inline bool				IsDebugDrawOn();
	inline bool				IsWireFrameOn();
	inline float			GetLightIntensity();

	//void AddText(const std::string& name);
	//void AddText(const std::string& name,float& var);
	void AddText(const char * fmt, ...);

	bool AddButton(const std::string& name);
	void AddInputFloat(std::string name, float&val);
	void AddSliderFloat3(const std::string& label_name, const float& min, const float& max, float *var);
	void AddSliderFloat(const std::string& label_name, const float& min, const float& max, float& var);
	//void AddSliderInt(const std::string & label_name, const float & min, const float & max, float & var);
	void AddSliderInt(const std::string& label_name, const int& min, const int& max, int& var);
	void AddCheckBox(const std::string& label_name, bool &val);
	void AddDragFloat(std::string name, const float& min, const float& max, const float& speed, float& var);

	void StartFrame(SDL_Window* a_window);
	void EndFrame();

	float pos_offset_;
protected:
	void CameraInfoWindow();
	void DebugCameraEnableWindow(bool a_bIsActive);
	void LightInfoWindow();
	void FogProperties();

private:

	int			m_LightModel;

	float		m_lightIntensity;
	
	bool		m_fogIsEnable;
	
	SDL_GLContext * m_context;
	const char* glslVersion = "#version 330";

    bool m_bIsCameraDebugOn;
	bool m_bIsDebugDrawOn;
	bool m_bIsWireFrameMode;

	glm::vec3 m_FogColor = vec3(0.736f,0.986f,1.0f);

	FogParameters m_FogParameters;
};

inline bool ImguiManager::IsDebugCameraOn()
{
	return m_bIsCameraDebugOn;
}

inline glm::vec3 ImguiManager::getFogColor()
{
	return m_FogColor;
}

inline bool ImguiManager::getIsFogEnable()
{
	return m_fogIsEnable;
}

inline ImguiManager::FogParameters ImguiManager::GetFogParameters()
{
	return m_FogParameters;
}

inline int ImguiManager::GetLightModel()
{
	return m_LightModel;
}

inline bool ImguiManager::IsDebugDrawOn()
{
	return m_bIsDebugDrawOn;
}

inline bool ImguiManager::IsWireFrameOn()
{
	return m_bIsWireFrameMode;
}

inline float ImguiManager::GetLightIntensity()
{
	return m_lightIntensity;
}