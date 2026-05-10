#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace NEXUS_RENDERING
{
  class Shader
  {
  public:
    Shader(GLuint programID, const std::string &vertFilePath, const std::string &fragFilePath);
    ~Shader();

    inline void Enable() const { glUseProgram(_ProgramID); }
    inline void Disable() const { glUseProgram(0); }

    inline const GLuint GetProgramID() const { return _ProgramID; }

    // TODO: Add more uniform Setter
    void SetUniformInt(const std::string &name, int value);
    void SetUniformMat4(const std::string &name, glm::mat4 &mat);
    
  private:
    GLint GetUniformLocation(const std::string& name);

  private:
    GLuint _ProgramID;
    std::string vertFilePath, fragFilePath;

    std::unordered_map<std::string, GLint> _UniformLocationMap;
  };
}
