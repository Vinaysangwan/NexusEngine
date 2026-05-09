project(NEXUS_EDITOR)

add_executable(${PROJECT_NAME})

target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  GLAD
  NEXUS_UTILITIES
  NEXUS_WINDOW
)
