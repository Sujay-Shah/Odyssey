//
//  Button.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Button.h"
#include "InputManager.h"
#include "EventManager.h"
#include "UIManager.h"
#include "SceneManager.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"

#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 60

Button::Button():Component(STRINGIFY(Button)),m_isButtonVisible(false)
{
}


Button::~Button()
{
}

void Button::Initialize()
{
	m_trans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	
}

void Button::Update(float a_dTime)
{
	m_ButtonPos = m_trans->GetPosition();
	m_isPressed = false;
	if (m_isButtonVisible)
	{
		ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowSize(ImVec2(165, 80));
		ImGui::SetNextWindowPos(ImVec2(m_ButtonPos.x, m_ButtonPos.y));
		ImGui::Begin(m_buttonName.c_str(), &m_isPressed, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::SetWindowFontScale(1.5);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));

		if (ImGui::IsWindowHovered()) {
			//TODO:via events?
			if (PiResource->GetConfig()->m_enableMouse)
			{
				if (PiInput->IsMouseButtonDown(EMouseButton::Left) || PiInput->IsControllerButtonDown(EControllerButton::ButtonA))
				{
					m_isPressed = true;
				}
			}
			else if( PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_RETURN) || PiInput->IsControllerButtonDown(EControllerButton::ButtonA))
			{
				m_isPressed = true;
			}
		}

		if (m_buttonName == "Resume Game")
		{
			if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed) 
			{

				//On CLick logic
				//PiEngine->SetIsPaused(false);
				OnResumeEvent* res = new OnResumeEvent;
				//TODO: call Handle events for each system that you want to this system
				/*PiUI->HandleEvent(&res);
				PiEngine->HandleEvent(&res);*/
				PiEvent->AddEventToManagersQueue(res);
				LOG_DEBUG("Resume clicked\n");
				//ImGui::End();
			}
		}
		else if (m_buttonName == "Restart Game")
		{
			if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed)
			{

				////On CLick logic
				////TODO: call Handle events for each system that you want to this system
				//Confirm with user
				OnDestructiveAction* od = new OnDestructiveAction;
				od->m_destructiveEvent = EEventType::OnRestart;

				PiUI->m_currentDestructiveEvent = EEventType::OnRestart;
				PiUI->HandleEvent(od);

				Event* e = new Event;
				e->SetEventType(EEventType::ResetPickupHUD);
				PiEvent->AddEventToQueue(e);
				//display confirm Menu
				//PiUI->ActivateCanvas(ECanvasType::Confirm);


				/*OnRestartEvent* OR = new OnRestartEvent();

				PiEvent->AddEventToManagersQueue(OR);*/
				//LOG_DEBUG("Restart clicked\n");
				//ImGui::End();
			}
		}
		
		else if (m_buttonName == "Quit Game")
		{
			if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed)
			{
				OnDestructiveAction* od = new OnDestructiveAction;
				od->m_destructiveEvent = EEventType::OnQuit;

				PiUI->m_currentDestructiveEvent = EEventType::OnQuit;
				//make UI pop up confirmation screen
				PiUI->HandleEvent(od);


				//OnQuitEvent* oq = new OnQuitEvent();
				////PiEngine->HandleEvent(&oq);
				//PiEvent->AddEventToManagersQueue(oq);
				//LOG_DEBUG("Quit clicked\n");
				//ImGui::End();
			}
			
		}
		else if (m_buttonName == "Options")
		{
			if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed)
			{
				Event* op = new Event();
				op->SetEventType(EEventType::OnOptionsPressed);
				//OnQuitEvent* oq = new OnQuitEvent();
				//PiEngine->HandleEvent(&oq);
				PiEvent->AddEventToManagersQueue(op);
				LOG_DEBUG("Options clicked\n");
			}

		}
		else if (m_buttonName == "Start Game")
		{
			if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed)
			{
				OnStart* os = new OnStart();
				//OnQuitEvent* oq = new OnQuitEvent();
				//PiEngine->HandleEvent(&os);
				PiEvent->AddEventToManagersQueue(os);
				LOG_DEBUG("Start clicked\n");
				//ImGui::End();
			}

		}
		else if (m_buttonName == "How to Play")
		{
			if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed)
			{
				OnControlsPressed* ip = new OnControlsPressed();

				//OnQuitEvent* oq = new OnQuitEvent();
				//PiEngine->HandleEvent(&oq);
				PiEvent->AddEventToManagersQueue(ip);
				LOG_DEBUG("Instructions clicked\n");
				//ImGui::End();
			}

		}
		else if (m_buttonName == "Credits")
		{
			if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed)
			{
				OnCreditsPressed* cp = new OnCreditsPressed();
				//OnQuitEvent* oq = new OnQuitEvent();
				//PiEngine->HandleEvent(&oq);
				PiEvent->AddEventToManagersQueue(cp);
				LOG_DEBUG("Credits clicked\n");
				//ImGui::End();
			}

		}
		else if (m_buttonName == "Yes")
		{
			if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed)
			{
				int choice = (int)PiUI->m_currentDestructiveEvent;
				switch (choice)
				{
					case EEventType::OnRestart:
					{
						OnRestartEvent *re = new OnRestartEvent();
						PiEvent->AddEventToManagersQueue(re);
						LOG_DEBUG("Restart clicked\n");
						break;
					}

					case EEventType::OnQuit:
					{
						OnQuitEvent *qe = new OnQuitEvent();
						PiEvent->AddEventToManagersQueue(qe);
						LOG_DEBUG("Quit clicked\n");
						break;
					}
					case EEventType::Exit2Main:
						Event* e = new Event;
						e->SetEventType(EEventType::Exit2Main);
						PiEvent->AddEventToManagersQueue(e);
						break;
				}
				
				//PiEngine->HandleEvent(&oq);
				
				//ImGui::End();
			}

		}
		else if (m_buttonName == "No")
		{
			if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed)
		{
			int choice = (int)PiUI->m_currentDestructiveEvent;
			
			//disable confirmation menu
			PiUI->DeactivateCanvas(ECanvasType::Confirm);

			//enable previous canvas
			//int choice = (int)PiUI->m_currentDestructiveEvent;
			
			PiUI->ActivateCanvas(PiUI->m_lastActiveCanvas);
			/*if (PiEngine->GetInGameStatus())
			{
				PiUI->ActivateCanvas(ECanvasType::MainMenu);
			}
			else
			{
				PiUI->ActivateCanvas(ECanvasType::PauseMenu);
			}*/
			//PiEngine->HandleEvent(&oq);
			//PiEvent->AddEventToManagersQueue(e);
			//LOG_DEBUG("Quit clicked\n");
			//ImGui::End();
		}

		}

		else if (m_buttonName == "Tutorial")
		{
			//LOG_DEBUG("Tutorial Button Pressed);
		}
		else if (m_buttonName == "Main Menu" || m_buttonName == "Back")
		{
		if (ImGui::Button(m_buttonName.c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)), m_isPressed)
		{
			if (_buttonFunction=="back_to_main_from_control")
			{
				/*PiUI->DeactivateCanvas(ECanvasType::Instructions);
				PiUI->ActivateCanvas(ECanvasType::MainMenu);*/
				BackEvent* b = new BackEvent;
				b->m_changeFrom = (int)ECanvasType::Instructions;
				b->m_changeTo = (int)ECanvasType::MainMenu;
				PiEvent->AddEventToManagersQueue(b);
			}
			else if (_buttonFunction == "back_to_main_from_credit")
			{
				/*PiUI->DeactivateCanvas(ECanvasType::Instructions);
				PiUI->ActivateCanvas(ECanvasType::MainMenu);*/
				BackEvent* bc = new BackEvent;
				bc->m_changeFrom = (int)ECanvasType::Credits;
				bc->m_changeTo = (int)ECanvasType::MainMenu;
				PiEvent->AddEventToManagersQueue(bc);
			}
			else if (_buttonFunction == "back_to_main_from_game")
			{
				
				/*PiUI->DeActivateAllCanvas();
				PiScene->ChangeScene(1);*/
				OnDestructiveAction* od = new OnDestructiveAction;
				od->m_destructiveEvent = EEventType::Exit2Main;

				PiUI->m_currentDestructiveEvent = EEventType::Exit2Main;
				//make UI pop up confirmation screen
				PiUI->HandleEvent(od);
			}
			else if (_buttonFunction == "back_to_pause_from_options")
			{
				BackEvent* bc = new BackEvent;
				bc->m_changeFrom = (int)ECanvasType::Options;
				bc->m_changeTo = (int)ECanvasType::PauseMenu;
				PiEvent->AddEventToManagersQueue(bc);
			}
			
			LOG_DEBUG("Instructions clicked\n");

		}

		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::End();



	}
	
	
}

void Button::Serialize(const json &j)
{
	m_buttonName = ParseString(j, "buttonName");
	_buttonFunction = ParseString(j, "buttonFunc");
}

void Button::HandleEvent(Event * a_pEvent)
{
}

void Button::Deactivate(void)
{

}

void Button::OverrideParams(const json &,float)
{
}
