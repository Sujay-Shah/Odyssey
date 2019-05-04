//
//  ResourceManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

namespace FMOD
{
	class Channel;
	class ChannelGroup;
	class Sound;
	namespace Studio
	{
		class EventInstance;
		class Bank;
		class System;
	}
	class System;
}

class FileParser;
struct DirectoryObject;
class ProjectConfig;
class TextureData;
class Model;
class Material;
class ResourceManager : public Singleton<ResourceManager>
{
public:
	ResourceManager(void);
	~ResourceManager(void);

	void Initialize();
	const DirectoryObject* GetFilePath(const string& a_fileName) const;
	const DirectoryObject* GetFilePath(const uint32 a_fileHash) const;

	// for AudioManager
	FMOD::Sound* GetSoundFromMap(const string &a_name);
	std::map<int, FMOD::Channel*> GetChannelMap();
	void LoadSound(const string &a_audioValue, const string &a_audioName, FMOD::System* a_system, bool a_isAudio3D = true, bool a_isAudioLooping = false, bool a_isAudioStreaming = false);
	void LoadMusicAudioLibrary();
	void LoadSFXAudioLibrary();
	void LoadScripts();
	//TODO: move this to loaders
		
	json* GetArchetype(string a_archetypeName) const;
	TextureData* GetTexture(string a_textureName) const;
	Model* GetModel(string a_modelName) const;
	ShaderManager* GetShader(string a_shaderName) const;
	TextureData* GetSkyBoxTextureId(string) const;
    Material*   GetMaterial(string& a_strMaterialName) const;
	unordered_map<string, string> GetScriptPath() const;

	void UnloadAllResources(void);
	ProjectConfig* GetConfig(void);
	void HandleEvent(Event* a_event);

	//For Stats Manager
	/*int getHighScore() const
	{
		return m_HighScore;
	}

	int m_HighScore;*/
private:
	FileParser* m_fp;
	ProjectConfig* m_projectConfig;
	unordered_map<uint32, DirectoryObject*> m_Elements;

	//Resources
	unordered_map<string, json*> m_archTypes;
	unordered_map<string, TextureData*> m_textures;
	unordered_map<string, Model*> m_models;
	unordered_map<string, ShaderManager*> m_shaders;
	unordered_map<string, TextureData*> m_SkyboxTextures;
    unordered_map<string, Material*> m_Materials;
	unordered_map<string, string> m_scriptPath;


	// for loading audio files
	std::map<string, FMOD::Sound*> soundMap;
	std::map<int, FMOD::Channel*> channelMap;

    // TODO: Rohit serialize the sky box textures
    std::vector<string> m_CubeFaces;

	//TODO: move this to specialized loaders
	void LoadArchetypes(const string& a_prefabPath);
	void LoadTextures(const string& a_texturePath, bool a_bIsSkyBox);
	void LoadModels(const string& a_model);
	void LoadShaders(const string& a_shaderPath);
    void LoadMaterials(const string& a_Materials);
	//int LoadHighScore(const string& a_HighScoreFilepath);
};


