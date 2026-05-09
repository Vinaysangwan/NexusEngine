add_library(${PROJECT_NAME} STATIC
  glad.c
)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/pch.h)

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${PROJECT_SOURCE_DIR}/include
)
