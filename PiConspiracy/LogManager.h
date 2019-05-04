//
//  LogManager.h
//  PiEngine
//
//  Created by PiConspiracy on 11/10/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include <spdlog/spdlog.h>

class LogManager : public Singleton<LogManager>
{
public:
                    LogManager();
    void            AddLogger(const char* pcszLoggerName);
    void            RemoveLogger(const char* pcszLoggerName);

    std::shared_ptr<spdlog::logger> GetLogger(const char* pcszLogggerName);

    static const char* s_defaultLogFileName;

private:
    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_Loggers;

};

#define LOG LogManager::GetInstance()

#define LOG_ADD_LOG(lg) LogManager::GetInstance()->AddLogger(lg)
#define REMOVE_LOG(lg)  LogManager::GetInstance()->RemoveLogger(lg)

#define LOG_CUS_TRACE(lg, msg) LogManager::GetInstance()->GetLogger(lg)->trace(msg)
#define LOG_CUS_TRACE_ARG(lg,msg,arg1,...) LogManager::GetInstance()->GetLogger(lg)->trace(msg,arg1,__VA_ARGS__)

#define LOG_CUS_INFO(lg,msg) LogManager::GetInstance()->GetLogger(lg)->info(msg)
#define LOG_CUS_INFO_ARG(lg,msg,arg1,...) LogManager::GetInstance()->GetLogger(lg)->info(msg,arg1,__VA_ARGS__)

#define LOG_CUS_DEBUG(lg,msg) LogManager::GetInstance()->GetLogger(lg)->debug(msg)
#define LOG_CUS_DEBUG_ARG(lg,msg,arg1,...) LogManager::GetInstance()->GetLogger(lg)->debug(msg,arg1,__VA_ARGS__)

#ifdef _DEBUG
#define LOG_CUS_WARN(lg,msg) LogManager::GetInstance()->GetLogger(lg)->warn(msg); __debugbreak()
#define LOG_CUS_WARN_ARG(lg,msg,arg1,...) LogManager::GetInstance()->GetLogger(lg)->warn(msg,arg1,__VA_ARGS__); __debugbreak()
#else
#define LOG_CUS_WARN(lg,msg) LogManager::GetInstance()->GetLogger(lg)->warn(msg);
#define LOG_CUS_WARN_ARG(lg,msg,arg1,...) LogManager::GetInstance()->GetLogger(lg)->warn(msg,arg1,__VA_ARGS__)
#endif // _DEBUG

#ifdef _DEBUG
#define LOG_CUS_ERROR(lg,msg) LogManager::GetInstance()->GetLogger(lg)->error(msg); __debugbreak()
#define LOG_CUS_ERROR_ARG(lg,msg,arg1,...) LogManager::GetInstance()->GetLogger(lg)->error(msg,arg1,__VA_ARGS__); __debugbreak()
#else
#define LOG_CUS_ERROR(lg,msg) LogManager::GetInstance()->GetLogger(lg)->error(msg)
#define LOG_CUS_ERROR_ARG(lg,msg,arg1,...) LogManager::GetInstance()->GetLogger(lg)->error(msg,arg1,__VA_ARGS__)
#endif // _DEBUG

#ifdef _DEBUG
#define LOG_CUS_CRITICAL(lg,msg) LogManager::GetInstance()->GetLogger(lg)->critical(msg); __debugbreak()
#define LOG_CUS_CRITICAL_ARG(lg,msg,arg1,...) LogManager::GetInstance()->GetLogger(lg)->critical(msg,arg1,__VA_ARGS__); __debugbreak()
#else
#define LOG_CUS_CRITICAL(lg,msg) LogManager::GetInstance()->GetLogger(lg)->critical(msg)
#define LOG_CUS_CRITICAL_ARG(lg,msg,arg1,...) LogManager::GetInstance()->GetLogger(lg)->critical(msg,arg1,__VA_ARGS__)
#endif // _DEBUG