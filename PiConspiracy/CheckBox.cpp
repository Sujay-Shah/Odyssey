//
//  CheckBox.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "CheckBox.h"
#include "InputManager.h"
#include "Engine.h"
#include "GameObjectManager.h"
#include "AudioManager.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"


CheckBox::CheckBox():Component(STRINGIFY(CheckBox))
{
}


CheckBox::~CheckBox()
{
}

void CheckBox::Initialize()
{
	m_trans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	pressed = false;
	code = 0;

	/*if (PiEngine->GetIsFullScreen() && m_OnClickFunc == "Togglefullscreen")
	{
		pressed = true;
	}*/
	
}

void CheckBox::Update(float a_dTime)
{
	/*if ()
	{

	}
	InputComponent* ic= PiGameObject->m_Player->GetComponent<InputComponent>(STRINGIFY(InputComponent));*/
	if (m_isVisible)
	{
		isFullScreen = PiEngine->GetIsFullScreen();
		m_checkBoxPos = m_trans->GetPosition();
		bool bOpened = true;
		
		ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowSize(ImVec2(300, 50));
		ImGui::SetNextWindowPos(ImVec2(m_checkBoxPos.x, m_checkBoxPos.y));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin(m_windowText.c_str(), &bOpened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
		//ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1, 0, 0, 1.0f* parent_scene_->get_alpha()));
		//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 1, 0, 1.0f* parent_scene_->get_alpha()));
		ImGui::SetWindowFontScale(2);
		
		code = 0;
		if (ImGui::IsWindowHovered()) {

			if (PiResource->GetConfig()->m_enableMouse)
			{
				if (PiInput->IsMouseButtonUp(EMouseButton::Left) || PiInput->IsControllerButtonDown(EControllerButton::ButtonA))
				{
					pressed = !pressed;
				}
			}
			else if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_RETURN) || PiInput->IsControllerButtonDown(EControllerButton::ButtonA))
			{
				pressed = !pressed;
				code = 1;
			}
			/*if (PiInput->IsMouseButtonUp(EMouseButton::Left))
			{
				pressed = !pressed;
			}*/
		}

		if (ImGui::Checkbox(m_CBname.c_str(), &pressed , code))
		{

			if (m_OnClickFunc == "Togglefullscreen")
			{
				isFullScreen = !isFullScreen;
				PiEngine->setIsFullScreen(isFullScreen);
			}
			else if (m_OnClickFunc == "ToggleController")
			{
				InputComponent* ic=  PiGameObject->m_Player->GetComponent<InputComponent>(STRINGIFY(InputComponent));
				/*inputType = ic->GetInputType();
				if (inputType==EInputType::Controller)
				{
					ic->SetInputType(EInputType::MouseKeyBoard);
				}
				else
				{
					ic->SetInputType(EInputType::Controller);
				}*/
				if (pressed)
				{
					ic->SetInputType(EInputType::MouseKeyBoard);
				}
				else
				{
					ic->SetInputType(EInputType::Controller);
				}
			}
			else if (m_OnClickFunc == "ToggleMasterVol")
			{
				isSFXMute = !isSFXMute;
				isMusicMute = !isMusicMute;
				PiAudio->MuteMasterVolume(true, isSFXMute, isSFXMute);

			}
			else if (m_OnClickFunc == "ToggleMusic")
			{
				//isSFXMute = !isSFXMute;
				//isMusicMute = PiAudio->isMusicPlaying();
				isMusicMute = !isMusicMute;
				PiAudio->MuteMusic(isMusicMute);

			}
			LOG_DEBUG("check box clicked");
		}
		
		ImGui::PopStyleColor();
		ImGui::End();
	}
	
}

void CheckBox::Serialize(const json& j)
{
	m_CBname = ParseString(j, "checkBoxField");
	m_OnClickFunc = ParseString(j, "OnClickFunc");
	m_windowText = m_CBname;

	if (PiEngine->GetIsFullScreen() && m_OnClickFunc == "Togglefullscreen")
	{
		pressed = true;
	}
}

void CheckBox::HandleEvent(Event* a_pEvent)
{
	
}

void CheckBox::Deactivate(void)
{
	
}

void CheckBox::OverrideParams(const json&, float)
{
	
}
