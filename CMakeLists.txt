cmake_minimum_required (VERSION 3.8)
project(fractal-trees)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)

include_directories(include extlibs ${SFML_INCLUDE_DIR})
file(GLOB_RECURSE sources src/*)
add_executable(demo ${sources})

set_target_properties(demo PROPERTIES CXX_STANDARD 17)
target_link_libraries(demo ${SFML_LIBRARIES})

target_compile_options(demo PUBLIC -Wall -Wextra -Wconversion -pedantic -Wno-unused-parameter -Wno-sign-conversion)
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -O0")
