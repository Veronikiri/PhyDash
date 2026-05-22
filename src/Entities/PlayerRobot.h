#pragma once
#include "Entities/Player.h"

class PlayerRobot : public Player {
public:
    PlayerRobot();
    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
};