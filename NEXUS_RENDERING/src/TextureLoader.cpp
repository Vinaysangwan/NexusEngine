#include "Rendering/Essentials/TextureLoader.h"
#include <Logger/Logger.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assert.h>

namespace NEXUS_RENDERING
{
  std::unique_ptr<Texture> TextureLoader::Create(Texture::TextureType type, const std::string &textureFilePath)
  {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height;

    switch (type)
    {
    case Texture::TextureType::NEAREST:
    {
      LoadTexture(textureFilePath, width, height, false);
    } break;

    case Texture::TextureType::LINEAR:
    {
      LoadTexture(textureFilePath, width, height, true);
    } break;

    default:
    {
      assert(false && "The current Texture Type is not defined!");
    }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    
    return std::make_unique<Texture>(textureID, width, height, textureFilePath, type);
  }

  bool TextureLoader::LoadTexture(const std::string &filePath, int &width, int &height, bool linear)
  {
    int nChannels;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nChannels, 4);
    if(!data)
    {
      NEXUS_ERROR("Failed to Open the texture: {}", filePath);
      return false;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    
    return true;
  }
}
