//
//  Component.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#pragma once


#define COMPONENT_DECLARE(Type) \
public: \
 \
Type(void); \
virtual Component* Create(void) const final { \
return new Type(); } \
private:

//TODO: Make sure if it should stay this way or find a better way
class Event;
class GameObject;

class Component
{
public:
	Component(string a_componentType)
		: m_type(a_componentType),
		m_owner(nullptr),
		m_activeFlag(false)
		{};

	virtual ~Component(void) {};
	
	//TODO: just for now will remove if not needed
	virtual void Initialize() = 0;

	virtual void Update(float a_dTime) = 0;
	virtual void ImguiUpdate(float a_dTime) {};
	virtual void Serialize(const json& ) = 0;
	virtual void HandleEvent(Event* a_pEvent) = 0;
	virtual void Deactivate(void) = 0;
	virtual Component* Create(void) const = 0;
	virtual void OverrideParams(const json&,float)=0;

	inline void SetParent(GameObject* a_obj) { m_owner = a_obj; }
	inline GameObject* GetOwner(void) { return m_owner; }

	inline string GetComponentType(void) { return m_type; }

	// for memory manager use
	inline void SetActiveFlag(bool a_activeFlag) { m_activeFlag = a_activeFlag; }
	inline bool GetActiveFlag(void) { return m_activeFlag; }
	
protected:
	GameObject* m_owner;		// Owner of the compoenent
	string m_type;
	bool m_activeFlag;
};