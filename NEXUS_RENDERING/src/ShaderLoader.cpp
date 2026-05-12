#include "Rendering/Essentials/ShaderLoader.h"
#include <Logger/Logger.h>

#include <iostream>
#include <fstream>

namespace NEXUS_RENDERING
{
  std::unique_ptr<Shader> ShaderLoader::Create(const std::string &vertFilePath, const std::string &fragFilePath)
  {
    GLuint programID = CreateProgram(vertFilePath, fragFilePath);
    if (programID)
    {
      return std::make_unique<Shader>(programID, vertFilePath, fragFilePath);
    }
    
    return nullptr;
  }

  GLuint ShaderLoader::CreateProgram(const std::string& vertFilePath, const std::string& fragFilePath)
  {
    GLuint vertID = ShaderLoader::CompileShader(GL_VERTEX_SHADER, vertFilePath);
    GLuint fragID = ShaderLoader::CompileShader(GL_FRAGMENT_SHADER, fragFilePath);

    if (vertID == 0 || fragID == 0)
      return 0;

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertID);
    glAttachShader(programID, fragID);
    glLinkProgram(programID);
    glValidateProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
      char infoLog[1024] = {0};
      glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
      NEXUS_ERROR("Failed to Link program[{}]", programID);

      glDetachShader(programID, vertID);
      glDetachShader(programID, fragID);
      glDeleteShader(vertID);
      glDeleteShader(fragID);

      return 0;
    }

    glDetachShader(programID, vertID);
    glDetachShader(programID, fragID);
    glDeleteShader(vertID);
    glDeleteShader(fragID);

    return programID;
  }

  GLuint ShaderLoader::CompileShader(GLenum type, const std::string &filePath)
  {
    std::ifstream file(filePath);
    if (file.fail())
    {
      NEXUS_ERROR("Failed to Open the Shader File: {}", filePath);
      return 0;
    }

    std::string content{""};
    std::string line{""};
    while(std::getline(file, line))
    {
      content += line + "\n";
    }
    file.close();

    const char* sourceCode = content.c_str();

    GLuint shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &sourceCode, nullptr);
    glCompileShader(shaderID);

    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
      char infoLog[1024] = {0};
      glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
      const char* shaderName = type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader";
      NEXUS_ERROR("Failed to Compile {}: {}", shaderName, infoLog);

      glDeleteShader(shaderID);
      return 0;
    }

    return shaderID;
  }
}