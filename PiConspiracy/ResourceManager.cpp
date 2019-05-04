//
//  ResourceManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ResourceManager.h"
#include "FileParser.h"
#include "DirectoryObject.h"
#include "ProjectConfig.h"
#include "TextureData.h"
#include "Model.h"
#include "AudioManager.h"
#include "ShaderManager.h"
#include "Material.h"

#include "FMOD/lowlevel/fmod.hpp"
#include "FMOD/studio/fmod_studio.hpp"


	ResourceManager::ResourceManager(void) 
	{
		m_fp = new FileParser();
		m_projectConfig = nullptr;

		m_CubeFaces = 
		{
			"Assets/Textures/SkyBox/Test2/right.tga",
			"Assets/Textures/SkyBox/Test2/left.tga",
			"Assets/Textures/SkyBox/Test2/top.tga",
			"Assets/Textures/SkyBox/Test2/bottom.tga",
			"Assets/Textures/SkyBox/Test2/back.tga",
			"Assets/Textures/SkyBox/Test2/front.tga",
		};
	}
	ResourceManager::~ResourceManager()
	{
		delete m_fp;
		delete m_projectConfig;
		m_Elements.clear();
		UnloadAllResources();
	}

	void ResourceManager::Initialize()
	{
		m_projectConfig = new ProjectConfig;
		m_projectConfig->LoadConfig("config.json");
		m_fp->ExploreDirectory(m_projectConfig->GetAssetDir(), nullptr);
		m_Elements = m_fp->GetListOfElements();

		//load Resources
		LoadArchetypes(m_projectConfig->GetArchetypeDir());
		LoadTextures(m_projectConfig->GetTextureDir(),false);			// for only textures 
		LoadTextures(m_projectConfig->GetTextureDir(),true);			// for sky box textures
		LoadShaders(m_projectConfig->GetShaderDir());
		LoadMaterials(m_projectConfig->GetMaterialDir());
		LoadModels(m_projectConfig->GetModelDir());
		LoadMusicAudioLibrary();
		LoadSFXAudioLibrary();
		LoadScripts();
		
		
	}

    const DirectoryObject* ResourceManager::GetFilePath(const string& a_fileName) const
	{
		auto it = m_Elements.find(Hash(a_fileName));
		if (it != m_Elements.end())
		{
			return it->second;
		}
		else
		{
            LOG_ERROR_ARG("File {} not found", a_fileName.c_str());
		}

		return nullptr;
	}

	FMOD::Sound * ResourceManager::GetSoundFromMap(const string & a_name)
	{
		auto foundIt = soundMap.find(a_name);
		if (foundIt == soundMap.end())
			return nullptr;
		else
			return foundIt->second;
	}

	std::map<int, FMOD::Channel*> ResourceManager::GetChannelMap()
	{
		return channelMap;
	}

	void ResourceManager::LoadMusicAudioLibrary()
	{
		//TODO: find better way to load 

		for (auto it : m_Elements)
		{
			if (it.second->m_objType == EDirectoryObjectType::File)
			{
				File* f = static_cast<File*>(it.second);
				if (f->m_parentFolder == "Audio")
				{
					//load music 
					const string path = f->m_path;
					const string value = f->m_name;
					string name;
					FILE_NAME_FROM_PATH(value, name);

					LoadSound(path, name, PiAudio->GetSystem(), false, true, false);
				}
			}
		}
	}

	void ResourceManager::LoadSFXAudioLibrary()
	{
		//TODO: find better way to load 

		for (auto it : m_Elements)
		{
			if (it.second->m_objType == EDirectoryObjectType::File)
			{
				File* f = static_cast<File*>(it.second);
				if (f->m_parentFolder == "SFX")
				{
					//load music 

					const string path = f->m_path;
					const string value = f->m_name;
					string name;
					FILE_NAME_FROM_PATH(value, name);

					LoadSound(path, name, PiAudio->GetSystem(), true, false, true);
				}
			}
		}
	}

	void ResourceManager::LoadScripts()
	{
		string parent, fileName;
		for (auto it : m_Elements)
		{
			if (it.second->m_objType == DirectoryObjectType::File)
			{
				File* file = static_cast<File*>(it.second);
				parent = file->m_parentFolder;
				if (parent == "Scripts" && (file->m_name != "class.lua") && (file->m_name != "utility.lua"))
				{
					m_scriptPath.insert({file->m_name, file->m_path});
					//LOG_DEBUG("\n" + output);
				}
			}
		}



		/*std::ifstream file;
		file.open((a_strFileName).c_str());

		string output;
		string line;

		if (file.is_open())
		{
			while (file.good())
			{
				getline(file, line);
				output.append(line + "\n");
			}
		}
		else
		{
			std::cerr << "Unable to load script: " << a_strFileName << std::endl;
		}

		return output;*/
	}

	void ResourceManager::LoadSound(const string & a_audioValue, const string & a_audioName, FMOD::System* a_system, bool a_isAudio3D, bool a_isAudioLooping, bool a_isAudioStreaming)
	{
		auto found_it = soundMap.find(a_audioName);
		if (found_it != soundMap.end())
			return;

		FMOD_MODE mode = FMOD_DEFAULT;
		mode |= a_isAudio3D ? FMOD_3D : FMOD_2D;
		mode |= a_isAudioLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		mode |= a_isAudioStreaming ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

		FMOD::Sound* sound = nullptr;
		AudioManager::ErrorCheck(a_system->createSound(a_audioValue.c_str(), mode, nullptr, &sound));

		if (sound)
		{
			soundMap[a_audioName] = sound;
		}
	}

	const DirectoryObject * ResourceManager::GetFilePath(const uint32 a_fileHash) const
	{
		auto it = m_Elements.find(a_fileHash);
		if (it != m_Elements.end())
		{
			return it->second;
		}
		else
		{
			cout<<"no object with hash "<< a_fileHash<<" is found \n";
		}
		return nullptr;
	}

	void ResourceManager::LoadArchetypes(const string & a_prefabPath)
	{
		//TODO : given a folder path, only load it's contents instead of traversing through the whole list
		//TODO : load stuff from cache
		string parent, fileName;
		for(auto it : m_Elements)
		{
			if (it.second->m_objType == DirectoryObjectType::File)
			{
				File* file = static_cast<File*>(it.second);
				 parent = file->m_parentFolder;
				if (parent == "Archetypes")
				{
					json* j = new json();
					*j = OpenJsonFile(file->m_path);
					fileName = file->m_name;
					string::size_type pos = fileName.find(".");
					if (pos != string::npos)
					{
						fileName = fileName.substr(0, pos);
					}
					m_archTypes[fileName] = j;
				}
			}
		}
	}

	//int ResourceManager::LoadHighScore(const string & a_HighScoreFilepath)
	//{
	//	std::ifstream file;
	//	file.open((a_HighScoreFilepath).c_str());

	//	string output;
	//	string line;

	//	if (file.is_open())//
	//	{
	//		while (file.good())
	//		{
	//			getline(file, line);
	//			output.append(line + "\n");
	//		}
	//	}
	//	else
	//	{
	//		std::cerr << "Unable to load High Score File: " << a_HighScoreFilepath << std::endl;
	//	}

	//	m_HighScore = std::atoi(output.c_str());

	//	return m_HighScore;

	//}

	void ResourceManager::LoadTextures(const string& a_texturePath, bool a_bIsSkyBox)
	{
		string parent;
		if (a_bIsSkyBox)
		{
			TextureData* textureData = new TextureData;
			textureData->LoadSkyBoxTexture(m_CubeFaces);
			if (textureData != nullptr)
			{
				m_SkyboxTextures.insert({ "skybox",textureData });
			}
		}
		else
		{
			for (auto it : m_Elements)
			{
				if (it.second->m_objType == DirectoryObjectType::File)
				{
					File* file = static_cast<File*>(it.second);
					parent = file->m_parentFolder;
					if (parent == "Textures")
					{
						TextureData* textureData = new TextureData;
						textureData->LoadTexture(file->m_path.c_str());
						if (textureData != nullptr)
						{
							m_textures.insert({ file->m_name,textureData });
						}
					}
				}
			}
		}

	}
	    
	void ResourceManager::LoadModels(const string & a_model)
	{
		string parent, fileName;
		for (auto it : m_Elements)
		{
			if (it.second->m_objType == DirectoryObjectType::File)
			{
				File* file = static_cast<File*>(it.second);
				parent = file->m_parentFolder;
				if (parent == "Models")
				{
					Model* model = new Model;
					bool bIsModelLoaded =model->LoadModel(file->m_path.c_str());
					if (bIsModelLoaded)
					{
						m_models.insert({ file->m_name,model });
					}
				}
			}
		}
	}

	void ResourceManager::LoadShaders(const string & a_shaderPath)
	{
		string parent,ext,shaderName;
		for (auto it : m_Elements)
		{
			if (it.second->m_objType == DirectoryObjectType::File)
			{
				File* file = static_cast<File*>(it.second);
				parent = file->m_parentFolder;
				if (parent == "Shaders")
				{
					ext = file->m_ext;
					if (ext == "VS")
					{
						ShaderManager* shader = new ShaderManager();
						//size_t start = fileName.begin();
						shaderName = file->m_path.substr( 0 , file->m_path.find("."));
						bool result = shader->Initialize(shaderName);
						if (result)
						{
							m_shaders.insert({ shaderName,shader });
						}
					}				
				}
			}
		}
	}

    void ResourceManager::LoadMaterials(const string& a_MaterialDir)
    {
        string parent, fileName;
        for (auto it : m_Elements)
        {
            if (it.second->m_objType == DirectoryObjectType::File)
            {
                File* pFile = static_cast<File*>(it.second);
                parent = pFile->m_parentFolder;
                if (parent == "Materials")
                {
                    Material* pMaterial = new Material;

                    json j = OpenJsonFile(pFile->m_path);

                    //TODO: no material type set in the json
                    if (ValueExists(j, "albedo"))
                    {
                        string albedo = ParseString(j, "albedo");
                        pMaterial->SetTexture(albedo,Material::eTextureType_Albedo);
                    }

					if (ValueExists(j, "CoolDownTexture"))
					{
						string albedo = ParseString(j, "CoolDownTexture");
						pMaterial->SetTexture(albedo, Material::eTextureType_CoolDownTexture);
					}

                    if (ValueExists(j, "normal"))
                    {
                        string normal = ParseString(j, "normal");
                        pMaterial->SetTexture(normal, Material::eTextureType_Normal);
                    }

					if (ValueExists(j, "perlin0"))
					{
						string normal = ParseString(j, "perlin0");
						pMaterial->SetTexture(normal, Material::eTextureType_PerlinNoise0);
					}

					if (ValueExists(j, "perlin1"))
					{
						string normal = ParseString(j, "perlin1");
						pMaterial->SetTexture(normal, Material::eTextureType_PerlinNoise1);
					}

					if (ValueExists(j, "perlin2"))
					{
						string normal = ParseString(j, "perlin2");
						pMaterial->SetTexture(normal, Material::eTextureType_PerlinNoise2);
					}

					if (ValueExists(j, "perlin3"))
					{
						string normal = ParseString(j, "perlin3");
						pMaterial->SetTexture(normal, Material::eTextureType_PerlinNoise3);
					}

					if (ValueExists(j, "left_tutorial"))
					{
						string albedo = ParseString(j, "left_tutorial");
						pMaterial->SetTexture(albedo, Material::eTextureType_Left);
					}

					if (ValueExists(j, "right_tutorial"))
					{
						string albedo = ParseString(j, "right_tutorial");
						pMaterial->SetTexture(albedo, Material::eTextureType_Right);
					}

					if (ValueExists(j, "jump_tutorial"))
					{
						string albedo = ParseString(j, "jump_tutorial");
						pMaterial->SetTexture(albedo, Material::eTextureType_Jump);
					}

					if (ValueExists(j, "boost_tutorial"))
					{
						string albedo = ParseString(j, "boost_tutorial");
						pMaterial->SetTexture(albedo, Material::eTextureType_Boost);
					}

					if (ValueExists(j, "extralife_tutorial"))
					{
						string albedo = ParseString(j, "extralife_tutorial");
						pMaterial->SetTexture(albedo, Material::eTextureType_ELife);
					}

					if (ValueExists(j, "pickups_tutorial"))
					{
						string albedo = ParseString(j, "pickups_tutorial");
						pMaterial->SetTexture(albedo, Material::eTextureType_Pickups);
					}




                    string shaderName = ParseString(j, "shader");
                    pMaterial->SetShader(shaderName);
					
					if (ValueExists(j, "isReceiveLight"))
					{
						bool isReceiveLight = ParseBool(j, "isReceiveLight");
						pMaterial->m_bIsReceiveLight = isReceiveLight;
					}

					if (ValueExists(j, "isInstanced"))
					{
						bool isInstanced = ParseBool(j, "isInstanced");
						pMaterial->m_bIsInstanced = isInstanced;
					}

					if (ValueExists(j, "isReflective"))
					{
						bool isReflective = ParseBool(j, "isReflective");
						pMaterial->m_bIsReflective = isReflective;
					}

					if (ValueExists(j, "InstanceCount"))
					{
						uint32 uInstanceCount = ParseInt(j, "InstanceCount");
						pMaterial->SetInstancedCout(uInstanceCount);
					}

                    Color3 vColor;
                    if (ValueExists(j, "color"))
                    {
                        vColor.x = ParseFloat(j["color"], "x");
                        vColor.y = ParseFloat(j["color"], "y");
                        vColor.z = ParseFloat(j["color"], "z");
                    }
                    pMaterial->SetColor(vColor);


					Color3 vDiffuseColor;
					if (ValueExists(j, "DiffuseColor"))
					{
						vDiffuseColor.x = ParseFloat(j["DiffuseColor"], "x");
						vDiffuseColor.y = ParseFloat(j["DiffuseColor"], "y");
						vDiffuseColor.z = ParseFloat(j["DiffuseColor"], "z");
					}
					pMaterial->SetDiffuseColor(vDiffuseColor);

					Color4 vSpecularColor;
					if (ValueExists(j, "SpecularColor"))
					{
						vSpecularColor.x = ParseFloat(j["SpecularColor"], "x");
						vSpecularColor.y = ParseFloat(j["SpecularColor"], "y");
						vSpecularColor.z = ParseFloat(j["SpecularColor"], "z");
						vSpecularColor.z = ParseFloat(j["SpecularColor"], "Shininess");
					}
					pMaterial->SetSpecularColor(vSpecularColor);

					if (ValueExists(j, "BrightColor"))
					{
						Color3 vBrightColor;

						vBrightColor.x = ParseFloat(j["BrightColor"], "x");
						vBrightColor.y = ParseFloat(j["BrightColor"], "y");
						vBrightColor.z = ParseFloat(j["BrightColor"], "z");
					
						pMaterial->SetBrightColor(vBrightColor);
					}
					else
					{
						Color3 vBrightColor;

						vBrightColor.x = 0.0f;
						vBrightColor.y = 0.0f;
						vBrightColor.z = 0.0f;

						pMaterial->SetBrightColor(vBrightColor);
					}

                    //finally add that pointer in the list alongwith its name,
                    // to use it lookup material name in list and use the pointer
                    m_Materials.insert({ pFile->m_name, pMaterial });
                }
            }
        }
    }

	json* ResourceManager::GetArchetype(string a_archetypeName) const
	{
		auto it = m_archTypes.find(a_archetypeName);
		if (it != m_archTypes.end())
		{
			return it->second;
		}
		else
			return nullptr;
	}

	TextureData* ResourceManager::GetTexture(string a_textureName) const
	{
		auto it = m_textures.find(a_textureName);
		if (it != m_textures.end())
		{
			return it->second;
		}
		else
			return nullptr;
	}

	Model * ResourceManager::GetModel(string a_modelName) const
	{
		auto it = m_models.find(a_modelName);
		if (it != m_models.end())
		{
			return it->second;
		}
		else
			return nullptr;
	}

	ShaderManager * ResourceManager::GetShader(string a_shaderName) const
	{
		auto it = m_shaders.find(a_shaderName);
		if (it != m_shaders.end())
		{
			return it->second;
		}
		else
			return nullptr;
	}

	TextureData* ResourceManager::GetSkyBoxTextureId(string a_skybox) const
	{
		auto it = m_SkyboxTextures.find(a_skybox);
		if (it != m_SkyboxTextures.end())
		{
			return it->second;
		}
		else
			return nullptr;
	}

    Material* ResourceManager::GetMaterial(string& a_strMaterialName) const
    {
        auto it = m_Materials.find(a_strMaterialName);
        if (it != m_Materials.end())
        {
            return it->second;
        }
        else
            return nullptr;
    }

	unordered_map<string, string> ResourceManager::GetScriptPath() const
	{
		return m_scriptPath;
	}

	void ResourceManager::UnloadAllResources(void)
	{
		for (auto it : m_archTypes)
		{
			SAFE_DELETE(it.second);
		}
		m_archTypes.clear();

		for (auto it : m_models)
		{
			SAFE_DELETE(it.second);
		}
		m_models.clear();

		for (auto it : m_textures)
		{
			SAFE_DELETE(it.second);
		}
		m_textures.clear();

		for (auto it : soundMap)
		{
			if (it.second)
			{
				PiAudio->ErrorCheck(it.second->release());
			}
		}
		soundMap.clear();


		for (auto it : channelMap)//mem leak add
		{
			if (it.second)
			{
				SAFE_DELETE(it.second);
			}
		}
		channelMap.clear();

		//TODO : clear channelMap if used, add further resources which need clearing


		for (auto it : m_Materials)//mem leak add
		{
			if (it.second)
			{
				SAFE_DELETE(it.second);
			}
		}
		m_Materials.clear();


		for (auto it : m_shaders)//mem leak add
		{
			if (it.second)
			{
				SAFE_DELETE(it.second);
			}
		}
		m_shaders.clear();
		
		for (auto it : m_SkyboxTextures)//mem leak add
		{
			if (it.second)
			{
				SAFE_DELETE(it.second);
			}
		}
		m_SkyboxTextures.clear();






	}

	ProjectConfig * ResourceManager::GetConfig(void)
	{
		return m_projectConfig;
	}

	void ResourceManager::HandleEvent(Event * a_event)
	{
		if (a_event->GetEventType() == EEventType::ReloadPrefabs)
		{
			LoadArchetypes(m_projectConfig->GetArchetypeDir());
			//TODO: remove after dev, will leak memory
			m_shaders.clear();
			LoadShaders(m_projectConfig->GetShaderDir());
			m_Materials.clear();
			LoadMaterials(m_projectConfig->GetMaterialDir());
		}
	}




