project(NEXUS_WINDOW)

file(GLOB_RECURSE SRC_FILES 
  src/**.cpp
)

file(GLOB_RECURSE HEADER_FILES 
  include/**.h
)

add_library(${PROJECT_NAME} STATIC
  ${SRC_FILES} ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  NEXUS_UTILITIES
  NEXUS_LOGGER
)
