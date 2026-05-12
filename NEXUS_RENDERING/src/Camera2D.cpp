#include "Rendering/Core/Camera2D.h"

namespace NEXUS_RENDERING
{
  Camera2D::Camera2D()
    : Camera2D(640, 480)
  {
  }

  Camera2D::Camera2D(int width, int height)
    : _Width{width}, _Height{height}, 
      _Zoom(1.0f), 
      _Position{glm::vec2(0.0f)},
      _CameraMatrix{1.0f}, _OrthoProjection{1.0f},
      _NeedsUpdate{true}
  {
    // Init Ortho Projection
    _OrthoProjection = glm::ortho(
      0.0f,                             // Left 
      static_cast<float>(_Width),       // Right 
      static_cast<float>(_Height),      // Bottom
      0.0f,                             // Top 
      -1.0f,                            // Near
      1.0f                              // Far
    );

    Update();
  }

  Camera2D::~Camera2D()
  {
  }

  void Camera2D::Update()
  {
    if (_NeedsUpdate)
    {
      // translate
      glm::vec3 translate{-_Position.x, -_Position.y, 0.0f};
      _CameraMatrix = glm::translate(_OrthoProjection, translate);

      // scale
      glm::vec3 scale{_Zoom, _Zoom, 0.0f};
      _CameraMatrix *= glm::scale(glm::mat4(1.0f), scale);

      _NeedsUpdate = false;
    }
  }
};
