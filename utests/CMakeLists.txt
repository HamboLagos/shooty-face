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

add_executable(
    entity_tests
    entity_tests.cpp
)
target_link_libraries(entity_tests ${GTEST_LIBRARIES} ${SFML_LIBRARIES})
add_test(NAME entity_tests COMMAND entity_tests)

add_executable(
    bullet_tests
    bullet_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/bullet.cpp
)
target_link_libraries(bullet_tests ${GTEST_LIBRARIES} ${SFML_LIBRARIES})
add_test(NAME bullet_tests COMMAND bullet_tests)

add_executable(
    gun_tests
    gun_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/gun.cpp
    ${CMAKE_SOURCE_DIR}/src/bullet.cpp
)
target_link_libraries(gun_tests ${GTEST_LIBRARIES} ${SFML_LIBRARIES})
add_test(NAME gun_tests COMMAND gun_tests)

add_executable(
    player_tests
    player_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/player.cpp
    ${CMAKE_SOURCE_DIR}/src/gun.cpp
    ${CMAKE_SOURCE_DIR}/src/bullet.cpp
)
target_link_libraries(player_tests ${GTEST_LIBRARIES} ${SFML_LIBRARIES})
add_test(NAME player_tests COMMAND player_tests)

add_executable(
    collision_tests
    collision_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/collision.cpp
)
target_link_libraries(collision_tests ${GTEST_LIBRARIES} ${SFML_LIBRARIES})
add_test(NAME collision_tests COMMAND collision_tests)
