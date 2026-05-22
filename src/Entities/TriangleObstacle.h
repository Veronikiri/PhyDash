#pragma once
#include "Entities/Obstacle.h"

class TriangleObstacle : public Obstacle {
public:
    TriangleObstacle(float x, float y);
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

private:
    sf::ConvexShape m_triangle;
};