#pragma once

#include "Shader.h"

#include <memory>

namespace NEXUS_RENDERING
{
  class ShaderLoader
  {
  public:
    ShaderLoader() = delete;

    static std::unique_ptr<Shader> Create(const std::string& vertFilePath, const std::string& fragFilePath);
    
  private:
    static GLuint CreateProgram(const std::string& vertFilePath, const std::string& fragFilePath);
    static GLuint CompileShader(GLenum type, const std::string &filePath);
  };
}
