#pragma once

#include <glad/glad.h>
#include <Windowing/Window.h>
#include <Core/ECS/Registry.h>

namespace NEXUS_EDITOR
{
  class Application
  {
  public:
    static Application& GetInstance();
    ~Application();

    void Run();

  private:
    Application();
  
    bool Initialize();
    bool LoadShaders();

    void ProcessEvents();
    void Update();
    void Render();

    void Cleanup();

  private:
    std::unique_ptr<NEXUS_WINDOWING::Window> _Window;
    std::unique_ptr<NEXUS_CORE::ECS::Registry> _Registry;

    SDL_Event _Event;
    bool _Running;

    GLuint VAO, VBO, IBO;
  };
}
