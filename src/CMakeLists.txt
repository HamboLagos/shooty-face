# Build the game executable
set(EXECUTABLE_NAME "shooty_face")
add_executable(${EXECUTABLE_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/player.cpp
)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
