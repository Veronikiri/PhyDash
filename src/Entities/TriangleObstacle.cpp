#include "Entities/TriangleObstacle.h"

TriangleObstacle::TriangleObstacle(float x, float y) {
    m_triangle.setPointCount(3);
    m_triangle.setPoint(0, { 25.f, 0.f });
    m_triangle.setPoint(1, { 0.f, 50.f });
    m_triangle.setPoint(2, { 50.f, 50.f });
    m_triangle.setFillColor(sf::Color(255, 50, 50));
    m_triangle.setOutlineThickness(2);
    m_triangle.setOutlineColor(sf::Color::White);
    m_triangle.setPosition({ x, y });
}

void TriangleObstacle::draw(sf::RenderWindow& window) {
    window.draw(m_triangle);
}

sf::FloatRect TriangleObstacle::getBounds() const {
    return m_triangle.getGlobalBounds();
}