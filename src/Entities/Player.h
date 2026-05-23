#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

enum class PlayerForm { Cube, Ship, Ball, Ufo, Wave, Robot, Spider };

class Player {
public:
    sf::Vector2f getVelocity() const { return m_velocity; }
    void setVelocity(sf::Vector2f vel) { m_velocity = vel; }
    sf::Color getPrimaryColor() const { return m_primaryColor; }
    sf::Color getSecondaryColor() const { return m_secondaryColor; }
    sf::Color getGlowColor() const { return m_glowColor; }
    bool isGlowEnabled() const { return m_glowEnabled; }

    void setOnOrb(bool on) { m_onOrb = on; }
    bool isOnOrb() const { return m_onOrb; }

    virtual ~Player() = default;
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

    void setPosition(sf::Vector2f pos) {
        m_shape.setPosition(pos);
    }
    void resetPosition() {
        m_shape.setPosition({ 200.f,525.f });
        m_velocity = { 0.f,0.f };
        m_isJumping = false;
    }

    bool isJumping() const { return m_isJumping; }
    void jump() {
        if (!m_isJumping || m_onOrb || m_onBlock) {
            m_velocity.y = -800.f;
            m_isJumping = true;
            m_onOrb = false;
            m_onBlock = false;
        }
    }

    void setColors(sf::Color primary, sf::Color secondary, sf::Color glow, bool glowEnabled) {
        m_primaryColor = primary;
        m_secondaryColor = secondary;
        m_glowColor = glow;
        m_glowEnabled = glowEnabled;
    }

    static std::unique_ptr<Player> createForm(PlayerForm form);

    void setOnGround(bool on) {
        if (on) {
            m_isJumping = false;
            m_velocity.y = 0.f;
        }
        else {
            m_isJumping = true;
        }
    }

    void setOnBlock(bool on) {
        m_onBlock = on;
        if (on) {
            m_isJumping = false;
            m_velocity.y = 0.f;
        }
    }
    bool isOnBlock() const { return m_onBlock; }


protected:
    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity = { 0.f, 0.f };
    bool m_isJumping = false;
    bool m_onOrb = false;
    bool m_onBlock = false;
    float m_gravity = 2200.f;

    sf::Color m_primaryColor = sf::Color::Cyan;
    sf::Color m_secondaryColor = sf::Color::White;
    sf::Color m_glowColor = sf::Color::Cyan;
    bool m_glowEnabled = true;

    void applyPhysics(sf::Time dt);
    void drawGlow(sf::RenderWindow& window, const sf::Shape& shape) const;
};