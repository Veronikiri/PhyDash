#pragma once
#include "Entities/Player.h"

class PlayerCube : public Player {
public:
    PlayerCube();
    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
    void setPattern(int pattern) { m_cubePattern = pattern; }

private:
    int m_cubePattern = 0;
    sf::Angle m_startRotation;
};