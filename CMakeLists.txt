cmake_minimum_required(VERSION 3.15)
project(NeonDash VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/Libraries/SFML-3.0.1/lib/cmake/SFML")

include_directories(include)

find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

set(SOURCES
    src/main.cpp
    src/Core/Game.cpp
    src/Entities/Player.cpp
    src/Entities/Obstacle.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/assets/fonts/FZeroGbaText1.ttf"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/FZeroGbaText1.ttf")