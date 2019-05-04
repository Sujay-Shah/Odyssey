//
//  FractureComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class RigidBodyComponent;
class Contact;
class FractureComponent : public Component
{
  COMPONENT_DECLARE(FractureComponent);
public:
  //physics::Contact* mp_Contact;
  RigidBodyComponent* mp_Target;
  bool m_Hit = false;
  uint32 cube_count = 8;
public:


  void Initialize() override final;
  void Update(float a_dTime) override final; // the update function
  void Serialize(const json&) override final;
  void HandleEvent(Event* a_pEvent) override final;
  void Deactivate(void) override final;

  void SetFractureHit(bool val) { m_Hit = val; }
  void SetFractureTarget(RigidBodyComponent* val) { mp_Target = val; }

  bool m_CanBeBroken_local = false;
  bool m_Breaker_local = false;
  int cur_type = 0; // 0 is box 1 is sphere
private:
  void _DivideBlock(const Contact& contact);

  void SettingSCubes(RigidBodyComponent &CommonSetting, RigidBodyComponent * smallerCube);

  // Inherited via Component
  virtual void OverrideParams(const json &,float offset) override;
  RigidBodyComponent* blocks[8];
};