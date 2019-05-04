//
//  Camera.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Camera.h"
#include "Renderer.h"
#include "Engine.h"
#include "FrameRateController.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"
#include "StatsManager.h"

Camera::Camera(uint32 a_uType):
m_fAngle((float)PiResource->GetConfig()->m_fovAngle),
m_fFieldOfView(glm::radians(m_fAngle)),
fLastMouseX((float)PiEngine->GetClientWidth()),
fLastMouseY((float)PiEngine->GetClientHeight()),
m_fAspectRatio((float)PiEngine->GetClientWidth()/(float)PiEngine->GetClientHeight()),
m_fNearPlane(0.1f),
m_fFarPlane(100000.0f),
m_vPosition(vec3(0.0f, 5.0f, 30.0f)),
m_bIsFreeMoveCamera(false),
m_bIsFirstMouseClick(true),
m_bIsPerspective(false),
m_Rotation(mat4(1)),
m_fYaw(-90.0f),
m_fPitch(0.0f),
m_fMovementSpeed(1000.0f),
m_fMouseSensitivity(0.1f),
m_fZoom(45.0f),
m_vFront(0.0f, 0.0f, -1.0f),
m_vRight(1.0f, 0.0f, 0.0f),
m_vUp(0.0f, 1.0f, 0.0f),
m_fShakeIntensity(0.0f),
m_vShakeOffset(vec3(0.0f)),
m_fTimeBetweenShakeOffsets(0.0167f),
m_fTimeSinceLastShakeOffset(0.0f),
m_uType(a_uType),
m_dtAlternative(0.0f)
{
	m_fFieldOfViewChange = m_fFieldOfView;
	m_vWorldUp = m_vUp;

	// Perspective Matrix
	m_ProjectionMatrix = glm::perspective(m_fFieldOfView, m_fAspectRatio, m_fNearPlane, m_fFarPlane);

	// OrthoGraphic Calculation
	{
		float fOrthoScale = 0.25f;
		float fLeft = 0.0f;//100 * m_fAspectRatio * fOrthoScale;
		float fRight = (float)PiEngine->GetClientWidth();
		float fTop = (float)PiEngine->GetClientHeight();//-100 / m_fAspectRatio * fOrthoScale;
		float fBottom = 0.0f;//100 / m_fAspectRatio * fOrthoScale;
		m_OrthoProjectionMatrix = glm::ortho(fLeft, fRight, fBottom, fTop, -1.0f, m_fFarPlane);
	}



}

Camera::~Camera()
{

}

bool Camera::Initialize()
{
	UpdateCameraVectors();
	setViewMatrix_();
	//used to reset view to normal
	originalRight_ = m_vRight;
	originalUp_ = m_vUp;
	return true;
}

void Camera::UpdateCameraVectors()
{
	vec3 vFront;
	vFront.x = cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
	vFront.y = sin(glm::radians(m_fPitch));
	vFront.z = sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));

    m_vFront = vFront = glm::normalize(vFront);

	m_vRight = glm::normalize(glm::cross(vFront, m_vWorldUp));
	m_vUp = glm::normalize(glm::cross(m_vRight, vFront));
	
}

void Camera::ProcessKeyBoardMovement(eCameraMovement a_eMovement, float a_fDeltaTime)
{
	float fVelocity = m_fMovementSpeed * a_fDeltaTime;

	if (a_eMovement == eCameraMovement_Forward)
	{
		m_vPosition += m_vFront * fVelocity;
	}

	else if (a_eMovement == eCameraMovement_Backward)
	{
		m_vPosition -= m_vFront * fVelocity;
	}

	else if (a_eMovement == eCameraMovement_Left)
	{
		m_vPosition -= m_vRight * fVelocity;
	}

	else if (a_eMovement == eCameraMovement_Right)
	{
		m_vPosition += m_vRight * fVelocity;
	}

	else if (a_eMovement == eCameraMovement_Down)
	{
		m_vPosition -= m_vUp * fVelocity;
	}

	else if (a_eMovement == eCameraMovement_Up)
	{
		m_vPosition += m_vUp * fVelocity;
	}
}

void Camera::ProcessMouseMovement(float a_fXOffset, float a_fYOffset, bool a_bClampPitch /* = true */)
{
    a_fXOffset *= m_fMouseSensitivity;
    a_fYOffset *= m_fMouseSensitivity;

	m_fYaw += a_fXOffset;
	m_fPitch += a_fYOffset;

	if (a_bClampPitch)
	{
		if (m_fPitch > 89.0f)
		{
			m_fPitch = 89.0f;
		}

		if (m_fPitch < -89.0f)
		{
			m_fPitch = -89.0f;
		}
	}

	UpdateCameraVectors();
}

void Camera::CameraShakeUpdate(float a_fDeltaTime)
{
	uint32 uShake = uint32(m_fShakeIntensity * 1000.0f);
	if (uShake > 0)
	{
		float fTimeScale = PiFPS->GetTimeScale();

		m_fTimeSinceLastShakeOffset += a_fDeltaTime;

		if (m_fTimeSinceLastShakeOffset > m_fTimeBetweenShakeOffsets)
		{
			m_vShakeOffset.x = (float)(rand() % (uShake - uShake / 2)) * (float)0.001f * fTimeScale;
			m_vShakeOffset.y = (float)(rand() % (uShake - uShake / 2)) * (float)0.001f * fTimeScale;
			m_vShakeOffset.z = (float)(rand() % (uShake - uShake / 2)) * (float)0.001f * fTimeScale;

			m_fTimeSinceLastShakeOffset = 0.0f;
		}

		InterpolateTowards(m_vAppliedShakeOffset, m_vShakeVelocity, m_vShakeOffset, 0.3f, 50.0f, a_fDeltaTime);
		
		m_fShakeIntensity *= (1.0f - 8.0f * a_fDeltaTime);
	}
	else
	{
		m_fShakeIntensity = 0.0f;
		m_vAppliedShakeOffset = vec3(0.0f);
	}
}

void Camera::CameraDeathUpdate(float a_fDeltaTime)
{
	m_dtAlternative += 0.016f;
	m_vPosition -= (glm::vec3(0, sinf(m_dtAlternative), -cosf(50)) ) ;
}

void Camera::setViewMatrix_()
{

	m_ViewMatrix = glm::lookAt(m_vPostShakePosition, m_vPostShakePosition + m_vFront, m_vUp);

	//std::cout << "eye : " << m_vPostShakePosition << " front :  " << m_vFront << " up : " << m_vUp << std::endl;
}

void Camera::SetCameraShake(float a_fShakeIntensity)
{
	if (m_fShakeIntensity < a_fShakeIntensity)
	{
		m_fShakeIntensity = a_fShakeIntensity;
	}
}

void Camera::SetRoll(float a_angleDeg)
{
	float cs = cos(RADIANS(a_angleDeg));
	float sn = sin(RADIANS(a_angleDeg));
	vec3 t = m_vRight; // remember old right
	m_vRight=glm::vec3(cs*t.x - sn*m_vUp.x, cs*t.y - sn*m_vUp.y, cs*t.z - sn*m_vUp.z);
	m_vUp = glm::vec3(sn*t.x + cs * m_vUp.x, sn*t.y + cs* m_vUp.y, sn*t.z + cs* m_vUp.z);
	setViewMatrix_();
}

void Camera::resetRoll(void)
{
	m_vUp = glm::mix(m_vUp, originalUp_, 0.8);
	m_vRight = glm::mix(m_vRight, originalRight_, 0.01);
}

void Camera::ProcessMouseScroll(float a_fYOffset)
{
	if (m_fFieldOfView >= 1.0f && m_fFieldOfView <= 45.0f)
	{
		m_fFieldOfView -= a_fYOffset;
	}
	if (m_fFieldOfView <= 1.0f)
	{
		m_fFieldOfView = 1.0f;
	}
	if (m_fFieldOfView >= 45.0f)
	{
		m_fFieldOfView = 45.0f;
	}
}

void Camera::Update(float a_fDeltaTime)
{
	if (m_bIsFreeMoveCamera)
	{
		FreeCameraUpdate(a_fDeltaTime);
	}

	// Perspective Matrix
    //m_ProjectionMatrix = glm::perspective(m_fFieldOfView, m_fAspectRatio, m_fNearPlane, m_fFarPlane);
	
	// OrthoGraphic Calculation
	{
		float fOrthoScale = 0.25f;
		float fLeft = 0.0f;//100 * m_fAspectRatio * fOrthoScale;
		float fRight = (float)PiEngine->GetClientWidth();
		float fTop = (float)PiEngine->GetClientHeight();//-100 / m_fAspectRatio * fOrthoScale;
		float fBottom = 0.0f;//100 / m_fAspectRatio * fOrthoScale;
		m_OrthoProjectionMatrix = glm::ortho(fLeft, fRight, fBottom,fTop, -1.0f, m_fFarPlane);
	}
	//SetRoll(0.0f);
	setViewMatrix_();
   

	/*if(PiInput->IsKeyBoardButtonUp(SDL_SCANCODE_SPACE))
	{
		SetCameraShake(500.0f);
	}*/

	CameraShakeUpdate(PiFPS->GetDeltaTime());
	m_vPostShakePosition = m_vPosition + m_vAppliedShakeOffset;

	if (PiEngine->getPanCamera())
	{
		
		CameraDeathUpdate(a_fDeltaTime);
	
	}

	if (!m_bIsFreeMoveCamera)
	{
		resetRoll();
	}
}

//int fps;
void Camera::FreeCameraUpdate(float a_fDeltaTime)
{
	if (PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_W))
	{
		ProcessKeyBoardMovement(eCameraMovement_Forward, a_fDeltaTime);
	}

	if (PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_A))
	{
		ProcessKeyBoardMovement(eCameraMovement_Left, a_fDeltaTime);
	}

	if (PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_S))
	{
		ProcessKeyBoardMovement(eCameraMovement_Backward, a_fDeltaTime);
	}

	if (PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_D))
	{
		ProcessKeyBoardMovement(eCameraMovement_Right, a_fDeltaTime);
	}

	if (PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_T))
	{
		ProcessKeyBoardMovement(eCameraMovement_Up, a_fDeltaTime);
	}

	if (PiInput->IsKeyBoardButtonHeld(SDL_SCANCODE_H))
	{
		ProcessKeyBoardMovement(eCameraMovement_Down, a_fDeltaTime);
	}


	if (PiInput->IsMouseButtonHeld(EMouseButton::Right))
	{
        vec2 vMousePosition = PiInput->GetMousePos();
        
        //vec2 vMouseDeltaPosition = PiInput->GetMouseDeltas();

        if (m_bIsFirstMouseClick)
        {
            fLastMouseX = vMousePosition.x;
            fLastMouseY = vMousePosition.y;
            m_bIsFirstMouseClick = false;
        }

        float fXOffset = vMousePosition.x - fLastMouseX;
        float fYOffset = vMousePosition.y - fLastMouseY;
        fLastMouseX = vMousePosition.x;
        fLastMouseY = vMousePosition.y;

        //if (++fps % 60 == 0) {
        //    std::cout << "offset X" << fXOffset << std::endl;
        //    std::cout << "offset Y" << fYOffset << std::endl;
        //    fps = 0;
        //}

        ProcessMouseMovement(fXOffset, fYOffset);
	}

    if (PiInput->IsMouseButtonUp(EMouseButton::Right)) 
	{
        m_bIsFirstMouseClick = true;
    }
}

void Camera::ResetProjectionMatrix()
{
	m_ProjectionMatrix = glm::perspective(m_fFieldOfView, m_fAspectRatio, m_fNearPlane, m_fFarPlane);
}
