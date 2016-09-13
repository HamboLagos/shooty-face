project(utests)

# Build the googletest archives
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/gtest)
add_subdirectory(googletest)

# Add the gtest headers and compiled archives to the search path
include_directories(googletest/googletest/include/gtest)
include_directories(googletest/googlemock/include/gmock)
include_directories(${CMAKE_BINARY_DIR}/bin/gtest)

# Add source files to the search path
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/utests)

# Build the tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/utests)

set(GTEST_LIBRARIES gmock gtest gmock_main)

include(AABB_tests.cmake)
include(bullet_tests.cmake)
include(collision_tests.cmake)
include(entity_tests.cmake)
include(gun_tests.cmake)
include(health_bar_tests.cmake)
include(healthy_tests.cmake)
include(player_tests.cmake)
