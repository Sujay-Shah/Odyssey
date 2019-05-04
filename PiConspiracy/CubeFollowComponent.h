//
//  DropComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class CubeFollowComponent : public Component
{
	COMPONENT_DECLARE(CubeFollowComponent);
public:

	~CubeFollowComponent(void);

	CubeFollowComponent(float a_speed);

	void Initialize() override final;
	void Update(float a_dTime) override final;
	void Serialize(const json&) override final;
	void HandleEvent(Event* a_pEvent) override final;

	virtual void Deactivate();

	void SetSpeed(float a_speed);
	float GetSpeed(void);
	// Inherited via Component
	virtual void OverrideParams(const json&, float offset) override;

	bool isStart;
	bool findloc = false;
	bool reached = false;
	bool initialCheck = true;
	bool fromLeft = false;
	bool fromRight = false;
	Transform* playerTransform;
	vec3 playerPos;
private:
	Transform* m_pTrans;
	RigidBodyComponent* m_rigidBody;

	vec3 m_pos;
};

