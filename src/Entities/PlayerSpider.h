#pragma once
#include "Entities/Player.h"

class PlayerSpider : public Player {
public:
    PlayerSpider();
    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
};