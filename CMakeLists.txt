cmake_minimum_required(VERSION 3.30)

project(ray_tracer_rederer CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

include(FetchContent)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  git
  GIT_TAG 1.0.1  # Specify GLM version or commit
)
FetchContent_MakeAvailable(glm)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

include_directories(third_party/include)
include_directories(src/include)

add_library(stb_image_write INTERFACE)
target_include_directories(stb_image_write INTERFACE third_party/include)


file(GLOB_RECURSE SOURCES "src/**.[ic]pp")
add_executable(rayTracer ${SOURCES})

target_link_libraries(rayTracer PRIVATE stb_image_write)
target_link_libraries(rayTracer PRIVATE glm)

install(TARGETS rayTracer DESTINATION dist)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION dist)
