#pragma once
#include "Entities/Player.h"

class PlayerBall : public Player {
public:
    PlayerBall();
    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
};