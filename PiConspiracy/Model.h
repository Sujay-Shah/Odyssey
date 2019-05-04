//
//  Model.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class ImportMesh;
class ShaderManager;
struct aiNode;
struct aiScene;
struct aiNodeAnim;
struct aiAnimation;
struct aiMesh;
struct aiMaterial;
//struct aiMatrix4x4;
struct BoneMatrix;
struct Texture;
//struct aiVector3D;
//struct aiQuaternion;
enum aiTextureType;

class Material;
class Model
{
public:
	Model();
	~Model();
	static const uint32 MAX_BONES = 100;

	bool Initialize(Material* a_pMaterial);
	bool LoadModel(const string& path);
	vector<ImportMesh> GetMeshes(void);
	vector<Texture> GetTexture(void);

	void DrawModel(Material* a_pMaterial, mat4 a_ModelMatrix, uint32 a_uCode, bool a_bIsFreeCamOn);

	inline ShaderManager*  GetShader();

	aiQuaternion Nlerp(aiQuaternion a_quatOne, aiQuaternion a_quatTwo, float a_blend);
	mat4 AiToGlm(aiMatrix4x4 a_aiMat);
	inline bool HasAnimation()
	{
		return animation;
	}
	const aiScene* m_scene;
	aiScene* m_realscene;
private:

	void ProcessNode(aiNode* a_node, const aiScene* a_scene);
	ImportMesh ProcessMesh(aiMesh* a_mesh, const aiScene* a_scene);
	vector<Texture> LoadMaterialTexture(aiMaterial* a_material, aiTextureType a_type, string a_typeName);

	vector<ImportMesh> m_meshes;
	vector<Texture> m_textures_loaded;
	string directory;
	/////////////////////////////////////////////
	std::map<string, uint32> m_boneMapping;
	uint32 m_numBones = 0;
	vector<BoneMatrix> m_boneMatrices;
	aiMatrix4x4 m_globalInverseTransform;
	float m_ticksPerSecond = 0.0f;
	GLuint m_boneLocation[100];
	bool animation = false;

	uint32 FindPosition(float a_animationTime, const aiNodeAnim* a_nodeAnim);
	uint32 FindRotation(float a_animationTime, const aiNodeAnim* a_nodeAnim);
	uint32 FindScaling(float a_animationTime, const aiNodeAnim* a_nodeAnim);

	const aiNodeAnim* findNodeAnim(const aiAnimation* a_animation, const string a_nodeName);

	aiVector3D calInterpolatedPosition(float a_animationTime, const aiNodeAnim* a_nodeAnim);
	aiQuaternion calInterpolatedRotation(float a_animationTime, const aiNodeAnim* a_nodeAnim);
	aiVector3D calInterpolatedScaling(float a_animationTime, const aiNodeAnim* a_nodeAnim);

	void readNodeHierarchy(float a_animationTime, const aiNode* a_node, const aiMatrix4x4 a_parentTransform);
	void boneTransform(double a_timeInSeconds, vector<aiMatrix4x4>& a_transforms);
//////////////////////////////////////////////////////////////////
	vec3 m_objectColor = vec3(0.0f, 0.0f, 0.0f);

	ShaderManager* m_pShader;
};

inline ShaderManager* Model::GetShader()
{
	return m_pShader;
}

