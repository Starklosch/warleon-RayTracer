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

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(third_party/include)
include_directories(src/include)

add_library(stb_image_write INTERFACE)
target_include_directories(stb_image_write INTERFACE third_party/include)


file(GLOB_RECURSE SOURCES "src/**.[ic]pp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
message("SOURCE FILES = ${SOURCES}")
add_executable(rayTracer src/main.cpp ${SOURCES})

target_link_libraries(rayTracer PRIVATE stb_image_write)
target_link_libraries(rayTracer PRIVATE glm)

install(TARGETS rayTracer DESTINATION dist)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION dist)

enable_testing()
file(GLOB_RECURSE TESTS "tests/**.cpp")
add_executable(mytests ${TESTS} ${SOURCES})
target_link_libraries(mytests GTest::gtest_main)
target_link_libraries(mytests glm)
target_link_libraries(mytests stb_image_write)
include(GoogleTest)
gtest_discover_tests(mytests)
