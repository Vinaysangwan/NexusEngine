#include "Core/Resource/AssetManager.h"

#include <Logger/Logger.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/ShaderLoader.h>

namespace NEXUS_RESOURCES
{
  bool AssetManager::AddTexture(const std::string &textureName, const std::string &texturePath, bool pixelArt)
  {
    if (_MapTextures.find(textureName) != _MapTextures.end())
    {
      NEXUS_ERROR("Failed to Add Texture[{}]: Already Exist", textureName);
      return false;
    }

    auto texture = std::move(NEXUS_RENDERING::TextureLoader::Create(
      pixelArt ? NEXUS_RENDERING::Texture::TextureType::NEAREST : NEXUS_RENDERING::Texture::TextureType::LINEAR,
      texturePath
    ));
    if (!texture)
    {
      NEXUS_ERROR("Failed to Load texture{} at path: {}", textureName, texturePath);
      return false;
    }

    _MapTextures.emplace(textureName, std::move(texture));
    return true;
  }

  const NEXUS_RENDERING::Texture &AssetManager::GetTexture(const std::string &textureName)
  {
    auto it = _MapTextures.find(textureName);
    if (it == _MapTextures.end())
    {
      NEXUS_ERROR("Failed to get texture[{}]: Doesn't exist", textureName);

      NEXUS_RENDERING::Texture texture{};
      return texture;
    }

    return *it->second;
  }

  bool AssetManager::AddShader(const std::string &shaderName, const std::string &vertPath, const std::string &fragPath)
  {
    if (_MapShaders.find(shaderName) != _MapShaders.end())
    {
      NEXUS_ERROR("Failed to Add Shader[{}]: Already Exist!", shaderName);
      return false;
    }

    auto shader = std::move(NEXUS_RENDERING::ShaderLoader::Create(vertPath, fragPath));
    if (!shader)
    {
      NEXUS_ERROR("Failed to Load Shader[{}]: \nvertPath: {}, \nfragPath: {}", shaderName, vertPath, fragPath);
      return false;
    }

    _MapShaders.emplace(shaderName, std::move(shader));
    return true;
  }

  const NEXUS_RENDERING::Shader &AssetManager::GetShader(const std::string &shaderName)
  {
    auto it = _MapShaders.find(shaderName);
    if (it == _MapShaders.end())
    {
      NEXUS_ERROR("Failed to get Shader[{}]: Doesn't Exist!", shaderName);

      NEXUS_RENDERING::Shader shader{};
      return shader; 
    }

    return *it->second;
  }
}
