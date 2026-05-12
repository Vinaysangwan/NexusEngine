#pragma once

#include "Texture.h"

#include <memory>

namespace NEXUS_RENDERING
{
  class TextureLoader
  {
  public:
    TextureLoader() = delete;
    
    static std::unique_ptr<Texture> Create(Texture::TextureType type, const std::string &textureFilePath);

  private:
    static bool LoadTexture(const std::string &filePath, int &width, int &height, bool linear = false);

  };
}
