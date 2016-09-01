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

# Build the tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/utests)

add_executable(
    player_tests
    player_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/player.cpp
)
target_link_libraries(player_tests gmock gtest gmock_main ${SFML_LIBRARIES})
add_test(NAME player_tests COMMAND player_tests)
