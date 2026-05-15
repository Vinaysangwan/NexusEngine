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

# SDL3
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF)
set(SDL_EXAMPLES OFF)
set(SDL_INSTALL OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.4.8
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3)

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# glm
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 1.0.3
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

# sol2
set(SOL2_BUILD_LUA OFF CACHE BOOL "" FORCE)
set(SOL2_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SOL2_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SOL2_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(SOL2_SINGLE OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  sol2
  GIT_REPOSITORY https://github.com/ThePhD/sol2.git
  GIT_TAG develop
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(sol2)

add_subdirectory(GLAD)
add_subdirectory(NEXUS_CORE)
add_subdirectory(NEXUS_LOGGER)
add_subdirectory(NEXUS_EDITOR)
add_subdirectory(NEXUS_RENDERING)
add_subdirectory(NEXUS_UTILITIES)
add_subdirectory(NEXUS_WINDOW)
