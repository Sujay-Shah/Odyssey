//
//  UIManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class UIManager : public Singleton<UIManager>
{
public:
	UIManager();
	~UIManager();
	void ActivateCanvas(const ECanvasType);
	void DeactivateCanvas(const ECanvasType);
	void RegisterCanvas(Canvas *);
	void DeregisterCanvas(Canvas *);
	void DeActivateAllCanvas();
	bool IsCanvasAvailable(const ECanvasType);
	void HandleEvent(Event* pEvent);
	void AddGameObjectToCanvas(ECanvasType type, GameObject* _pGO);
	void Initialize(string );
	void AddUIGameObjects(GameObject*);
	void Update(float dt);
	void DeActivateUIGOs(void);
	Canvas* GetCurrentActiveCanvas(void);

	EEventType m_currentDestructiveEvent;
	ECanvasType m_lastActiveCanvas;
	bool m_isConfirmCanvasTriggered = false;
private:
	json m_menuJson;
	Canvas* m_Canvas[(int)ECanvasType::Num];
	vector<GameObject*> m_UIgameObj;
	Canvas* m_lastActive;
	float dt_here = 0.0f;
};

