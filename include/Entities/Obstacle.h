#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    Obstacle(float x, float y);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

private:
    sf::ConvexShape m_triangle;
};