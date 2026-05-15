project(NEXUS_EDITOR)

file(GLOB_RECURSE SRC_FILES 
  src/**.cpp
)

file(GLOB_RECURSE HEADER_FILES 
  include/**.h
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
  ASSET_PATH="${CMAKE_SOURCE_DIR}/NEXUS_EDITOR/assets/"
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/vendor/stb
  ${CMAKE_SOURCE_DIR}/vendor/entt
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  GLAD
  NEXUS_CORE
  NEXUS_LOGGER
  NEXUS_RENDERING
  NEXUS_UTILITIES
  NEXUS_WINDOW

  glm::glm
)
