set(TEST_NAME "gun_tests")

add_executable(
    ${TEST_NAME}
    ${TEST_NAME}.cpp
    ${CMAKE_SOURCE_DIR}/src/bullet.cpp
    ${CMAKE_SOURCE_DIR}/src/entity.cpp
    ${CMAKE_SOURCE_DIR}/src/gun.cpp
    ${CMAKE_SOURCE_DIR}/src/utils.cpp
)
target_link_libraries(${TEST_NAME} ${GTEST_LIBRARIES} ${SFML_LIBRARIES})

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
