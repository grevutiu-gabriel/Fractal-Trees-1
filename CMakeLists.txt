cmake_minimum_required (VERSION 3.8)
project(fractal-trees)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)

include_directories(include extlibs ${SFML_INCLUDE_DIR})
aux_source_directory(src sources)
add_executable(main ${sources})

set_target_properties(main PROPERTIES CXX_STANDARD 17)
target_link_libraries(main ${SFML_LIBRARIES})

# target_compile_options(main PUBLIC -Wall -Wextra -Wconversion -pedantic -Wno-unused-parameter -Wno-sign-conversion)
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -O0")
