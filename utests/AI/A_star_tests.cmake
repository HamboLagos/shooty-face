set(TEST_NAME "A_star_tests")

add_executable(
    ${TEST_NAME}
    AI/${TEST_NAME}.cpp
    ${CMAKE_SOURCE_DIR}/src/AI/A_star.cpp
    ${CMAKE_SOURCE_DIR}/src/utils.cpp
)
target_link_libraries(${TEST_NAME} ${GTEST_LIBRARIES} ${SFML_LIBRARIES})

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

