set(TEST_NAME "rate_limit_tests")

add_executable(
    ${TEST_NAME}
    ${TEST_NAME}.cpp
)
target_link_libraries(${TEST_NAME} ${GTEST_LIBRARIES} ${SFML_LIBRARIES})

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

