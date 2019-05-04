//
//  ProjectConfig.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ProjectConfig.h"
#include "JsonReader.h"
#include "nlohmann/json.hpp"
//temp
///
#include <winerror.h> //for HRESULT
#include <comutil.h> //for _bstr_t (used in the string conversion)
#include "shlobj.h"
//
//temp
#pragma comment(lib, "comsuppw")
//temp

namespace fs = std::experimental::filesystem;

	ProjectConfig::ProjectConfig(void)
	{

	}

	ProjectConfig::~ProjectConfig(void)
	{
	}

	void ProjectConfig::LoadConfig(const string& a_filePath)
	{
		json j = OpenJsonFile(a_filePath);
		json gameSettings = j["GAME_SETTINGS"];

		m_assetDir = ParseString(gameSettings,"assetDir");
		m_textureDir = m_assetDir + ParseString(gameSettings, "texturesDir");
		m_archetypeDir = m_assetDir + ParseString(gameSettings, "archetypesDir");
		m_sceneDir = m_assetDir + ParseString(gameSettings, "scenesDir");
		m_modelDir = m_assetDir + ParseString(gameSettings, "modelsDir");
		m_materialDir = m_assetDir + ParseString(gameSettings, "materialsDir");
		m_scriptDir = m_assetDir + ParseString(gameSettings, "scriptsDir");
		m_shaderDir = m_assetDir + ParseString(gameSettings, "shadersDir");
		m_highScoreDir =  ParseString(gameSettings, "highScoreDir");

		m_selectedInputType = (EInputType)ParseInt(gameSettings,"inputType");
		m_isMusicOn = ParseBool(gameSettings, "IsMusicOn");
		m_autoMove = ParseBool(gameSettings, "autoAcceleration");
		m_startScene = ParseInt(gameSettings, "startScene");
		m_fovAngle = ParseFloat(gameSettings, "fovAngle");
		m_updateUI = ParseBool(gameSettings, "updateUI");
		m_withPlayer = ParseBool(gameSettings, "withPlayer");
		m_isMute = ParseBool(gameSettings, "isMute");
		m_enableMouse = ParseBool(gameSettings, "enableJustMouseForUI");
		m_enableWindowEvents = ParseBool(gameSettings, "enableWindowEvents");
		m_presentation = ParseBool(gameSettings, "presentation");

		LPWSTR wszPath = NULL;
		HRESULT hr;
		string file_name;
		hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &wszPath);

		if (SUCCEEDED(hr)) {
			_bstr_t bstrPath(wszPath);
			file_name = std::string((char*)bstrPath) + "\\Odyssey\\";
			//CreateDirectory(file_name.c_str(), NULL);
			file_name += GetHighScoreDir();
		}

		m_HighScore = LoadHighScore(file_name);

	}
	string ProjectConfig::GetAssetDir(void) const
	{
		return m_assetDir;
	}
	string ProjectConfig::GetTextureDir(void) const
	{
		return m_textureDir;
	}
	string ProjectConfig::GetShaderDir(void) const
	{
		return m_shaderDir;
	}
	string ProjectConfig::GetSFXDir(void) const
	{
		return m_sfxDir;
	}

	string ProjectConfig::GetSceneDir(void) const
	{
		return m_sceneDir;
	}

	string ProjectConfig::GetModelDir(void) const
	{
		return m_modelDir;
	}

	string ProjectConfig::GetArchetypeDir(void) const
	{
		return m_archetypeDir;
	}

	string ProjectConfig::GetMaterialDir(void) const
	{
		return m_materialDir;
	}

	EInputType ProjectConfig::GetInputConfig(void) const
	{
		return m_selectedInputType;
	}

	bool ProjectConfig::GetIsMusicOn(void) const
	{
		return m_isMusicOn;
	}

	const string & ProjectConfig::GetHighScoreDir()
	{
		return m_highScoreDir;
	}

	int ProjectConfig::LoadHighScore(const string & a_HighScoreFilepath)
	{
		std::ifstream file;
		file.open((a_HighScoreFilepath).c_str());

		string output;
		string line;

		if (file.is_open())//
		{
			while (file.good())
			{
				getline(file, line);
				output.append(line + "\n");
			}
		}
		else
		{
			//return 0;
			std::cerr << "Unable to load High Score File: " << a_HighScoreFilepath << std::endl;
		}

		m_HighScore = std::atoi(output.c_str());

		return m_HighScore;
	}
