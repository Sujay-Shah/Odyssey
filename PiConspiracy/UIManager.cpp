//
//  UIManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "UIManager.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"
#include "DirectoryObject.h"
#include "Engine.h"
#include "FrameRateController.h"
#include "StatsManager.h"


UIManager::UIManager()
{
}


UIManager::~UIManager()
{
	/*for (auto it : m_UIgameObj)
	{
		delete it;
	}*/
	//DeActivateUIGOs();
	//m_UIgameObj.clear();
}

void UIManager::ActivateCanvas(ECanvasType c)
{
	if (c != ECanvasType::Confirm)
	{
		m_lastActiveCanvas = c;
		m_isConfirmCanvasTriggered = false;
	}
	else
	{
		m_isConfirmCanvasTriggered = false;
	}

	if (m_Canvas[(int)c] != nullptr)
		m_Canvas[(int)c]->ActivateCanvas();
	else
		std::cout << "no canvas to activate\n";
}

void UIManager::DeactivateCanvas(ECanvasType c)
{
	if (m_Canvas[(int)c] != nullptr)
	{
		
		m_Canvas[(int)c]->DeactivateCanvas();
	}
	else
		std::cout << "no canvas to de-activate\n";
}

void UIManager::RegisterCanvas(Canvas* c)
{
	//std::cout << "Canvas #  "<<(int)c->m_canvasType << std::endl;
	m_Canvas[(int)c->m_canvasType] = c;
}

bool UIManager::IsCanvasAvailable(const ECanvasType c) {
	return m_Canvas[(int)c];
}

void UIManager::DeregisterCanvas(Canvas* c)
{
	m_Canvas[(int)c->m_canvasType] = nullptr;
}

void UIManager::DeActivateAllCanvas()
{
	for (int i = 0; i < ECanvasType::Num; ++i)
	{
		DeactivateCanvas((ECanvasType)i);
	}
}

void UIManager::HandleEvent(Event * pEvent)
{
	if (pEvent->GetEventType() == EEventType::OnPause)
	{
		Canvas* c=GetCurrentActiveCanvas();
		if (c || PiEngine->m_isMinimized)
		{
			return;
		}
		else
		{
			m_Canvas[(int)CanvasType::PauseMenu]->ActivateCanvas();
		}
	}
	else if (pEvent->GetEventType() == EEventType::OnResume) {
		DeActivateAllCanvas();
	}
	else if (pEvent->GetEventType() == EEventType::OnRestart) {
		DeActivateAllCanvas();
		PiEngine->setPanCamera(true);
		PiEngine->setIsPlayerDead(false);
		/*for (auto obj: m_UIgameObj)
		{
			if ()
			{

			}
		}*/
	}
	else if (pEvent->GetEventType() == EEventType::OnCurrentRestart) {
		DeActivateAllCanvas();
	}
	else if (pEvent->GetEventType() == EEventType::ReloadPrefabs) {
		//TODO: remove hardcoded
		m_UIgameObj.clear();
		Initialize("MenuCollection.json");
	}
	else if (pEvent->GetEventType() == EEventType::OnDeathCamera)
	{	
		
		PiEngine->setPanCamera(false);
		PiEngine->setIsPlayerDead(true);
		m_Canvas[(int)CanvasType::DeathMenu]->ActivateCanvas();
	}
	else if (pEvent->GetEventType() == EEventType::OnStart)
	{
		DeActivateAllCanvas();
	}
	else if (pEvent->GetEventType() == EEventType::OnControlsPressed)
	{
		//DeActivateAllCanvas();
		Canvas* c1 = GetCurrentActiveCanvas();
		c1->DeactivateCanvas();
		m_Canvas[(int)CanvasType::Instructions]->ActivateCanvas();
		//Canvas* c = GetCurrentActiveCanvas();
		//std::cout << "dummy";
	}
	else if (pEvent->GetEventType() == EEventType::OnCreditsPressed)
	{
		//DeActivateAllCanvas();
		Canvas* c1 = GetCurrentActiveCanvas();
		c1->DeactivateCanvas();
		m_Canvas[(int)CanvasType::Credits]->ActivateCanvas();
		//Canvas* c = GetCurrentActiveCanvas();
		//std::cout << "dummy";
	}
	else if (pEvent->GetEventType() == EEventType::OnOptionsPressed)
	{
		//DeActivateAllCanvas();
		Canvas* c1 = GetCurrentActiveCanvas();
		c1->DeactivateCanvas();
		m_Canvas[(int)CanvasType::Options]->ActivateCanvas();
		//Canvas* c = GetCurrentActiveCanvas();
		//std::cout << "dummy";
	}
	else if (pEvent->GetEventType() == EEventType::OnDestrcutiveAction)
	{
		DeActivateAllCanvas();
		m_Canvas[(int)CanvasType::Confirm]->ActivateCanvas();
	}
	else if (pEvent->GetEventType() == EEventType::GameStart)
	{
		ActivateCanvas(ECanvasType::MainMenu);
	}
	else if (pEvent->GetEventType() == EEventType::BackEvent)
	{
		BackEvent* b = static_cast<BackEvent*>(pEvent);
		m_Canvas[(int)b->m_changeTo]->ActivateCanvas();
		m_Canvas[(int)b->m_changeFrom]->DeactivateCanvas();
	}
	else if (pEvent->GetEventType() == EEventType::Exit2Main)
	{
		DeActivateAllCanvas();
		ActivateCanvas(ECanvasType::MainMenu);
	}
	//else if (pEvent->GetEventType() == EEventType::Minimized)
	//{
	//	//DeActivateAllCanvas();
	//	Canvas* c1 = GetCurrentActiveCanvas();
	//	c1->DeactivateCanvas();
	//	m_Canvas[(int)CanvasType::Credits]->ActivateCanvas();
	//	//Canvas* c = GetCurrentActiveCanvas();
	//	//std::cout << "dummy";
	//}
	else 
	{
		for (size_t i = 0; i < m_UIgameObj.size(); ++i)
		{
			m_UIgameObj[i]->HandleEvents(pEvent);
		}
	}
	
}

void UIManager::AddGameObjectToCanvas(ECanvasType type, GameObject* _pGO)
{
	m_Canvas[(int)type]->AddGameObjectToCanvas(_pGO);
	//_pGO->SetParent(m_Canvas[(int)type]->pGO);
}

void UIManager::Initialize(string a_fileName)
{
	//pre add all UI menus
	const DirectoryObject* menu = PiResource->GetFilePath(a_fileName);
	m_menuJson = OpenJsonFile(menu->m_path);

	string menu_name;
	size_t num = m_menuJson["MENUS"].size();
	for (size_t i = 0; i < num; ++i)
	{
		menu_name = ParseString(m_menuJson["MENUS"][i],"prefab");
		 ObjectFactory *objFactory = PiGameObject->GetObjectFactoryInstance();
		 GameObject* pGO = objFactory->CreateGameObject(menu_name, false);
		 
		 pGO->OverrideComponents(m_menuJson["MENUS"][i], 0);
	}
	//ActivateCanvas(ECanvasType::MainMenu);
	m_currentDestructiveEvent = EEventType::NUM;
}

void UIManager::AddUIGameObjects(GameObject *a_pGO)
{
	m_UIgameObj.push_back(a_pGO);
	
	if (a_pGO->GetGameObjectType()==EGameObjectType::Menu)
	{
		Canvas* can = a_pGO->GetComponent<Canvas>(STRINGIFY(Canvas));
		RegisterCanvas(can);
	}
}

void UIManager::Update(float dt)
{
	//for working on custom scenes
	if (!PiResource->GetConfig()->m_updateUI)
		return;

	if (!PiEngine->GetInGameStatus())
	{
		if (!m_isConfirmCanvasTriggered)
		{
			Canvas* c = GetCurrentActiveCanvas();
			if(c)
			c->ActivateCanvas();
		}
	}

	for (auto it : m_UIgameObj)
	{
		if (!PiEngine->GetInGameStatus() && (it->GetGameObjectType() == EGameObjectType::HUD || (it->GetGameObjectType() == EGameObjectType::SUN)))
		{
			continue;
		}
		it->Update(dt);
	}
}

void UIManager::DeActivateUIGOs(void)
{
	for (auto it : m_UIgameObj)
	{
		it->DeActivate();
	}
	m_UIgameObj.clear();
}

Canvas * UIManager::GetCurrentActiveCanvas(void)
{
	for (int i = 0; i < ECanvasType::Num; ++i)
	{
		if (m_Canvas[(ECanvasType)i])
		{
			if (m_Canvas[(ECanvasType)i]->GetActiveStatus())
			{
				return m_Canvas[(ECanvasType)i];
			};
		}
		
	}
	return nullptr;
}
