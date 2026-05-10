#include "Rendering/Essentials/Shader.h"

#include <iostream>

namespace NEXUS_RENDERING
{
  Shader::Shader(GLuint programID, const std::string &vertFilePath, const std::string &fragFilePath)
    : _ProgramID(programID), vertFilePath(vertFilePath), fragFilePath(fragFilePath)
  {
  }

  Shader::~Shader()
  {
    glDeleteProgram(_ProgramID);
  }

  void Shader::SetUniformInt(const std::string &name, int value)
  {
    glUniform1i(GetUniformLocation(name), value);
  }

  void Shader::SetUniformMat4(const std::string &name, glm::mat4 &mat)
  {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
  }

  GLint Shader::GetUniformLocation(const std::string &name)
  {
    auto it = _UniformLocationMap.find(name);
    if (it != _UniformLocationMap.end())
    {
      return it->second;
    }

    GLint location = glGetUniformLocation(_ProgramID, name.c_str());
    if (location < 0)
    {
      std::cout<<"For Shader Program["<<_ProgramID<<"] => Failed to Get Uniform Location: "<<name<<std::endl;
      return -1;
    }

    _UniformLocationMap.emplace(name, location);
    return location;
  }
}
