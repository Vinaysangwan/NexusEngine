project(NEXUS_EDITOR)

file(GLOB_RECURSE SRC_FILES 
  src/**.cpp
)

file(GLOB_RECURSE HEADER_FILES 
  include/**.h
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
  ASSET_PATH="${CMAKE_SOURCE_DIR}/assets/"
)

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/vendor/stb
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  GLAD
  NEXUS_UTILITIES
  NEXUS_WINDOW
)
