//
//  InputManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

#include "SDL2\SDL.h"
#include "InputEnums.h"
#define CONTROLLER_DEAD_ZONE 0.25


class InputManager : Singleton<InputManager>
{
public:
	InputManager(void);
	~InputManager(void);

	void Update(float dt);
	void ProcessEvent(SDL_Event* a_event);

	//tutorial God mode
	bool m_tutorialGodMode;

// KEYBOARD
private:

	int m_keyboardKeys;
	bool m_dTapped;
	Uint8* m_currKeyBoardState; // Current KeyBoard States
	const Uint8* m_interKeyBoardState; // Intermediate KeyBoard States
	Uint8* m_prevKeyBoardState; // Previous KeyBoard States
	
public:
	bool IsKeyBoardButtonDown(SDL_Scancode a_scanCode); // use SDL_Scancode...
	bool IsKeyBoardButtonUp(SDL_Scancode a_scanCode); // use SDL_Scancode...
	bool IsKeyBoardButtonHeld(SDL_Scancode a_scanCode); // use SDL_Scancode...
	bool IsKeyDoubleTapped(SDL_Scancode a_scanCode);

// MOUSE
private:
	bool m_prevMouseButtonStates[3];
	bool m_currMouseButtonStates[3];

	uint8 m_prevScrollValue;
	uint8 m_currScrollValue;

	float m_dt, m_t1, m_t2;
	bool m_dclick;
	int m_prevMousePosX;
	int m_prevMousePosY;
	int m_currMousePosX;
	int m_currMousePosY;

	int m_mouseDeltaX;
	int m_mouseDeltaY;

	

public:
	void ClampMouseMovementToScreen();
	void WarpMousetoPos(std::pair<float,float>&);
	bool IsMouseButtonDown(EMouseButton a_button);
	bool IsMouseButtonUp(EMouseButton a_button);
	bool IsMouseButtonHeld(EMouseButton a_button);
	bool IsDoubleClicked(EMouseButton a_button);
	bool IsScrolledUp();
	bool IsScrollDown();


	// get the position of the cursor on the screen
	glm::vec2 GetMousePos();

	// get the relative position of the mouse on screen
	// based on it's position in previous frame
	glm::vec2 GetMouseDeltas();

	//Skip Tutorial Counter
	int m_Skipcounter = 0;


// CONTROLLER
private:
	SDL_GameController *m_controller;
	Uint8 m_controllerID;

	bool m_cdTapped;

	bool m_prevControllerButtonStates[14];
	bool m_currControllerButtonStates[EControllerButton::NUM];

	float m_controllerAxesStates[4];

	float m_currTriggerStates[2];
	float m_prevTriggerStates[2];
	
	float NormalizeAxis(Sint16 a_normal);
	float NormalizeTrigger(Sint16 a_normal);

	//bool is_death;

public:
	void SetupConnectedController(SDL_Event a_event);

	bool IsControllerConnected(void);
	bool IsControllerEnabled(void);

	bool IsControllerButtonUp(EControllerButton a_button);
	bool IsControllerButtonDown(EControllerButton a_button);
	bool IsControllerButtonHeld(EControllerButton a_button);
	bool IsControllerDoubleTapped(EControllerButton a_button);

	float GetControllerAxisValue(EControllerAxis a_axis);
	float GetControllerTriggerValue(EControllerAxis a_axis);	

	inline int GetControllerID() { return m_controllerID; }
	//inline bool GetIsDead() { return is_death; }
	//inline void SetIsDead(bool a_isDead) { is_death = a_isDead; }

};

