//  ScriptManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ScriptManager.h"
#include "ResourceManager.h"


lua_State* ScriptManager::m_luaState = nullptr;
ScriptManager::ScriptManager()
{
}


ScriptManager::~ScriptManager()
{
	lua_gc(m_luaState, LUA_GCCOLLECT, 0);
	lua_gc(m_luaState, LUA_GCCOLLECT, 0);
	lua_close(m_luaState);

	clearAllScripts();
}

void ScriptManager::Initialize(void)
{
	ScriptManager::m_luaState = luaL_newstate();
	if (ScriptManager::m_luaState==nullptr)
	{
		LOG_WARN("failed to create Lua state\n");
	}
	luaL_openlibs(ScriptManager::m_luaState);
	//lua_pcall(m_luaState, 0, 0, 0);

	m_nameMap = PiResource->GetScriptPath();

	RegisterClasses();
	LoadAllScripts();
}

void ScriptManager::Update(float a_dt)
{
}

void ScriptManager::RegisterClasses()
{
	luabridge::getGlobalNamespace(m_luaState).
		beginClass<Transform>("Transform")
		.addFunction("set_pos", &Transform::SetPositionFromLua)
		.addFunction("get_pos", &Transform::GetPosition)
		.endClass()
		.beginClass<vec3>("vec3")
		.addData("x", &glm::vec3::x)
		.addData("y", &glm::vec3::y)
		.addData("z", &glm::vec3::z)
		.endClass();
	//keep adding all classes that you want to register, that you want to expose to lua
}

void ScriptManager::clearAllScripts()
{
	/*for (auto it : m_scriptMap)
	{
		delete it.second;
	}*/
	//delete m_luaState;
}

void ScriptManager::LoadAllScripts()
{
	            //name     path
	for (auto it: m_nameMap)
	{
		if (m_scriptMap.find(it.first) != m_scriptMap.end())
		{
			m_scriptMap[it.first];
		}

		std::cout << "Loading script " << it.first<< std::endl;
		                                  //use filepath
		int error = luaL_dofile(m_luaState, it.second.c_str());
		if (error) {
			HandleError();
		}
		else {

			LuaRef registerfunc = getGlobal(m_luaState, "register_script");
			if (registerfunc.isFunction()) {
				LuaRef classdef = registerfunc();
				if (classdef.isTable()) {
					m_scriptMap[it.first] = new LuaRef(classdef);
					//return *(m_scriptMap[it.first]);
				}
			}
			else {
				std::cout << "Error loading script " << it.first << " no register_script function" << std::endl;
			}
		}
	}

}

LuaRef ScriptManager::GetScript(string name)
{
	if (m_scriptMap.find(name) != m_scriptMap.end())
	{
			return *(m_scriptMap[name]);
	}
	else
	{
		LOG_WARN_ARG("Script not found\t: {}", name);	
	}
	return nullptr;
}

void ScriptManager::TestMethod() {
	std::cout << "hello";
}

void ScriptManager::HandleError()
{
	std::string str = lua_tostring(m_luaState, lua_gettop(m_luaState));
	lua_pop(m_luaState, 1);

	lua_Debug entry;
	int depth = 0;

	int v;
	while ((v = lua_getstack(m_luaState, depth, &entry)) != 0)
	{
		int status = lua_getinfo(m_luaState, "Sln", &entry);
		assert(status);

		std::cout << entry.short_src << "(" << entry.currentline << ") " << (entry.name ? entry.name : "?") << std::endl;
		depth++;
	}
	std::cout << "Lua Error: " << str.c_str() << std::endl;
}

void ScriptManager::HandleEvent(Event * a_event)
{
	if (a_event->GetEventType() == EEventType::ReloadScripts)
	{
		LoadAllScripts();
	}
}

//lua_State * ScriptManager::GetLuaState(void)
//{
//	return m_luaState;
//}
