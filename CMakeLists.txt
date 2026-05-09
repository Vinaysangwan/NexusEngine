cmake_minimum_required(VERSION 3.28)
project(NEXUS_ENGINE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(SDL_SHARED OFF)
set(SDL_STATIC ON)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.4.8
)

FetchContent_MakeAvailable(SDL3)

add_subdirectory(GLAD)
add_subdirectory(NEXUS_UTILITIES)
add_subdirectory(NEXUS_WINDOW)
add_subdirectory(NEXUS_EDITOR)
