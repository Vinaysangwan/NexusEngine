#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace NEXUS_RENDERING
{
  struct Color
  {
    GLubyte r, g, b, a;
  };
  
  struct Vertex
  {
    glm::vec2 position{0.0f};
    glm::vec2 uvs{0.0f};
    Color color {.r = 255, .g = 255, .b = 255, .a = 255};

    inline void set_color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
    {
      color.r = r;
      color.g = g;
      color.b = b;
      color.a = a;
    }

    inline void set_color(GLuint newColor)
    {
      color.r = (newColor >> 24) & 0xFF;
      color.g = (newColor >> 16) & 0xFF;
      color.b = (newColor >> 8) & 0xFF;
      color.a = (newColor) & 0xFF;
    }
  };
}
