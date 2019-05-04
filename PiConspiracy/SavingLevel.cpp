#include "PiEnginePreCompiled.h"
void InputVec3(std::ofstream &file, vec3 v, string name, bool last);
void transform(std::ofstream &file, GameObject* pGo);
void LevelEditor::SaveLevel()
{
  

  char str0[128] = "";
  string filename = "Temporary Name";
  strcpy(str0, filename.c_str());
  ImGui::InputText("Level Name", str0, IM_ARRAYSIZE(str0));
  filename = str0;
  
  //ImGui::SameLine();
  if (ImGui::Button("Save"))
  {
    filename = "Assets/Scenes" + filename + ".json";
    std::ofstream file(filename);
    file << "{ " << std::endl;
    file << "\"GAMEOBJECTS\": [" << std::endl;

    std::vector<PrefabProperty*>::iterator itr_end = PrefabPropertyPool.end();
    for (std::vector<PrefabProperty*>::iterator it = PrefabPropertyPool.begin(); it != itr_end; ++it)
    {
      file << "{ " << std::endl;
      file << "  \"prefab\":  \"" + (*it)->Prename + "\"," << std::endl;

      std::vector<std::string>::iterator itother_end = (*it)->componemtNames.end();
      for (std::vector<std::string>::iterator itother = (*it)->componemtNames.begin(); itother != itother_end; ++itother)
      {
        if ((*itother) == "Transform")
          transform(file, (*it)->pGo);
      }

      // is last one
      if (it + 1 == itr_end)
        file << "} " << std::endl;
      else
        file << "}, " << std::endl;
    }

    file << "] " << std::endl;
    file << "} " << std::endl;
    file.close();
  }

  

 
  //json::value event_;
}

void InputVec3(std::ofstream &file, vec3 v, string name, bool last)
{

  file << "			\""<< name <<"\": {" << std::endl;
  file << "				\"x\": " << std::setprecision(4) << std::to_string(v.x) << "," << std::endl;
  file << "				\"y\": " << std::setprecision(4) << std::to_string(v.y) << "," << std::endl;
  file << "				\"z\": " << std::setprecision(4) << std::to_string(v.z) << std::endl;
  if(last)
   file << "			}" << std::endl;
  else
   file << "			}," << std::endl;
}

void transform(std::ofstream &file, GameObject* pGo)
{
  auto cur_component = pGo->GetComponent<Transform>("Transform");
  InputVec3(file, cur_component->GetPosition(), "position", false);
  InputVec3(file, cur_component->GetEulerAngles(), "angle", false);
  InputVec3(file, cur_component->GetScale(), "scale", true);
}

void rigidbody(std::ofstream &file, GameObject* pGo)
{
  /*auto cur_component = pGo->GetComponent<RigidBodyComponent>("RigidBodyComponent");
  InputVec3(file, cur_component->, "position", false);
  InputVec3(file, cur_component->GetEulerAngles(), "angle", false);
  InputVec3(file, cur_component->GetScale(), "scale", true);*/
}