//
//  FrameRateController.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/26/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "FrameRateController.h"

FrameRateController::FrameRateController(double a_dFPS) :
    m_uFrameStartTime(0),
    m_dPerformanceFrequency(1.0 / SDL_GetPerformanceFrequency()),
    m_dFrameTimeDouble(1.0 / a_dFPS),
    m_fFrameTimeFloat(0.0f),
    m_fTime(0.0f),
    m_dUnscaledTime(0.0),
    m_dIdealDeltaTime((1.0 / a_dFPS)),
    m_fDeltaTime(1.0f / (float)a_dFPS),
    m_fTimeScale(1.0f),
    m_dMaxFPS(a_dFPS)
{

}

FrameRateController::~FrameRateController()
{

}

void FrameRateController::FrameStart()
{
    m_uFrameStartTime = SDL_GetPerformanceCounter();
}

void FrameRateController::FrameEnd()
{
    uint64 uFrameEndTime = SDL_GetPerformanceCounter();
    m_dFrameTimeDouble = (double)((uFrameEndTime - m_uFrameStartTime)) * m_dPerformanceFrequency;

    while (m_dFrameTimeDouble < m_dIdealDeltaTime)
    {
        uFrameEndTime = SDL_GetPerformanceCounter();
        m_dFrameTimeDouble = (double)((uFrameEndTime - m_uFrameStartTime)) * m_dPerformanceFrequency;
    }
    //m_fFrameTimeFloat = (float)m_dFrameTimeDouble;
    //m_fDeltaTime = m_fFrameTimeFloat;
	m_dUnscaledTime += m_dFrameTimeDouble;
	m_fFrameTimeFloat = (float)m_dFrameTimeDouble;

	// for Slow Motion - Time.TimeScale
	m_fDeltaTime = m_fFrameTimeFloat * m_fTimeScale;

	m_fDeltaTime = fminf(m_fDeltaTime, (float)m_dIdealDeltaTime * 2.0f);			// Clamps to 30.0 FPS

	m_fTime += m_fDeltaTime;
}

