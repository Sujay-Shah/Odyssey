//
//  LogManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/10/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "LogManager.h"

LogManager::LogManager()
{
    m_Loggers["PiEngine"] = spdlog::stdout_color_mt("PiEngine");
    m_Loggers["PiEngine"]->set_pattern("[%n] [%^%l%$] %v");
    m_Loggers["PiEngine"]->set_level(spdlog::level::trace);

    //m_Loggers["LYViewer"] = spdlog::stdout_color_mt("LYViewer");
    //m_Loggers["LYViewer"]->set_pattern("[%n] [%^%l%$] %v");
    //m_Loggers["LYViewer"]->set_level(spdlog::level::trace);
}

void LogManager::AddLogger(const char* pLoggerName)
{
    assert(m_Loggers.find(pLoggerName) == m_Loggers.end());
    m_Loggers[pLoggerName] = spdlog::stdout_color_mt(pLoggerName);
}

void LogManager::RemoveLogger(const char* pLoggerName)
{
    assert(m_Loggers.find(pLoggerName) != m_Loggers.end());
    m_Loggers.erase(pLoggerName);
}

std::shared_ptr<spdlog::logger> LogManager::GetLogger(const char* pcszLoggerName)
{
    assert(m_Loggers.find(pcszLoggerName) != m_Loggers.end());
    return m_Loggers[pcszLoggerName];
}

const char* LogManager::s_defaultLogFileName = "PiLog.log";