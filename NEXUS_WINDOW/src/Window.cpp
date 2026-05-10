#include "Windowing/Window.h"

#include <iostream>

namespace NEXUS_WINDOWING
{
  Window::Window()
    : Window("Default Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, true, NULL)
  {
  }

  Window::Window(const std::string &title, int xPos, int yPos, int width, int height, bool v_sync, Uint32 flags)
    : p_Window{nullptr},
      _GLContext{},
      _Title{title},
      _XPos{xPos},
      _YPos{yPos},
      _Width{width},
      _Height{height},
      _WindowFlags{flags}
  {
    CreateWindow();

    // Enable VSync
    if (v_sync)
    {
      if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
        std::cout<<"Failed to enable vSync"<<std::endl;
    }

    std::cout<<"Window Created Successfully"<<std::endl;
  }

  Window::~Window()
  {
  }

  void Window::SetWindowTitle(const std::string &title)
  {
    _Title = title;
    SDL_SetWindowTitle(p_Window.get(), _Title.c_str());
  }

  void Window::SetPosition(int x, int y)
  {
    _XPos = x;
    _YPos = y;
    SDL_SetWindowPosition(p_Window.get(), x, y);
  }

  void Window::SetSize(int width, int height)
  {
    _Width = width;
    _Height = height;
    SDL_SetWindowSize(p_Window.get(), width, height);
  }

  void Window::CreateWindow()
  {
    p_Window = WindowPtr(SDL_CreateWindow(_Title.c_str(), _Width, _Height, _WindowFlags));

    if (!p_Window)
    {
      std::string error = SDL_GetError();
      std::cout<<"Failed to Create Window: "<<error<<std::endl;
    }
  }
}
