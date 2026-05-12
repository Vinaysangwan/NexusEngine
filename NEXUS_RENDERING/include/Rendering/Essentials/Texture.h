#pragma once

#include <glad/glad.h>
#include <string>

namespace NEXUS_RENDERING
{
  class Texture
  {
  public:
    enum class TextureType
    {
      NEAREST = 0,
      LINEAR,
      NONE
    };

  public:
    Texture();
    Texture(GLuint id, int width, int height, const std::string &path, TextureType type = TextureType::NEAREST);
    ~Texture();

    void Bind() const;
    void unBind() const;

    inline const int GetWidth() const {return _Width;}
    inline const int GetHeight() const {return _Height;}
    inline const GLuint GetID() const {return _TextureID;}


  private:
    GLuint _TextureID;
    int _Width, _Height;
    std::string _Path;
    TextureType _eType;
  };
}
