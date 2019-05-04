//
//  InputComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "InputComponent.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "AudioManager.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"
#include "StatsManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "Camera.h"


//temp
///
#include <winerror.h> //for HRESULT
#include <comutil.h> //for _bstr_t (used in the string conversion)
#include "shlobj.h"
//
//temp
#pragma comment(lib, "comsuppw")
//temp

namespace fs = std::experimental::filesystem;


#define epsilon 0.0000000001f

InputComponent::InputComponent(void)
	: Component(STRINGIFY(InputComponent)), m_inputType(EInputType::MouseKeyBoard), m_pickupSoundMultiplier(1), m_Skipcounter(0)
{
	
}

InputComponent::~InputComponent()
{
}

void InputComponent::Initialize()
{

	/*LPWSTR wszPath = NULL;
	HRESULT hr;
	string file_name;
	hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &wszPath);

	if (SUCCEEDED(hr)) {
		_bstr_t bstrPath(wszPath);
		file_name = std::string((char*)bstrPath) + "\\Odyssey";
		bool val = CreateDirectory(file_name.c_str(), NULL);
		file_name += "\\HighScore.txt";
	}

	std::ofstream file(file_name);

	if (!file)
	{
		std::cerr << "unable to open file :" << file_name << std::endl;
	}*/


	// init
	m_pTrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_pRBodyComp = m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent));

	m_inputType = PiResource->GetConfig()->GetInputConfig();

	// motion bools
	m_grounded = true;

	m_controllerDeadZone = 0.4f;
	m_dashCooldown = 0.0f;

	//to maintain dash and boost states
	m_dashStateTime = 0.0f;
	m_boostStateTime = 1.65f;
	m_shieldStateTime = 0.0f;

	// movement rate variables
	m_acceleration = 6.0f;
	m_turnRate = 100.0f;
	m_tiltRate = RADIANS(1.0f);
	m_tiltMultiplier = 1.0f;
	
	m_tiltValue = 0.0f;
	m_dashValue = 2750.0f;
	m_rollRate = RADIANS(360.0f);
	m_jumpValue = 1500.0f;
	
	// max values
	m_maxVelocity = 2000.0f;
	m_maxAcceleration = 50.0f;

	m_maxTurnVelocity = 1000.0f;
	m_maxTilt = RADIANS(45.0f);

	m_mute = false;
	m_godMode = false;

	m_rotation = m_pRBodyComp->getRotation().toVec3();

}

void InputComponent::ImguiUpdate(float a_dt)
{
	START_WINDOW("InputComponent Params");
	PiImgui->AddCheckBox("Auto Accelerate", m_autoAcc);
	PiImgui->AddCheckBox("GodMode", m_godMode);
	PiImgui->AddSliderFloat("player max velocity",1500.0f,2500.0f, m_maxVelocity);
	PiImgui->AddSliderFloat("dash cooldown",1.0f,5.0f, m_dashCooldown);
	PiImgui->AddText("currentVelocity : %f ", m_maxVelocity);
	END_WINDOW;

	//std::cout << "eye : " << m_vPostShakePosition << " front :  " << m_vFront << " up : " << m_vUp << std::endl;

	/*Camera* cam = PiRenderer->GetMainCamera();
	START_WINDOW("Camera params");
	PiImgui->AddText("right - x: %f , y: %f , z: %f ",cam->m_vRight.x, cam->m_vRight.y, cam->m_vRight.z);
	PiImgui->AddText("up - x: %f , y: %f , z: %f ", cam->m_vUp.x, cam->m_vUp.y, cam->m_vUp.z);
	PiImgui->AddText("front - x: %f , y: %f , z: %f ", cam->m_vFront.x, cam->m_vFront.y, cam->m_vFront.z);
	PiImgui->AddText("m_vPostShakePosition - campos - x: %f , y: %f , z: %f ", cam->m_vPostShakePosition.x - cam->GetPosition().x, cam->m_vPostShakePosition.y - cam->GetPosition().y, cam->m_vPostShakePosition.z- cam->GetPosition().z);
	PiImgui->AddText("tilt - %f ", m_tiltValue);
	END_WINDOW;*/
}

void InputComponent::Update(float a_dTime)
{
	//if game is paused
	if (a_dTime == 0.0f|| !PiEngine->GetInGameStatus())
		return;

	//Slow Motion Tutorial Part
	if (isSlowed == true)
	{
		if ((int)dt_here == 1)
		{
			PiEngine->getFrameRateController()->SetTimeScale(1.0f);
			isSlowed = false;
			PiEngine->SetIsGameSlowed(isSlowed);
			dt_here = 0;
		}
		else
		{
			dt_here += PiEngine->getFrameRateController()->GetDeltaTime();
		}
	}

	dt = a_dTime;

	if (m_dashCooldown > epsilon)
	{
		m_dashCooldown -= dt;
	}

	m_velocity = m_pRBodyComp->getVelocity().toVec3();
	m_tiltRotation = m_pRBodyComp->getRotation().toVec3();
	m_position = m_pRBodyComp->getPosition().toVec3();
	

	if (m_autoAcc && !IsShipState(EShipState::Shield))
	{
		Forward();
	}
	
	if (m_inputType == EInputType::MouseKeyBoard)
	{
		if (!m_autoAcc)
		{
			if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_UP) || PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_UP))
			{
				Forward();
			}

			if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_DOWN) || PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_DOWN))
			{
				Backward();
			}
		}
	}

	if (!IsShipState(EShipState::DashLeft) && !IsShipState(EShipState::DashRight))
	{
		UpdateInputState();
	}

	switch (m_inputState)
	{
	case 0:
		ActivateShipState(EShipState::Forward);
		BackToNormal();
		break;

	case EInputState::TurnRight:
		ActivateShipState(EShipState::TurnRight);
		break;

	case EInputState::TurnLeft:
		ActivateShipState(EShipState::TurnLeft);
		break;

	case EInputState::DashLeft:
	case EInputState::TurnLeft | EInputState::DashLeft:
		DeactivateShipState(EShipState::TurnLeft);
		ActivateShipState(EShipState::DashLeft);
		QuickDash(0);
		break;

	case EInputState::DashRight:
	case EInputState::TurnRight | EInputState::DashRight:
		DeactivateShipState(EShipState::TurnRight);
		ActivateShipState(EShipState::DashRight);
		QuickDash(1);
		break;

	case EInputState::Boost:
	case EInputState::Boost | EInputState::TurnLeft:
	case EInputState::Boost | EInputState::TurnRight:
		if (PiStats->m_pierceCount >= 1 && !IsShipState(EShipState::Shield))
		{
			--PiStats->m_pierceCount;
			ActivateShipState(EShipState::Boost);
			PowerPierce();
		}
		break;

	case EInputState::Jump:
	case EInputState::Jump | EInputState::TurnLeft:
	case EInputState::Jump | EInputState::TurnRight:
		if (m_grounded && PiStats->m_jumpCount >= 1 && !IsShipState(EShipState::Shield))
		{
			--PiStats->m_jumpCount;
			ActivateShipState(EShipState::Jump);
			PowerJump();
		}
		break;
	}
		
	m_inputState = 0;

	switch (m_shipState)
	{
	case EShipState::TurnLeft:
	case EShipState::TurnLeft | EShipState::Shield:
	case EShipState::TurnLeft | EShipState::Boost:
	case EShipState::TurnLeft | EShipState::Jump:
		Left();
		DeactivateShipState(EShipState::TurnLeft);
		break;

	case EShipState::TurnRight:
	case EShipState::TurnRight | EShipState::Shield:
	case EShipState::TurnRight | EShipState::Boost:
	case EShipState::TurnRight | EShipState::Jump:
		Right();
		DeactivateShipState(EShipState::TurnRight);
		break;

	case EShipState::DashLeft:
	case EShipState::DashLeft | EShipState::TurnLeft:
	case EShipState::DashLeft | EShipState::Shield:
	case EShipState::DashLeft | EShipState::Boost:
	case EShipState::DashLeft | EShipState::Jump:
		if (m_dashStateTime > epsilon)
		{
			m_dashStateTime -= dt;
		}
		else
		{
			DeactivateShipState(EShipState::DashLeft);
		}
		break;

	case EShipState::DashRight:
	case EShipState::DashRight | EShipState::TurnRight:
	case EShipState::DashRight | EShipState::Shield:
	case EShipState::DashRight | EShipState::Boost:
	case EShipState::DashRight | EShipState::Jump:
		if (m_dashStateTime > epsilon)
		{
			m_dashStateTime -= dt;
		}
		else
		{
			DeactivateShipState(EShipState::DashRight);
		}
		break;

	case EShipState::Shield:
		if (m_shieldStateTime > epsilon)
		{
			m_shieldStateTime -= dt;
			m_velocity.y = 0.0f;
		}
		break;

	case EShipState::Shield | EShipState::Jump:
		DeactivateShipState(EShipState::Jump);
		break;

	case EShipState::Shield | EShipState::Boost:
		DeactivateShipState(EShipState::Boost);
		break;
	}


	if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_G))
	{

		if (!m_godMode)
		{
			m_godMode = true;
			PiStats->m_pierceCount=1;
			PiStats->m_jumpCount=1;
			PiStats->m_extraLife = 1;
			OnPowerUpPicked* p = new OnPowerUpPicked();
			p->m_powerUpType = "jump";
			PiEvent->AddEventToQueue(p);

			OnPowerUpPicked* p2 = new OnPowerUpPicked();
			p2->m_powerUpType = "respawn";
			PiEvent->AddEventToQueue(p2);

			OnPowerUpPicked* p3 = new OnPowerUpPicked();
			p3->m_powerUpType = "boost";
			PiEvent->AddEventToQueue(p3);
		}
		else
		{
			m_godMode = false;
		}
	}

	if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_K))
	{
		if (m_inputType == EInputType::Controller)
			m_inputType = EInputType::MouseKeyBoard;

		else if (m_inputType == EInputType::MouseKeyBoard)
			m_inputType = EInputType::Controller;
	}

	if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_S) || PiInput->IsControllerButtonDown(EControllerButton::DPadLeft))
	{
		++PiInput->m_Skipcounter; // This will be useful if we are 2 "A" button presses on GamePad

		if (PiInput->m_Skipcounter == 1)
		{
			OnRestartEvent* rs = new OnRestartEvent;
			PiEvent->AddEventToManagersQueue(rs);

			PiInput->m_tutorialGodMode = false;
			PiStats->is_TutorialDone = (true);
		}
	}

	
	m_tiltRotation.z = -m_tiltValue;
	
	if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_M) || PiInput->IsControllerButtonDown(EControllerButton::DPadDown))
	{
	
		if (!m_mute)
		{
			PiAudio->MuteMasterVolume(false, false, true);
			m_mute = true;
		}
		else
		{
			PiAudio->MuteMasterVolume(false, false, false);
			m_mute = false;
		}
	}

	ClampValues(m_tiltRotation.x, 0.1f, 0.0f);
	ClampValues(m_tiltRotation.y, 0.1f, 0.0f);

	m_pRBodyComp->setOrientation(glm::quat(m_tiltRotation));
	
	vec3 acc = vec3(m_pRBodyComp->getAcceleration().toVec3().x, 0.0f , m_pRBodyComp->getAcceleration().toVec3().z);
	vec3 accG = vec3(m_pRBodyComp->getAcceleration().toVec3().x, -9.81f*dt, m_pRBodyComp->getAcceleration().toVec3().z);
	
	if (m_grounded)
	{
		m_pRBodyComp->setAcceleration(acc);
		m_velocity.y = 0.0f;
		m_pRBodyComp->setPosition(m_position.x, 45.0f, m_position.z);
	}

	else if(!m_grounded)
	{
		m_pRBodyComp->setAcceleration(accG);
		m_velocity.y -= 10.0f;
	}
	if (m_position.y < 45.0f && !m_grounded)
	{
		if (IsShipState(EShipState::Shield))
		{
			DeactivateShipState(EShipState::Shield);
			m_godMode = false;
		}

		else if (IsShipState(EShipState::Jump))
		{
			DeactivateShipState(EShipState::Jump);
		}

		m_grounded = true;
		m_velocity.y = 0.0f;
		m_velocity.z += 50.0f;
	}

	m_pRBodyComp->setVelocity(m_velocity);

	PiStats->m_scoreCounter = (int32)-m_position.z/100;

	//if (m_velocity.z <= -150.0f)
	//	m_started = true;

	//TODO: DeBUG
	if (PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_T))
	{
		GameObject* go;
		if (PiGameObject->GetIsLevel())
		{
			go = PiGameObject->GetGameObjectbyName("ENDLEVEL_TRIGGER", true);
		}
		else
		{
			go = PiGameObject->GetGameObjectbyName("STARTLEVEL_TRIGGER", false);
		}
		Vector3D pos = go->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent))->getPosition();
		
		m_owner->GetComponent<RigidBodyComponent>(STRINGIFY(RigidBodyComponent))->setPosition(pos.x, pos.y, pos.z + 1000.0f);
	}
	//LOG_DEBUG_ARG("tilt values - x : {} , y: {} , z: {} ",m_tiltRotation.x, m_tiltRotation.y, m_tiltRotation.z);
	//reset view 
	if ( !m_isDashed /*&& m_tiltValue!=0.0f*/)
	{
		PiRenderer->GetMainCamera()->SetRoll(m_tiltValue * 2.0f);
	}
	else if (m_isDashed && m_tiltValue<=std::fabsf(EPSILON))
	{
		m_isDashed = false;
		PiRenderer->GetMainCamera()->resetRoll();
	}
	/*PiRenderer->GetCamera(false)->resetViewMatrix();*/


	//tutorial level god mode
	if (PiInput->m_Skipcounter == 0)
	{
		PiInput->m_tutorialGodMode = true;
	}

}

void InputComponent::Forward(void)
{
	m_acceleration += 2.0f;
	if (m_acceleration >= m_maxAcceleration)
	{
		m_acceleration = m_maxAcceleration;
	}
	
	m_velocity.z -= m_acceleration;

	if (!IsShipState(EShipState::Boost))
	{
		if (m_velocity.z <= -m_maxVelocity)
		{
			m_velocity.z = -m_maxVelocity;
		}
	}
}

void InputComponent::Backward(void)
{
	m_velocity.z += m_acceleration;

	if (m_velocity.z >= m_maxVelocity)
	{
		m_velocity.z = m_maxVelocity;
	}
}

void InputComponent::Right(void)
{
	m_velocity.x += m_turnRate;
	m_tiltValue += m_tiltRate * m_tiltMultiplier;
	m_tiltMultiplier += dt;

	if (!IsShipState(EShipState::DashRight))
	{
		if (m_velocity.x >= m_maxTurnVelocity)
		{
			m_velocity.x = m_maxTurnVelocity;
		}
	}

	if (m_tiltValue >= m_maxTilt)
	{
		m_tiltValue = m_maxTilt;
	}
}

void InputComponent::Left(void)
{	
	m_velocity.x -= m_turnRate;
	m_tiltValue -= m_tiltRate * m_tiltMultiplier;
	m_tiltMultiplier += dt;

	if (!IsShipState(EShipState::DashLeft))
	{
		if (m_velocity.x <= -m_maxTurnVelocity)
		{
			m_velocity.x = -m_maxTurnVelocity;;
		}
	}

	if (m_tiltValue <= -m_maxTilt)
	{
		m_tiltValue = -m_maxTilt;
	}
}

void InputComponent::BackToNormal(void)
{
	m_tiltValue = glm::mix(0.0f, m_tiltValue, 0.95f);
	m_velocity.x = glm::mix(0.0f, m_velocity.x, 0.95f);
	m_tiltMultiplier = 1.0f;
}

void InputComponent::QuickDash(int a_side)
{
	if (m_dashCooldown > 0.0f)
	{
		return;
	}

	m_velocity.x = 0.0f;
	m_dashCooldown = 2.0f;
	m_dashStateTime = 0.55f;

	if (a_side == 0)
	{
		m_velocity.x -= m_dashValue;
		m_tiltValue += m_rollRate;
	}
	else if (a_side == 1)
	{
		m_velocity.x += m_dashValue;
		m_tiltValue -= m_rollRate;
	}

	DashEvent* d = new DashEvent;
	d->m_coolDownVal = m_dashCooldown;
	PiEvent->AddEventToQueue(d);
}

void InputComponent::PowerJump(void)
{
	m_velocity.y += m_jumpValue; // glm::mix(m_jumpValue, m_velocity.y, 0.6f);
	m_grounded = false;
	OnPowerUpUsed *powerUpUsed = new OnPowerUpUsed();
	powerUpUsed->m_powerUpType = "jump";
	PiEvent->AddEventToQueue(powerUpUsed);
}

void InputComponent::PowerShield(void)
{
	m_godMode = true;
	m_grounded = false;
	m_shieldStateTime = 2.0f;
	m_acceleration = 0.0f;
	m_velocity.z = 0.0f;
	m_velocity.x = 0.0f;
	m_pRBodyComp->setPosition(0.0f, 2000.0f, m_position.z + 2500.0f);
	OnPowerUpUsed *powerUpUsed = new OnPowerUpUsed();
	powerUpUsed->m_powerUpType = "respawn";
	PiEvent->AddEventToQueue(powerUpUsed);
}

void InputComponent::UpdateInputState(void)
{

	m_inputState = 0;
	if (m_inputType == EInputType::MouseKeyBoard)
	{
		m_inputState |= PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_LEFT) ? EInputState::TurnLeft : 0;
		m_inputState |= PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_RIGHT) ? EInputState::TurnRight : 0;
		m_inputState |= PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_C) ? EInputState::DashLeft : 0;
		m_inputState |= PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_V) ? EInputState::DashRight : 0;
		m_inputState |= PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_Z) ? EInputState::Boost : 0;
		m_inputState |= PiInput->IsKeyBoardButtonDown(SDL_SCANCODE_X) ? EInputState::Jump : 0;
	}
	else if (m_inputType == EInputType::Controller)
	{
		m_inputState |= PiInput->GetControllerAxisValue(EControllerAxis::AxisLeftX) < -m_controllerDeadZone ? EInputState::TurnLeft : 0;
		m_inputState |= PiInput->GetControllerAxisValue(EControllerAxis::AxisLeftX) > m_controllerDeadZone ? EInputState::TurnRight : 0;
		m_inputState |= PiInput->IsControllerButtonDown(EControllerButton::LeftShoulder) ? EInputState::DashLeft : 0;
		m_inputState |= PiInput->IsControllerButtonDown(EControllerButton::RightShoulder) ? EInputState::DashRight : 0;
		m_inputState |= PiInput->IsControllerButtonDown(EControllerButton::ButtonA) ? EInputState::Boost : 0;
		m_inputState |= PiInput->IsControllerButtonDown(EControllerButton::ButtonB) ? EInputState::Jump : 0;
	}
}

bool InputComponent::IsShipState(EShipState a_state)
{
	return (a_state & m_shipState) == a_state;
}

void InputComponent::ActivateShipState(EShipState a_state)
{
	m_shipState |= a_state;
}

void InputComponent::DeactivateShipState(EShipState a_state)
{
	m_shipState &= ~a_state;
}

bool InputComponent::IsInputState(EInputState a_state)
{
	return (a_state & m_inputState) == a_state;
}

void InputComponent::ActivateInputState(EInputState a_state)
{
	m_inputState |= a_state;
}

void InputComponent::DeactivateInputState(EInputState a_state)
{
	m_inputState &= ~a_state;
}

void InputComponent::PowerPierce(void)
{
	Event *boost = new Event();
	boost->SetEventType(EventType::PiercePowerUp);
	boost->SetEventTime(m_boostStateTime);
	PiEvent->AddEventToQueue(boost);

	OnPowerUpUsed *powerUpUsed = new OnPowerUpUsed();
	//powerUpUsed->SetEventType(EventType::PowerUpUsed);
	powerUpUsed->m_powerUpType = "boost";
	PiEvent->AddEventToQueue(powerUpUsed);

	m_tiltValue += m_rollRate * 7.0f;
	m_acceleration = 0.0f;
	m_velocity.z -= 2500.0f;
	m_isDashed = true;
}

EInputType InputComponent::GetInputType()
{
	return m_inputType;
}

void InputComponent::ResetPos(SDL_Scancode a_scancode)
{
	if (PiInput->IsKeyBoardButtonDown(a_scancode))
	{
		ResetPos();
	}
}

void InputComponent::ResetPos()
{
	if(PiResource->GetConfig()->m_presentation)
		m_pRBodyComp->setPosition(0.0f, 5.0f, 100.0f);

	m_pRBodyComp->setVelocity(0.0f, 0.0f, 0.0f);
	//m_pTrans->SetScale(m_scale);
	//m_started = false;
	m_tiltValue = 0.0f;
	m_velocity = vec3(0.0f);
	//m_health->SetHealthToMax();
}

void InputComponent::Serialize(const json & a_j)
{
	//m_inputType = (EInputType)(ParseInt(a_j,"type"));
	m_autoAcc = PiResource->GetConfig()->m_autoMove;
}

void InputComponent::HandleEvent(Event * a_Event)
{
	if (a_Event->GetEventType() == EEventType::IncreaseSpeed)
	{
		m_maxVelocity += 10;
	}

	else if (a_Event->GetEventType() == EEventType::Collision)
	{
		CollisionEvent* cevent = static_cast<CollisionEvent*>(a_Event);
		//TODO: eventually once control and shooting is done this should look much better
		if (cevent->m_gameObject->GetGameObjectType() == EGameObjectType::PickUp)
		{
			int pickUpId = cevent->m_gameObject->GetComponent<Pickups>(STRINGIFY(Pickups))->GetId();

			if (pickUpId == 0)
			{
				++PiStats->m_pickupMultiplier;

				OnMultiplierPickup* m = new OnMultiplierPickup();
				Event* e=nullptr;

				if (PiStats->m_pickupMultiplier % 5 == 1)
				{
					PiAudio->PlaySounds("1stNote", m_position, 30.0f, true);
					//meshComp->GetMesh()->GetMaterial()->m_isCoolDown = true;

					m->m_hudId = 1;
				}

				else if (PiStats->m_pickupMultiplier % 5 == 2)
				{
					PiAudio->PlaySounds("2ndNote", m_position, 30.0f, true);
					m->m_hudId = 2;
				}

				else if (PiStats->m_pickupMultiplier % 5 == 3)
				{
					PiAudio->PlaySounds("3rdNote", m_position, 30.0f, true);
					m->m_hudId = 3;
				}

				else if (PiStats->m_pickupMultiplier % 5 == 4)
				{
					PiAudio->PlaySounds("4thNote", m_position, 30.0f, true);
					m->m_hudId = 4;
				}

				else if (PiStats->m_pickupMultiplier % 5 == 0)
				{
					PiAudio->PlaySounds("5thNote", m_position, 30.0f, true);
					m->m_hudId = 5;

					e = new Event;
					e->SetEventType(EEventType::ResetPickupHUD);
					e->SetEventTime(0.1f);
				}

				PiEvent->AddEventToQueue(m);
				if (e)
				{
					PiEvent->AddEventToQueue(e);
				}

			}
			else if (pickUpId == 1)
			{
				if (PiStats->m_jumpCount < 1)
				{
					OnPowerUpPicked* p = new OnPowerUpPicked();
					p->m_powerUpType = "jump";
					PiEvent->AddEventToQueue(p);
					PiStats->m_jumpCount++;
					PiAudio->PlaySounds("jump", m_position, 30.0f, true);
				}
				else if (PiStats->m_jumpCount >= 1)
				{
					PiStats->m_multiplier++;
				}
			}
			else if (pickUpId == 2)
			{
				if (PiStats->m_pierceCount < 1)
				{
					OnPowerUpPicked* p = new OnPowerUpPicked();
					p->m_powerUpType = "boost";
					PiEvent->AddEventToQueue(p);
					PiStats->m_pierceCount++;
					PiAudio->PlaySounds("boost", m_position, 30.0f, true);
				}
				else if (PiStats->m_pierceCount >= 1)
				{
					PiStats->m_multiplier++;
				}
			}
			else if (pickUpId == 3)
			{
				if (PiStats->m_extraLife < 1)
				{
					OnPowerUpPicked* p = new OnPowerUpPicked();
					p->m_powerUpType = "respawn";
					PiEvent->AddEventToQueue(p);
					PiStats->m_extraLife++;
					PiAudio->PlaySounds("slowMo", m_position, 30.0f, true);
				}
				else if (PiStats->m_extraLife >= 1)
				{
					PiStats->m_multiplier++;
				}
			}
		}
		 
		else if (cevent->m_gameObject->GetGameObjectName() == "STARTLEVEL_TRIGGER")
		{
			LoadLevelEvent *e = new LoadLevelEvent();
			e->index = 0;
			e->isLevel = true;
			//TODO: give end of lvl as an offset
			//TODO: place trigger to end of lvl to change state of bool in order for managers to update respective lists
			// offset by end_levelTrigger, search for it in activeGameObject queue of level
			e->offset = cevent->m_gameObject->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition().z;
			PiEvent->AddEventToManagersQueue(e);
			LOG_DEBUG("HIt start level trigger\n");
		}
		
		else if (cevent->m_gameObject->GetGameObjectName() == "ENDLEVEL_TRIGGER")
		{
			StartBufferRegionEvent *e = new StartBufferRegionEvent();
	
			e->isLevel = false;
			PiEvent->AddEventToManagersQueue(e);
			LOG_DEBUG("HIt end level trigger\n");

			/*
			//If player dies in the Tutorial level restart tutorial level
			if (PiScene->GetCurrentSceneIndex() == 6)//tutorial level index
				PiInput->m_Skipcounter = 0;
			*/

		}


		else if (cevent->m_gameObject->GetGameObjectName() == "TUTORIAL_LEVEL_END_TRIGGER")
		{
			OnRestartEvent* rs = new OnRestartEvent;
			PiEvent->AddEventToManagersQueue(rs);

			++PiInput->m_Skipcounter;
			PiInput->m_tutorialGodMode = false;

			//PiStats->is_TutorialDone = (true);

		}

		else if (cevent->m_gameObject->GetGameObjectName() == "STARTBUFFER_TRIGGER")
		{
			//TODO: this is here just for now needs to be changed
			PiStats->m_running = true;

			LoadLevelEvent *e = new LoadLevelEvent();
			e->index = 1;
			e->isLevel = false;

			e->offset = cevent->m_gameObject->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition().z;
			PiEvent->AddEventToManagersQueue(e);
			LOG_DEBUG("HIt start buffer trigger\n");
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "Trigger")
		{
			TriggerComponent* trig = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
			TriggeredEvent *rise = new TriggeredEvent();
			rise->id = trig->GetID();
			rise->behaviour = "Rise";
			PiEvent->AddEventToQueue(rise);
			//LOG_DEBUG("HIt Rise Trigger\n");
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "SLOWMOTION_TRIGGER")
		{
			//TriggerComponent* trig_Comp = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
			//TriggeredEvent* trig_event = new TriggeredEvent();
			//
			//trig_event->id = trig_Comp->GetID();
			//trig_event->behaviour = "Slow_Motion";
			//PiEvent->AddEventToQueue(trig_event);

			LOG_DEBUG("Hit the Slow_Motion Trigger\n");

			PiEngine->getFrameRateController()->SetTimeScale(0.25f);
			isSlowed = true;
			PiEngine->SetIsGameSlowed(true);


		}

		else if (cevent->m_gameObject->GetGameObjectName() == "JUMP_TUTORIAL_TRIGGER")
		{

		TutorialTextureEvent* a_eve = new TutorialTextureEvent();
		a_eve->TutorialTexture_Type = "Jump";

		PiEvent->AddEventToQueue(a_eve);
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "BOOST_TUTORIAL_TRIGGER")
		{

		TutorialTextureEvent* a_eve = new TutorialTextureEvent();
		a_eve->TutorialTexture_Type = "Boost";
		PiEvent->AddEventToQueue(a_eve);
		} 

		else if (cevent->m_gameObject->GetGameObjectName() == "LEFT_TUTORIAL_TRIGGER")
		{
			
			
			TutorialTextureEvent* a_eve = new TutorialTextureEvent();
			a_eve->TutorialTexture_Type = "Left";
			PiEvent->AddEventToQueue(a_eve);

		}

		else if (cevent->m_gameObject->GetGameObjectName() == "RIGHT_TUTORIAL_TRIGGER")
		{
	
		TutorialTextureEvent* a_eve = new TutorialTextureEvent();
		a_eve->TutorialTexture_Type = "Right";
		PiEvent->AddEventToQueue(a_eve);
		}


		else if (cevent->m_gameObject->GetGameObjectName() == "EXTRA_LIFE_TUTORIAL_TRIGGER")
		{
	
		TutorialTextureEvent* a_eve = new TutorialTextureEvent();
		a_eve->TutorialTexture_Type = "ELife";
		PiEvent->AddEventToQueue(a_eve);
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "PICKUP_TUTORIAL_TRIGGER")
		{
		
		TutorialTextureEvent* a_eve = new TutorialTextureEvent();
		a_eve->TutorialTexture_Type = "PickUp";
		PiEvent->AddEventToQueue(a_eve);
		}


		else if (cevent->m_gameObject->GetGameObjectName() == "Trigger_rise_R")
		{
			TriggerComponent* trig = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
			TriggeredEvent *rise = new TriggeredEvent();
			rise->id = trig->GetID();
			rise->behaviour = "Rise";
			PiEvent->AddEventToQueue(rise);
			//LOG_DEBUG("HIt Rise Trigger\n");
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "TriggerDrop_Zo")
		{
			TriggerComponent* trig = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
			TriggeredEvent *drop = new TriggeredEvent();
			drop->id = trig->GetID();
			drop->behaviour = "Drop";
			PiEvent->AddEventToQueue(drop);
			//LOG_DEBUG("HIt Drop Trigger\n");
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "Trigger_drop_R")
		{
		TriggerComponent* trig = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
		TriggeredEvent *drop = new TriggeredEvent();
		drop->id = trig->GetID();
		drop->behaviour = "Drop";
		PiEvent->AddEventToQueue(drop);
		//LOG_DEBUG("HIt Drop Trigger\n");
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "Trigger_fall_cube_pusher_left_R")
		{
		TriggerComponent* trig = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
		TriggeredEvent *tumble = new TriggeredEvent();
		tumble->id = trig->GetID();
		tumble->behaviour = "Tumble_Left";
		PiEvent->AddEventToQueue(tumble);
		//LOG_DEBUG("HIt Tumble Left Trigger\n");
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "Trigger_fall_cube_pusher_right_R")
		{
		TriggerComponent* trig = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
		TriggeredEvent *tumble = new TriggeredEvent();
		tumble->id = trig->GetID();
		tumble->behaviour = "Tumble_Right";
		PiEvent->AddEventToQueue(tumble);
		//LOG_DEBUG("HIt Tumble Right Trigger\n");
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "Trigger_cube_static_to_dynamic_R")
		{
		TriggerComponent* trig = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
		TriggeredEvent *setDyn = new TriggeredEvent();
		setDyn->id = trig->GetID();
		setDyn->behaviour = "Set_Dynamic";
		PiEvent->AddEventToQueue(setDyn);
		//LOG_DEBUG("Static to Dynamic Trigger\n");
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "Trigger_up_cube_pusher_R")
		{
		TriggerComponent* trig = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
		TriggeredEvent *goUp = new TriggeredEvent();
		goUp->id = trig->GetID();
		goUp->behaviour = "goUp_Tumble";
		PiEvent->AddEventToQueue(goUp);
		//LOG_DEBUG("Tumble pusher up Trigger\n");
		}

		else if (cevent->m_gameObject->GetGameObjectName() == "Trigger_cube_follow_R")
		{
		TriggerComponent* trig = cevent->m_gameObject->GetComponent<TriggerComponent>(STRINGIFY(TriggerComponent));
		TriggeredEvent *followCube = new TriggeredEvent();
		followCube->id = trig->GetID();
		followCube->behaviour = "follow_cube";
		PiEvent->AddEventToQueue(followCube);
		//LOG_DEBUG("Follow Cube Trigger Trigger\n");
		}
			
		else if (cevent->m_gameObject->GetGameObjectType()==EGameObjectType::Obstacle)
		{
			if (m_godMode || PiInput->m_tutorialGodMode)
			{
				return;
			}
			if (PiStats->m_extraLife > 0)
			{
				--PiStats->m_extraLife;
				ActivateShipState(EShipState::Shield);
				PowerShield();
				return;
			}

			m_maxVelocity = 2000.0f;
			m_dashCooldown = 0.0f;
			m_shipState = 0;

			//death menu
			OnDeathEvent* death = new OnDeathEvent();
			
			
			
			//PiStats->m_running = false;
			PiAudio->PlaySounds("crash2", m_position, 20.0f);
			PiEvent->AddEventToManagersQueue(death);

			//Update High Score
			if (PiStats->m_score >= PiResource->GetConfig()->m_HighScore)
			{
				//Write into file


				LPWSTR wszPath = NULL;
				HRESULT hr;
				string file_name;
				hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &wszPath);

				if (SUCCEEDED(hr)) {
					_bstr_t bstrPath(wszPath);
					file_name = std::string((char*)bstrPath) + "\\Odyssey\\";
					CreateDirectory(file_name.c_str(), NULL);
					file_name += PiResource->GetConfig()->GetHighScoreDir();
				}


				std::ofstream file(file_name);

				if (!file)
				{
					std::cerr << "unable to open file :" << file_name << std::endl;
				}

				PiResource->GetConfig()->m_HighScore = PiStats->m_score;
				file << PiStats->m_score;
				file.close();


				PiResource->GetConfig()->m_HighScore = PiStats->m_score;
				death->m_score = PiResource->GetConfig()->m_HighScore;
			}

			else
			{
				death->m_score = PiStats->m_score;
			}
		}
	}

	else if (a_Event->GetEventType() == EEventType::Exit2Main)
	{
		PiEngine->setIsPlayerDead(false);
	}
	
	else if (a_Event->GetEventType() == EEventType::PiercePowerUp)
	{
		DeactivateShipState(EShipState::Boost);
		ActivateShipState(EShipState::Forward);
	}
}

void InputComponent::Deactivate(void)
{
}

void InputComponent::OverrideParams(const json&,float offset)
{
	m_pTrans = nullptr;
	m_pRBodyComp = nullptr;
}

EInputType InputComponent::GetInputType(void) const
{
	return m_inputType;
}

void InputComponent::SetInputType(EInputType e)
{
	m_inputType = e;
}

void InputComponent::ClampValues(float &a_value, float a_max, float a_min)
{
	if (a_value >= a_max)
		a_value = a_max;
	if (a_value <= a_min)
		a_value = a_min;
}