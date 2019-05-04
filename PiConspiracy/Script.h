//  Script.h
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class Script : public Component
{
	COMPONENT_DECLARE(Script)
public:
	~Script();

	// Inherited via Component
	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;
	virtual void OverrideParams(const json &, float) override;
	void LuaPrint();

	void LoadScript(lua_State* L, const std::string& scriptFilename, const std::string& tableName);
private:
	LuaRef* mLuaClass;
	vector<string> m_scriptName;
	std::unique_ptr<LuaRef> /*LuaRef**/ luaFunc;
	std::unique_ptr<LuaRef> updateFunc;

	void SetLuaClass(LuaRef classRef);

};

