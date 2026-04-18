#pragma once
#include <SFML/Graphics.hpp>

enum class PlayerForm {
    Cube, Ship, Ball, Ufo, Wave, Robot, Spider
};

class Player {
public:
    Player();
    void handleInput(const sf::Event& event);
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }
    void resetPosition() { m_shape.setPosition({ 200.f, 500.f }); m_velocity = { 0.f, 0.f }; }

    void setForm(PlayerForm form) { m_form = form; }
    void setColors(sf::Color primary, sf::Color secondary, sf::Color glow, bool glowEnabled);
    PlayerForm getForm() const { return m_form; }
    sf::Color getPrimaryColor() const { return m_primaryColor; }
    sf::Color getSecondaryColor() const { return m_secondaryColor; }
    sf::Color getGlowColor() const { return m_glowColor; }
    bool isGlowEnabled() const { return m_glowEnabled; }

    void setCubePattern(int pattern) { m_cubePattern = pattern; }
    int getCubePattern() const { return m_cubePattern; }

    void setPosition(sf::Vector2f pos) { m_shape.setPosition(pos); }

private:
    void drawCube(sf::RenderWindow& window);
    void drawShip(sf::RenderWindow& window);
    void drawBall(sf::RenderWindow& window);
    void drawUfo(sf::RenderWindow& window);
    void drawWave(sf::RenderWindow& window);
    void drawRobot(sf::RenderWindow& window);
    void drawSpider(sf::RenderWindow& window);

    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
    bool m_isJumping;
    float m_gravity;
    sf::Angle m_startRotation;

    PlayerForm m_form;
    sf::Color m_primaryColor;
    sf::Color m_secondaryColor;
    sf::Color m_glowColor;
    bool m_glowEnabled;
    int m_cubePattern;
};