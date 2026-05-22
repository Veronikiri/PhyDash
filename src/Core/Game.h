#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include "Scene.h"

class Game {
public:
    Game();
    void run();

    void pushScene(std::unique_ptr<Scene> scene);
    void popScene();
    void changeScene(std::unique_ptr<Scene> scene);
    const sf::Font& getFont() const;
    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    std::stack<std::unique_ptr<Scene>> m_scenes;
};