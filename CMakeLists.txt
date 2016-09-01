cmake_minimum_required(VERSION 3.5)
project(ShootyFace)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

# Set version information in a config.h file
set(shooty_face_VERSION_MAJOR 0)
set(shooty_face_VERSION_MINOR 0)
set(shooty_face_VERSION_REVIS 1)
configure_file(
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_BINARY_DIR}/src/config.h"
)
include_directories("${PROJECT_BINARY_DIR}/src/")

# find SFML (built separately in vendor/SFML)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/" ${CMAKE_MODULE_PATH})
set(SFML_ROOT "${PROJECT_SOURCE_DIR}/lib/SFML/")
find_package(SFML 2.3 REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})

# Builds the game executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(src)

# Builds the tests
enable_testing()
add_subdirectory(utests)
