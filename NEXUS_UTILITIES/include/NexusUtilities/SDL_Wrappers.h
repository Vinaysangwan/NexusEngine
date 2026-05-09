#pragma once

#include <SDL3/SDL.h>
#include <memory>

namespace NEXUS_UTIL
{
  struct SDL_Destroyer
  {
    void operator()(SDL_Window* window) const;
    void operator()(SDL_Gamepad* controller) const;
    void operator()(SDL_Cursor* cursor) const;
  };
}

using Controller = std::unique_ptr<SDL_Gamepad, NEXUS_UTIL::SDL_Destroyer>;
static Controller make_shared_controller(SDL_Gamepad* controller);

using Cursor = std::unique_ptr<SDL_Cursor, NEXUS_UTIL::SDL_Destroyer>;
static Cursor make_shared_cursor(SDL_Cursor* cursor);

using WindowPtr = std::unique_ptr<SDL_Window, NEXUS_UTIL::SDL_Destroyer>;
