#pragma once
#include "Entities/Player.h"

class PlayerUfo : public Player {
public:
    PlayerUfo();
    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
};