//
//  Pickups.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class Pickups : public Component
{
	COMPONENT_DECLARE(Pickups);

public:
	~Pickups();
	

// Inherited via Component
virtual void Initialize() override;
virtual void Update(float a_dTime) override;
virtual void Serialize(const json &) override;
virtual void HandleEvent(Event * a_pEvent) override;
virtual void Deactivate(void) override;
virtual void OverrideParams(const json &, float offset) override;

int GetId(void);

private:
	int32 m_id;
};

