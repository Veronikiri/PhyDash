cmake_minimum_required(VERSION 3.15)
project(NeonDash VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/Libraries/SFML-3.0.1/lib/cmake/SFML")

include_directories(include src)

find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

set(SOURCES
    src/main.cpp
    src/Core/Game.cpp
    src/Scenes/MenuScene.cpp
    src/Scenes/PlayingScene.cpp
    src/Scenes/CustomizeScene.cpp
    src/Scenes/FixedLevelScene.cpp
    src/UI/Button.cpp
    src/UI/Slider.cpp
    src/Entities/Player.cpp
    src/Entities/PlayerCube.cpp
    src/Entities/PlayerShip.cpp
    src/Entities/PlayerBall.cpp
    src/Entities/PlayerUfo.cpp
    src/Entities/PlayerWave.cpp
    src/Entities/PlayerRobot.cpp
    src/Entities/PlayerSpider.cpp
    src/Entities/TriangleObstacle.cpp
    src/Entities/obstacles.cpp
    src/Utils/LevelLoader.cpp
    src/Scenes/SettingsScene.cpp 
    src/UI/BackButton.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCES} "src/Scenes/SettingsScene.h" "src/Scenes/SettingsScene.cpp"  "src/UI/BackButton.cpp" "src/UI/BackButton.h")

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/assets/fonts/FZeroGbaText1.ttf"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/FZeroGbaText1.ttf")