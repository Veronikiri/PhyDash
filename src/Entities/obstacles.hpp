#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Entities/Player.h"

extern float speed;

struct Spike {
    sf::ConvexShape shape;
    sf::RectangleShape hitbox;

    Spike(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct Block {
    sf::RectangleShape shape;
    sf::RectangleShape hitbox;

    Block(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct Orb {
    sf::CircleShape shape;
    sf::RectangleShape hitbox;

    Orb(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct Pad {
    sf::RectangleShape shape;
    sf::RectangleShape hitbox;

    Pad(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct Portal {
    sf::RectangleShape hitbox;
    PlayerForm targetForm;

    Portal(float x, float y, PlayerForm form);
    void update(float dt);
    PlayerForm getForm() const;
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct ObstaclePattern {
    std::vector<Spike> spikes;
    std::vector<Block> blocks;
    std::vector<Orb> orbs;
    std::vector<Pad> pads;
    std::vector<Portal> portals;

    void addSpike(float x, float y);
    void addBlock(float x, float y);
    void addBigBlock(float x, float y, int w, int h);
    void addOrb(float x, float y);
    void addPad(float x, float y);
    void addPortal(float x, float y, PlayerForm form);
};

std::vector<ObstaclePattern> createPatternPool();