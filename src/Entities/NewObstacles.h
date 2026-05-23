#pragma once
#include <SFML/Graphics.hpp>

// 1. Декоративная Цепь
class ChainObstacle {
public:
    sf::RectangleShape shape;
    float speed = -250.f;

    ChainObstacle(float x, float y) {
        shape.setSize({ 10.f, 50.f });
        shape.setFillColor(sf::Color(200, 200, 200, 150));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.f);
        shape.setPosition({ x + 20.f, y });
    }

    void update(float dt) { shape.move({ speed * dt, 0.f }); }
    void draw(sf::RenderWindow& window) { window.draw(shape); }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
};

// 2. Наклонный блок
class SlopeObstacle {
public:
    sf::ConvexShape shape;
    float speed = -250.f;

    SlopeObstacle(float x, float y) {
        shape.setPointCount(3);
        shape.setPoint(0, { 0.f, 50.f });
        shape.setPoint(1, { 50.f, 0.f });
        shape.setPoint(2, { 50.f, 50.f });
        shape.setFillColor(sf::Color(30, 80, 200, 200));
        shape.setOutlineColor(sf::Color::Cyan);
        shape.setOutlineThickness(2.f);
        shape.setPosition({ x, y });
    }

    void update(float dt) { shape.move({ speed * dt, 0.f }); }
    void draw(sf::RenderWindow& window) { window.draw(shape); }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
};

// 3. Маленький шип
class HalfSpikeObstacle {
public:
    sf::ConvexShape shape;
    sf::RectangleShape hitbox;
    float speed = -250.f;

    HalfSpikeObstacle(float x, float y) {
        shape.setPointCount(3);
        shape.setPoint(0, { 0.f, 0.f });
        shape.setPoint(1, { -15.f, 25.f });
        shape.setPoint(2, { 15.f, 25.f });
        shape.setFillColor(sf::Color(255, 100, 100));
        shape.setOutlineColor(sf::Color::Red);
        shape.setOutlineThickness(2.f);
        shape.setPosition({ x + 25.f, y + 25.f });

        hitbox.setSize({ 14.f, 15.f });
        hitbox.setFillColor(sf::Color::Transparent);
        hitbox.setOutlineColor(sf::Color::Red);
        hitbox.setOutlineThickness(1.f);
        hitbox.setPosition({ x + 18.f, y + 35.f });
    }

    void update(float dt) {
        shape.move({ speed * dt, 0.f });
        hitbox.move({ speed * dt, 0.f });
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getBounds() const { return hitbox.getGlobalBounds(); }
};