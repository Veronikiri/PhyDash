#include "Entities/obstacles.hpp"
#include "Entities/Player.h"
#include <random>

float speed = -300.f;

Spike::Spike(float x, float y) {
    shape.setPointCount(3);
    shape.setPoint(0, { 0.f, 0.f });
    shape.setPoint(1, { -25.f, 50.f });
    shape.setPoint(2, { 25.f, 50.f });
    shape.setFillColor(sf::Color(255, 50, 50, 180));
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(2.f);
    shape.setPosition({ x + 25.f, y });

    hitbox.setSize({ 20.f, 30.f });
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(2.f);
    hitbox.setPosition({ x + 15.f, y + 20.f });
}

void Spike::update(float dt) {
    shape.move({ speed * dt, 0.f });
    hitbox.move({ speed * dt, 0.f });
}

void Spike::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(hitbox);
}

sf::FloatRect Spike::getBounds() const {
    return hitbox.getGlobalBounds();
}

bool Spike::onscreen() const {
    return shape.getPosition().x + 50.f > 0;
}

Block::Block(float x, float y) {
    shape.setSize({ 50.f, 50.f });
    shape.setFillColor(sf::Color(50, 50, 255, 180));
    shape.setOutlineColor(sf::Color::Blue);
    shape.setOutlineThickness(2.f);
    shape.setPosition({ x, y });

    hitbox.setSize({ 50.f, 50.f });
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Blue);
    hitbox.setOutlineThickness(2.f);
    hitbox.setPosition({ x, y });
}

void Block::update(float dt) {
    shape.move({ speed * dt, 0.f });
    hitbox.move({ speed * dt, 0.f });
}

void Block::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(hitbox);
}

sf::FloatRect Block::getBounds() const {
    return hitbox.getGlobalBounds();
}

bool Block::onscreen() const {
    return shape.getPosition().x + shape.getSize().x > 0;
}

Orb::Orb(float x, float y) {
    shape.setRadius(25.f);
    shape.setFillColor(sf::Color(255, 200, 50, 200));
    shape.setOutlineColor(sf::Color::Yellow);
    shape.setOutlineThickness(3.f);
    shape.setPosition({ x, y - 25.f });

    hitbox.setSize({ 50.f, 50.f });
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setOutlineThickness(2.f);
    hitbox.setPosition({ x, y - 25.f });
}

void Orb::update(float dt) {
    shape.move({ speed * dt, 0.f });
    hitbox.move({ speed * dt, 0.f });
}

void Orb::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(hitbox);
}

sf::FloatRect Orb::getBounds() const {
    return hitbox.getGlobalBounds();
}

bool Orb::onscreen() const {
    return shape.getPosition().x > 0;
}

Pad::Pad(float x, float y) {
    shape.setSize({ 50.f, 10.f });
    shape.setFillColor(sf::Color(0, 255, 100, 220));
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    shape.setPosition({ x, y + 40 });

    hitbox.setSize({ 40.f, 8.f });
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setOutlineThickness(2.f);
    hitbox.setPosition({ x + 5, y + 42 });
}

void Pad::update(float dt) {
    shape.move({ speed * dt, 0.f });
    hitbox.move({ speed * dt, 0.f });
}

void Pad::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(hitbox);
}

sf::FloatRect Pad::getBounds() const {
    return hitbox.getGlobalBounds();
}

bool Pad::onscreen() const {
    return shape.getPosition().x + shape.getSize().x > 0;
}

Portal::Portal(float x, float y, PlayerForm form)
    : targetForm(form) {
    hitbox.setSize({ 25.f, 100.f });
    hitbox.setFillColor(sf::Color::Transparent);
    if (form == PlayerForm::Cube)
        hitbox.setOutlineColor(sf::Color::Green);
    else if (form == PlayerForm::Ship)
        hitbox.setOutlineColor(sf::Color::Magenta);
    else
        hitbox.setOutlineColor(sf::Color::Cyan);
    hitbox.setOutlineThickness(5.f);
    hitbox.setPosition({ x, y });
}

void Portal::update(float dt) {
    hitbox.move({ speed * dt, 0.f });
}

PlayerForm Portal::getForm() const {
    return targetForm;
}

void Portal::draw(sf::RenderWindow& window) {
    window.draw(hitbox);
}

sf::FloatRect Portal::getBounds() const {
    return hitbox.getGlobalBounds();
}

bool Portal::onscreen() const {
    return hitbox.getPosition().x + hitbox.getSize().x > 0;
}

void ObstaclePattern::addSpike(float x, float y) {
    spikes.emplace_back(x, y);
}

void ObstaclePattern::addBlock(float x, float y) {
    blocks.emplace_back(x, y);
}

void ObstaclePattern::addBigBlock(float x, float y, int w, int h) {
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j)
            blocks.emplace_back(x + 50.f * i, y - 50.f * j);
}

void ObstaclePattern::addOrb(float x, float y) {
    orbs.emplace_back(x, y);
}

void ObstaclePattern::addPad(float x, float y) {
    pads.emplace_back(x, y);
}

void ObstaclePattern::addPortal(float x, float y, PlayerForm form) {
    portals.emplace_back(x, y, form);
}

std::vector<ObstaclePattern> createPatternPool() {
    std::vector<ObstaclePattern> pool;

    {
        ObstaclePattern p;
        p.addPortal(800.f, 400.f, PlayerForm::Ship);
        p.addBigBlock(1200.f, 500.f, 12, 1);
        p.addBigBlock(1200.f, 300.f, 12, 10);

        p.addBigBlock(2400.f, 500.f, 12, 5);
        p.addBigBlock(2400.f, 100.f, 12, 10);

        p.addBigBlock(3600.f, 500.f, 12, 3);
        p.addBigBlock(3600.f, 200.f, 12, 10);

        
        p.addPortal(4200.f, 275.f, PlayerForm::Cube);
        pool.push_back(p);
    }

    {
        ObstaclePattern p;
        p.addPad(800.f, 500.f);
        p.addSpike(850.f, 500.f);
        p.addSpike(900.f, 500.f);
        p.addSpike(950.f, 500.f);
        p.addSpike(1000.f, 500.f);
        pool.push_back(p);
    }

    {
        ObstaclePattern p;
        p.addSpike(800.f, 500.f);
        p.addSpike(850.f, 500.f);
        p.addSpike(900.f, 500.f);
        p.addOrb(900.f, 425.f);
        p.addSpike(950.f, 500.f);
        p.addSpike(1000.f, 500.f);
        pool.push_back(p);
    }

    {
        ObstaclePattern p;
        p.addOrb(800.f, 400.f);
        p.addBigBlock(900.f, 500.f, 1, 2);
        p.addSpike(900.f, 400.f);
        p.addOrb(1000.f, 350.f);
        p.addBigBlock(1100.f, 500.f, 1, 3);
        p.addSpike(1100.f, 350.f);
        pool.push_back(p);
    }

    {
        ObstaclePattern p;
        p.addSpike(800.f, 500.f);
        p.addSpike(850.f, 500.f);
        p.addSpike(900.f, 500.f);
        p.addSpike(1300.f, 500.f);
        p.addSpike(1350.f, 500.f);
        pool.push_back(p);
    }

    return pool;
}