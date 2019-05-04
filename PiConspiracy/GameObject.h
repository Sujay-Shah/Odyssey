//
//  GameObject.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once




class GameObject
{
public:
	GameObject(size_t a_size);		// added size to set component vec capacity for Memory Manager
	~GameObject();
	void AddComponent(Component* a_component);
	void SetGameObjectType(string a_typeName);
	EGameObjectType GetGameObjectType(void) const;

	template <typename C>
	C* GetComponent(string type) 
	{
		for (auto it : m_activeComponents)
		{
			if (it->GetComponentType() == type)
			{
				return static_cast<C*>(it);
			}
		}
		return nullptr;
	}

	void Update(float a_dt);
	void HandleEvents(Event* a_event);
	void OverrideComponents(const json&, float);

	const string GetGameObjectName(void) const;
	void SetGameObjectName(string a_name);
	// for memory manager use
	inline virtual void SetActiveFlag(bool a_activeFlag) { m_activeFlag = a_activeFlag; }
	inline virtual bool GetActiveFlag(void) { return m_activeFlag; }
	void DeActivate(void);
  vector<Component*> GetComponentsName() {
    return m_activeComponents;
  }
  vector<Component*> m_activeComponents;
private:
	//vector<Component*> m_activeComponents;
	EGameObjectType m_type;
	string m_gameObjectName;
	bool m_isRenderable;
	bool m_activeFlag;				// added for memory manager 
};
