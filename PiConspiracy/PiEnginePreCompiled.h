//
//  PiEnginePreCompiled.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#pragma once
#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

#define FWD_RENDERING 

// External Libraries 
#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtc/quaternion.hpp>
#include "../ImguiLibrary/imgui.h"
#include "../ImguiLibrary/imgui_impl_sdl.h"
#include "../ImguiLibrary/imgui_impl_opengl3.h"

//consist all STL containers and std libs
#include "Type.h"
//Core Engine utils
#include "Hash.h"
#include "NonCopyable.h"

// Engine includes 
#include "Macro.h"
#include "Singleton.h"
#include "ShapeGenerator.h"
#include "GlobalManagers.h"
#include "LogManager.h"
#include "ImguiManager.h"

//Physics/Math
#include "MathDefs.h"
#include "Vector3D.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "physics_math.h"

//contacts and colliders
#include "contacts.h"
#include "BaseCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

//TODO : Move unnecessary includes
#include <nlohmann/json.hpp>
#include "JsonReader.h"
using json = nlohmann::json;
using namespace JsonReader;

//os
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <random>
#include <cstdint>
#include <chrono>
#include <wtypes.h>
#include <fstream>

//TODO: determine best location for lua includes
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <LuaBridge/LuaBridge.h>
using namespace luabridge;

//events
#include "Event.h"

//components
#include "Component.h"
#include "Transform.h"
#include "InputComponent.h"
#include "LightComponent.h"
#include "NPC_SideMovementComponent.h"
#include "MeshComponent.h"
//#include "CameraComponent.h"
#include "RigidBodyComponent.h"
#include "FractureComponent.h"
#include "NPC_DropBallComponent.h"
#include "TumblingCube.h"
#include "Pickups.h"
#include "PlayerFollow.h"
#include "Button.h"
#include "TriggerComponent.h"
#include "TextComponent.h"
#include "NPC_RotateComponent.h"
#include "RiseComponent.h"
#include "Rotations.h"
#include "DropComponent.h"
#include "AIComponent.h"
#include "RotateComponent.h"
#include "CubeFollowComponent.h"
#include "ParticleComponent.h"
#include "ScaleUI.h"
#include "CheckBox.h"

//UI
#include"CanvasType.h"
#include "Canvas.h"
#include "Script.h"
#include "UIController.h"

#include "Tags.h"
//GameObjects
#include "GameObject.h"


//level editor 
//#include "LevelEditor.h"
//#include "PrefabProperty.h"

//utility
#include "RandomNumberGenerator.h"

