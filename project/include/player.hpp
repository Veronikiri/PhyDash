#pragma once
#include <SFML/Graphics.hpp>

enum class gamemode {
    cube,
    ship
};

class Player {
private:
    sf::RectangleShape shape;
    sf::RectangleShape hitbox;
    float g;
    float vy = 0.f;

    bool onGround = true;
    bool onBlock = false;
    bool onOrb = false;

    gamemode curmode = gamemode::cube;
public:
    Player();
    void setvy(float v);
    float getvy();
    void update(float dt);
    void clickaction();
    void setmode(gamemode newmode);
    gamemode getmode();
    void draw(sf::RenderWindow& window);
    void setpos(float x, float y);
    sf::Vector2f getpos();
    sf::FloatRect getBounds();

    void setonGround(bool ong);
    bool getonGround();
    void setonBlock(bool onb);
    bool getonBlock();
    void setonOrb(bool ono);
    bool getonOrb();
};