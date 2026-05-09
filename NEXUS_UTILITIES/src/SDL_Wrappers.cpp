#include "pch.h"
#include "NexusUtilities/SDL_Wrappers.h"

namespace NEXUS_UTIL
{
  void SDL_Destroyer::operator()(SDL_Window* window) const
  {
    SDL_DestroyWindow(window);
    std::cout<<"Destroyed Window"<<std::endl;
  }

  void SDL_Destroyer::operator()(SDL_Gamepad *controller) const
  {
  }

  void NEXUS_UTIL::SDL_Destroyer::operator()(SDL_Cursor *cursor) const
  {
  }
}

Controller make_shared_controller(SDL_Gamepad* controller)
{

}

Cursor make_shared_cursor(SDL_Cursor* cursor)
{

}
