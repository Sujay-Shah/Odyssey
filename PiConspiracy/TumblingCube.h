//
//  TumblingCube.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class RigidBodyComponent;

class TumblingCube : public Component
{
	COMPONENT_DECLARE(TumblingCube);
public:
	~TumblingCube();

	void Initialize() override final;
	void Update(float a_dTime) override final; // the update function for transform
	//void LateUpdate(float a_dTime) override final;
	void Serialize(const json& a_j) override final;
	void HandleEvent(Event* a_pEvent) override final;
	void Deactivate(void) override final;
	// Inherited via Component
	virtual void OverrideParams(const json&,float) override;

private:
	GameObject* m_player;
	Transform* m_pTrans;
	RigidBodyComponent* m_pRBodyComp;
	Transform* m_playerTrans;
	RigidBodyComponent* m_playerRBComp;

	int m_counter;

	float m_turn;
};

