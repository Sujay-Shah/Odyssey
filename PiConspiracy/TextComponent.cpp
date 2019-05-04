//
//  TextComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 12/01/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "TextComponent.h"
#include "TextRenderer.h"
#include "StatsManager.h"
#include "Renderer.h"
#include "RenderableContainer.h"
#include "MemoryManager.h"
#include "Engine.h"
#include "UIManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"

TextComponent::TextComponent() :
Component(STRINGIFY(TextComponent)),
m_strFontName("Assets/TextModel/Optimus.otf"),
m_strShaderName("Assets/Shaders/TextShader"),
m_strText(" "),
m_uFontSize(50),
m_fScale(1.0f),
m_vPosition(vec2(float(PiEngine->GetClientWidth()/ 2) - 250, float(PiEngine->GetClientHeight() / 2) + 300)),
m_vColor(1.0f, 1.0f, 1.0f)
{
}


TextComponent::~TextComponent()
{
}

void TextComponent::Initialize()
{
	m_pTextRenderer = static_cast<TextRenderer*>(PiMemory->GetRenderableFromMemory("TextRenderer"));
	m_pTextRenderer->LoadText(m_strFontName, m_uFontSize);
	m_pTextRenderer->Initialize(m_strShaderName);
}

void TextComponent::Update(float a_dTime)
{
	
	//std::string pickup_disp = "MULTIPLY X" + multiplier_count;

	std::string game_Name = "ODYSSEY";

	//TextComponent * name1 = this;//m_owner->GetComponent<TextComponent>("TextComponent");
	
	
	if (m_owner->GetGameObjectName() == "ScoreText")
	{
		string score = std::to_string((int)PiStats->GetScore());
		m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 550, float(PiEngine->GetClientHeight()) - 100);
		m_pTextRenderer->SetPosition(m_vPosition);
		m_pTextRenderer->SetText(m_strText + score);
		m_pTextRenderer->Update();
		/*if (PiEngine->getIsPlayerDead())
		{
			m_pTextRenderer->SetRenderable(false);
		}
		else
		{
			m_pTextRenderer->SetRenderable(true);
		}*/
	}
	
	if (m_owner->GetGameObjectName() == "DeathHighScore")
	{
		string score = std::to_string(PiResource->GetConfig()->m_HighScore);
		m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 1190, float(PiEngine->GetClientHeight()) - 130);
		m_pTextRenderer->SetPosition(m_vPosition);
		m_pTextRenderer->SetText(m_strText + score);
		m_pTextRenderer->Update();
		if (PiEngine->getIsPlayerDead())
		{
			m_pTextRenderer->SetRenderable(true);
		}
		else
		{
			m_pTextRenderer->SetRenderable(false);
		}
	}
	

	if (m_owner->GetGameObjectName() == "PickUpText")
	{
		string multiplier_count = std::to_string((int)PiStats->GetMultiplier());
		m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 550, float(PiEngine->GetClientHeight()) - 150);
		m_pTextRenderer->SetPosition(m_vPosition);
		m_pTextRenderer->SetText(m_strText + multiplier_count);
		m_pTextRenderer->Update();
		if (PiEngine->getIsPlayerDead())
		{
			m_pTextRenderer->SetRenderable(false);
		}
		else
		{
			m_pTextRenderer->SetRenderable(true);
		}
	}

	if (m_owner->GetGameObjectName() == "deathMenu")
	{
		string multiplier_count = std::to_string((int)PiStats->GetScore());//PiResource->getHighScore()
		m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 1190, float(PiEngine->GetClientHeight()) - 230);
		m_pTextRenderer->SetPosition(m_vPosition);
		m_pTextRenderer->SetText(m_strText + multiplier_count);
		m_pTextRenderer->Update();
		if (PiEngine->getIsPlayerDead())
		{
			m_pTextRenderer->SetRenderable(true);
		}
		else
		{
			m_pTextRenderer->SetRenderable(false);
		}
	}

	if (m_owner->GetGameObjectName() == "UI_videoText")
	{
		m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 1150, float(PiEngine->GetClientHeight()) - 200);
		m_pTextRenderer->SetPosition(m_vPosition);
		m_pTextRenderer->SetText(m_strText);
		m_pTextRenderer->Update();

		Canvas* c = PiUI->GetCurrentActiveCanvas();
		if (c != nullptr && c->m_canvasType == ECanvasType::Options)
		{
			m_pTextRenderer->SetRenderable(true);
		}
		else
		{
			m_pTextRenderer->SetRenderable(false);
		}
	}

	if (m_owner->GetGameObjectName() == "UI_OptionsHeaderText")
	{
		m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 1080, float(PiEngine->GetClientHeight()) - 100);
		m_pTextRenderer->SetPosition(m_vPosition);
		m_pTextRenderer->SetText(m_strText);
		m_pTextRenderer->Update();

		Canvas* c = PiUI->GetCurrentActiveCanvas();
		if (c != nullptr && c->m_canvasType == ECanvasType::Options)
		{
			m_pTextRenderer->SetRenderable(true);
		}
		else
		{
			m_pTextRenderer->SetRenderable(false);
		}

	}
	if (m_owner->GetGameObjectName() == "UI_PauseHeaderText")
	{
		m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 1050, float(PiEngine->GetClientHeight()) - 150);
		m_pTextRenderer->SetPosition(m_vPosition);
		m_pTextRenderer->SetText(m_strText);
		m_pTextRenderer->Update();

		Canvas* c = PiUI->GetCurrentActiveCanvas();
		if (c != nullptr && c->m_canvasType == ECanvasType::PauseMenu)
		{
			m_pTextRenderer->SetRenderable(true);
		}
		else
		{
			m_pTextRenderer->SetRenderable(false);
		}

	}
	if (m_owner->GetGameObjectName() == "UI_audioText")
	{
		m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 1150, float(PiEngine->GetClientHeight()) - 500);
		m_pTextRenderer->SetPosition(m_vPosition);
		m_pTextRenderer->SetText(m_strText);
		m_pTextRenderer->Update();

		Canvas* c = PiUI->GetCurrentActiveCanvas();
		if (c != nullptr && c->m_canvasType == ECanvasType::Options)
		{
			m_pTextRenderer->SetRenderable(true);
		}
		else
		{
			m_pTextRenderer->SetRenderable(false);
		}

	}
	if (m_owner->GetGameObjectName() == "UI_confirmText")
	{
		m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 1170, float(PiEngine->GetClientHeight()) - 300);
		m_pTextRenderer->SetPosition(m_vPosition);
		m_pTextRenderer->SetText(m_strText);
		m_pTextRenderer->Update();

		Canvas* c = PiUI->GetCurrentActiveCanvas();
		if (c != nullptr && c->m_canvasType == ECanvasType::Confirm)
		{
			m_pTextRenderer->SetRenderable(true);
		}
		else
		{
			m_pTextRenderer->SetRenderable(false);
		}

	}
	
	//===================================
	//Text showup when player used sheild
	//===================================
	if (PiGameObject->m_Player != nullptr)
	{
		InputComponent* state_player = PiGameObject->m_Player->GetComponent<InputComponent>(STRINGIFY(InputComponent));
		if (state_player != nullptr && state_player->IsShipState(EShipState::Shield))
		{
			//Respawn text
			string respawn_text = "Respawning...";
			m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 1100, float(PiEngine->GetClientHeight()) - 500);
			m_pTextRenderer->SetPosition(m_vPosition);
			m_pTextRenderer->SetText(respawn_text);
			m_pTextRenderer->Update();

			m_pTextRenderer->SetRenderable(true);
			
		}
	}
	
	
	
	//=================================

	
	/*if (m_owner->GetGameObjectName() == "UI_jumpText")
	{
		string jumpCount = std::to_string((int)PiStats->GetJumpCount());
		m_pTextRenderer->SetText(m_strText + jumpCount);
	}
	if (m_owner->GetGameObjectName() == "UI_boostText")
	{
		string boostCount = std::to_string((int)PiStats->GetPierceCount());
		m_pTextRenderer->SetText(m_strText + boostCount);
	}*/
	//
	//if (m_owner->GetGameObjectName() == "gameNameText")
	//{
	//	name1->m_vPosition = vec2(float((PiEngine->GetClientWidth()/2) - 100), float(PiEngine->GetClientHeight() /4 * 3));
	//	name1->m_pTextRenderer->SetPosition(m_vPosition);
	//	name1->m_pTextRenderer->SetText(game_Name);
	//	name1->m_pTextRenderer->Update();
	//}
	//m_pTextRenderer->SetRenderable(true);
	
	PiRenderer->GetUIContainer()->PushRenderable(m_pTextRenderer);
	//m_pTextRenderer->SetPosition(m_vPosition);
	//m_pTextRenderer->Update();
}

void TextComponent::Serialize(const json &read_value)
{
	

	//============================================================

	m_vPosition.x = ParseFloat(read_value["position"], "x");
	m_vPosition.y = ParseFloat(read_value["position"], "y");
	
	
	m_vColor.x = ParseFloat(read_value["Color"], "x");
	m_vColor.y = ParseFloat(read_value["Color"], "y");
	m_vColor.z = ParseFloat(read_value["Color"], "z");
	
	m_uFontSize = ParseInt(read_value, "FontSize");
	
	m_strText = ParseString(read_value, "StartText");

	m_fScale= ParseFloat(read_value, "Scale");

	//============================================================
	
	//To convert integer to string
	string tolo = m_pTextRenderer->GetText();
	
	//m_vPosition = vec2(float(PiEngine->GetClientWidth()) - 500, float(PiEngine->GetClientHeight()) - 100);

	//TextComponent * name1 = m_owner->GetComponent<TextComponent>("TextComponent");

	//if (m_owner->GetGameObjectName() == "ScoreText")
	//{

	//}
	//
	m_pTextRenderer->SetScale(m_fScale);
	m_pTextRenderer->SetText(m_strText);
	m_pTextRenderer->SetColor(m_vColor);
	m_pTextRenderer->SetPosition(m_vPosition);


}

void TextComponent::HandleEvent(Event * a_pEvent)
{
	
}

void TextComponent::Deactivate(void)
{
	PiMemory->SetRendrableBackInMemory(m_pTextRenderer, STRINGIFY(TextRenderer));
	m_pTextRenderer = nullptr;
}

void TextComponent::OverrideParams(const json &j, float fOffset)
{
	if (ValueExists(j, "Color"))
	{
		m_vColor.x = ValueExists(j["Color"], "x") ? ParseFloat(j["Color"], "x") : m_vColor.x;
		m_vColor.y = ValueExists(j["Color"], "y") ? ParseFloat(j["Color"], "y") : m_vColor.y;
		m_vColor.z = ValueExists(j["Color"], "z") ? ParseFloat(j["Color"], "z") : m_vColor.z;

		m_pTextRenderer->SetColor(m_vColor);
	}

	if (ValueExists(j, "position"))
	{
		m_vPosition.x = ValueExists(j["position"], "x") ? ParseFloat(j["position"], "x") : m_vPosition.x;
		m_vPosition.y = ValueExists(j["position"], "y") ? ParseFloat(j["position"], "y") : m_vPosition.y;

		m_pTextRenderer->SetPosition(m_vPosition);
	}

	if (ValueExists(j, "Scale"))
	{
		m_fScale = ValueExists(j, "Scale") ? ParseFloat(j, "Scale") : m_fScale;

		m_pTextRenderer->SetScale(m_fScale);
	}
	
	if (ValueExists(j, "StartText"))
	{
		m_strText = ValueExists(j, "StartText") ? ParseString(j, "StartText") : m_strText;
		
		m_pTextRenderer->SetText(m_strText);
	}
}
