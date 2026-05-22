#include "Entities/PlayerUfo.h"

PlayerUfo::PlayerUfo() {
    m_shape.setSize({ 50.f, 50.f });
    m_shape.setOrigin({ 25.f, 25.f });
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setPosition({ 200.f, 525.f });
}

void PlayerUfo::handleInput(const sf::Event& event) {
    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space && !m_isJumping) {
            jump();
        }
    }
}

void PlayerUfo::update(sf::Time dt) {
    applyPhysics(dt);
}

void PlayerUfo::draw(sf::RenderWindow& window) {
    sf::ConvexShape body(4);
    body.setPoint(0, { -30.f, 0.f });
    body.setPoint(1, { 30.f, 0.f });
    body.setPoint(2, { 20.f, 20.f });
    body.setPoint(3, { -20.f, 20.f });
    body.setOrigin({ 0.f, 10.f });
    body.setPosition(m_shape.getPosition());
    body.setRotation(m_shape.getRotation());
    body.setFillColor(m_primaryColor);
    body.setOutlineThickness(3);
    body.setOutlineColor(m_secondaryColor);

    if (m_glowEnabled) drawGlow(window, body);
    window.draw(body);

    sf::CircleShape dome(12.f);
    dome.setFillColor(m_secondaryColor);
    dome.setOrigin({ 12.f, 12.f });
    dome.setPosition(m_shape.getPosition() + sf::Vector2f(0.f, -10.f));
    dome.setRotation(m_shape.getRotation());
    window.draw(dome);
}