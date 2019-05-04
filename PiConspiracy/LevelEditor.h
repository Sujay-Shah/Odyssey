#pragma once


class PrefabProperty;
class LEArchetypesLoader;
class LightComponent;
class NPC_SideMovementComponent;

extern PrefabProperty* CurObject;

class LevelEditor : public Singleton<LevelEditor>
{

public:
  void Initialize(SDL_Window* window, SDL_GLContext* gl_context);
  void Update(SDL_Window* window, SDL_GLContext * context);
  void Destroy();




  void draw();
  void LoadShaders();
  void LoadArchetypes();
  void PrefabList();
  void CurrentObects();
  void PropertyWindow(PrefabProperty *cur);

  void system();
  void SaveLevel();


  void ClearAll();
  void Load();


  void About();
private:
  //LoadArchetypes
  std::string path = "Assets/Archetypes";
  std::string Levelpath = "Assets/Scenes";
  std::string shaderpath = "Assets/Shaders";
  std::vector<string> ArchetypesDir;  // should be a map<name, info>
  std::vector<string> LevelDir;
  std::vector<string> ShaderpesDir;
  //PrefabList
  bool PrefabList_OpenWindow = true;
  ImGuiWindowFlags PrefabList_WindowFlags = 0;
  ImGuiWindowFlags menubar = 0;

  std::vector<PrefabProperty*> PrefabPropertyPool;

  //save and load
  bool LoadWindow = false;

  //other
  bool AboutWindow = false;
  Camera* camobj;
};


/*
class ObjectList
{
  
};*/


/*

read all filename in vector<> Archetypes



*/

class LEArchetypesLoader
{
public:
  std::string m_dir;
  
  void load();
  void render();


private:
  bool loaded = false;
  

  //components
  Transform *mp_Transform = NULL;
  RigidBodyComponent *mp_RigidBody = NULL;
  FractureComponent *mp_FractureComponent = NULL;
  InputComponent *mp_InputComponent = NULL;
  LightComponent *mp_LightComponent = NULL;
  NPC_SideMovementComponent *mp_NPC_SideMovementComponent = NULL;


  void Serialize(json &j);

};

void LEArchetypesLoader::load()
{
  string path;
  json j;

  //mp_RigidBody->Serialize(j);
}

void LEArchetypesLoader::Serialize(json &j)
{

}
void LEArchetypesLoader::render() {}