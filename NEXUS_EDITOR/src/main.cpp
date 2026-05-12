#define SDL_MAIN_HANDLED 1
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <Windowing/Window.h>
#include <Logger/Logger.h>
#include <Rendering/Essentials/ShaderLoader.h> 
#include <Rendering/Essentials/TextureLoader.h> 
#include <Rendering/Core/Camera2D.h>

#include <iostream>

std::string AssetPath(const std::string& path)
{
  return std::string(ASSET_PATH) + path;
}

struct UVs
{
  float x, y, width, height;

  UVs() : x{0}, y{0}, width{0}, height{0} { }
  UVs(float x, float y, float width, float height) { }
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

  // Generating UVs
  UVs uvs;
  auto generateUVs = [&](float gridX, float gridY, float spriteSizeX, float spriteSizeY)
  {
    uvs.width = spriteSizeX / static_cast<float>(texture->GetWidth());
    uvs.height = spriteSizeY / static_cast<float>(texture->GetHeight());

    uvs.x = gridX * uvs.width;
    uvs.y = gridY * uvs.height;
  };
  generateUVs(11, 8, 16, 16);

  // vertices
  glm::vec2 quadPos{50.0f};
  glm::vec2 quadSize{32.0f};
  float vertices[] = {
    quadPos.x - quadSize.x / 2, quadPos.y - quadSize.y / 2, 0.0f,
    quadPos.x - quadSize.x / 2, quadPos.y + quadSize.y / 2, 0.0f,
    quadPos.x + quadSize.x / 2, quadPos.y + quadSize.y / 2, 0.0f,
    quadPos.x + quadSize.x / 2, quadPos.y - quadSize.y / 2, 0.0f,
  };

  // texture coords
  float texCoords[] = {
    uvs.x, uvs.y,
    uvs.x, uvs.y + uvs.height,
    uvs.x + uvs.width, uvs.y + uvs.height,
    uvs.x + uvs.width, uvs.y,
  };
  
  // indices
  unsigned int indices[] = {
    0, 1, 3,
    3, 1, 2
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  GLuint tex_vbo;
  glGenBuffers(1, &tex_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

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
    glViewport(
      0, 0,
      window.GetWidth(), window.GetHeight()
    );

    camera.Update();

    // Render
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->Enable();
    glBindVertexArray(vao);

    auto projectionMatrix = camera.GetCameraMatrix();
    shader->SetUniformMat4("uProjection", projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    texture->Bind();

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
