//
//  ImportMesh.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#define NUM_BONES_PER_VERTEX 4

struct TexData;
//struct aiMatrix4x4;

struct Vertex
{
	vec3 m_position;
	vec3 m_normal;
	vec2 m_texCoords;
	uint32 m_ids[NUM_BONES_PER_VERTEX];
	float m_weights[NUM_BONES_PER_VERTEX];

	vec3 m_tangents;
	vec3 m_biTangents;

	

	Vertex()
	{
		memset(m_ids, 0, sizeof(m_ids));
		memset(m_weights, 0, sizeof(m_weights));
	}
	void addBoneData(uint32 a_boneId, float a_weight);

};

struct Texture
{
	uint32 m_id;
	string m_type;
	string m_path;
	TexData *m_texData;
};

struct BoneMatrix
{
	aiMatrix4x4 m_offsetMatrix;
	aiMatrix4x4 m_finalWorldTransform;
	aiMatrix4x4 m_finalWorldTransformBoneDraw;
};

//struct VertexBoneData
//{
//	uint32 m_ids[NUM_BONES_PER_VERTEX];
//	float m_weights[NUM_BONES_PER_VERTEX];
//
//	VertexBoneData()
//	{
//		memset(m_ids, 0, sizeof(m_ids));
//		memset(m_weights, 0, sizeof(m_weights));
//	}
//	void addBoneData(uint32 a_boneId, float a_weight);
//
//};

class Material;
class ImportMesh
{
public:
							ImportMesh();
							ImportMesh(vector<Vertex> a_vertices, vector<uint32> a_indicies, vector<Texture> a_texture);
							~ImportMesh();

	void					Draw(Material* a_pMaterial);

protected:
	void					Initialize();

private:
	
	uint32					m_uVAO;
	uint32					m_uVBO;
	uint32					m_uEBO;

	vector<Vertex>			m_vertices;
	vector<GLuint>			m_indices;
	vector<Texture>			m_textures;
	//vector<Vertex>	m_boneIdWeights;

	//Instancing Part

	uint32 m_instanceCount = 0;

	std::vector<glm::vec3>	m_InstancetranslationVector;
	
	uint32 m_instanceVBO;



};

