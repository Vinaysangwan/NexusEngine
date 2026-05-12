#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NEXUS_RENDERING
{
  class Camera2D
  {
  public:
    Camera2D();
    Camera2D(int width, int height);
    ~Camera2D();

    inline glm::mat4 &GetCameraMatrix() { return _CameraMatrix; }

    inline const float GetZoom() const {return _Zoom;}
    inline void SetZoom(float zoom) 
    {
      _Zoom = zoom; 
      _NeedsUpdate = true;
    }

    inline const glm::vec2 GetPosition() const {return _Position;}
    inline void SetPosition(glm::vec2 pos)
    {
      _Position = pos;
      _NeedsUpdate = true;
    }

    void Update();

  private:
    int _Width, _Height;
    float _Zoom;

    glm::vec2 _Position;
    glm::mat4 _CameraMatrix, _OrthoProjection;

    bool _NeedsUpdate;
  };
}
