//  Script.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Script.h"
#include "ResourceManager.h"
#include "ScriptManager.h"

Script::Script():Component(STRINGIFY(Script)),luaFunc(nullptr)
{
	//std::cout << "e";
}


Script::~Script()
{
}

void Script::Initialize()
{
	//LoadScript(PiScript->m_luaState, "Assets/Scripts/print.lua", "script");
}

void Script::Update(float a_dTime)
{

		if (mLuaClass == NULL || mLuaClass->isNil())
			return;
	
		LuaRef tickFunc = (*mLuaClass)["tick"];


		if (tickFunc.isFunction()) {
			tickFunc((*mLuaClass), a_dTime);
		}
		else {
			std::cout << " No function found : " << tickFunc.type() << std::endl;
		}
}



void Script::Serialize(const json &j)
{
	int64 size = j["SCRIPTS"].size();
	string name;
	for (int i = 0; i < size; ++i)
	{
		name = ParseString(j["SCRIPTS"][i], "scriptName");
		m_scriptName.push_back(name);
	}

	LuaRef scriptRef = PiScript->GetScript(name);
	Transform* ptrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));

	SetLuaClass(scriptRef(ptrans));
}

void Script::HandleEvent(Event * a_pEvent)
{
	if (a_pEvent->GetEventType() == EEventType::ReloadScripts)
	{
		mLuaClass = NULL;
		LuaRef scriptRef = PiScript->GetScript(m_scriptName[0]);
		Transform* ptrans = m_owner->GetComponent<Transform>(STRINGIFY(Transform));

		SetLuaClass(scriptRef(ptrans));
	}
}

void Script::Deactivate(void)
{
}

void Script::OverrideParams(const json &,float)
{
}
void Script::LoadScript(lua_State * L, const std::string & scriptFilename, const std::string & tableName)
{
}

void Script::SetLuaClass(LuaRef classRef)
{
	/**
	* tell the gameobhect which class to run in Lua
	*/
	
		assert(mLuaClass == NULL); //make sure we're not leaking a class structure
		assert(classRef.isTable()); //make sure we're not getting a duff class.

		mLuaClass = new LuaRef(classRef);
	
}

void Script::LuaPrint()
{
	cout << "Printing" << std::endl;
	/*if (luaFunc)
	{
		try {
			(*luaFunc)();
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "LuaException: " << e.what() << std::endl;
		}
	}/**/
}



