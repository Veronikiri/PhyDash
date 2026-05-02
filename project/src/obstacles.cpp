#include "obstacles.hpp"
#include "player.hpp"
#include <random>

Spike::Spike(float x, float y)
{
    shape.setPointCount(3);
    shape.setPoint(0, sf::Vector2f(0.f, 0.f));
    shape.setPoint(1, sf::Vector2f(-25.f, 50.f));
    shape.setPoint(2, sf::Vector2f(25.f, 50.f));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(2.f);
    shape.setPosition(sf::Vector2f(x + 25.f, y));

    hitbox.setSize(sf::Vector2f(20.f, 30.f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(2.f);
    hitbox.setPosition(sf::Vector2f(x + 15.f, y + 20.f));
}

void Spike::update(float dt)
{
    shape.move(sf::Vector2f(speed * dt, 0.f));
    hitbox.move(sf::Vector2f(speed * dt, 0.f));
}

void Spike::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(hitbox);
}

sf::FloatRect Spike::getBounds() const
{
    return sf::FloatRect(hitbox.getPosition(), hitbox.getSize());
}

bool Spike::onscreen() const
{
    return shape.getPosition().x + 50.f > 0;
}

Block::Block(float x, float y)
{
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Blue);
    shape.setOutlineThickness(2.f);
    shape.setPosition(sf::Vector2f(x, y));

    hitbox.setSize(sf::Vector2f(50.f, 50.f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Blue);
    hitbox.setOutlineThickness(2.f);
    hitbox.setPosition(sf::Vector2f(x, y));
}

void Block::update(float dt)
{
    shape.move(sf::Vector2f(speed * dt, 0.f));
    hitbox.move(sf::Vector2f(speed * dt, 0.f));
}

void Block::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(hitbox);
}

sf::FloatRect Block::getBounds() const
{
    return sf::FloatRect(hitbox.getPosition(), hitbox.getSize());
}

bool Block::onscreen() const
{
    return shape.getPosition().x + shape.getSize().x > 0;
}

Orb::Orb(float x, float y)
{
    shape.setRadius(25.f);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Yellow);
    shape.setOutlineThickness(3.f);
    shape.setPosition(sf::Vector2f(x + 0.f, y - 0.f));

    hitbox.setSize(sf::Vector2f(50.f, 50.f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setOutlineThickness(2.f);
    hitbox.setPosition(sf::Vector2f(x + 0.f, y - 0.f));
}

void Orb::update(float dt)
{
    shape.move(sf::Vector2f(speed * dt, 0.f));
    hitbox.move(sf::Vector2f(speed * dt, 0.f));
}

void Orb::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(hitbox);
}

sf::FloatRect Orb::getBounds() const
{
    return sf::FloatRect(hitbox.getPosition(), hitbox.getSize());
}

bool Orb::onscreen() const
{
    return shape.getPosition().x > 0;
}

Pad::Pad(float x, float y)
{
    shape.setSize(sf::Vector2f(50.f, 10.f));
    shape.setFillColor(sf::Color::Yellow);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    shape.setPosition(sf::Vector2f(x, y+40));

    hitbox.setSize(sf::Vector2f(40.f, 8.f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setOutlineThickness(2.f);
    hitbox.setPosition(sf::Vector2f(x+5, y+42));
}

void Pad::update(float dt)
{
    shape.move(sf::Vector2f(speed * dt, 0.f));
    hitbox.move(sf::Vector2f(speed * dt, 0.f));
}

void Pad::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(hitbox);
}

sf::FloatRect Pad::getBounds() const
{
    return sf::FloatRect(hitbox.getPosition(), hitbox.getSize());
}

bool Pad::onscreen() const
{
    return shape.getPosition().x + shape.getSize().x > 0;
}

Portal::Portal(float x, float y, gamemode mode)
    : mode(mode)
{
    hitbox.setSize(sf::Vector2f(25.f, 100.f));
    hitbox.setFillColor(sf::Color::Transparent);
    
    if (mode == gamemode::cube)
        hitbox.setOutlineColor(sf::Color::Green);
    else
        hitbox.setOutlineColor(sf::Color::Magenta);
    
    hitbox.setOutlineThickness(5.f);
    hitbox.setPosition(sf::Vector2f(x, y));
}

void Portal::update(float dt)
{
    hitbox.move(sf::Vector2f(speed * dt, 0.f));
}

gamemode Portal::getmode() {
    return mode;
}

void Portal::draw(sf::RenderWindow& window)
{
    window.draw(hitbox);
}

sf::FloatRect Portal::getBounds() const
{
    return hitbox.getGlobalBounds();
}

bool Portal::onscreen() const
{
    return hitbox.getPosition().x + hitbox.getSize().x > 0;
}

void obstacle::addSpike(float x, float y)
{
    spikes.push_back(Spike(x, y));
}

void obstacle::addBlock(float x, float y)
{
    blocks.push_back(Block(x, y));
}

void obstacle::addBigBlock(float x, float y, int w, int h)
{
    for (int i = 0; i<w; i++) {
        for (int j = 0; j<h; j++) {
            blocks.push_back(Block(x+50*i, y-50*j));
        }
    }
}

void obstacle::addOrb(float x, float y)
{
    orbs.push_back(Orb(x, y));
}

void obstacle::addPad(float x, float y)
{
    pads.push_back(Pad(x, y));
}
void obstacle::addPortal(float x, float y, gamemode mode)
{
    portals.push_back(Portal(x, y, mode));
}



std::vector<obstacle> createpool()
{
    std::vector<obstacle> pool;
    {
        obstacle pattern;
        pattern.addPad(800.f, 500.f);
        pattern.addSpike(850.f, 500.f);
        pattern.addSpike(900.f, 500.f);
        pattern.addSpike(950.f, 500.f);
        pattern.addSpike(1000.f, 500.f);
        pool.push_back(pattern);
    }
    {
        obstacle pattern;
        pattern.addSpike(800.f, 500.f);
        pattern.addSpike(850.f, 500.f);
        pattern.addSpike(900.f, 500.f);
        pattern.addOrb(900.f, 425.f);
        pattern.addSpike(950.f, 500.f);
        pattern.addSpike(1000.f, 500.f);
        pool.push_back(pattern);
    }
    {
        obstacle pattern;
        pattern.addOrb(800.f, 400.f);
        pattern.addBigBlock(900.f, 500.f, 1 , 2);
        pattern.addSpike(900.f, 400.f);
        pattern.addOrb(1000.f, 350.f);
        pattern.addBigBlock(1100.f, 500.f, 1 , 3);
        pattern.addSpike(1100.f, 350.f);
        pool.push_back(pattern);
    }
    {
        obstacle pattern;
        pattern.addSpike(800.f, 500.f);
        pattern.addSpike(850.f, 500.f);
        pattern.addSpike(900.f, 500.f);
        pattern.addSpike(1300.f, 500.f);
        pattern.addSpike(1350.f, 500.f);
        pool.push_back(pattern);
    }
    

    return pool;
}