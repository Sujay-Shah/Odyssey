//
//  Model.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Model.h"
#include "Renderer.h"
#include "Camera.h"
#include "ImportMesh.h"
#include "ImguiManager.h"
#include "TextureData.h"
#include "Material.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "FrameRateController.h"

#include "FrameBuffer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"


Model::Model():
m_scene(nullptr),
m_pShader(nullptr)
{

}


Model::~Model()
{

}

bool Model::Initialize(Material* a_pMaterial)
{
	m_pShader = a_pMaterial->GetShader();
	m_pShader->Use();
	
	uint32 uShaderId = m_pShader->GetShaderID();
	m_pShader->SetInt(uShaderId, "AlbedoTexture", 0);
	m_pShader->SetInt(uShaderId, "NormalMapTexture", 1);
	m_pShader->SetInt(uShaderId, "textureskybox", 0);
	m_pShader->SetInt(uShaderId, "BaseNoiseTexture", 3);
	m_pShader->SetInt(uShaderId, "depthMap", 7);

	return true;
}

bool Model::LoadModel(const string& a_path)
{
	Assimp::Importer import;

	m_scene = import.ReadFile(a_path, ASSIMP_LOAD_FLAGS);
	if (!m_scene)
	{
        //LOG_DEBUG_ARG("Assimp Error: {}", import.GetErrorString());
        return false;
	}
	m_realscene = import.GetOrphanedScene();
	m_globalInverseTransform = m_scene->mRootNode->mTransformation;
	m_globalInverseTransform.Inverse();

	if (m_scene->HasAnimations())
	{
		if (m_scene->mAnimations[0]->mTicksPerSecond != 0.0)
		{
			m_ticksPerSecond = (float)m_scene->mAnimations[0]->mTicksPerSecond;
		}
		else
		{
			m_ticksPerSecond = 25.0f;
		}
	}
	
	directory = a_path.substr(0, a_path.find_last_of('/'));

	ProcessNode(m_scene->mRootNode, m_scene);
	return true;
}

void Model::ProcessNode(aiNode* a_node, const aiScene* a_scene)
{
	ImportMesh mesh;
	for (uint32 i = 0; i < m_scene->mNumMeshes; i++)
	{
		aiMesh* aiM = a_scene->mMeshes[i];
		mesh = ProcessMesh(aiM, a_scene);
		m_meshes.push_back(mesh);
	}
}

ImportMesh Model::ProcessMesh(aiMesh* a_mesh, const aiScene* a_scene)
{
	vector<Vertex> vertices;
	vector<uint32> indices;
	vector<Texture> textures;

	vertices.reserve(a_mesh->mNumVertices);
	indices.reserve(a_mesh->mNumVertices);

	//Vertices
	for (uint32 i = 0; i < a_mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vec3 vector;

		//positions
		vector.x = a_mesh->mVertices[i].x;
		vector.y = a_mesh->mVertices[i].y;
		vector.z = a_mesh->mVertices[i].z;
		vertex.m_position = vector;

		//normals
		if (a_mesh->HasNormals())
		{
			vector.x = a_mesh->mNormals[i].x;
			vector.y = a_mesh->mNormals[i].y;
			vector.z = a_mesh->mNormals[i].z;
			vertex.m_normal = vector;
		}
		else
		{
			vertex.m_normal = vec3();
		}
		//Texture Coordinates
		if (a_mesh->mTextureCoords[0])
		{
			vec2 vec;
			vec.x = a_mesh->mTextureCoords[0][i].x;
			vec.y = a_mesh->mTextureCoords[0][i].y;
			vertex.m_texCoords = vec;
		}
		else
		{
			vertex.m_texCoords = vec2(0.0f, 0.0f);
		}

		//Tangents and Bitangents
		if (a_mesh->HasTangentsAndBitangents())
		{
			//Tangents
			vector.x = a_mesh->mTangents[i].x;
			vector.y = a_mesh->mTangents[i].y;
			vector.z = a_mesh->mTangents[i].z;
			vertex.m_tangents = vector;

			//Bitangents
			vector.x = a_mesh->mBitangents[i].x;
			vector.y = a_mesh->mBitangents[i].y;
			vector.z = a_mesh->mBitangents[i].z;
			vertex.m_biTangents = vector;
		}
		else
		{
			vertex.m_tangents = vec3();
			vertex.m_biTangents = vec3();
		}
		vertices.push_back(vertex);
	}

	//Indices
	for (uint32 i = 0; i <a_mesh->mNumFaces; i++)
	{
		aiFace face = a_mesh->mFaces[i];
		for (uint32 j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//Material Textures
	if (a_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = a_scene->mMaterials[a_mesh->mMaterialIndex];

		//diffuse
		vector<Texture> diffuse_maps = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		//specular
		vector<Texture> specular_maps = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	}

	// Bones
	if (a_mesh->HasBones())
	{
		//boneIdWeights.resize(a_mesh->mNumVertices);

		for (uint32 i = 0; i < a_mesh->mNumBones; i++)
		{
			uint32 boneIndex = 0;
			string boneName(a_mesh->mBones[i]->mName.data);

			if (m_boneMapping.find(boneName) == m_boneMapping.end())
			{
				boneIndex = m_numBones;
				m_numBones++;
				BoneMatrix bi;
				m_boneMatrices.push_back(bi);
				m_boneMatrices[boneIndex].m_offsetMatrix = a_mesh->mBones[i]->mOffsetMatrix;
				m_boneMapping[boneName] = boneIndex;
			}
			else
			{
				boneIndex = m_boneMapping[boneName];
			}

			for (uint32 j = 0; j < a_mesh->mBones[i]->mNumWeights; j++)
			{
				uint32 vertexId = a_mesh->mBones[i]->mWeights[j].mVertexId;
				float weight = a_mesh->mBones[i]->mWeights[j].mWeight;
				vertices[vertexId].addBoneData(boneIndex, weight);
			}
		}

	}
	animation = m_scene->HasAnimations();
	return ImportMesh(vertices, indices, textures);
}

vector<Texture> Model::LoadMaterialTexture(aiMaterial* a_material, aiTextureType a_type, string a_typename)
{
	vector<Texture> textures;
	TextureData texLoad;
	for (uint32 i = 0; i < a_material->GetTextureCount(a_type); i++)
	{
		aiString ai_str;
		a_material->GetTexture(a_type, i, &ai_str);

		string filename = string(ai_str.C_Str());
		filename = directory + '/' + filename;

		bool skip = false;

		for (uint32 j = 0; j < m_textures_loaded.size(); j++)
		{
			if (m_textures_loaded[j].m_path == filename)
			{
				textures.push_back(m_textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;

			texture.m_texData = texLoad.LoadTexture(filename.c_str());
			texture.m_id = 0;
			texture.m_type = a_typename;
			texture.m_path = filename;
			textures.push_back(texture);

			this->m_textures_loaded.push_back(texture);
		}
	}
	return textures;
}

void Model::DrawModel(Material* a_pMaterial, mat4 a_ModelMatrix, uint32 a_uCode, bool a_bIsFreeCamOn)
{
	PiRenderer->SetDebugCamMode(a_bIsFreeCamOn);
	Camera* pCamera = PiRenderer->GetCamera();

	#pragma region MVP-matrix
		//Model Matrix
		m_pShader->Use();
		glm::mat4 lsm = PiRenderer->m_LightProjectionMatrix * PiRenderer->m_LightView;
		m_pShader = a_pMaterial->GetShader();
	

		m_pShader->SetUniformMatrix4fv(m_pShader->GetShaderID(), "LightSpaceMatrix", lsm);

		m_pShader->SetUniformMatrix4fv(m_pShader->GetShaderID(), "model_Matrix",a_ModelMatrix);

		//if (a_pMaterial != PiRenderer->GetShadowMaterial())
		//{
		//	FrameBuffer* pFrameBuffer = PiRenderer->GetFrameBuffer();
		//	pFrameBuffer->BindForReading(GL_TEXTURE7);
		//}
		
		//View Matrix
		mat4 View_matrix = pCamera->GetViewMatrix();//glm::translate(mat4(1), -(Camer_Pos));

		/************************debug from LPOV*/
		m_pShader->SetUniformMatrix4fv(m_pShader->GetShaderID(), "view_Matrix", pCamera->GetViewMatrix());

		//Projection Matrix
		mat4 Projection_matrix = pCamera->GetProjectionMatrix();
		m_pShader->SetUniformMatrix4fv(m_pShader->GetShaderID(), "projection_Matrix", pCamera->GetProjectionMatrix());
		/************************/

		glm::mat4 MVP = Projection_matrix * View_matrix * a_ModelMatrix;
		m_pShader->SetUniformMatrix4fv(m_pShader->GetShaderID(), "MVP", MVP);

#pragma endregion
		
	Color3 vModelColor = a_pMaterial->GetColor();
	m_pShader->SetUniform3f(m_pShader->GetShaderID(), "strmaterial.objectCol", vModelColor.x, vModelColor.y, vModelColor.z);

	#pragma region Material-Properties

		m_pShader->SetUniform3f(m_pShader->GetShaderID(), "strmaterial.diffuse", 0.7f, 0.7f, 0.7f);
		m_pShader->SetUniform3f(m_pShader->GetShaderID(), "strmaterial.specular", 0.5f, 0.5f, 0.5f);
		m_pShader->SetUniform1f(m_pShader->GetShaderID(), "strmaterial.shininess", 256.0f);

#pragma endregion


	#pragma region Fog-Properties

		//ImguiManager::FogParameters fogParameters = PiImgui->GetFogParameters();
		Renderer::FogParameters fogParameters = PiRenderer->GetFogParameters();
		glm::vec3 fog_color = fogParameters.m_vColor;// PiImgui->getFogColor();

		int Lmode = PiRenderer->GetFogParameters().m_uLightMode;//2;
		m_pShader->SetInt(m_pShader->GetShaderID(), "LightMode", Lmode);


		m_pShader->SetUniform3f(m_pShader->GetShaderID(), "fogColor", fog_color.x, fog_color.y, fog_color.z);
		m_pShader->SetUniform1f(m_pShader->GetShaderID(), "fogStartDistance", fogParameters.m_fFogStartDistance);
		m_pShader->SetUniform1f(m_pShader->GetShaderID(), "fogEndDistance", fogParameters.m_fFogEndDistance);
		m_pShader->SetUniform1f(m_pShader->GetShaderID(), "fogDensity", fogParameters.m_fFogDensity);//m_fFogDensity
		m_pShader->SetInt(m_pShader->GetShaderID(), "fogDepth", fogParameters.m_uFogDepth);//m_uDepthType
		m_pShader->SetInt(m_pShader->GetShaderID(), "fogSelector", fogParameters.m_uFogSelector);
		m_pShader->SetInt(m_pShader->GetShaderID(), "fogSelector", fogParameters.m_uFogSelector);

		bool fog_isEnable = fogParameters.m_bfogEnabled;// PiImgui->getIsFogEnable();
		if (fog_isEnable == true)
		{
			m_pShader->SetInt(m_pShader->GetShaderID(), "FogEnable", 1);
		}
		else
		{
			m_pShader->SetInt(m_pShader->GetShaderID(), "FogEnable", 0);
		}

#pragma endregion



	#pragma region Animation-Part
	if (m_scene->HasAnimations())
	{
		vector<aiMatrix4x4> transforms;
		boneTransform((double)SDL_GetTicks() / 1000.0f, transforms);

		for (uint32 i = 0; i <transforms.size(); i++) 
		{
			string name = "bones[" + std::to_string(i) + "]";
			m_boneLocation[i] = glGetUniformLocation(m_pShader->GetShaderID(), name.c_str());
			glUniformMatrix4fv(m_boneLocation[i], 1, GL_TRUE, (const GLfloat*)&transforms[i]);
		}
	}
#pragma endregion

		for (uint32 i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i].Draw(a_pMaterial);
		}
}

vector<ImportMesh> Model::GetMeshes(void)
{
	return m_meshes;
}

vector<Texture> Model::GetTexture(void)
{
	return m_textures_loaded;
}

uint32 Model::FindPosition(float a_animationTime, const aiNodeAnim* a_nodeAnim)
{
	for (uint32 i = 0; i < a_nodeAnim->mNumPositionKeys - 1; i++)
	{
		if (a_animationTime < (float)a_nodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

uint32 Model::FindRotation(float a_animationTime, const aiNodeAnim* a_nodeAnim)
{
	for (uint32 i = 0; i < a_nodeAnim->mNumRotationKeys - 1; i++)
	{
		if (a_animationTime < (float)a_nodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

uint32 Model::FindScaling(float a_animationTime, const aiNodeAnim* a_nodeAnim)
{
	for (uint32 i = 0; i < a_nodeAnim->mNumScalingKeys - 1; i++)
	{
		if (a_animationTime < (float)a_nodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

aiVector3D Model::calInterpolatedPosition(float a_animationTime, const aiNodeAnim* a_nodeAnim)
{
	if (a_nodeAnim->mNumPositionKeys == 1)
	{
		return a_nodeAnim->mPositionKeys[0].mValue;
	}

	uint32 positionIndex = FindPosition(a_animationTime, a_nodeAnim);
	uint32 nextPositionIndex = positionIndex + 1;

	assert(nextPositionIndex < a_nodeAnim->mNumPositionKeys);

	float deltaTime = (float)(a_nodeAnim->mPositionKeys[nextPositionIndex].mTime - a_nodeAnim->mPositionKeys[positionIndex].mTime);
	float factor = (a_animationTime - (float)a_nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;

	//assert(factor >= 0.0f && factor <= 1.0f);

	aiVector3D start = a_nodeAnim->mPositionKeys[positionIndex].mValue;
	aiVector3D end = a_nodeAnim->mPositionKeys[nextPositionIndex].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

aiQuaternion Model::calInterpolatedRotation(float a_animationTime, const aiNodeAnim* a_nodeAnim)
{
	if (a_nodeAnim->mNumRotationKeys == 1)
	{
		return a_nodeAnim->mRotationKeys[0].mValue;
	}

	uint32 rotationIndex = FindRotation(a_animationTime, a_nodeAnim);
	uint32 nextRotationIndex = rotationIndex + 1;

	assert(nextRotationIndex < a_nodeAnim->mNumRotationKeys);

	float deltaTime = (float)(a_nodeAnim->mRotationKeys[nextRotationIndex].mTime - a_nodeAnim->mRotationKeys[rotationIndex].mTime);
	float factor = (a_animationTime - (float)a_nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;

	//assert(factor >= 0.0f && factor <= 1.0f);

	aiQuaternion startQuat = a_nodeAnim->mRotationKeys[rotationIndex].mValue;
	aiQuaternion endQuat = a_nodeAnim->mRotationKeys[nextRotationIndex].mValue;

	return Nlerp(startQuat, endQuat, factor);
}

aiVector3D Model::calInterpolatedScaling(float a_animationTime, const aiNodeAnim* a_nodeAnim)
{
	if (a_nodeAnim->mNumScalingKeys == 1)
	{
		return a_nodeAnim->mScalingKeys[0].mValue;
	}

	uint32 scalingIndex = FindScaling(a_animationTime, a_nodeAnim);
	uint32 nextScalingIndex = scalingIndex + 1;

	assert(nextScalingIndex < a_nodeAnim->mNumScalingKeys);

	float deltaTime = (float)(a_nodeAnim->mScalingKeys[nextScalingIndex].mTime - a_nodeAnim->mScalingKeys[scalingIndex].mTime);
	float factor = (a_animationTime - (float)a_nodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;

	//assert(factor >= 0.0f && factor <= 1.0f);

	aiVector3D start = a_nodeAnim->mScalingKeys[scalingIndex].mValue;
	aiVector3D end = a_nodeAnim->mScalingKeys[nextScalingIndex].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

const aiNodeAnim* Model::findNodeAnim(const aiAnimation* a_animation, const string a_nodeAnim)
{
	for (uint32 i = 0; i < a_animation->mNumChannels; i++)
	{
		const aiNodeAnim* nodeAnim = a_animation->mChannels[i];

		if (string(nodeAnim->mNodeName.data) == a_nodeAnim)
		{
			return nodeAnim;
		}
	}

	return nullptr;
}

void Model::readNodeHierarchy(float a_animationTime, const aiNode* a_node, const aiMatrix4x4 a_parentTransform)
{
	string nodeName(a_node->mName.data);

	const aiAnimation* animation = m_scene->mAnimations[0];
	aiMatrix4x4 nodeTransform = a_node->mTransformation;

	const aiNodeAnim* nodeAnim = findNodeAnim(animation, nodeName);

	if (nodeAnim)
	{
		//Scaling
		aiVector3D scalingVector = calInterpolatedScaling(a_animationTime, nodeAnim);
		aiMatrix4x4 scalingMatrix;
		aiMatrix4x4::Scaling(scalingVector, scalingMatrix);

		//Rotation
		aiQuaternion rotatingQuat = calInterpolatedRotation(a_animationTime, nodeAnim);
		aiMatrix4x4 rotatingMatrix = aiMatrix4x4(rotatingQuat.GetMatrix());

		//Translation
		aiVector3D translatingVector = calInterpolatedPosition(a_animationTime, nodeAnim);
		aiMatrix4x4 translatingMatrix;
		aiMatrix4x4::Translation(translatingVector, translatingMatrix);

		nodeTransform = translatingMatrix * rotatingMatrix *scalingMatrix;
	}

	aiMatrix4x4 globalTransform = a_parentTransform * nodeTransform;

	if (m_boneMapping.find(nodeName) != m_boneMapping.end())
	{
		uint32 boneIndex = m_boneMapping[nodeName];
		m_boneMatrices[boneIndex].m_finalWorldTransform = m_globalInverseTransform * globalTransform * m_boneMatrices[boneIndex].m_offsetMatrix;
	}

	for (uint32 i = 0; i < a_node->mNumChildren; i++)
	{
		readNodeHierarchy(a_animationTime, a_node->mChildren[i], globalTransform);
	}
}

void Model::boneTransform(double a_timeInSeconds, vector<aiMatrix4x4>& a_transforms)
{
	aiMatrix4x4 identityMatrix;

	if (m_scene->HasAnimations())
	{
		double timeInTicks = a_timeInSeconds * m_ticksPerSecond;
		float animationTime =(float)fmod(timeInTicks, m_scene->mAnimations[0]->mDuration);

		readNodeHierarchy(animationTime, m_realscene->mRootNode, identityMatrix);
	}

	a_transforms.resize(m_numBones);

	for (uint32 i = 0; i < m_numBones; i++)
	{
		a_transforms[i] = m_boneMatrices[i].m_finalWorldTransform;
	}
}

aiQuaternion Model::Nlerp(aiQuaternion a_quatOne, aiQuaternion a_quatTwo, float a_blend)
{
	a_quatOne.Normalize();
	a_quatTwo.Normalize();

	aiQuaternion result;

	float dotProduct = a_quatOne.x * a_quatTwo.x + a_quatOne.y * a_quatTwo.y + a_quatOne.z * a_quatTwo.z + a_quatOne.w * a_quatTwo.w;
	float oneMinusBlend = 1.0f - a_blend;

	if (dotProduct < 0.0f)
	{
		result.x = a_quatOne.x * oneMinusBlend + a_blend * -a_quatTwo.x;
		result.y = a_quatOne.y * oneMinusBlend + a_blend * -a_quatTwo.y;
		result.z = a_quatOne.z * oneMinusBlend + a_blend * -a_quatTwo.z;
		result.w = a_quatOne.w * oneMinusBlend + a_blend * -a_quatTwo.w;
	}
	else
	{
		result.x = a_quatOne.x * oneMinusBlend + a_blend * a_quatTwo.x;
		result.y = a_quatOne.y * oneMinusBlend + a_blend * a_quatTwo.y;
		result.z = a_quatOne.z * oneMinusBlend + a_blend * a_quatTwo.z;
		result.w = a_quatOne.w * oneMinusBlend + a_blend * a_quatTwo.w;
	}

	return result.Normalize();
}

mat4 Model::AiToGlm(aiMatrix4x4 a_aiMat)
{
	mat4 result;

	result[0].x = a_aiMat.a1; result[0].y = a_aiMat.b1; result[0].z = a_aiMat.c1; result[0].w = a_aiMat.d1;
	result[1].x = a_aiMat.a2; result[1].y = a_aiMat.b2; result[1].z = a_aiMat.c2; result[1].w = a_aiMat.d2;
	result[2].x = a_aiMat.a3; result[2].y = a_aiMat.b3; result[2].z = a_aiMat.c3; result[2].w = a_aiMat.d3;
	result[3].x = a_aiMat.a4; result[3].y = a_aiMat.b4; result[3].z = a_aiMat.c4; result[3].w = a_aiMat.d4;

	return result;
}
