cmake_minimum_required(VERSION 3.28)
project(NEXUS_ENGINE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT NEXUS_EDITOR
  )
endif()

include(FetchContent)

set(SDL_SHARED OFF)
set(SDL_STATIC ON)

set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF)
set(SDL_EXAMPLES OFF)
set(SDL_INSTALL OFF)
set(BUILD_SHARED_LIBS OFF)

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
