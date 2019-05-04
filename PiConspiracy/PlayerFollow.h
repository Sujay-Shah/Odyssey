//
//  PlayerFollow.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class Camera;
class PlayerFollow : public Component
{
	COMPONENT_DECLARE(PlayerFollow);

public:
	~PlayerFollow();

	// Inherited via Component
	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;
	virtual void OverrideParams(const json &, float offset) override;

	virtual void ImguiUpdate(float a_dTime) override;

private:
	Camera* m_cam;
	Transform* m_trans,*m_playerTrans;
	vec3 m_offset;
	bool m_isCam;
};

