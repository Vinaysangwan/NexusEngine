#define SDL_MAIN_HANDLED 1
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <Windowing/Window.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
public:
  Camera2D()
    : Camera2D(640, 480)
  { }

  Camera2D(int width, int height)
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
  }
  
  ~Camera2D()
  { }

  inline glm::mat4 &GetCameraMatrix() { return _CameraMatrix; }

  inline void SetZoom(float zoom) 
  {
    _Zoom = zoom; 
    _NeedsUpdate = true;
  }

  void Update()
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

private:
  int _Width, _Height;
  float _Zoom;

  glm::vec2 _Position;
  glm::mat4 _CameraMatrix, _OrthoProjection;

  bool _NeedsUpdate;
};

struct UVs
{
  float x, y, width, height;

  UVs() : x{0}, y{0}, width{0}, height{0} { }
  UVs(float x, float y, float width, float height) { }
};

GLuint LoadTexture(const std::string& filePath, int &width, int &height, GLint wrap = GL_CLAMP_TO_EDGE, GLint filter = GL_NEAREST)
{
  std::string texturePath = std::string(ASSET_PATH) + "textures/" + filePath;
  
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  int nChannels;
  unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nChannels, 4);
  if(!data)
  {
    std::cout<<"Failed to Open the texture: "<<texturePath<<std::endl;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  return textureID;
}

int main()
{
  // init SDL
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
  {
    std::string error = SDL_GetError();
    std::cout<<"Failed to Init SDL: "<<error<<std::endl;
    return -1;
  }

  // Set Up OpenGL
  if (!SDL_GL_LoadLibrary(NULL) != 0)
  {
    std::string error = SDL_GetError();
    std::cout<<"Failed to Open OpenGL Library: "<<error<<std::endl;
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
    std::cout<<"Failed to Create Window!"<<std::endl;
    return -1;
  }

  // Create the OpenGL Context
  window.SetGLContext(SDL_GL_CreateContext(window.GetWindow().get()));
  if (!window.GetGLContext())
  {
    std::string error = SDL_GetError();
    std::cout<<"Failed to Create OpenGL Context: "<<error<<std::endl;
    return -1;
  }
  SDL_GL_MakeCurrent(window.GetWindow().get(), window.GetGLContext());
  SDL_GL_SetSwapInterval(1);

  // Init Glad
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cout<<"Failed to Init GLAD"<<std::endl;
    return -1;
  }

  // Enable Alpha Blend
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Display GPU Info
  std::cout << "Vender:   " << glGetString(GL_VENDOR) << '\n';
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
  std::cout << "Version:  " << glGetString(GL_VERSION) << '\n';
  std::cout << "GLSL:     " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

  // Init camera
  Camera2D camera;
  camera.SetZoom(2.0f);

  const char *vertCode = 
  "#version 440 core\n"
  "layout(location = 0) in vec3 inPos;\n"
  "layout(location = 1) in vec2 inTexCoords;\n"

  "layout(location = 0) out vec2 outTexCoords;\n"

  "uniform mat4 uProjectionMatrix;\n"
  // "uniform mat4 uCameraMatrix;\n"

  "void main(){\n"
  " gl_Position = uProjectionMatrix * vec4(inPos, 1.0f);\n"
  " outTexCoords = inTexCoords;\n"
  "}";

  const char *fragCode = 
  "#version 440 core\n"
  "layout(location = 0) in vec2 inTexCoords;\n"
  "layout(location = 0) out vec4 outColor;\n"
  "uniform sampler2D uTextureSampler;\n"
  "void main(){\n"
  " vec4 textureColor = texture(uTextureSampler, inTexCoords);\n"
  " outColor = textureColor;\n"
  "}";

  GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertID, 1, &vertCode, nullptr);
  glCompileShader(vertID);
  int success;
  glGetShaderiv(vertID, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE)
  {
    char infoLog[1024] = {0};
    glGetShaderInfoLog(vertID, 1024, nullptr, infoLog);
    std::cout<<"Failed to Compile VertexShader: "<<infoLog<<std::endl;
    return -1;
  }

  GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragID, 1, &fragCode, nullptr);
  glCompileShader(fragID);
  glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE)
  {
    char infoLog[1024] = {0};
    glGetShaderInfoLog(fragID, 1024, nullptr, infoLog);
    std::cout<<"Failed to Compile FragmentShader: "<<infoLog<<std::endl;
    return -1;
  }

  GLuint programID = glCreateProgram();
  glAttachShader(programID, vertID);
  glAttachShader(programID, fragID);
  glLinkProgram(programID);
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (success == GL_FALSE)
  {
    char infoLog[1024] = {0};
    glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
    std::cout<<"Failed to Link Program: "<<infoLog<<std::endl;
    return -1;
  }
  glDetachShader(programID, vertID);
  glDetachShader(programID, fragID);
  glDeleteShader(vertID);
  glDeleteShader(fragID);

  // texture id
  int textureWidth = 0;
  int textureHeight = 0;
  GLuint textureID = LoadTexture("world_tileset.png", textureWidth, textureHeight);

  UVs uvs;

  auto generateUVs = [&](float gridX, float gridY, float spriteSizeX, float spriteSizeY)
  {
    uvs.width = spriteSizeX / static_cast<float>(textureWidth);
    uvs.height = spriteSizeY / static_cast<float>(textureHeight);

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

    glViewport(
      0, 0,
      window.GetWidth(), window.GetHeight()
    );

    camera.Update();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programID);
    glBindVertexArray(vao);

    auto projectionMatrix = camera.GetCameraMatrix();

    GLint location = glGetUniformLocation(programID, "uProjectionMatrix");
    glUniformMatrix4fv(location, 1, GL_FALSE, &projectionMatrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glUseProgram(0);

    SDL_GL_SwapWindow(window.GetWindow().get());
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &ebo);
  glDeleteProgram(programID);

  std::cout<<"Closing"<<std::endl;
}
