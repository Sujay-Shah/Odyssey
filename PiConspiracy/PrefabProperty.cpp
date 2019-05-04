#include "PiEnginePreCompiled.h"
#include "GameObjectManager.h"
#include "ObjectFactory.h"
#include "ImguiManager.h"

PrefabProperty::PrefabProperty()
{
  //ObjectClike_WindowFlags |= ImGuiWindowFlags_NoTitleBar;
  ObjectClike_WindowFlags |= ImGuiWindowFlags_NoMove;
  //ObjectClike_WindowFlags |= ImGuiWindowFlags_NoBackground;
  ObjectClike_WindowFlags |= ImGuiWindowFlags_NoResize;
  //ObjectClike_WindowFlags |= ImGuiWindowFlags_MenuBar;
}

void PrefabProperty::draw()
{
  //ImGui::Text(name.c_str());
  //ObjectClike();
  if (OpenProperty)
  {
    DrawProperty();
  }
}

void PrefabProperty::ObjectClike()
{
  ImGui::Begin("Click Point", NULL, ObjectClike_WindowFlags);
  /*ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImVec2 center(0, 0);
  ImVec4 col = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
  const ImU32 col32 = ImColor(col);
  const ImVec2 p = ImGui::GetCursorScreenPos();
  float x = p.x + 4.0f, y = p.y + 4.0f, spacing = 8.0f; float sz = 36.0f;
  draw_list->AddCircleFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz*0.5f, col32);*/
  if (ImGui::Button("*"))
  {
    // show Property
    OpenProperty = true;
  }
  ImGui::End();
}

void PrefabProperty::DrawProperty()
{
  /*if (DrawProperty_OpenWindow)
  {
    ImGui::SetNextWindowPos(ImVec2(250, 120), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 380), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Property", &DrawProperty_OpenWindow))
    {
      // Early out if the window is collapsed, as an optimization.
      OpenProperty = false;
      ImGui::End();
    }
    else
    {
      ImGui::Text(name.c_str());
      for (int i = 0; i < componemtNamesSize; i++)
      {
        if (ImGui::TreeNode(componemtNames[i].c_str()))
        {
          //position
          static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
          ImGui::InputFloat3("Position", vec4a);

          ImGui::TreePop();
        }
      }

      ImGui::End();
    }
  }*/


}

void SetFloatToVec3(vec3 &temp, float *vec3a)
{
  temp.x = vec3a[0];
  temp.y = vec3a[1];
  temp.z = vec3a[2];
}

void PrefabProperty::DrawComponent(std::string name)
{
  if (name == "Transform")
  {
    auto cur_component = pGo->GetComponent<Transform>("Transform");
    // Position;
    vec3 temp = cur_component->GetPosition();
    float vec3a[3] = { temp.x, temp.y, temp.z };
    ImGui::InputFloat3("Position", vec3a);
    cur_component->SetPosition(vec3(vec3a[0], vec3a[1], vec3a[2]));

    //rotation
    vec3 temp2 = cur_component->GetEulerAngles();
    float vec3a2[3] = { temp2.x, temp2.y, temp2.z };
    ImGui::InputFloat3("Euler Angles", vec3a2);
    cur_component->SetEulerAngles(vec3(vec3a2[0], vec3a2[1], vec3a2[2]));

    //scale
    vec3 temp3 = cur_component->GetScale();
    float vec3a3[3] = { temp3.x, temp3.y, temp3.z };
    ImGui::InputFloat3("scale", vec3a3);
    cur_component->SetScale(vec3(vec3a3[0], vec3a3[1], vec3a3[2]));
  }
  else if (name == "MeshComponent")
  {
    auto cur_component = pGo->GetComponent<MeshComponent>("MeshComponent");
    // type
    int type = (int)cur_component->GetType();
    ImGui::InputInt("Type", &type);
    cur_component->SetType(type);

    //shader
    const char* items[] = { "Basic", "Cube", "Light", "SkyBox", "Sphere" };
    auto cur_ShaderName = cur_component->GetShaderName();
    int item_current = 0;
    for (int i = 0; i < 5; i++)
    {
      if (strcmp(items[i], cur_ShaderName) == 0)
        item_current = i;
    }
    ImGui::Combo("Shader", &item_current, items, IM_ARRAYSIZE(items));
  }
  else if (name == "RigidBodyComponent")
  {
    auto cur_component = pGo->GetComponent<RigidBodyComponent>("RigidBodyComponent");
    auto RigidBodyType = cur_component->cur_type;
    if (RigidBodyType == RigidBodyComponent::RigidBodyType::BoxRigidBody)
    {
      auto BoxPointer = cur_component->GetBoxPointer();
      float mass_ = BoxPointer->body->getMass();
      vec3 position_ = BoxPointer->body->getPosition().toVec3(),
             velocity_ = BoxPointer->body->getVelocity().toVec3(),
          Acceleration_ = BoxPointer->body->getAcceleration().toVec3(),
             halfSize_ = BoxPointer->halfSize.toVec3();
      glm::quat rotation_ = BoxPointer->body->getOrientation().toQuat();
      float LinearDamping = BoxPointer->body->getLinearDamping(), AngularDamping = BoxPointer->body->getAngularDamping();
      bool ActiveGravity_ = cur_component->ActiveGravity, BodyStatic_ = BoxPointer->body->bodytype;
      int CollisionLayer = BoxPointer->body->CollisionLayer;

      //mass
      ImGui::InputFloat("Mass", &mass_);

      //position
      /*static float vec3Ria[3] = { position_.x, position_.y, position_.z };
      ImGui::InputFloat3("Position", vec3Ria);*/

      //rotation_
      /*static float vec3Rie[4] = { rotation_.x, rotation_.y, rotation_.z, rotation_.w };
      ImGui::InputFloat4("Position", vec3Rie);*/

      //velocity
      float vec3Rib[3] = { velocity_.x, velocity_.y, velocity_.z };
      ImGui::InputFloat3("Velocity", vec3Rib);

      //Acceleration
      float vec3Ric[3] = { Acceleration_.x, Acceleration_.y, Acceleration_.z };
      ImGui::InputFloat3("Acceleration", vec3Ric);

      //halfSize
     /* static float vec3Rid[3] = { halfSize_.x, halfSize_.y, halfSize_.z };
      ImGui::InputFloat3("Position", vec3Rid);*/

      //Damping
      ImGui::InputFloat("Linear Damping", &LinearDamping);
      ImGui::InputFloat("Angular Damping", &AngularDamping);

      //CollisionLayer
      ImGui::InputInt("Collision Layer", &CollisionLayer);

      ImGui::Checkbox("Active Gravity", &ActiveGravity_);
      ImGui::Checkbox("Static", &BodyStatic_);

      BoxPointer->SetState(mass_, position_, vec3(vec3Rib[0], vec3Rib[1], vec3Rib[2]), rotation_,
        vec3(vec3Ric[0], vec3Ric[1], vec3Ric[2]), halfSize_, LinearDamping, AngularDamping, ActiveGravity_, BodyStatic_
        , CollisionLayer, BoxPointer->body->mp_com);
    }
    else if (RigidBodyType == RigidBodyComponent::RigidBodyType::SphereRigidBody)
    {
      auto SpherePointer = cur_component->GetSpherePointer();
      float mass_ = SpherePointer->body->getMass();
      vec3 position_ = SpherePointer->body->getPosition().toVec3(),
        velocity_ = SpherePointer->body->getVelocity().toVec3(),
        Acceleration_ = SpherePointer->body->getAcceleration().toVec3();
       // halfSize_ = SpherePointer->halfSize.toVec3();
      float LinearDamping = SpherePointer->body->getLinearDamping(), AngularDamping = SpherePointer->body->getAngularDamping();
      bool ActiveGravity_ = cur_component->ActiveGravity, BodyStatic_ = SpherePointer->body->bodytype;
      int CollisionLayer = SpherePointer->body->CollisionLayer;

      //mass
      ImGui::InputFloat("Mass", &mass_);
      //velocity
      float vec3RiSb[3] = { velocity_.x, velocity_.y, velocity_.z };
      ImGui::InputFloat3("Velocity", vec3RiSb);
      //Acceleration
      float vec3RiSc[3] = { Acceleration_.x, Acceleration_.y, Acceleration_.z };
      ImGui::InputFloat3("Acceleration", vec3RiSc);
      //Damping
      ImGui::InputFloat("Linear Damping", &LinearDamping);
      ImGui::InputFloat("Angular Damping", &AngularDamping);

      //CollisionLayer
      ImGui::InputInt("Collision Layer", &CollisionLayer);

      ImGui::Checkbox("Active Gravity", &ActiveGravity_);
      ImGui::Checkbox("Static", &BodyStatic_);

      SpherePointer->setState(mass_, SpherePointer->radius, position_, vec3(vec3RiSb[0], vec3RiSb[1], vec3RiSb[2]),
        vec3(vec3RiSc[0], vec3RiSc[1], vec3RiSc[2]), LinearDamping, AngularDamping, ActiveGravity_, BodyStatic_
        , CollisionLayer, SpherePointer->body->mp_com);
    }
    else if (RigidBodyType == RigidBodyComponent::RigidBodyType::PlaneRigidBody)
    {
      auto PlanePointer = cur_component->GetPlanePointer();
      float offset_ = PlanePointer->offset;
      vec3 direction_ = PlanePointer->direction.toVec3();

      float vec3RiPa[3] = { direction_.x, direction_.y, direction_.z };
      ImGui::InputFloat3("Direction", vec3RiPa);
      ImGui::InputFloat("Offset", &offset_);
      PlanePointer->setState(vec3(direction_.x, direction_.y, direction_.z), offset_);
    }
  }
}

void PrefabProperty::DeleteObject()
{
  componemtNames.shrink_to_fit();
  PiGameObject->DeActivateGameObject(CurObject->pGo);
}