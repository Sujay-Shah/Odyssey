#include "PiEnginePreCompiled.h"
#include "GameObjectManager.h"
#include "ObjectFactory.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Renderer.h"
#include <Objbase.h>

PrefabProperty* CurObject;


void LevelEditor::Destroy() { ClearAll(); }
void LevelEditor::Initialize(SDL_Window* window, SDL_GLContext* gl_context)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 330");

  //Preferred Style of ImGui
  ImGui::StyleColorsDark();


  // class setting
  menubar |= ImGuiWindowFlags_MenuBar;
  LoadArchetypes();
  LoadShaders();
  ArchetypesDir.reserve(50);
  ShaderpesDir.reserve(50);
  PrefabPropertyPool.reserve(300);
  
  CurObject = nullptr;
}


void LevelEditor::Update(SDL_Window* window, SDL_GLContext * context)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();

  draw();

  ImGui::Render();
  //SDL_GL_MakeCurrent(window, context);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}


void LevelEditor::draw()
{
  PrefabList();
  CurrentObects();
  /*std::vector<PrefabProperty*>::iterator itr_end = PrefabPropertyPool.end();
  for (std::vector<PrefabProperty *>::iterator it = PrefabPropertyPool.begin(); it != itr_end; ++it)
  {
    (*it)->draw();
  }*/

  PropertyWindow(CurObject);
  system();
  if (LoadWindow)
    Load();
  if (AboutWindow)
    About();
}

inline void LevelEditor::LoadArchetypes()
{
  // load all Archetypes
  //for (auto& p : std::filesystem::directory_iterator(path))
  //{
  //  string temp = std::filesystem::path(p).filename().string();
  //  temp = temp.substr(0, temp.size() - 5);
  //  ArchetypesDir.push_back(temp);
  //}

}	

inline void LevelEditor::LoadShaders()
{
  // load all Archetypes
  //for (auto& p : std::filesystem::directory_iterator(shaderpath))
  //{
  //  string temp = std::filesystem::path(p).filename().string();
  //  temp = temp.substr(0, temp.size() - 3);
  //  ShaderpesDir.push_back(temp);
  //}
  //sort(ShaderpesDir.begin(), ShaderpesDir.end());
  //ShaderpesDir.erase(unique(ShaderpesDir.begin(), ShaderpesDir.end()), ShaderpesDir.end());

  

  for (int i = 0; i < ShaderpesDir.size(); i++)
  {
    //char *temp = ShaderpesDir[i].c_str();
    //shadername[i] = ShaderpesDir[i].c_str();
    //strcpy(shadername, ShaderpesDir[i].c_str());
  }
  int a = 0;
}

void LevelEditor::PrefabList()
{
  ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
  if (PrefabList_OpenWindow)
  {
    //if (!ImGui::Begin("Prefab list", &PrefabList_OpenWindow, PrefabList_WindowFlags))
    if (!ImGui::Begin("Prefab list", 0, PrefabList_WindowFlags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
    }
    else
    {

      if (ImGui::TreeNode("Prefab list"))
      {
        ImGui::SetNextWindowContentSize(ImVec2(1500.0f, 0.0f));
        ImGui::BeginChild("##ScrollingRegion", ImVec2(0, ImGui::GetFontSize() * 20), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Columns(1);
        int ITEMS_COUNT = (int)ArchetypesDir.size(); // count of row
        ImGuiListClipper clipper(ITEMS_COUNT);  // Also demonstrate using the clipper for large list
        int index = 0;
        while (clipper.Step())
        {
          for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
          {
            if (ImGui::Selectable(ArchetypesDir[i].c_str()))
            {
              //GameObject* pGo = PiGameObject->GetObjectFactoryInstance()->CreateGameObject("physicsTestCube2");
              GameObject* pGo = PiGameObject->GetObjectFactoryInstance()->CreateGameObject(ArchetypesDir[i]);
              PrefabProperty *object = new PrefabProperty();

              std::vector<Component*>::iterator itr_end =  pGo->m_activeComponents.end();
              for (std::vector<Component*>::iterator it =  pGo->m_activeComponents.begin(); it != itr_end; ++it)
              {
                object->componemtNames.push_back((*it)->GetComponentType());
              }
              
              object->name = ArchetypesDir[i];
              object->Prename = ArchetypesDir[i];
              object->pGo = pGo;
              object->OpenProperty = true;
              CoCreateGuid(&object->ID);
              PrefabPropertyPool.push_back(object);

              CurObject = object;

            }
            index++;
          }

        }
        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::TreePop();
      }

      ImGui::End();
    }
  }
}

void LevelEditor::CurrentObects()
{
  ImGui::Begin("Game Objects");
  if (ImGui::TreeNode("Game Objects list"))
  {
    ImGui::SetNextWindowContentSize(ImVec2(1500.0f, 0.0f));
    ImGui::BeginChild("##ScrollingRegion", ImVec2(0, ImGui::GetFontSize() * 20), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Columns(1);
    int ITEMS_COUNT = (int)PrefabPropertyPool.size(); // count of row
    ImGuiListClipper clipper(ITEMS_COUNT);  // Also demonstrate using the clipper for large list
    while (clipper.Step())
    {
      for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
      {
        // todo all Prefab
        char label[100];
        sprintf(label, PrefabPropertyPool[i]->name.c_str());
        if (ImGui::Selectable(label))
        {
          //todo move camera
          CurObject = PrefabPropertyPool[i];
        }

        ImGui::NextColumn();
      }

    }
    ImGui::Columns(1);
    ImGui::EndChild();
    ImGui::TreePop();
  }
  ImGui::End();
}

void LevelEditor::PropertyWindow(PrefabProperty *cur)
{
  ImGui::Begin("Property");
  if (cur != nullptr)
  {
    char str0[128] = "";
    strcpy(str0, cur->name.c_str());
    ImGui::InputText("Name", str0, IM_ARRAYSIZE(str0));

    cur->name = str0;

    std::vector<std::string>::iterator itr_end = cur->componemtNames.end();
    for (std::vector<std::string>::iterator it = cur->componemtNames.begin(); it != itr_end; ++it)
    {
      if (ImGui::TreeNode((*it).c_str()))
      {
        
        cur->DrawComponent((*it));

        ImGui::TreePop();
      }
    }


    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
    if (ImGui::Button("Delete This Object"))
    {
      PrefabPropertyPool.erase(std::remove(PrefabPropertyPool.begin(), PrefabPropertyPool.end(), CurObject), PrefabPropertyPool.end());
      CurObject->DeleteObject();
      auto temp = CurObject;
      delete temp;
      CurObject = nullptr;
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();
  }

 

  

  ImGui::End();
}

void LevelEditor::ClearAll()
{
  std::vector<PrefabProperty*>::iterator itr_end = PrefabPropertyPool.end();
  for (std::vector<PrefabProperty*>::iterator it = PrefabPropertyPool.begin(); it != itr_end; ++it)
  {
    (*it)->componemtNames.shrink_to_fit();
    PiGameObject->DeActivateGameObject((*it)->pGo);
    delete (*it);
  }
  PrefabPropertyPool.clear();
  CurObject = nullptr;

  
}


void LevelEditor::system()
{
  ImGui::Begin("System", 0, menubar);
  //ImGui::SameLine();
  SaveLevel();
  ImGui::Separator();

  ImGui::Text("Total objects: %i", PrefabPropertyPool.size());
  ImGui::Separator();


  ImGui::Text("Camera Position");
  vec3 camera_pos = PiRenderer->GetCamera(true)->GetPosition();
  float vec3a3[3] = { camera_pos.x, camera_pos.y, camera_pos.z };
  ImGui::InputFloat3("", vec3a3)
    
    ;
  // Menu
  /*if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Menu"))
    {
      if (ImGui::MenuItem("New")) {}
      if (ImGui::MenuItem("Save")) {}
      if (ImGui::MenuItem("Load")) 
      {
        ShaderpesDir.clear();
        for (auto& p : std::filesystem::directory_iterator(Levelpath))
        {
          string temp = std::filesystem::path(p).filename().string();
          temp = temp.substr(0, temp.size() - 5);
          ShaderpesDir.push_back(temp);
        }
        LoadWindow = true; 
      }

      ImGui::Separator();
      if (ImGui::BeginMenu("Advance"))
      {
        if (ImGui::MenuItem("Clear All Objects")) 
        {
          ClearAll();
        }
        ImGui::EndMenu();
      }
      ImGui::Separator();
      if (ImGui::MenuItem("About")) { AboutWindow = true; }
      
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }*/
  

  ImGui::End();

  
}


void LevelEditor::Load()
{
  if (LoadWindow)
  {
    ImGui::Begin("Load File", &LoadWindow);

    ImGui::Columns(3, "mycolumns3", false);  // 3-ways, no border
    ImGui::Separator();
    for (int n = 0; n < ShaderpesDir.size(); n++)
    {
      char label[100];
      sprintf(label, ShaderpesDir[n].c_str());
      if (ImGui::Selectable(label))
      {
        //ShaderpesDir[n] + ".json";
      }
      ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Separator();

    ImGui::Text(" ");
    ImGui::Text(" ");
    ImGui::Text("Click to open level");
    ImGui::End();
  }
  
}

void LevelEditor::About()
{
  if (AboutWindow)
  {
    ImGui::Begin("About", &AboutWindow);

    ImGui::Text("    Orange Level Editor");
    ImGui::Text(" ");
    ImGui::Text("     Version: 0.5 Beta");
    ImGui::Text(" ");
    ImGui::Text("    Author: Jiyun Ruan");
    ImGui::End();
  }
}


