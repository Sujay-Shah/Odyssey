//
//  MeshComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/30/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class Mesh;

class MeshComponent : public Component
{
	COMPONENT_DECLARE(MeshComponent);

public:
						    ~MeshComponent(void);

	void				    Initialize() override final;
	void				    Update(float a_dTime) override final;
	void				    Serialize(const json&) override final;
	void				    HandleEvent(Event* a_pEvent) override final;

	virtual void		    Deactivate(void) override;
	
    virtual void		    OverrideParams(const json&,float offset) override;

	inline uint32			GetType();
	inline void				SetType(uint32 type);
    inline Mesh*			GetMesh();

	Mesh*				    m_pMesh;
	//HUD id
	int _hudID;
	//int _hudIDbyEvent;
private:
	void				   _UpdatePowerUpHuD();
	uint32				    m_Type;

	string				    m_strModelName;
    string                  m_strMaterialName;

	Transform*			    m_pTransform; 
	
    bool                    m_bReceiveShadows;

	bool					m_bIsRenderable;

	
};

inline Mesh* MeshComponent::GetMesh()
{
    return m_pMesh;
}

inline void	MeshComponent::SetType(uint32 type) 
{ 
	m_Type = type; 
}

inline uint32 MeshComponent::GetType() 
{ 
	return m_Type; 
}