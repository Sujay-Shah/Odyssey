//
//  InputComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
//TODO : find better way for enums
#include "InputEnums.h"
//class RigidBodyComponent;
//class Camera;
//class InputManager;
//class LightComponent;
//class Health;

struct InputType
{
	enum Enum
	{
		MouseKeyBoard,
		Controller
	};
}; typedef InputType::Enum EInputType;

struct InputState
{
	enum Enum : unsigned
	{
		Idle			= 0,
		TurnLeft		= 1,
		TurnRight		= 1 << 1,
		DashLeft		= 1 << 2,
		DashRight		= 1 << 3,
		Boost			= 1 << 4,
		Jump			= 1 << 5
	};
}; TYPEDEF_ENUM(InputState);

struct ShipState
{
	enum Enum : unsigned
	{
		Forward		= 0,
		TurnLeft	= 1,
		TurnRight	= 1 << 1,
		DashLeft	= 1 << 2,
		DashRight	= 1 << 3,
		Boost		= 1 << 4,
		Jump		= 1 << 5,
		Shield		= 1 << 6
	};
}; TYPEDEF_ENUM(ShipState);


class InputComponent : public Component
{
	COMPONENT_DECLARE(InputComponent);
public:
	~InputComponent();

	void Initialize() override final;
	void ImguiUpdate(float) override;
	void Update(float a_dTime) override final; // the update function for transform
	//void LateUpdate(float a_dTime) override final;
	void Serialize(const json& a_j) override final;
	void HandleEvent(Event* a_pEvent) override final;
    void Deactivate(void) override final;
	// Inherited via Component
	virtual void OverrideParams(const json&,float) override;
	EInputType GetInputType(void) const;
	void SetInputType(EInputType e);
	EInputType GetInputType();
	void ResetPos();
	bool IsShipState(EShipState a_state);

	bool m_godMode;
	
	bool m_isDashed;
private:
	EInputType m_inputType;
	Transform* m_pTrans;
	RigidBodyComponent* m_pRBodyComp;
	
	float dt;

	unsigned m_inputState;
	unsigned m_shipState;

	float m_controllerDeadZone;
	float m_dashCooldown;

	float m_dashStateTime;
	float m_boostStateTime;
	float m_shieldStateTime;

	// movement rate variables
	float m_turnRate;
	float m_acceleration;
	float m_tiltRate;
	float m_tiltMultiplier;

	float m_tiltValue;
	float m_rollRate;
	float m_dashValue;
	float m_jumpValue;
	
	// max values
	float m_maxTilt;
	float m_maxVelocity;
	float m_maxAcceleration;
	float m_maxTurnVelocity;
	float m_maxRoll;

	// PowerUp bools
	bool m_grounded;

	//Testing cooldown value
	float dt_here = 0.0f;
	bool  isSlowed = false;

	// motion vectors
	vec3 m_velocity;
	vec3 m_rotation;
	vec3 m_position;
	vec3 m_tiltRotation;

	//audio 
	int m_mute;

	//Skip level
	int m_Skipcounter;

	// motion functions
	void Forward(void);
	void Backward(void);
	void Right(void);
	void Left(void);

	void BackToNormal(void);

	void QuickDash(int a_side);		// 0 left, 1 right

	//Power functions
	void PowerJump(void);
	void PowerPierce(void);
	void PowerShield(void);

	//to keep track of current state
	void UpdateInputState(void);
	bool IsInputState(EInputState a_state);
	void ActivateShipState(EShipState a_state);
	void ActivateInputState(EInputState a_state);
	void DeactivateShipState(EShipState a_state);
	void DeactivateInputState(EInputState a_state);

	int m_pickupSoundMultiplier;
	void ResetPos(SDL_Scancode a_scancode);
	
	float m_camDiff;

	// serializable variables
	bool m_autoAcc;

	void ClampValues(float &a_value, float a_max, float a_min);
};

