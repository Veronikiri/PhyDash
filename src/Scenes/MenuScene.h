#pragma once
#include "Core/Scene.h"
#include "UI/Button.h"
#include <vector>
#include <memory>

class Game;

class MenuScene : public Scene {
public:
    MenuScene(Game& game);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void updateHover(sf::Vector2f mousePos);

    Game& m_game;
    std::unique_ptr<sf::Text> m_title;
    std::unique_ptr<sf::Text> m_shadow;
    std::vector<Button> m_buttons;
};