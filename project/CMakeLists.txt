cmake_minimum_required(VERSION 3.20)
project(kubik)

include(FetchContent)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
)

FetchContent_MakeAvailable(sfml)

add_executable(game 
    src/main.cpp
    src/obstacles.cpp
)

add_executable(level1 
    src/level1.cpp
    src/obstacles.cpp
    src/player.cpp
)

target_include_directories(game PRIVATE include)

target_link_libraries(game 
    sfml-graphics
    sfml-window 
    sfml-system
    sfml-audio
)

target_include_directories(level1 PRIVATE include)

target_link_libraries(level1 
    sfml-graphics
    sfml-window 
    sfml-system
    sfml-audio
)
