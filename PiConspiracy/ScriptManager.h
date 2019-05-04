//
//  ScriptManager.h
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class ScriptManager : public Singleton<ScriptManager>
{
public:
	ScriptManager();
	~ScriptManager();

	void Initialize(void);
	void Update(float a_dt);
	//lua_State* GetLuaState(void);

	static lua_State* m_luaState;
	void clearAllScripts();
	void LoadAllScripts();
	LuaRef GetScript(string name);
	void TestMethod();
	void HandleError();
	void HandleEvent(Event* a_event);
private:
	std::unordered_map<std::string, luabridge::LuaRef*> m_scriptMap;
	unordered_map<string, string> m_nameMap;
	void RegisterClasses();
};

