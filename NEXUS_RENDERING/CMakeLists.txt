project(NEXUS_RENDERING)

file(GLOB_RECURSE SRC_FILES 
  src/**.cpp
)

file(GLOB_RECURSE HEADER_FILES 
  include/**.h
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/vendor/stb
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  GLAD
  glm::glm
)
