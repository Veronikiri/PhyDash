#include "player.hpp"
#include <SFML/Graphics.hpp>

const float gcube = 1200.f;
const float gship = 600.f;
const float jump = -550.f;
const float flyingup = -400.f;

Player::Player()
{
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setFillColor(sf::Color::Green);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(2.f);
    shape.setPosition(sf::Vector2f(100.f, 500.f));

    hitbox.setSize(sf::Vector2f(50.f, 50.f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Blue);
    hitbox.setOutlineThickness(2.f);
    hitbox.setPosition(sf::Vector2f(100.f, 500.f));

    vy = 0.f;
    curmode = gamemode::cube;

    onGround = true;
    onBlock = false;
    onOrb = false;
}

void Player::setvy(float v)
{
    vy = v;
}

float Player::getvy()
{
    return vy;
}

void Player::update(float dt)
{
    if (curmode == gamemode::cube)
    {
        vy = vy + gcube * dt;
    }
    else if (curmode == gamemode::ship)
    {
        vy =  vy + gship * dt;
    }

    if (hitbox.getPosition().y<50.f) {
        vy = 0.f;
    }
    if (hitbox.getPosition().y>500.f) {
        vy = 0.f;
        onGround = true;
    }
    if (hitbox.getPosition().y < 500.f - 5.f)
    {
        onGround = false;
        onBlock = false;
        onOrb = false;
    }

    shape.move(sf::Vector2f(0.f, vy * dt));
    hitbox.move(sf::Vector2f(0.f, vy * dt));
}

void Player::clickaction()
{
    if (curmode == gamemode::cube)
    {
        if (onGround || onBlock || onOrb) {
            vy = jump;
            onGround = false;
            onBlock = false;
            onOrb = false;
        }
    }
    else if (curmode == gamemode::ship)
    {
        if (onOrb) {
            vy = jump;
            onOrb = false;
        }
        else {
            vy = flyingup;
        }
    }
}

void Player::setmode(gamemode newmode)
{
    curmode = newmode;
    if (curmode == gamemode::ship)
    {
        vy = 0.f;
    }
}

gamemode Player::getmode()
{
    return curmode;
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(hitbox);
}

void Player::setpos(float x, float y)
{
    shape.setPosition(sf::Vector2f(x, y));
    hitbox.setPosition(sf::Vector2f(x + 2.5f, y + 2.5f));
}

sf::FloatRect Player::getBounds()
{
    return hitbox.getGlobalBounds();
}

sf::Vector2f Player::getpos()
{
    return shape.getPosition();
}
void Player::setonGround(bool ong) {
    onGround = ong;
}
bool Player::getonGround() {
    return onGround;
}
void Player::setonBlock(bool onb) {
    onBlock = onb;
}
bool Player::getonBlock() {
    return onBlock;
}
void Player::setonOrb(bool ono) {
    onOrb = ono;
}
bool Player::getonOrb() {
    return onOrb;
}
