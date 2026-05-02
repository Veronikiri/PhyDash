#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "player.hpp" 

struct Spike {
    sf::ConvexShape shape;
    sf::RectangleShape hitbox;
    float speed = -250.f;
    
    Spike(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct Block {
    sf::RectangleShape shape;
    sf::RectangleShape hitbox;
    float speed = -250.f;
    
    Block(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct Orb {
    sf::CircleShape shape;
    sf::RectangleShape hitbox;
    float speed = -250.f;
    
    Orb(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct Pad {
    sf::RectangleShape shape;
    sf::RectangleShape hitbox;
    float speed = -250.f;
    
    Pad(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct Portal {
    sf::RectangleShape hitbox;
    gamemode mode;
    float speed = -250.f;
    
    Portal(float x, float y, gamemode mode);
    void update(float dt);
    gamemode getmode();
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool onscreen() const;
};

struct obstacle {
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
    void addPortal(float x, float y, gamemode mode);
};

std::vector<obstacle> createpool();