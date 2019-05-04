#pragma once

class PrefabProperty
{
public:
  PrefabProperty();
  PrefabProperty(string name_) { name = name_; }
  void draw();


  void ObjectClike();
  void DrawProperty();
  void DrawComponent(std::string name);
  void DeleteObject();

  std::string name;
  std::string Prename;

  GameObject* pGo;
  bool OpenProperty = false;
  std::vector<std::string> componemtNames;

  GUID ID;
private:
  //ObjectClike
  ImGuiWindowFlags ObjectClike_WindowFlags = 0;
  
  //DrawProperty
  
  bool DrawProperty_OpenWindow = true;

 

  //
};
