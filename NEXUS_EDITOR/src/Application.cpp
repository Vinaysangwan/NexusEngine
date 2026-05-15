#include "Application.h"

#define SDL_MAIN_HANDLED 1
#define NOMINMAX

#include <SDL3/SDL.h>
#include <Logger/Logger.h>
#include <Rendering/Essentials/ShaderLoader.h> 
#include <Rendering/Essentials/TextureLoader.h> 
#include <Rendering/Essentials/Vertex.h> 
#include <Rendering/Core/Camera2D.h>
#include <Core/ECS/Entity.h>
#include <Core/ECS/Components/TransformComponent.h>
#include <Core/ECS/Components/SpriteComponent.h>
#include <Core/ECS/Components/Identification.h>
#include <Core/Resource/AssetManager.h>
#include <Core/Systems/ScriptingSystem.h>

#include <vector>

namespace NEXUS_EDITOR
{
  std::string AssetPath(const std::string& path)
  {
    return std::string(ASSET_PATH) + path;
  }
  
  Application& Application::GetInstance()
  {
    static Application app{};
    return app;
  }

  Application::~Application()
  {
  }

  Application::Application()
    : _Window{nullptr}, _Registry{nullptr}, _Event{}, _Running{true},
      VAO{0}, VBO{0}, IBO{0}
  {
  }

  void Application::Run()
  {
    if (!Initialize())
    {
      NEXUS_ERROR("Failed to Initialize Application");
      return;
    }
    
    // application loop
    while(_Running)
    {
      ProcessEvents();
      
      Update();

      Render();
    }

    Cleanup();
  }

  bool Application::Initialize()
  {
    // init logger
    NEXUS_INIT_LOGS(true, true);
    
    // init SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
      std::string error = SDL_GetError();
      NEXUS_ERROR("Failed to Init SDL: {}", error);
      return false;
    }

    // Set Up OpenGL
    if (!SDL_GL_LoadLibrary(NULL) != 0)
    {
      std::string error = SDL_GetError();
      NEXUS_ERROR("Failed to Open OpenGL Library: {}", error);
      return false;
    }

    // OpenGL Attribs
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // number of bits per channel
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    // Init Window
    _Window = std::make_unique<NEXUS_WINDOWING::Window>("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
      true, SDL_WINDOW_OPENGL);
    if (!_Window->GetWindow())
    {
      NEXUS_ERROR("Failed to Create Window!");
      return false;
    }

    // Create the OpenGL Context
    _Window->SetGLContext(SDL_GL_CreateContext(_Window->GetWindow().get()));
    if (!_Window->GetGLContext())
    {
      std::string error = SDL_GetError();
      NEXUS_ERROR("Failed to Create OpenGL Content: {}", error);
      return false;
    }
    SDL_GL_MakeCurrent(_Window->GetWindow().get(), _Window->GetGLContext());
    SDL_GL_SetSwapInterval(1);

    // Init Glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
      NEXUS_ERROR("Failed to Init GLAD");
      return false;
    }

    // Enable Alpha Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Display GPU Info
    NEXUS_LOG(
      R"(GPU INFO:
      Vendor: {}
      Renderer: {}
      OpenGL: {}
      GLSL: {})",

      reinterpret_cast<const char*>(glGetString(GL_VENDOR)),
      reinterpret_cast<const char*>(glGetString(GL_RENDERER)),
      reinterpret_cast<const char*>(glGetString(GL_VERSION)),
      reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))
    );  

    // Init registry
    _Registry = std::make_unique<NEXUS_CORE::ECS::Registry>();

    // Init Asset Manager
    auto assetManager = std::make_shared<NEXUS_RESOURCES::AssetManager>();
    if (!assetManager)
    {
      NEXUS_ERROR("Failed to Create Asset Manager");
      return false;
    }
    // NEXUS_LOG("AssetManager ptr: {}", (void*)assetManager.get());

    if (!_Registry->AddToContext(assetManager))
    {
      NEXUS_ERROR("Failed to add AssetManager to the Registry Context");
      return false;
    }

    // init lua state
    auto lua = std::make_shared<sol::state>();
    if (!lua)
    {
      NEXUS_ERROR("Failed to Create Lua State");
      return false;
    }

    lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::table, sol::lib::io, sol::lib::string);

    if(!_Registry->AddToContext(lua))
    {
      NEXUS_ERROR("Failed to add lua state to Registry Context");
      return false;
    }

    // init script system
    auto scriptSystem = std::make_shared<NEXUS_CORE::Systems::ScriptingSystem>(*_Registry);
    if (!scriptSystem)
    {
      NEXUS_ERROR("Failed to Create the Script System");
      return false;
    }

    if (!scriptSystem->LoadMainScript(*lua))
    {
      NEXUS_ERROR("Failed to load the main lua script");
      return false;
    }

    if(!_Registry->AddToContext(scriptSystem))
    {
      NEXUS_ERROR("Failed to add Script System to Registry Context");
      return false;
    }

    // Init camera
    auto camera = std::make_shared<NEXUS_RENDERING::Camera2D>();
    camera->SetZoom(2.0f);

    if (!_Registry->AddToContext(camera))
    {
      NEXUS_ERROR("Failed to add Camera2D to Register Context!");
      return false;
    }

    // Init Shaders
    if (!LoadShaders())
    {
      NEXUS_ERROR("Failed to Load Shaders");
      return false;
    }

    // init texture
    if (!assetManager->AddTexture("WorldTileSet", AssetPath("textures/world_tileset.png")))
    {
      return false;
    }
    auto &texture = assetManager->GetTexture("WorldTileSet");

    NEXUS_LOG(
      R"(Texture Loaded Successfully:
      ID: {},
      Width: {},
      Height: {}
      )",
      texture.GetID(), texture.GetWidth(), texture.GetHeight()
    );

    // init entity
    NEXUS_CORE::ECS::Entity entity1{*_Registry, "Ent1", "Test"};

    auto &transform = entity1.AddComponent<NEXUS_CORE::ECS::TransformComponent>(NEXUS_CORE::ECS::TransformComponent{
      .position = glm::vec2{50.0f},
      .scale = glm::vec2{1.0f},
      .rotation = 0.0f
    });

    auto &sprite = entity1.AddComponent<NEXUS_CORE::ECS::SpriteComponent>(NEXUS_CORE::ECS::SpriteComponent{
      .startX = 11,
      .startY = 8,
      .width = 16.0f,
      .height = 16.0f,
      .color = {.r = 255, .g = 255, .b = 255, .a = 255}
    });
    sprite.generate_uvs(texture.GetWidth(), texture.GetHeight());

    // init vertices
    std::vector<NEXUS_RENDERING::Vertex> vertices;
    NEXUS_RENDERING::Vertex vTL = {
      .position = transform.position,
      .uvs = {sprite.uvs.u, sprite.uvs.v},
      .color = sprite.color
    };
    NEXUS_RENDERING::Vertex vTR = {
      .position = transform.position + glm::vec2{sprite.width, 0},
      .uvs = {sprite.uvs.u + sprite.uvs.width, sprite.uvs.v},
      .color = sprite.color
    };
    NEXUS_RENDERING::Vertex vBL = {
      .position = transform.position + glm::vec2{0, sprite.height},
      .uvs = {sprite.uvs.u, sprite.uvs.v + sprite.uvs.height},
      .color = sprite.color
    };
    NEXUS_RENDERING::Vertex vBR = {
      .position = transform.position + glm::vec2{sprite.width, sprite.height},
      .uvs = {sprite.uvs.u + sprite.uvs.width, sprite.uvs.v + sprite.uvs.height},
      .color = sprite.color
    };

    vertices.push_back(vTL);
    vertices.push_back(vTR);
    vertices.push_back(vBL);
    vertices.push_back(vBR);

    auto& id = entity1.GetComponent<NEXUS_CORE::ECS::Identification>();
    NEXUS_LOG("Name: {}, Group: {}, id: {}", id.name, id.group, id.entity_id);

    // indices
    unsigned int indices[] = {
      0, 1, 2,
      1, 2, 3
    };

    // vao
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // ibo
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vbo
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(NEXUS_RENDERING::Vertex), vertices.data(), GL_STATIC_DRAW);

    // vertex attrib pointers
    // inPos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 
      sizeof(NEXUS_RENDERING::Vertex),
      (void*)offsetof(NEXUS_RENDERING::Vertex, position)
    );
    glEnableVertexAttribArray(0);

    // inUVs
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
      sizeof(NEXUS_RENDERING::Vertex),
      (void*)offsetof(NEXUS_RENDERING::Vertex, uvs)
    );
    glEnableVertexAttribArray(1);

    // inColor
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 
      sizeof(NEXUS_RENDERING::Vertex),
      (void*)offsetof(NEXUS_RENDERING::Vertex, color)
    );
    glEnableVertexAttribArray(2);

    return true;
  }

  bool Application::LoadShaders()
  {
    auto& assetManager = _Registry->GetContext<std::shared_ptr<NEXUS_RESOURCES::AssetManager>>();
    if (!assetManager)
    {
      NEXUS_ERROR("Falied to Get AssetManager Context");
      return false;
    }
    
    if (!assetManager->AddShader("Quad", AssetPath("shaders/quad.vert"), AssetPath("shaders/quad.frag")))
    {
      return false;
    }
    auto &shader = assetManager->GetShader("Quad");
    NEXUS_LOG("Shader Loaded Successfully: \n\tID: {}", shader.GetProgramID());

    return true;
  }

  void Application::ProcessEvents()
  {
    while (SDL_PollEvent(&_Event))
    {
      switch(_Event.type)
      {
      case SDL_EVENT_QUIT:
      {
        _Running = false;
      } break;

      case SDL_EVENT_KEY_DOWN:
      {
        if (_Event.key.key == SDLK_ESCAPE)
        {
          _Running = false;
        }
      } break;
      }
    }
  }

  void Application::Update()
  {
    auto &camera = _Registry->GetContext<std::shared_ptr<NEXUS_RENDERING::Camera2D>>();
    if (!camera)
    {
      NEXUS_ERROR("Failed to get Camera2D context");
      return;
    }

    camera->Update();

    auto& scriptSystem = _Registry->GetContext<std::shared_ptr<NEXUS_CORE::Systems::ScriptingSystem>>();
    scriptSystem->Update();
  }

  void Application::Render()
  {
    auto& assetManager = _Registry->GetContext<std::shared_ptr<NEXUS_RESOURCES::AssetManager>>();
    if (!assetManager)
    {
      NEXUS_ERROR("Falied to Get AssetManager Context");
      return;
    }

    auto &camera = _Registry->GetContext<std::shared_ptr<NEXUS_RENDERING::Camera2D>>();
    if (!camera)
    {
      NEXUS_ERROR("Failed to get Camera2D context");
      return;
    }

    auto& shader = assetManager->GetShader("Quad");
    const auto& texture = assetManager->GetTexture("WorldTileSet");

    glViewport(0, 0, _Window->GetWidth(), _Window->GetHeight());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.Enable();
    glBindVertexArray(VAO);

    // bind texture
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    // Set Projection Matrix
    glm::mat4 projectionMatrix = camera->GetCameraMatrix();
    shader.SetUniformMat4("uProjection", projectionMatrix);

    // Draw elements
    auto& scriptSystem = _Registry->GetContext<std::shared_ptr<NEXUS_CORE::Systems::ScriptingSystem>>();
    scriptSystem->Render();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    texture.unBind();
    glBindVertexArray(0);
    shader.Disable();

    SDL_GL_SwapWindow(_Window->GetWindow().get());
  }

  void Application::Cleanup()
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &IBO);

    SDL_Quit();

    NEXUS_LOG("Closing");
  }
}
