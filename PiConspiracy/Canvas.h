//
//  Canvas.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class Canvas : public Component
{
	COMPONENT_DECLARE(Canvas)
public:
	~Canvas();

	// Inherited via Component
	virtual void Initialize(void) override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &j) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;
	virtual void OverrideParams(const json &,float) override;

	void ActivateCanvas(void);
	void AddGameObjectToCanvas(GameObject* pGO);
	void DeactivateCanvas(void);

	bool GetActiveStatus(void) { return m_isCanvasActive; };
	
	ECanvasType m_canvasType;
	string m_canvasName;
	int m_canvasWidth;
	int m_canvasHeight;
	//will contain location of all the buttons sorted Y val
	vector<std::pair<float,float>> m_buttonPos;
private:
	bool m_displayCanvas;
	vector<GameObject*> m_UIelements;
	bool m_isCanvasActive;
	Transform* m_trans;
	vec3 m_canvasPos;
};

