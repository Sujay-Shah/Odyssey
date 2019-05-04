//
//  InputManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "InputManager.h"
#include "Engine.h"

InputManager::InputManager(void)
{
	m_keyboardKeys = 0;
	m_controllerID = 0;
	m_prevKeyBoardState = new uint8[512];
	m_currKeyBoardState = new uint8[512];
	m_dclick = 0;
	m_dt = 0;
	m_interKeyBoardState = SDL_GetKeyboardState(&m_keyboardKeys);

	std::memcpy(m_currKeyBoardState, m_interKeyBoardState, m_keyboardKeys * sizeof(uint8));

	m_controller = nullptr;

	m_currTriggerStates[0] = m_currTriggerStates[1] = 0;
	m_prevTriggerStates[0] = m_prevTriggerStates[1] = 0;

	m_tutorialGodMode = false;
}

InputManager::~InputManager(void)
{
	delete[] m_prevKeyBoardState;
	delete[] m_currKeyBoardState;
}

void InputManager::Update(float dt)
{
	m_dt += dt;

	memcpy(m_prevKeyBoardState, m_currKeyBoardState, m_keyboardKeys * sizeof(uint8));
	memcpy(m_currKeyBoardState, m_interKeyBoardState, m_keyboardKeys * sizeof(uint8));


	for (int i = 0; i < MouseButton::NUM; ++i)
	{
		m_prevMouseButtonStates[i] = m_currMouseButtonStates[i];
	}
	m_prevMousePosX = m_currMousePosX;
    m_prevMousePosY = m_currMousePosY;
    

	if (m_currScrollValue >= 256)
		m_currScrollValue = 0;
	m_prevScrollValue = m_currScrollValue;

	SDL_GetMouseState(&m_currMousePosX, &m_currMousePosY);
	SDL_GetRelativeMouseState(&m_mouseDeltaX, &m_mouseDeltaY);
    m_currMousePosY = PiEngine->GetClientHeight() - m_currMousePosY;

	for (int i = ControllerButton::NONE + 1; i < ControllerButton::NUM; ++i)
	{
		m_prevControllerButtonStates[i] = m_currControllerButtonStates[i];
	}
	
	//SDL_PumpEvents();

	for (int i = ControllerButton::NONE + 1; i < ControllerButton::NUM; ++i)
	{
		m_currControllerButtonStates[i] = SDL_GameControllerGetButton(m_controller, (SDL_GameControllerButton)i);
	}

	for (int i = ControllerAxis::NONE + 1; i < ControllerAxis::NUM; ++i)
	{
		m_controllerAxesStates[i] = NormalizeAxis(SDL_GameControllerGetAxis(m_controller, (SDL_GameControllerAxis)i));
	}
}

void InputManager::ProcessEvent(SDL_Event * a_event)
{
	switch (a_event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
	{
		if (a_event->button.button == SDL_BUTTON_LEFT)
			m_currMouseButtonStates[MouseButton::Left] = true;
		if (a_event->button.button == SDL_BUTTON_RIGHT)
			m_currMouseButtonStates[MouseButton::Right] = true;
		if (a_event->button.button == SDL_BUTTON_MIDDLE)
			m_currMouseButtonStates[MouseButton::Middle] = true;
		
	}break;

	case SDL_MOUSEBUTTONUP:
	{
		if (a_event->button.button == SDL_BUTTON_LEFT)
			m_currMouseButtonStates[MouseButton::Left] = false;
		if (a_event->button.button == SDL_BUTTON_RIGHT)
			m_currMouseButtonStates[MouseButton::Right] = false;
		if (a_event->button.button == SDL_BUTTON_MIDDLE)
			m_currMouseButtonStates[MouseButton::Middle] = false;
	}break;

	case SDL_MOUSEWHEEL:
	{
		if (a_event->wheel.y > 0)
			m_currScrollValue++;
		if (a_event->wheel.y < 0)
			m_currScrollValue--;
	}break;

	case SDL_JOYDEVICEADDED:
	{
		if (SDL_NumJoysticks > 0)
		{

			m_controller = SDL_GameControllerOpen(a_event->cdevice.which);
			if (m_controller == nullptr)
			{
				//std::cout << "\n Controller not connected." << a_event->cdevice.which;
                LOG_DEBUG_ARG("Controller not connected. {}", a_event->cdevice.which);
			}
			else
			{
				SDL_GameControllerOpen(a_event->cdevice.which);
				SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_controller));
                LOG_DEBUG_ARG("Controller Connected! ID:  {}", a_event->cdevice.which);
			}
			//m_controllerID++;
		}
	}break;
	case SDL_JOYDEVICEREMOVED:
	{
		if (!a_event->cdevice.which)
		{
            LOG_DEBUG_ARG("Controller REMOVED! ID: {}", a_event->cdevice.which);
			m_controller = nullptr;
			//m_controllerID--;
		}
	}break;

	}
}

bool InputManager::IsKeyBoardButtonDown(SDL_Scancode a_scanCode)
{
	if (!m_prevKeyBoardState[a_scanCode] && m_currKeyBoardState[a_scanCode])
		return true;
	return false;
}

bool InputManager::IsKeyBoardButtonUp(SDL_Scancode a_scanCode)
{
	if (m_prevKeyBoardState[a_scanCode] && !m_currKeyBoardState[a_scanCode])
		return true;
	return false;
}

bool InputManager::IsKeyBoardButtonHeld(SDL_Scancode a_scanCode)
{
	if (m_prevKeyBoardState[a_scanCode] && m_currKeyBoardState[a_scanCode])
		return true;
	return false;
}

bool InputManager::IsKeyDoubleTapped(SDL_Scancode a_scanCode)
{
	if (!m_dTapped)
	{
		if (IsKeyBoardButtonDown(a_scanCode))
		{
			m_t1 = m_dt;
			m_dTapped = true;
		//	cout << "\n t1 :" << m_t1;
			m_t2 = 0.0f;
			return false;
		}
	}
	else
	{
		if (IsKeyBoardButtonDown(a_scanCode))
		{
			m_t2 = m_dt;
			if ((double)abs(m_t2 - m_t1) <= 0.500000)
			{
				m_dTapped = false;
			//	cout << "\n t2 :" << m_t2;

				return true;
			}
			//cout << "\n t2 :" << m_t2;
			m_t1 = m_t2 = 0.0f;
			m_dTapped = false;
			return false;
		}
	}
	
	return false;
}

void InputManager::ClampMouseMovementToScreen()
{
	// WORK ON THIS
}

void InputManager::WarpMousetoPos(std::pair<float, float>&p)
{
	//TODO: change it to be dynamic?
	SDL_WarpMouseInWindow(PiEngine->GetWindow(), (int)p.first + 75, (int)p.second + 30);
}

bool InputManager::IsMouseButtonDown(EMouseButton a_button)
{
	if (!m_prevMouseButtonStates[a_button] && m_currMouseButtonStates[a_button] )
		return true;
	return false;
}

bool InputManager::IsMouseButtonUp(EMouseButton a_button)
{
	if (!m_currMouseButtonStates[a_button] && m_prevMouseButtonStates[a_button])
		return true;
	return false;
}

bool InputManager::IsMouseButtonHeld(EMouseButton a_button)
{
	if (m_currMouseButtonStates[a_button] && m_prevMouseButtonStates[a_button])
		return true;
	return false;
}

bool InputManager::IsDoubleClicked(EMouseButton a_button)
{
		
	if (!m_dclick)
	{
		if (IsMouseButtonDown(a_button))
		{
			m_t1 = m_dt;
			m_dclick = true;
			//cout << "\n t1 :" << m_t1;
			m_t2 = 0.0f;
			return false;
		}
	}
	else
	{
		if (IsMouseButtonDown(a_button))
		{
			m_t2 = m_dt;
			if ((double)abs(m_t2 - m_t1) < 0.500000)
			{
				m_dclick = false;
				//cout << "\n t2 :" << m_t2;

				return true;
			}
			//cout << "\n t2 :" << m_t2;
			m_t1 = m_t2 = 0.0f;
			m_dclick = false;
			return false;
		}
	}

	return false;
}

bool InputManager::IsScrolledUp()
{
	if (m_currScrollValue > m_prevScrollValue)
		return true;
	return false;
}

bool InputManager::IsScrollDown()
{
	if (m_currScrollValue < m_prevScrollValue)
		return true;
	return false;
}

glm::vec2 InputManager::GetMousePos()
{
	return glm::vec2((float)m_currMousePosX, (float)m_currMousePosY);
}

glm::vec2 InputManager::GetMouseDeltas()
{
	return glm::vec2((float)m_mouseDeltaX, (float)m_mouseDeltaY);
}

float InputManager::NormalizeAxis(Sint16 a_normal)
{
	float axisVal = a_normal;

	// normalize axis values to -1 to 1;
	float normalized = axisVal / SDL_MAX_SINT16;

	return normalized;
}

float InputManager::NormalizeTrigger(Sint16 a_normal)
{
	double axisVal = (double)a_normal;

	// normalize axis values to 0 to 1;
	double normalized = a_normal / SDL_MAX_SINT16;
	
	return (float)normalized;
}

void InputManager::SetupConnectedController(SDL_Event a_event)
{
	if (SDL_NumJoysticks > 0)
	{
		//SDL_Event event;
		m_controller = SDL_GameControllerOpen(a_event.cdevice.which);
		if (m_controller == nullptr)
		{
            LOG_DEBUG("Controller not connected : USE LOG FOR THIS");
		}
	}
}

bool InputManager::IsControllerConnected(void)
{

	return m_controller != nullptr;
}

bool InputManager::IsControllerEnabled(void)
{

	return false;
}

bool InputManager::IsControllerButtonUp(EControllerButton a_button)
{
	if (m_prevControllerButtonStates[a_button] && !m_currControllerButtonStates[a_button])
		return true;
	return false;
}

bool InputManager::IsControllerButtonDown(EControllerButton a_button)
{
	if (m_currControllerButtonStates[a_button] && !m_prevControllerButtonStates[a_button])
		return true;
	return false;
}

bool InputManager::IsControllerButtonHeld(EControllerButton a_button)
{
	if (m_currControllerButtonStates[a_button] && m_prevControllerButtonStates[a_button])
		return true;
	return false;
}

bool InputManager::IsControllerDoubleTapped(EControllerButton a_button)
{
	if (!m_cdTapped)
	{
		if (IsControllerButtonDown(a_button))
		{
			m_t1 = m_dt;
			m_cdTapped = true;
			//cout << "\n t1 :" << m_t1;
			m_t2 = 0.0f;
			return false;
		}
	}
	else
	{
		if (IsControllerButtonDown(a_button))
		{
			m_t2 = m_dt;
			if ((double)abs(m_t2 - m_t1) <= 0.500000)
			{
				m_cdTapped = false;
				//cout << "\n t2 :" << m_t2;

				return true;
			}
			//cout << "\n t2 :" << m_t2;
			m_t1 = m_t2 = 0.0f;
			m_cdTapped = false;
			return false;
		}
	}

	return false;
}

float InputManager::GetControllerAxisValue(EControllerAxis a_axis)
{
	float axisVal = m_controllerAxesStates[a_axis];

	if (fabs(axisVal) < CONTROLLER_DEAD_ZONE)
		axisVal = 0.0f;

	return axisVal;
}

float InputManager::GetControllerTriggerValue(EControllerAxis a_axis)
{
	float axisVal = m_controllerAxesStates[a_axis];

	if (axisVal < CONTROLLER_DEAD_ZONE/2.0f)
		axisVal = 0.0f;

	return axisVal;
}
