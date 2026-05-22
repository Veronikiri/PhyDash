#pragma once
#include "Entities/Player.h"

class PlayerShip : public Player {
public:
    PlayerShip();
    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
};