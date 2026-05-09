add_library(${PROJECT_NAME} STATIC
  SDL_Wrappers.cpp
)

target_precompile_headers(${PROJECT_NAME} PRIVATE 
  ${PROJECT_SOURCE_DIR}/include/pch.h
)

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/vendor/SDL/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  SDL3::SDL3-static
)
