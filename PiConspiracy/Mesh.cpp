//
//  Mesh.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/01/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ShaderManager.h"
#include "ShapeGenerator.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Model.h"
#include "ImguiManager.h"
#include "Material.h"

Mesh::Mesh():
m_eType(eType_Triangle), // default
m_pShapeGenerator(nullptr),
m_bIsMeshAnimated(false)
{
    
}

Mesh::~Mesh()
{
	//SAFE_DELETE(m_pShapeGenerator);
}

bool Mesh::Initialize(eType a_eType, string& a_strMaterialName, string& a_strModelName)
{
    m_eType = a_eType;

    m_Material = PiResource->GetMaterial(a_strMaterialName);

    if (m_eType != eType_Model)
    {
        m_pShapeGenerator = PiRenderer->GetShapeGenerator();
        if (!m_pShapeGenerator->InitializePrimitive(m_eType, m_Material))
        {
            return false;
        }

        m_MeshPrimitiveData = m_pShapeGenerator->GetPrimitiveMeshContent(ShapeGenerator::eShapeType(m_eType));
    }
    else if (m_eType == eType_Model)
    {
        m_pModel = PiResource->GetModel(a_strModelName);
        m_pModel->Initialize(m_Material);
    }
    return true;
}

void Mesh::Update()
{

}

void Mesh::Draw(Material* a_pMaterial)
{
	bool bIsDebugCameraOn = PiImgui->IsDebugCameraOn();


	if (m_eType != eType_Model)
	{
		if (m_bIsRenderable)
		{
			if (a_pMaterial)
			{	
				m_pShapeGenerator->Draw(a_pMaterial,m_MeshPrimitiveData,m_ModelMatrix, m_eType, bIsDebugCameraOn);
			}
			else
			{
				m_pShapeGenerator->Draw(m_Material,m_MeshPrimitiveData,m_ModelMatrix, m_eType, bIsDebugCameraOn);
			}
		}

	}
	else
	{
		if (m_bIsRenderable)
		{
			if (a_pMaterial)
			{
				m_pModel->DrawModel(a_pMaterial, m_ModelMatrix, m_eType, bIsDebugCameraOn);
			}
			else
			{
				m_pModel->DrawModel(m_Material, m_ModelMatrix, m_eType, bIsDebugCameraOn);
			}
		}
	}
}