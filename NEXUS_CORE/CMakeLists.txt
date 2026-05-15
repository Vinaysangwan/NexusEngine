project(NEXUS_CORE)

file(GLOB_RECURSE SRC_FILES 
  src/**.cpp
)

file(GLOB_RECURSE HEADER_FILES
  include/**.h
  include/**.inl
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${INCLUDE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/vendor/entt
  ${CMAKE_SOURCE_DIR}/vendor/lua54/include
  ${sol2_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  NEXUS_LOGGER
  NEXUS_RENDERING

  glm::glm
  ${CMAKE_SOURCE_DIR}/vendor/lua54/lib/lua54.lib
)
