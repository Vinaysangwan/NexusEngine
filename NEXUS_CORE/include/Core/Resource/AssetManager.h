#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include <Rendering/Essentials/Shader.h>
#include <Rendering/Essentials/Texture.h>

namespace NEXUS_RESOURCES
{
  class AssetManager
  {
  public:
    AssetManager() = default;
    ~AssetManager() = default;

    bool AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt = true);
    NEXUS_RENDERING::Texture& GetTexture(const std::string& textureName);

    bool AddShader(const std::string& shaderName, const std::string& vertPath, const std::string& fragPath);
    NEXUS_RENDERING::Shader& GetShader(const std::string& shaderName);

  private:
    std::unordered_map<std::string, std::shared_ptr<NEXUS_RENDERING::Texture>> _MapTextures{};
    std::unordered_map<std::string, std::shared_ptr<NEXUS_RENDERING::Shader>> _MapShaders{};
  };
}
