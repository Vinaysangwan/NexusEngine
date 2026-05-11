project(NEXUS_LOGGER LANGUAGES CXX)

file(GLOB_RECURSE SRC_FILES 
  src/**.cpp
)

file(GLOB_RECURSE HEADER_FILES 
  include/**.h
  include/**.inl
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
)
