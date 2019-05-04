//
//  ShaderManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
    ZERO_MEM(m_Shaders);
}

ShaderManager::~ShaderManager()
{
    ClearShader();
}

// TO DO: (Load from Resource Manager)
string ShaderManager::LoadShader(const string& a_strFileName)
{
    std::ifstream file;
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
        std::cerr << "Unable to load shader: " << a_strFileName << std::endl;
    }

    return output;
}

void ShaderManager::CheckShaderError(GLuint a_uShader, GLuint a_uFlag, bool a_bIsProgram, const string& a_strErrorMsg,const std::string a_strFileName)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if (a_bIsProgram)
    {
        glGetProgramiv(a_uShader, a_uFlag, &success);
    }
    else
    {
        glGetShaderiv(a_uShader, a_uFlag, &success);
    }

    if (success == GL_FALSE)
    {
        if (a_bIsProgram)
        {
            glGetProgramInfoLog(a_uShader, sizeof(error), NULL, error);
        }
        else
        {
            glGetShaderInfoLog(a_uShader, sizeof(error), NULL, error);
        }
		std::cerr << a_strErrorMsg << ": " << error << "'" << "failed for file " << a_strFileName << std::endl;
    }
}

GLuint ShaderManager::CreateShader(const std::string& a_strFileData, GLenum a_eShaderType , const std::string a_strFileName)
{
    GLuint uShader = glCreateShader(a_eShaderType);

    if (uShader == 0)
    {
        std::cerr << "Error : Shader Creation failed! " << std::endl;
    }

    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringLengths[1];

    shaderSourceStrings[0] = a_strFileData.c_str();
    shaderSourceStringLengths[0] = (GLint)a_strFileData.length();

    glShaderSource(uShader, 1, shaderSourceStrings, NULL);

    glCompileShader(uShader);

    CheckShaderError(uShader, GL_COMPILE_STATUS, false, "Error: Shader Compilation failed: \n" , a_strFileName);

    return uShader;
}

bool ShaderManager::Initialize(const string& a_strFileName)
{
    m_Shaders[eShader_Vertex] = CreateShader(LoadShader(a_strFileName + ".VS"), GL_VERTEX_SHADER , a_strFileName);
    m_Shaders[eShader_Fragment] = CreateShader(LoadShader(a_strFileName + ".FS"), GL_FRAGMENT_SHADER, a_strFileName);
   
	if (a_strFileName == "Assets/Shaders/Particle")
	{
		m_Shaders[eShader_Geometry] = CreateShader(LoadShader(a_strFileName + ".GS"), GL_GEOMETRY_SHADER, a_strFileName);
	}

	if (a_strFileName == "Assets/Shaders/Billboard")
	{
		m_Shaders[eShader_Geometry] = CreateShader(LoadShader(a_strFileName + ".GS"), GL_GEOMETRY_SHADER, a_strFileName);
	}

    m_uShaderID = glCreateProgram();
    for (uint32 i = 0; i < eShader_Max; ++i)
    {
        glAttachShader(m_uShaderID, m_Shaders[i]);
    }

    LinkShader();
	//TODO : remove use from here after, renderer queries shader resources from resource Manager
    Use();

    return true;
}

void ShaderManager::LinkShader()
{
    glLinkProgram(m_uShaderID);
    CheckShaderError(m_uShaderID, GL_LINK_STATUS, true, " Error: Shader Program failed to link: \n");
	
    glValidateProgram(m_uShaderID);
    CheckShaderError(m_uShaderID, GL_VALIDATE_STATUS, true, " Error: Shader Program is invalid: \n");
}


void ShaderManager::Use()
{
	glUseProgram(m_uShaderID);
}

void ShaderManager::UnUse()
{
	glUseProgram(0);
}


void ShaderManager::ClearShader()
{
    for (uint32 i = 0; i < eShader_Max; ++i)
    {
        glDetachShader(m_uShaderID, m_Shaders[i]);
        glDeleteShader(m_Shaders[i]);
    }

	glDeleteProgram(m_uShaderID);
}

void ShaderManager::SetInt(unsigned int uShaderID, const std::string& name, int32 value) const
{
	glUniform1i(glGetUniformLocation(uShaderID, name.c_str()), value);
}

void ShaderManager::SetUniform1f(unsigned int shader_ID, const char * uniname, float v1)
{
	glUniform1f(glGetUniformLocation(shader_ID, uniname), v1);
}

void ShaderManager::SetUniform2f(unsigned int shader_ID, const char * uniname, float v1, float v2)
{
	glUniform2f(glGetUniformLocation(shader_ID, uniname), v1, v2);
}

void ShaderManager::SetUniform3f(unsigned int shader_ID, const char* uniname,float v1, float v2, float v3)
{
	glUniform3f(glGetUniformLocation(shader_ID, uniname), v1, v2, v3);

}

void ShaderManager::SetUniform4f(unsigned int shader_ID, const char* uniname, float v1, float v2, float v3, float v4)
{
	glUniform4f(glGetUniformLocation(shader_ID, uniname), v1, v2, v3, v4);
}

void ShaderManager::SetUniformMatrix4fv(unsigned int shader_ID, const char * uniname, glm::mat4 matrix4x4)
{
	glUniformMatrix4fv(glGetUniformLocation(shader_ID, uniname), 1, GL_FALSE, glm::value_ptr(matrix4x4));
}
