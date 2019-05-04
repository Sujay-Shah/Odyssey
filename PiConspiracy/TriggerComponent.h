//
//  TriggerComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class TriggerComponent : public Component
{
	COMPONENT_DECLARE(TriggerComponent);
public:
	~TriggerComponent(void);

	void Initialize() override final;
	void Update(float a_dTime) override final;
	void Serialize(const json& a_j) override final;
	void HandleEvent(Event* a_pEvent) override final;
	void Deactivate(void) override final;
	void OverrideParams(const json& j,float offset) override final;

	inline int GetID();

private:
	int m_id;
};


inline int TriggerComponent::GetID()
{
	return m_id;
}
