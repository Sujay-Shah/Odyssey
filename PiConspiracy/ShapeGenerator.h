//
//  ShapeGenerator.h
//  PiEngine
//
//  Created by PiConspiracy on 10/26/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class ShaderManager;
class Model;
class LightComponent;
class Material;
class ShapeGenerator
{
public:

    struct MeshPrimitive
    {
    public:
        uint32          m_uVAO;
        uint32          m_uVBO;
        uint32          m_uIBO;
        uint32          m_uTextureID;
    };

    enum eShapeType : uint32
    {
        eShapeType_Triangle,
        eShapeType_Quad,
        eShapeType_Cube,
        eShapeType_Sphere,

        eShapeType_Max
    };
						    ShapeGenerator();
						    ~ShapeGenerator();
	
	bool				    InitializePrimitive(uint32 a_uType, Material* a_pMaterial);

	void				    Triangle_Generator();

	void				    Quad_Generator();

	void				    Cube_Generator(bool a_bIsInstancingOn, uint32 a_uInstanceCount);

	void				    Sphere_Generator();

	void				    Draw(Material* a_pMaterial, MeshPrimitive a_Primitive, mat4 a_modelMatrix, uint32 a_uCode, bool a_bIsFreeCamOn);
	
	void				    Destroy();
	
    inline MeshPrimitive    GetPrimitiveMeshContent(eShapeType a_eType);

	ShaderManager*          m_shaderID;
	//void SetIsCoolDown(bool);
	//void SetIsJumpCoolDown(bool);
	//void SetIsRespwanCoolDown(bool);
	//void SetIsDashCoolDown(bool);
private:

	uint32 m_vao;
	uint32 m_vbo;
	uint32 m_ibo;
	uint32 uTexture;

    MeshPrimitive       m_PrimitiveMeshes[eShapeType_Max];

	uint32 m_vertex;
	uint32 m_normals;
	uint32 m_texture;

	uint64 m_indexCount = 0;

	uint32 m_instanceCount = 0;

	std::vector<glm::vec3>	m_InstancetranslationVector;
	uint32 m_instanceVBO;

	bool m_isCoolDown;
};

inline ShapeGenerator::MeshPrimitive ShapeGenerator::GetPrimitiveMeshContent(eShapeType a_eShapeType)
{
    return m_PrimitiveMeshes[a_eShapeType];
}

//inline void ShapeGenerator::SetIsCoolDown(bool val)
//{
//	m_isCoolDown = val;
//}

//inline void ShapeGenerator::SetIsJumpCoolDown(bool val)
//{
//	m_isJumpCoolDown = val;
//}
//
//inline void ShapeGenerator::SetIsRespwanCoolDown(bool val)
//{
//	m_isRespawnCoolDown = val;
//}
//
//inline void ShapeGenerator::SetIsDashCoolDown(bool val)
//{
//	m_isDashCoolDown = val;
//}

