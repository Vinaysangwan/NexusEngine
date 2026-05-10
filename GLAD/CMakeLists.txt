project(GLAD)

file(GLOB_RECURSE SRC_FILES 
  src/**.c
)

file(GLOB_RECURSE HEADER_FILES
  include/**.h
)

add_library(${PROJECT_NAME} STATIC
  ${SRC_FILES} ${INCLUDE_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${PROJECT_SOURCE_DIR}/include
)
