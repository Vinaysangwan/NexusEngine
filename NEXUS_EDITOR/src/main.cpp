#define SDL_MAIN_HANDLED 1
#include "pch.h"

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
  NEXUS_WINDOWING::Window window("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, true, SDL_WINDOW_OPENGL);
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

  // Initialize Glad
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cout<<"Failed to Init GLAD"<<std::endl;
    return -1;
  }

  std::cout << "Vendor:   " << glGetString(GL_VENDOR) << '\n';
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
  std::cout << "Version:  " << glGetString(GL_VERSION) << '\n';
  std::cout << "GLSL:     " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

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
      window.GetXPos(), window.GetYPos(),
      window.GetWidth(), window.GetHeight()
    );

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window.GetWindow().get());
  }

  std::cout<<"Closing"<<std::endl;
}
