//
//  UIController.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "UIController.h"
#include "Engine.h"
#include "InputManager.h"
#include "Canvas.h"
#include "UIManager.h"
//#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"

UIController::UIController():Component(STRINGIFY(UIController)),m_allowSwitch(true)
{
}


UIController::~UIController()
{
}

void UIController::Initialize()
{

}

void UIController::Update(float a_dTime)
{
	if (PiEngine->m_isMinimized)
	{
		PiResource->GetConfig()->m_enableMouse = true;
		//LOG_DEBUG("free mouse enabled\n");
	}
	else
	{
		PiResource->GetConfig()->m_enableMouse = false;
		//LOG_DEBUG("free mouse disabled\n");
	}
	if (PiResource->GetConfig()->m_enableMouse)
	{
		return;
	}
	//get current active canvas
	m_currentActiveCanvas = PiUI->GetCurrentActiveCanvas();
	if (!m_currentActiveCanvas)
	{
		return;
	}
	float leftStickval = PiInput->GetControllerAxisValue(EControllerAxis::AxisLeftY);
	float rightStickval = PiInput->GetControllerAxisValue(EControllerAxis::AxisRightY);

	//std::cout << "LeftStick Val : " << leftStickval << std::endl; //- ^ , + v
	//std::cout << "RightStick Val : " << rightStickval << std::endl;
	if (leftStickval == 0 && rightStickval==0 )
		m_allowSwitch = true;

	if (m_allowSwitch)
	{
		//InputComponent* input = PiGameObject->m_Player->GetComponent<InputComponent>(STRINGIFY(InputComponent));
		
		//if (/*input->GetInputType()==EInputType::Controller*/PiResource->GetConfig()->GetInputConfig()== EInputType::Controller)
		//{
			if (leftStickval == 1.0f || rightStickval==1.0f)
			{
				++m_offset;
				m_allowSwitch = false;
			}
			else if (leftStickval == -1.0f || rightStickval == -1.0f)
			{
				m_allowSwitch = false;
				--m_offset;
			}
		/*}
		else
		{*/
			if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_UP) || PiInput->IsControllerButtonDown(EControllerButton::DPadUp))
			{
				--m_offset;
			}
			else if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_DOWN) || PiInput->IsControllerButtonDown(EControllerButton::DPadDown))
			{
				++m_offset;
			}
			
		//}
		
	}
	
	//get ui pos
	pos = m_currentActiveCanvas->m_buttonPos[(m_offset%m_currentActiveCanvas->m_buttonPos.size())];
	//warp to mouse to that pos
	PiInput->WarpMousetoPos(pos);
}

void UIController::Serialize(const json &j)
{
}

void UIController::HandleEvent(Event * a_pEvent)
{
	//if (a_pEvent->GetEventType()==EEventType::Minimized)
	//{
	//	//bool t;
	//	PiResource->GetConfig()->m_enableMouse = true;
	//}

	//if (a_pEvent->GetEventType() == EEventType::Maximized)
	//{
	//	PiResource->GetConfig()->m_enableMouse = false;
	//}
}

void UIController::Deactivate(void)
{
}

void UIController::OverrideParams(const json &, float)
{
}
