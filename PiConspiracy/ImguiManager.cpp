//
//  ImguiManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "FrameRateController.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Renderer.h"
#include "Engine.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "StatsManager.h"
#include "LightManager.h"


void ImguiManager::Initialize(SDL_Window* window, SDL_GLContext* gl_context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;


	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glslVersion);


	//Preferred Style of ImGui
	ImGui::StyleColorsDark();
	
    m_bIsCameraDebugOn = false;
	m_bIsDebugDrawOn = true;
	m_bIsWireFrameMode = false;

	m_FogParameters.m_fFogStartDistance = 0.04f;
	m_FogParameters.m_fFogEndDistance = 6000.0f;
	m_FogParameters.m_fFogDensity = 0.001f;
	m_FogParameters.m_uDepthType = 1;
	m_FogParameters.m_uFogSelector = 1;
}

bool showDebugWindow = true;

void DebugInfoWindow()
{
	ImGui::Text("frame time : %f ms", 1000.0f / ImGui::GetIO().Framerate);

	ImGui::Text("FPS %f", ImGui::GetIO().Framerate);

}

void ImguiManager::CameraInfoWindow()
{
	//PiRenderer->SetDebugCamMode(m_bIsCameraDebugOn);
	Camera* camobj = PiRenderer->GetCamera();
	
	vec3 position = camobj->GetPosition();

	ImGui::Text("Camera Position x: %f", &position.x);
	ImGui::Text("Camera Position y: %f", &position.y);
	ImGui::Text("Camera Position z: %f", &position.z);

}


bool showDebugCamerawindow = false;

void ImguiManager::DebugCameraEnableWindow(bool a_bIsActive)
{
	//PiRenderer->SetDebugCamMode(m_bIsCameraDebugOn);
    Camera* camobj = PiRenderer->GetCamera();
	
	if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_SPACE) && PiGameObject->m_Player!=nullptr)
	{
		camobj->SetPosition(PiGameObject->m_Player->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition() + glm::vec3(0,3,5));
	}
    camobj->SetFreeCameraOn(a_bIsActive);
}

bool showLightinfo = false;
void ImguiManager::LightInfoWindow()
{
	
}

bool showFogProperties = true;
bool showFog = true;
void ImguiManager::FogProperties()
{
	ImGui::Begin("Fog Properties");
	
	//m_FogParameters = PiRenderer->GetFogParameters();
	ImGui::SliderFloat("Color X", &m_FogColor.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Color Y", &m_FogColor.y, 0.0f, 1.0f);
	ImGui::SliderFloat("Color Z", &m_FogColor.z, 0.0f, 1.0f);

	//ImGui::SliderFloat("Fog Start Distance", &PiRenderer->GetFogParameters().m_fFogStartDistance, 0.0f, 100.0f);
	ImGui::SliderFloat("Fog End Distance", &m_FogParameters.m_fFogEndDistance, 0.0f, 10000.0f);
	ImGui::SliderFloat("Fog Density", &m_FogParameters.m_fFogDensity, 0.0f, 0.02f);
	ImGui::SliderInt("Fog Depth Type", &m_FogParameters.m_uDepthType, 1, 2);
	ImGui::SliderInt("Fog Selector", &m_FogParameters.m_uFogSelector, 1, 3);

	ImGui::Checkbox("FogEnable", &showFog);

	if (showFog)
	{
		m_fogIsEnable = true;
	}
	else
	{
		m_fogIsEnable = false;
	}

	ImGui::End();
}	

bool showRotationAngleWindow = false;


bool show_CameraPos = true;
void CameraInfo(Camera* camobj)
{
	
	vec3 camera_pos = camobj->GetPosition();

	if (PiGameObject->m_Player == nullptr)
	{
		return;
	}

	vec3 playerPos = PiGameObject->m_Player->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition();

	ImGui::Begin("Camera Info");


	ImGui::Text("Cam Pos X: %f ", camera_pos.x);
	ImGui::Text("Cam Pos Y: %f ", camera_pos.y);
	ImGui::Text("Cam Pos Z: %f ", camera_pos.z);

	ImGui::Text("Player Position x: %f", playerPos.x);
	ImGui::Text("Player Position y: %f", playerPos.y);
	ImGui::Text("Player Position z: %f", playerPos.z);

	ImGui::Text("offset: %f", playerPos.z - camera_pos.z);

	ImGui::Text("FOV : %f", camobj->GetFeildofView());

	ImGui::End();

}


float score = 0;

void ImguiManager::Update(SDL_Window* window, SDL_GLContext * context)
{

	//ImGui::NewFrame();
	
	//==========================================================
	//Add all the Imgui Window calls here
	//==========================================================


	if (showDebugWindow)
	{
		DebugInfoWindow();
	}

	if (m_bIsCameraDebugOn)
	{
		DebugCameraEnableWindow(m_bIsCameraDebugOn);
	}
    else
    {
        DebugCameraEnableWindow(m_bIsCameraDebugOn);
    }
	//PiRenderer->SetDebugCamMode(m_bIsCameraDebugOn);
	Camera* camobj = PiRenderer->GetCamera();

	if (show_CameraPos)
	{
		CameraInfo(camobj);
	}


	if (showFogProperties)
	{
		FogProperties();
	}


	if (showLightinfo)
	{
		LightInfoWindow();
	}


	ImGui::Begin("DEBUG");
	
	
	ImGui::Checkbox("Frame Info"		, &showDebugWindow);
	ImGui::Checkbox("Debug Camera"		, &m_bIsCameraDebugOn);
	ImGui::Checkbox("Camera Info"		, &show_CameraPos);
	//ImGui::Checkbox("Fog Properties"	, &showFogProperties);
	
	ImGui::SliderInt("LightMode", &m_LightModel, 1, 2);
	ImGui::SliderFloat("Light Intensity", &m_lightIntensity, 0, 1);
	
	ImGui::Checkbox("Debug Draw", &m_bIsDebugDrawOn);
	ImGui::Checkbox("WireFrameMode", &m_bIsWireFrameMode);

	ImGui::End();

}

void ImguiManager::Destroy()
{

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}


//helper functions

void ImguiManager::AddInputFloat(std::string name, float&val) {
	ImGui::InputFloat(name.c_str(), &val);
}

void ImguiManager::AddSliderFloat3(const std::string& label_name, const float& min, const float& max, float *var) {
	ImGui::SliderFloat3(label_name.c_str(), var, min, max);
}

bool ImguiManager::AddButton(const std::string& name) {
	if (ImGui::Button(name.c_str())) {
		return true;
	}
	return false;
}

void ImguiManager::AddSliderFloat(const std::string& label_name, const float& min, const float& max, float& var) {
	ImGui::SliderFloat(label_name.c_str(), &var, min, max);
}

//void ImguiManager::AddSliderInt(const std::string& label_name, const int& min, const int& max, int& var) {
//	ImGui::SliderInt(label_name.c_str(), &var, min, max);
//}

void ImguiManager::AddDragFloat(std::string name, const float& min, const float& max, const float& speed, float& var) {
	ImGui::DragFloat(name.c_str(), &var, speed, min, max);
}

void ImguiManager::StartFrame(SDL_Window* a_pWindow )
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(a_pWindow);
	ImGui::NewFrame();
}

void ImguiManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiManager::AddSliderInt(const std::string& label_name, const int& min, const int& max, int& var) {
	ImGui::SliderInt(label_name.c_str(), &var, min, max);
}

void ImguiManager::AddCheckBox(const std::string& label_name, bool &val) {
	ImGui::Checkbox(label_name.c_str(), &val);
}

void ImguiManager::AddText(const char* fmt, ...) {
	//Text(name.c_str());
		va_list args;
		va_start(args, fmt);
		ImGui::TextV(fmt, args);
		va_end(args);
	
}

//void ImguiManager::AddText(const std::string & name, float & var)
//{
//	ImGui::Text(name.c_str(),&var);
//}
