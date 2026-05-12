#include "Rendering/Essentials/Texture.h"

namespace NEXUS_RENDERING
{
  Texture::Texture()
    : Texture(0, 0, 0, "", TextureType::NONE)
  {
  }

  Texture::Texture(GLuint id, int width, int height, const std::string &path, TextureType type)
    : _TextureID(id),
      _Width(width), _Height(height),
      _Path(path),
      _eType(type)
  {
  }

  Texture::~Texture()
  {
    glDeleteTextures(1, &_TextureID);
  }

  void Texture::Bind() const
  {
    glBindTexture(GL_TEXTURE_2D, _TextureID);
  }

  void Texture::unBind() const
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}
