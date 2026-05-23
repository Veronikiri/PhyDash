#include "Entities/Player.h"
#include "Entities/PlayerCube.h"
#include "Entities/PlayerShip.h"
#include "Entities/PlayerBall.h"
#include "Entities/PlayerUfo.h"
#include "Entities/PlayerWave.h"
#include "Entities/PlayerRobot.h"
#include "Entities/PlayerSpider.h"
#include <cmath>

void Player::applyPhysics(sf::Time dt) {
    if (!m_isJumping && m_onBlock) return;
    float dtSec = dt.asSeconds();
    m_velocity.y += m_gravity * dtSec;
    m_shape.move(m_velocity * dtSec);
    if (m_shape.getPosition().y >= 525.f) {
        m_shape.setPosition({ m_shape.getPosition().x, 525.f });
        m_velocity.y = 0.f;
        m_isJumping = false;
    }
}

void Player::drawGlow(sf::RenderWindow& window, const sf::Shape& shape) const {
    if (!m_glowEnabled) return;
    sf::Color glowColor(m_glowColor.r, m_glowColor.g, m_glowColor.b, 80);
    if (const auto* rect = dynamic_cast<const sf::RectangleShape*>(&shape)) {
        sf::RectangleShape copy = *rect;
        copy.setFillColor(glowColor);
        copy.setOutlineThickness(6);
        copy.setOutlineColor(sf::Color(m_glowColor.r, m_glowColor.g, m_glowColor.b, 40));
        window.draw(copy);
    }
    else if (const auto* circ = dynamic_cast<const sf::CircleShape*>(&shape)) {
        sf::CircleShape copy = *circ;
        copy.setFillColor(glowColor);
        copy.setOutlineThickness(6);
        copy.setOutlineColor(sf::Color(m_glowColor.r, m_glowColor.g, m_glowColor.b, 40));
        window.draw(copy);
    }
    else if (const auto* conv = dynamic_cast<const sf::ConvexShape*>(&shape)) {
        sf::ConvexShape copy = *conv;
        copy.setFillColor(glowColor);
        copy.setOutlineThickness(6);
        copy.setOutlineColor(sf::Color(m_glowColor.r, m_glowColor.g, m_glowColor.b, 40));
        window.draw(copy);
    }
}

std::unique_ptr<Player> Player::createForm(PlayerForm form) {
    switch (form) {
    case PlayerForm::Cube:  return std::make_unique<PlayerCube>();
    case PlayerForm::Ship:  return std::make_unique<PlayerShip>();
    case PlayerForm::Ball:  return std::make_unique<PlayerBall>();
    case PlayerForm::Ufo:   return std::make_unique<PlayerUfo>();
    case PlayerForm::Wave:  return std::make_unique<PlayerWave>();
    case PlayerForm::Robot: return std::make_unique<PlayerRobot>();
    case PlayerForm::Spider:return std::make_unique<PlayerSpider>();
    }
    return nullptr;
}