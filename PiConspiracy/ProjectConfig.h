//
//  ProjectConfig.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

	class JsonParser;

	class ProjectConfig : public Singleton<ProjectConfig>
	{
	public:
		ProjectConfig(void);
		~ProjectConfig(void);

		void LoadConfig(const string& a_filePath);

		// Accessors
		string GetAssetDir(void) const;
		string GetTextureDir(void) const;
		string GetShaderDir(void) const;
		string GetSFXDir(void) const;
		string GetSceneDir(void) const;
		string GetModelDir(void) const;
		string GetArchetypeDir(void) const;
		string GetMaterialDir(void) const;
		EInputType GetInputConfig(void) const;
		bool GetIsMusicOn(void) const;

		const string& GetHighScoreDir();

		
		//move to private
		bool m_isMute;
		bool m_isMusicOn;
		bool m_autoMove;
		int m_startScene;
		bool m_presentation;
		//temp dev vars, remove after development is complete
		float m_fovAngle;
		bool m_updateUI;
		bool m_withPlayer;
		bool m_enableMouse;
		bool m_enableWindowEvents;

		/*int getHighScore() const
		{
			return m_HighScore;
		}*/
		int m_HighScore;
	private:
		string m_assetDir;
		string m_textureDir;
		string m_shaderDir;
		string m_sfxDir;
		string m_archetypeDir;
		string m_sceneDir;
		string m_modelDir;
		string m_scriptDir;
		string m_materialDir;
		string m_highScoreDir;
		
		EInputType m_selectedInputType;
		int LoadHighScore(const string& a_HighScoreFilepath);
	};



