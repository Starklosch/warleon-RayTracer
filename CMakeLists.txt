cmake_minimum_required(VERSION 3.30)

project(ray_tracer_rederer CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message("COMPILER = ${CMAKE_CXX_COMPILER_ID}")

include(FetchContent)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  git
  GIT_TAG 1.0.1  
)
FetchContent_MakeAvailable(glm)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

FetchContent_Declare(
  tobj
  GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader
  git
  GIT_TAG v2.0.0rc13
)
FetchContent_MakeAvailable(tobj)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.0.2
)
FetchContent_MakeAvailable(fmt)


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
target_include_directories(stb_image_write INTERFACE third_party/include/stb_image)


file(GLOB_RECURSE SOURCES "src/**.[ic]pp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
message("SOURCE FILES = ${SOURCES}")
add_executable(rayTracer src/main.cpp ${SOURCES})
target_compile_options(rayTracer PRIVATE -Wall -Wextra)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options(rayTracer PRIVATE -fsanitize=address)
	target_link_libraries(rayTracer PRIVATE -fsanitize=address)
endif()

target_link_libraries(rayTracer PRIVATE stb_image_write)
target_link_libraries(rayTracer PRIVATE glm)
target_link_libraries(rayTracer PRIVATE tinyobjloader)
target_link_libraries(rayTracer PRIVATE fmt::fmt)

install(TARGETS rayTracer DESTINATION dist)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION dist)

enable_testing()
file(GLOB_RECURSE TESTS "tests/**.cpp")
add_executable(mytests ${TESTS} ${SOURCES})
target_link_libraries(mytests GTest::gtest_main)
target_link_libraries(mytests glm)
target_link_libraries(mytests tinyobjloader)
target_link_libraries(mytests stb_image_write)
target_link_libraries(mytests  fmt::fmt)
include(GoogleTest)
gtest_discover_tests(mytests)



# CREATE FORMAT COMMAND
file(GLOB_RECURSE SRC_FILES "src/**.[chi]pp" "tests/**.[chi]pp")
add_custom_target(format
	COMMAND clang-format  -i ${SRC_FILES}
    WORKING_DIRECTORY 
    COMMENT "Formatting source and test files with clang-format"
)



