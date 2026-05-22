#include "Entities/PlayerShip.h"

PlayerShip::PlayerShip() {
    m_shape.setSize({ 50.f, 50.f });
    m_shape.setOrigin({ 25.f, 25.f });
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setPosition({ 200.f, 525.f });
}

void PlayerShip::handleInput(const sf::Event& event) {
    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space && !m_isJumping) jump();
    }
}

void PlayerShip::update(sf::Time dt) { applyPhysics(dt); }

void PlayerShip::draw(sf::RenderWindow& window) {
    sf::ConvexShape ship(4);
    ship.setPoint(0, { 0.f, -30.f });
    ship.setPoint(1, { 25.f, 20.f });
    ship.setPoint(2, { 0.f, 10.f });
    ship.setPoint(3, { -25.f, 20.f });
    ship.setOrigin({ 0.f, 0.f });
    ship.setPosition(m_shape.getPosition());
    ship.setRotation(m_shape.getRotation());
    ship.setFillColor(m_primaryColor);
    ship.setOutlineThickness(3);
    ship.setOutlineColor(m_secondaryColor);
    if (m_glowEnabled) drawGlow(window, ship);
    window.draw(ship);

    sf::CircleShape cockpit(8.f);
    cockpit.setFillColor(m_secondaryColor);
    cockpit.setOrigin({ 8.f, 8.f });
    cockpit.setPosition(m_shape.getPosition() + sf::Vector2f(0.f, -15.f));
    window.draw(cockpit);
}