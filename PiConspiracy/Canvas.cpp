//
//  Canvas.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Canvas.h"
#include "Engine.h"
#include "UIManager.h"
#include "GameObjectManager.h"
#include "Mesh.h"

Canvas::Canvas() : Component(STRINGIFY(Canvas)),m_displayCanvas(false)
{
}


Canvas::~Canvas()
{
}

void Canvas::Initialize()
{
	m_trans = this->m_owner->GetComponent<Transform>(STRINGIFY(Transform));
	m_canvasPos = m_trans->GetPosition();
}

void Canvas::Update(float a_dTime)
{
	/*if (!m_isCanvasActive || m_canvasType==ECanvasType::MainMenu)
	{
		return;
	}
	bool pressed;
	ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowSize(ImVec2(900, 600));
	ImGui::SetNextWindowPos(ImVec2(480.0f, 270.0f));
	ImGui::Begin(m_canvasName.c_str(), &pressed, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 0.5f));

	ImGui::PopStyleColor();
	ImGui::End();*/
}

void Canvas::Serialize(const json &j)
{
	m_canvasName = ParseString(j, "canvasName");	
	m_canvasType = (ECanvasType)ParseInt(j, "canvasType");
	PiUI->RegisterCanvas(this);

	string prefabName;
	size_t numberOfElements = j["UIElements"].size();
	vec3 pos;
	for (size_t i = 0; i < numberOfElements; ++i)
	{
		GameObject *pObject;
		prefabName = ParseString(j["UIElements"][i], "prefab");

		//Doesn't matter what bool you pass for UI, cos it will be added in diff UI list anyways
		pObject = PiGameObject->GetObjectFactoryInstance()->CreateGameObject(prefabName, false);

		if (ValueExists(j["UIElements"][i], "position"))
		{
			pos.x = ParseFloat(j["UIElements"][i]["position"], "x");
			pos.y = ParseFloat(j["UIElements"][i]["position"], "y");
			pos.z = ParseFloat(j["UIElements"][i]["position"], "z");
			pObject->GetComponent<Transform>(STRINGIFY(Transform))->SetPosition(pos);

			Button* b = pObject->GetComponent<Button>(STRINGIFY(Button));
			CheckBox* c = pObject->GetComponent<CheckBox>(STRINGIFY(CheckBox));
			if (b || c)
			{
				m_buttonPos.push_back(std::make_pair(pos.x, pos.y));
			}
		}
		m_UIelements.push_back(pObject);	
	}
	//std::sort(v.begin(), v.end(), sort_pair_second<int, int>());
	std::sort(m_buttonPos.begin(), m_buttonPos.end(), [](auto &left, auto &right) {
		return left.second < right.second;
	});
}

void Canvas::HandleEvent(Event * a_pEvent)
{
}

void Canvas::Deactivate(void)
{
	m_owner = nullptr;
	m_activeFlag = false;
}


void Canvas::OverrideParams(const json &,float offset)
{
}

void Canvas::ActivateCanvas(void)
{
	if (m_canvasType != ECanvasType::Confirm)
	{
		PiUI-> m_lastActiveCanvas = m_canvasType;
		PiUI->m_isConfirmCanvasTriggered = false;
	}
	else
	{
		PiUI->m_isConfirmCanvasTriggered = true;
	}
	m_isCanvasActive = true;
	Button* b;
	CheckBox* c;
	m_displayCanvas = true;
	for (auto it : m_UIelements)
	{
		b = it->GetComponent<Button>(STRINGIFY(Button));
		c = it->GetComponent<CheckBox>(STRINGIFY(CheckBox));
		if (b)
		{
			b->m_isButtonVisible = true;
		}
		else if (c)
		{
			c->m_isVisible = true;
		}
		else
		{
			MeshComponent* mc = it->GetComponent<MeshComponent>(STRINGIFY(MeshComponent));
			if (mc)
			{
				Mesh* m = mc->GetMesh();
				m->SetRenderable(true);
			}
		}
	}
}

void Canvas::AddGameObjectToCanvas(GameObject * pGO)
{
}

void Canvas::DeactivateCanvas(void)
{
	m_isCanvasActive = false;
	m_displayCanvas = false;
	Button* b;
	CheckBox* c;
	for (auto it : m_UIelements)
	{
		b = it->GetComponent<Button>(STRINGIFY(Button));
		c = it->GetComponent<CheckBox>(STRINGIFY(CheckBox));
		if (b)
		{
			b->m_isButtonVisible = false;
		}
		else if (c)
		{
			c->m_isVisible = false;
		}
		else
		{
			//dont render that gameObject
			MeshComponent* mc = it->GetComponent<MeshComponent>(STRINGIFY(MeshComponent));
			if (mc)
			{
				Mesh* m = mc->GetMesh();
				m->SetRenderable(false);
			}
		}
	}
}
