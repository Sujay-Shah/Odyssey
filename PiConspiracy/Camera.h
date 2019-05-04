//
//  Camera.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class Camera
{
public:

	enum eCameraMovement
	{
		eCameraMovement_Forward,
		eCameraMovement_Backward,
		eCameraMovement_Left,
		eCameraMovement_Right,
		eCameraMovement_Up,
		eCameraMovement_Down

	};

                                    Camera(uint32 a_uType);
                                    ~Camera();
    
	bool							Initialize();
    const inline mat4               GetViewMatrix();
    const inline mat4               GetProjectionMatrix();
	const inline mat4				GetOrthoProjectionMatrix();

	void							ProcessKeyBoardMovement(eCameraMovement a_eMovement, float a_fDeltaTime);
	void							ProcessMouseMovement(float a_fXOffset, float a_fYOffset, bool a_bClampPitch = true);
	void							ProcessMouseScroll(float a_fYOffset);

	const inline vec3               GetPosition() const;
	inline void						SetPosition(vec3 a_camPos);

	inline void						SetFieldOfView(float a_fNewFOV);
    inline void                     SetFreeCameraOn(bool a_bIsActive);
    inline bool                     IsFreeCameraActive();
	inline float					GetFeildofView();
	inline void						SetYaw(float a_fDegrees);		// Y Axis
	inline void						SetPitch(float a_fDegrees);		// X Axis

    void                            Update(float a_fDeltaTime);
	//void							EnableFreeMoveCamera(bool a_istrue);
	//bool							m_bIsFreeMoveCamera;

	void							SetCameraShake(float a_fShakeIntensity);
	void							SetRoll(float a_angleDeg);
	void						    resetRoll(void);

	// Camera Attributes
	vec3                            m_vPosition;
	vec3							m_vPostShakePosition;
	vec3							m_vFront;
	vec3                            m_vUp;
	vec3							m_vRight;
	vec3							m_vWorldUp;

	vec3							originalUp_;
	vec3							originalRight_;
protected:

	void							UpdateCameraVectors();

	void							FreeCameraUpdate(float a_fDeltaTime);
	void							ResetProjectionMatrix();

	void							CameraShakeUpdate(float a_fDeltaTime);
	void							CameraDeathUpdate(float a_fDeltaTime);

private:
	float							m_dtAlternative;
	void							setViewMatrix_(void);
    float                           m_fAngle;
    float                           m_fFieldOfView;
	float							m_fFieldOfViewChange;
    float                           m_fAspectRatio;
    float                           m_fNearPlane;
    float                           m_fFarPlane;

	bool							m_bIsFreeMoveCamera;
    bool                            m_bIsFirstMouseClick;

	bool							m_bIsPerspective;

	

	// Camera's Euler Angles ( Free Camera)
	float							m_fYaw;
	float							m_fPitch;

	// Camera Options 
	float							m_fMovementSpeed;
	float							m_fMouseSensitivity;
	float							m_fZoom;


	// Camera Shake Parameters
	float							m_fShakeIntensity;
	vec3							m_vShakeOffset;
	vec3							m_vAppliedShakeOffset;
	vec3							m_vShakeVelocity;
	float							m_fTimeBetweenShakeOffsets;
	float							m_fTimeSinceLastShakeOffset;


    vec3                            m_vCameraUp;
    vec3                            m_vCameraRight;
	
	vec3							m_EulerAngles;
	vec3							m_vSpringVelocity;

    mat4                            m_ViewMatrix;
    mat4                            m_ProjectionMatrix; // Perspective Camera
	mat4							m_OrthoProjectionMatrix; // Orthographic Camera
	mat4							m_Rotation;

    float                           fLastMouseX;
    float                           fLastMouseY;

	uint32							m_uType;
};

const inline mat4 Camera::GetViewMatrix()
{
    return m_ViewMatrix;
}

const inline mat4 Camera::GetProjectionMatrix()  // return perspective projection
{
    return m_ProjectionMatrix;
}

const inline mat4 Camera::GetOrthoProjectionMatrix()
{
	return m_OrthoProjectionMatrix;
}

const inline vec3 Camera::GetPosition() const
{
    return m_vPosition;
}

inline void Camera::SetPosition(vec3 a_camPos)
{
	m_vPosition = a_camPos;
}

inline void Camera::SetFieldOfView(float a_fNewFOV)
{
	m_fFieldOfViewChange = a_fNewFOV;
}

inline void Camera::SetFreeCameraOn(bool a_bIsActive)
{
    m_bIsFreeMoveCamera = a_bIsActive;
}

inline bool Camera::IsFreeCameraActive()
{
    return m_bIsFreeMoveCamera;
}

inline float Camera::GetFeildofView()
{
	return m_fFieldOfView;
}

inline void Camera::SetPitch(float a_fDegrees)
{
	m_fPitch = a_fDegrees;
}

inline void Camera::SetYaw(float a_fDegrees)
{
	m_fPitch = a_fDegrees;
}
