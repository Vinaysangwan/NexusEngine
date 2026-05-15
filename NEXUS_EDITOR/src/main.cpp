#define SDL_MAIN_HANDLED 1
#define NOMINMAX

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <Windowing/Window.h>
#include <Logger/Logger.h>
#include <Rendering/Essentials/ShaderLoader.h> 
#include <Rendering/Essentials/TextureLoader.h> 
#include <Rendering/Essentials/Vertex.h> 
#include <Rendering/Core/Camera2D.h>

#include <vector>
#include <entt.hpp>

std::string AssetPath(const std::string& path)
{
  return std::string(ASSET_PATH) + path;
}

struct UVs
{
  float u{0.0f};
  float v{0.0f};
  float width{0.0f};
  float height{0.0f};
};

struct TransformComponent
{
  glm::vec2 position{glm::vec2{0.0f}};
  glm::vec2 scale{glm::vec2{1.0f}};
  float rotation{0.0f};
};

struct SpriteComponent
{
  int startX{0}, startY{0};
  float width{0.0f}, height{0.0f};

  NEXUS_RENDERING::Color color{.r=255, .g=255, .b=255, .a=255};

  UVs uvs{.u = 0.f, .v = 0.f, .width = 0.f, .height = 0.f};

  void generate_uvs(int textureWidth, int textureHeight)
  {
    uvs.width = width / textureWidth;
    uvs.height = height / textureHeight;

    uvs.u = startX * uvs.width;
    uvs.v = startY * uvs.height;
  }
};

int main()
{
  // init logger
  NEXUS_INIT_LOGS(true, true);
  
  // init SDL
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
  {
    std::string error = SDL_GetError();
    NEXUS_ERROR("Failed to Init SDL: {}", error);
    return -1;
  }

  // Set Up OpenGL
  if (!SDL_GL_LoadLibrary(NULL) != 0)
  {
    std::string error = SDL_GetError();
    NEXUS_ERROR("Failed to Open OpenGL Library: {}", error);
    return -1;
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
  NEXUS_WINDOWING::Window window("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
    true, SDL_WINDOW_OPENGL);
  if (!window.GetWindow())
  {
    NEXUS_ERROR("Failed to Create Window!");
    return -1;
  }

  // Create the OpenGL Context
  window.SetGLContext(SDL_GL_CreateContext(window.GetWindow().get()));
  if (!window.GetGLContext())
  {
    std::string error = SDL_GetError();
    NEXUS_ERROR("Failed to Create OpenGL Content: {}", error);
    return -1;
  }
  SDL_GL_MakeCurrent(window.GetWindow().get(), window.GetGLContext());
  SDL_GL_SetSwapInterval(1);

  // Init Glad
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    NEXUS_ERROR("Failed to Init GLAD");
    return -1;
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
  auto registry = std::make_unique<entt::registry>();
  if(!registry)
  {
    NEXUS_ERROR("Failed to Create entity registry");
    return -1;
  }

  // Init camera
  NEXUS_RENDERING::Camera2D camera;
  camera.SetZoom(2.0f);

  // Init Shader
  std::unique_ptr<NEXUS_RENDERING::Shader> shader = NEXUS_RENDERING::ShaderLoader::Create(
    AssetPath("shaders/quad.vert"),
    AssetPath("shaders/quad.frag")
  );

  // init texture
  std::unique_ptr<NEXUS_RENDERING::Texture> texture = NEXUS_RENDERING::TextureLoader::Create(
    NEXUS_RENDERING::Texture::TextureType::NEAREST,
    AssetPath("textures/world_tileset.png")
  );
  NEXUS_LOG(
    R"(Texture Loaded Successfully:
    ID: {},
    Width: {},
    Height: {}
    )",
    texture->GetID(), texture->GetWidth(), texture->GetHeight()
  );

  // init entity
  auto ent1 = registry->create();

  auto &transform = registry->emplace<TransformComponent>(ent1, TransformComponent{
    .position = glm::vec2{50.0f},
    .scale = glm::vec2{1.0f},
    .rotation = 0.0f
  });

  auto &sprite = registry->emplace<SpriteComponent>(ent1, SpriteComponent{
    .startX = 11,
    .startY = 8,
    .width = 16.0f,
    .height = 16.0f,
    .color = {.r = 255, .g = 255, .b = 255, .a = 255}
  });
  sprite.generate_uvs(texture->GetWidth(), texture->GetHeight());

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

  // Init texture coords
  float texCoords[] = {
    sprite.uvs.u, sprite.uvs.v,
    sprite.uvs.u, sprite.uvs.v + sprite.uvs.height,
    sprite.uvs.u + sprite.uvs.width, sprite.uvs.v + sprite.uvs.height,
    sprite.uvs.u + sprite.uvs.width, sprite.uvs.v,
  };
  
  // indices
  unsigned int indices[] = {
    0, 1, 2,
    1, 2, 3
  };

  // vao
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // ebo
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // vbo
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(NEXUS_RENDERING::Vertex), vertices.data(), GL_STATIC_DRAW);

  // vertex attrib pointers
  // inPos
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
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

  glBindVertexArray(0);
  glDeleteBuffers(1, &vbo);

  // game loop
  bool running = true;
  while(running)
  {
    // process events
    SDL_Event event = {0};
    while (SDL_PollEvent(&event))
    {
      switch(event.type)
      {
      case SDL_EVENT_QUIT:
      {
        running = false;
      } break;

      case SDL_EVENT_KEY_DOWN:
      {
        if (event.key.key == SDLK_ESCAPE)
        {
          running = false;
        }
      } break;
      }
    }

    // Update
    glViewport(0, 0, window.GetWidth(), window.GetHeight());

    camera.Update();

    // Render
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->Enable();
    glBindVertexArray(vao);

    // bind texture
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();

    // Set Projection Matrix
    glm::mat4 projectionMatrix = camera.GetCameraMatrix();
    shader->SetUniformMat4("uProjection", projectionMatrix);

    // Draw elements
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
    
    texture->unBind();
    glBindVertexArray(0);
    shader->Disable();

    SDL_GL_SwapWindow(window.GetWindow().get());
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &ebo);

  NEXUS_LOG("Closing");
}
