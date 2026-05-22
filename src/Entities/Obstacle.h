#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    virtual ~Obstacle() = default;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;
};