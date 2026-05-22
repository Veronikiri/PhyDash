#pragma once
#include "Entities/Player.h"

class PlayerWave : public Player {
public:
    PlayerWave();
    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
};