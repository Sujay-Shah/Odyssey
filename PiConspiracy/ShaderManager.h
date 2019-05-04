//
//  ShaderManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class ShaderManager
{
public:
 
    enum eShader
    {
        eShader_Vertex,     //0
        eShader_Fragment,   //1
        eShader_Geometry,   //2
        
        eShader_Max,        

        // Later when Needed 
        eShader_Compute,    //3
    };

	                    ShaderManager();
                        ~ShaderManager();
    
    bool                Initialize(const string& a_strFileName);

    string              LoadShader(const string& a_strFileName);
    void                CheckShaderError(GLuint a_uShader, GLuint a_uFlag, bool a_bIsProgram, const string& a_strErrorMsg,const std::string a_strFileName = "");

    GLuint              CreateShader(const string& a_strFileData, GLenum a_eShaderType, const std::string a_strFileName);
	
    void                Use();
	void				UnUse();
	void                LinkShader();
	void                ClearShader();


	void SetInt(unsigned int uShaderID, const std::string& name, int32 value) const;
	void SetUniform1f(unsigned int ShaderManager_ID, const char* uniname, float v1);
	void SetUniform2f(unsigned int ShaderManager_ID, const char* uniname, float v1, float v2);
	void SetUniform3f(unsigned int ShaderManager_ID, const char* uniname, float v1, float v2, float v3);
	void SetUniform4f(unsigned int ShaderManager_ID, const char* uniname, float v1, float v2, float v3, float v4);

    void SetUniformMatrix4fv(unsigned int ShaderManager_ID, const char* uniname, glm::mat4 matrix4x4);

    inline uint32 GetShaderID();

private: 
	uint32              m_uShaderID;

    GLuint              m_Shaders[eShader_Max];
};

inline uint32 ShaderManager::GetShaderID() 
{
    return m_uShaderID;
}
