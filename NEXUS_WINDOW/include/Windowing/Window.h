#pragma once

#include <NexusUtilities/SDL_Wrappers.h>
#include <string>

namespace NEXUS_WINDOWING
{
  class Window
  {
  public:
    Window();
    Window(const std::string &title, int xPos, int yPos, int width, int height, bool v_sync = true, 
      Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_CAPTURE);
    ~Window();

    void SetWindowTitle(const std::string& title);

    void SetPosition(int x, int y);
    void SetSize(int width, int height);

    inline void SetGLContext(const SDL_GLContext& glContext) { _GLContext = glContext; }
    inline SDL_GLContext& GetGLContext() { return _GLContext; }

    inline WindowPtr& GetWindow() { return p_Window; }
    
    inline const std::string& GetWindowTitle() const { return _Title; };
    
    inline const int GetXPos() const { return _XPos; }
    inline const int GetYPos() const { return _YPos; }

    inline const int GetWidth() const { return _Width; }
    inline const int GetHeight() const { return _Height; }
    
  private:
    void CreateWindow();

  private:
    WindowPtr p_Window;
    SDL_GLContext _GLContext;
    std::string _Title;
    int _XPos, _YPos;
    int _Height, _Width;
    Uint32 _WindowFlags;
  };
}
