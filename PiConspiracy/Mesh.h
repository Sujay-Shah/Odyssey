//
//  Mesh.h
//  PiEngine
//
//  Created by PiConspiracy on 11/01/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once


#include "Renderable.h"

class ShapeGenerator;
class Model;
class Material;
class Mesh : public Renderable
{
public:

	enum eType : uint32
	{
		eType_Triangle,
		eType_Quad,
		eType_Cube,
		eType_Sphere,
		eType_Model,

		eType_Max
	};
                                    Mesh();
    virtual                        ~Mesh() override;
	
	bool							Initialize(eType a_eType, string& a_strMaterialName, string& a_strModelName);
	
	void							Update();
	
	virtual void					Draw(Material* a_pMaterial) override;

	inline Material*				GetMaterial() const;
	inline void						SetMaterial(Material* a_Material)
	{
		m_Material = a_Material;
	}

private:

	eType							m_eType;
	Material*						m_Material;

	ShapeGenerator*                 m_pShapeGenerator;
	Model*							m_pModel;

	//Instance and Light Count Data
	uint32		                    m_InstanceCount;
	bool	                        m_IsInstanced;
	bool	                        m_IsLightOn;
	Color3							m_meshColor;
	uint32							m_isMeshReflective;

    // TODO:: Zoheb 
    bool                            m_bIsMeshAnimated;

    ShapeGenerator::MeshPrimitive   m_MeshPrimitiveData;
};

inline Material* Mesh::GetMaterial() const
{
	return m_Material;
}
