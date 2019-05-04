//
//  FrameRateController.h
//  PiEngine
//
//  Created by PiConspiracy on 10/26/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class FrameRateController : Singleton<FrameRateController>
{
public:
	                    FrameRateController(double a_dFPS);
	                    ~FrameRateController();

	void                FrameStart();
	void                FrameEnd();

    inline float        GetDeltaTime();
    inline float        GetFrameTime();
    inline float        GetTime();
    inline float        GetTimeScale();
    inline double       GetFrameTimeDouble();
    inline double       GetUnscaledTime();
    inline double       GetFPS();

    inline void         ResetTime();
    inline void         SetTimeScale(float a_fTimeScale);

	double          m_dIdealDeltaTime;
private:
	
    uint64          m_uFrameStartTime;
    double          m_dPerformanceFrequency;
    double          m_dFrameTimeDouble;
    float           m_fFrameTimeFloat;
    float           m_fDeltaTime;
    float           m_fTimeScale;
    float           m_fTime;
    
    double          m_dMaxFPS;
    double          m_dUnscaledTime;
};

inline float FrameRateController::GetDeltaTime()
{
    return m_fDeltaTime;
}

inline float FrameRateController::GetFrameTime()
{
    return m_fFrameTimeFloat;
}

inline float FrameRateController::GetTime()
{
    return m_fTime;
}

inline float FrameRateController::GetTimeScale()
{
    return m_fTimeScale;
}

inline double FrameRateController::GetFrameTimeDouble()
{
    return m_dFrameTimeDouble;
}

inline double FrameRateController::GetFPS()
{
    return (1.0 / m_dFrameTimeDouble);
}

inline double FrameRateController::GetUnscaledTime()
{
    return m_dUnscaledTime;
}

inline void FrameRateController::ResetTime()
{
    m_dUnscaledTime = 0.0;
}

inline void FrameRateController::SetTimeScale(float a_fTimeScale)
{
    m_fTimeScale = a_fTimeScale;
}