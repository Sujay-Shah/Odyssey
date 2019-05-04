//
//  Macro.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

//Entry Point
#define ENTER_PiENGINE()                \
int main(int argc, char* args[])        \
{										\
Engine engine;						    \
if (PiEngine->Initialize())             \
{                                       \
    PiEngine->Run();                    \
    PiEngine->Finalize();               \
                                        \
    return EXIT_SUCCESS;                \
}                                       \
return EXIT_FAILURE;                    \
}

//delete pointer
#define SAFE_DELETE(a_pPointer)        \
    if (a_pPointer)                     \
    {                                   \
        delete a_pPointer;              \
        a_pPointer = nullptr;           \
    }                                   

//delete array
#define SAFE_DELETE_ARRAY(a_pPointer)  \
    if (a_pPointer)                     \
    {                                   \
        delete []a_pPointer;            \
        a_pPointer = nullptr;           \
    }

// delete array of pointers
#define SAFE_DELETE_ARRAY_POINTERS(a_pPointers, a_uCount)          \
    for(size_t ii = 0; ii < a_uCount; ++ii)                         \
    {                                                               \
        SAFE_DELETE(a_pPointers[ii]);                              \
    }

//disable copy
#define DISALLOW_COPY(Typename) \
	Typename(const Typename&) = delete

//disable assign
#define DISALLOW_ASSIGN(Typename) \
	Typename operator=(const Typename&) = delete

//disable copy and assign 
#define DISALLOW_COPY_AND_ASSIGN(Typename) \
	Typename operator=(const Typename&) = delete; \
	Typename(const Typename&) = delete;

// bool function check( Use only when no pointer needed)
#define FUNCTION_INITIALIZE(a_FunctionName, a_LogMsg, a_bIsInit)        \
if (!a_FunctionName()) {                                                \
if (a_bIsInit) {LOG_CRITICAL(a_LogMsg"() failed to initialize.");}       \
else {LOG_CRITICAL(a_LogMsg"() failed to finalize.");} }                 \
else {                                                                  \
if(a_bIsInit) {LOG_INFO(a_LogMsg"() successfully initialized.");}       \
else {LOG_INFO(a_LogMsg"() successfully finalized.");} }

// bool Pointer Initialize function check(Use only when no argument init)
#define PTR_INITIALIZE(a_Ptr, a_LogMsg)                                 \
LOG_INFO(a_LogMsg" begins Initialize()...");                         \
if (!a_Ptr->Initialize())                                               \
{ LOG_CRITICAL(a_LogMsg"->Initialize() failed to initialize."); }        //\
//else                                                                    \
//{ LOG_INFO(a_LogMsg"->Initialize() successfully initialized."); }

// memset
#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

// Concationation of strings
#define STRING_CONCATINATE_INTERNAL(x, y) x##y
#define STRING_CONCATINATE(x, y) STRING_CONCATINATE_INTERNAL(x, y)

// Stringification of a macro expression
#define STRINGIFY_INTERNAL(x) #x
#define STRINGIFY(x) STRINGIFY_INTERNAL(x)


// Creates the typedef for out enums
#define TYPEDEF_ENUM(x) typedef x::Enum STRING_CONCATINATE(E, x)

// used for debuglog
#define GLOBAL_MSG(msg) PRAGMA(message(msg))

// Assimp Flags
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

// GL error
#define GLCheckError() (glGetError() == GL_NO_ERROR)

#define FILE_NAME_FROM_PATH(x, p) \
size_t i = x.find_last_of("."); \
p = x.substr(0,i);

#define RADIANS(angle) \
angle * (PI / 180)


#define LERP_FLOATS(end, start, t)

// Log Macros

#define LOG_TRACE(msg)                   LOG_CUS_TRACE("PiEngine", msg)
#define LOG_TRACE_ARG(msg,arg1,...)      LOG_CUS_TRACE_ARG("PiEngine",msg,arg1,__VA_ARGS__)

#define LOG_INFO(msg)                    LOG_CUS_INFO("PiEngine", msg)
#define LOG_INFO_ARG(msg,arg1,...)       LOG_CUS_INFO_ARG("PiEngine",msg,arg1,__VA_ARGS__)

#define LOG_ERROR(msg)                   LOG_CUS_ERROR("PiEngine", msg)
#define LOG_ERROR_ARG(msg,arg1,...)      LOG_CUS_ERROR_ARG("PiEngine",msg,arg1,__VA_ARGS__)

#define LOG_DEBUG(msg)                   LOG_CUS_DEBUG("PiEngine", msg)
#define LOG_DEBUG_ARG(msg,arg1,...)      LOG_CUS_DEBUG_ARG("PiEngine",msg,arg1,__VA_ARGS__)

#define LOG_WARN(msg)                    LOG_CUS_WARN("PiEngine", msg)
#define LOG_WARN_ARG(msg,arg1,...)       LOG_CUS_WARN_ARG("PiEngine",msg,arg1,__VA_ARGS__)

#define LOG_CRITICAL(msg)                 LOG_CUS_CRITICAL("PiEngine", msg)
#define LOG_CRITICAL_ARG(msg,arg1,...)    LOG_CUS_CRITICAL_ARG("PiEngine",msg,arg1,__VA_ARGS__)

#define ASSERT(express)                  if(express == false) { LOG_CRITICAL("Assertion triggered!"); }
#define ASSERT_MSG(express, msg)         if(express == false) { LOG_CRITICAL(msg); }
#define ASSERT_RF(express, msg)          if(express == false) { LOG_CRITICAL(msg); return false; }
