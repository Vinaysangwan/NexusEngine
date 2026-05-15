#pragma once

#include <Rendering/Essentials/Vertex.h>

namespace NEXUS_CORE::ECS
{
  struct UVs
  {
    float u{0.0f};
    float v{0.0f};
    float width{0.0f};
    float height{0.0f};
  };
  
  struct SpriteComponent
  {
    int startX{0}, startY{0};
    float width{0.0f}, height{0.0f};

    NEXUS_RENDERING::Color color{.r=255, .g=255, .b=255, .a=255};

    UVs uvs{.u = 0.f, .v = 0.f, .width = 0.f, .height = 0.f};

    inline void generate_uvs(int textureWidth, int textureHeight)
    {
      uvs.width = width / textureWidth;
      uvs.height = height / textureHeight;

      uvs.u = startX * uvs.width;
      uvs.v = startY * uvs.height;
    }
  };
}
